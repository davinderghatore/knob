import AVFoundation
import Foundation

/// Starts and stops knob-engine through knob-supervisor (both in Knob.app/Contents/Helpers).
/// The supervisor puts the user's previous output back whenever the engine stops or crashes.
@MainActor
final class EngineLauncher {
    enum Problem: LocalizedError {
        case helpersMissing, deviceMissing, noOutput, permissionDenied, launchFailed

        /// Plain-language text shown on the panel.
        var errorDescription: String? {
            switch self {
            case .helpersMissing: "This copy of Knob is missing its sound engine. Rebuild Knob."
            case .deviceMissing: "Knob's audio device isn't installed yet."
            case .noOutput: "There are no speakers or displays to play through."
            case .permissionDenied: "Knob needs audio access. Allow it in System Settings, Privacy & Security, Microphone."
            case .launchFailed: "The sound engine couldn't start. Try again."
            }
        }
    }

    private var supervisor: Process?

    var isRunning: Bool { supervisor?.isRunning == true }

    func start(preset: URL?) async throws {
        let helpers = Bundle.main.bundleURL.appendingPathComponent("Contents/Helpers")
        let engine = helpers.appendingPathComponent("knob-engine")
        let supervisorURL = helpers.appendingPathComponent("knob-supervisor")
        let files = FileManager.default
        guard files.isExecutableFile(atPath: engine.path), files.isExecutableFile(atPath: supervisorURL.path)
        else { throw Problem.helpersMissing }
        guard HardwareDevices.all().contains(where: { $0.uid == HardwareDevices.knobUID }) else { throw Problem.deviceMissing }
        guard let output = HardwareDevices.preferredPhysicalOutput() else { throw Problem.noOutput }
        guard await Self.audioAccess() else { throw Problem.permissionDenied }

        // The supervisor restores whatever output was current when it started, so make sure
        // that is real hardware and not some other virtual device.
        if !HardwareDevices.all().contains(where: { $0.id == HardwareDevices.defaultOutputID && $0.isPhysicalOutput }) {
            HardwareDevices.setDefaultOutput(output.id)
        }

        var arguments = ["--engine", engine.path, "--output", output.uid, "--activate"]
        let fallback = Bundle.main.resourceURL?.appendingPathComponent("presets/Default.fac")
        if let preset { arguments += ["--preset", preset.path] }
        if let fallback, files.fileExists(atPath: fallback.path) { arguments += ["--fallback-preset", fallback.path] }

        let process = Process()
        process.executableURL = supervisorURL
        process.arguments = arguments
        do { try process.run() } catch { throw Problem.launchFailed }
        supervisor = process
    }

    /// Asks the supervisor to stop: it restores the previous output, then stops the engine.
    func stop() {
        guard let process = supervisor else { return }
        supervisor = nil
        guard process.isRunning else { return }
        process.terminate()
        DispatchQueue.global(qos: .utility).async { process.waitUntilExit() }
    }

    private static func audioAccess() async -> Bool {
        switch AVCaptureDevice.authorizationStatus(for: .audio) {
        case .authorized: return true
        case .notDetermined: return await AVCaptureDevice.requestAccess(for: .audio)
        default: return false
        }
    }
}
