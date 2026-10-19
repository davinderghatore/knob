import AppKit
import Foundation
import SwiftUI

/// Everything the panel shows, and every action it can take. Views only talk to this object;
/// it talks to the engine and to the Mac's volume.
@MainActor
final class KnobModel: ObservableObject {
    enum Connection { case connecting, online, offline }

    @Published private(set) var connection: Connection = .connecting
    @Published var volume = 0.0
    @Published private(set) var muted = false
    @Published private(set) var volumeControllable = true
    @Published private(set) var powerOn = false
    @Published private(set) var presets: [Preset] = []
    @Published private(set) var currentPreset: Preset?
    @Published var effects: [Double] = Array(repeating: 0, count: EffectSlot.allCases.count)
    @Published var eq: [Double] = []
    @Published private(set) var frequencies: [Double] = []
    @Published private(set) var outputs: [OutputDevice] = []
    @Published private(set) var outputUID: String?
    @Published private(set) var spectrum: [Double] = Array(repeating: 0, count: 10)
    @Published private(set) var peakLeft = 0.0
    @Published private(set) var peakRight = 0.0
    /// A plain-language message for the panel, such as why the engine couldn't start.
    @Published private(set) var notice: String?
    @Published private(set) var starting = false

    private let engine: SoundEngine
    private let volumeControl: VolumeControl
    /// Starts the real engine; nil for the demo engine, which is always running.
    private let launcher: EngineLauncher?
    private var liveTask: Task<Void, Never>?
    private var pending: [String: Double] = [:]
    private var flushTask: Task<Void, Never>?
    private var editing = 0
    private var spectrumPeak = 0.05

    init(engine: SoundEngine, volume: VolumeControl, launcher: EngineLauncher? = nil) {
        self.engine = engine
        self.volumeControl = volume
        self.launcher = launcher
    }

    func start() {
        volumeControl.onChange = { [weak self] volume, muted, controllable in
            guard let self else { return }
            self.volume = volume
            self.muted = muted
            self.volumeControllable = controllable
        }
        volumeControl.start()
        presets = Preset.factory()
        Task { await refresh() }
    }

    // MARK: Live updates (only while the panel is open)

    /// Counts open panels: the menu bar panel, plus the dev window in dev mode.
    private var openPanels = 0

    func panelOpened() {
        openPanels += 1
        guard liveTask == nil else { return }
        liveTask = Task { [weak self] in
            while !Task.isCancelled {
                await self?.poll()
                try? await Task.sleep(for: .milliseconds(66))
            }
        }
    }

    func panelClosed() {
        openPanels = max(0, openPanels - 1)
        guard openPanels == 0 else { return }
        liveTask?.cancel()
        liveTask = nil
    }

    func refresh() async {
        do {
            outputs = OutputDevice.list(json: try await engine.send("getDevices", [:]))
            apply(EngineState(json: try await engine.send("getState", [:])), includeControls: true)
            connection = .online
        } catch {
            connection = .offline
        }
    }

    private func poll() async {
        guard connection == .online else {
            await refresh()
            if connection == .offline { try? await Task.sleep(for: .seconds(2)) }
            return
        }
        do { apply(EngineState(json: try await engine.send("getState", [:])), includeControls: editing == 0 && pending.isEmpty) }
        catch { connection = .offline }
    }

    private func apply(_ state: EngineState, includeControls: Bool) {
        powerOn = !state.bypassed
        outputUID = state.outputUID
        peakLeft = state.peakLeft
        peakRight = state.peakRight
        spectrumPeak = max(spectrumPeak * 0.995, state.spectrum.max() ?? 0, 0.05)
        let target = state.spectrum.map { min($0 / spectrumPeak, 1) }
        if target.count == spectrum.count {
            spectrum = zip(spectrum, target).map { old, new in new > old ? old + (new - old) * 0.7 : old + (new - old) * 0.25 }
        } else {
            spectrum = target
        }
        guard includeControls else { return }
        if state.effects.count == effects.count { effects = state.effects.map { $0 / 10 } }
        eq = state.eq.map { $0 / 12 }
        frequencies = state.frequencies
    }

    // MARK: Actions

    func setVolume(_ value: Double) {
        volume = value
        volumeControl.setVolume(value)
    }

    func toggleMute() { volumeControl.setMuted(!muted) }

    /// ON starts the engine if it isn't running; OFF stops the engine Knob started (giving the
    /// Mac its normal output back), or just bypasses effects on an engine Knob didn't start.
    func setPower(_ on: Bool) {
        guard !starting else { return }
        if on, connection != .online {
            Task { await startEngine() }
            return
        }
        if !on, let launcher, launcher.isRunning {
            Task { await stopEngine() }
            return
        }
        powerOn = on
        notice = nil
        Task { _ = try? await engine.send("setBypass", ["value": !on]) }
    }

    private func startEngine() async {
        guard let launcher else { return }
        starting = true
        notice = "Starting the sound engine..."
        defer { starting = false }
        do {
            try await launcher.start(preset: currentPreset?.url)
        } catch {
            notice = error.localizedDescription
            return
        }
        for _ in 0..<50 where connection != .online {
            try? await Task.sleep(for: .milliseconds(200))
            await refresh()
        }
        guard connection == .online else {
            launcher.stop()
            notice = EngineLauncher.Problem.launchFailed.errorDescription
            return
        }
        _ = try? await engine.send("setBypass", ["value": false])
        powerOn = true
        notice = nil
    }

    /// Hands the Mac its normal output back, then stops the engine.
    func stopEngine() async {
        guard let launcher, launcher.isRunning else { return }
        _ = try? await engine.send("activateRouting", ["value": false])
        _ = try? await engine.send("shutdown", [:])
        launcher.stop()
        powerOn = false
        connection = .offline
        notice = nil
    }

    func quit() {
        Task {
            await stopEngine()
            NSApp.terminate(nil)
        }
    }

    func setEffect(_ slot: EffectSlot, _ value: Double) {
        effects[slot.rawValue] = value
        queue(slot.parameter, value * 10)
    }

    func setEQ(band: Int, _ value: Double) {
        guard eq.indices.contains(band) else { return }
        eq[band] = value
        queue("eq\(band + 1)", value * 12)
    }

    func editingChanged(_ active: Bool) { editing = max(0, editing + (active ? 1 : -1)) }

    func select(_ preset: Preset) {
        currentPreset = preset
        Task {
            _ = try? await engine.send("setPreset", ["path": preset.url.path])
            if let state = try? await engine.send("getState", [:]) { apply(EngineState(json: state), includeControls: true) }
        }
    }

    func stepPreset(_ offset: Int) {
        guard !presets.isEmpty else { return }
        let index = currentPreset.flatMap { presets.firstIndex(of: $0) } ?? 0
        select(presets[(index + offset + presets.count) % presets.count])
    }

    func selectOutput(_ device: OutputDevice) {
        outputUID = device.uid
        Task { _ = try? await engine.send("setOutputUID", ["outputUID": device.uid]) }
    }

    /// Coalesces fast knob movement into at most one engine message per name every 50ms.
    private func queue(_ name: String, _ value: Double) {
        pending[name] = value
        guard flushTask == nil else { return }
        flushTask = Task { [weak self] in
            try? await Task.sleep(for: .milliseconds(50))
            guard let self else { return }
            let batch = self.pending
            self.pending.removeAll()
            self.flushTask = nil
            for (name, value) in batch { _ = try? await self.engine.send("setParam", ["name": name, "value": value]) }
        }
    }
}
