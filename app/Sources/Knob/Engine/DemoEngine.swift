import Foundation

/// A stand-in engine for `--demo` and `--snapshot`: same commands, same reply shapes, no audio.
/// Lets the full panel be seen and screenshotted on a Mac where the engine isn't installed.
final class DemoEngine: SoundEngine {
    private var bypassed = false
    private var effects: [Double] = [6, 4, 3, 5, 7]
    private var eq: [Double] = [3, 2, 0.5, -1, -2, -0.5, 1, 2.5, 3, 2]
    private let frequencies: [Double] = [32, 64, 125, 250, 500, 1000, 2000, 4000, 8000, 16000]
    private var outputUID = "demo-hdmi"
    private var tick = 0.0

    func send(_ command: String, _ arguments: [String: Any] = [:]) async throws -> Any? {
        switch command {
        case "getDevices":
            return [["uid": "demo-hdmi", "name": "LS27D80xE", "outputs": 2, "virtual": false],
                    ["uid": "demo-speakers", "name": "MacBook Air Speakers", "outputs": 2, "virtual": false]]
        case "getState", "getMeters":
            tick += 0.07
            let spectrum = (0..<10).map { band -> Double in
                let shape = 0.85 - Double(band) * 0.055
                return max(0.05, shape * (0.72 + 0.28 * sin(tick * (1.3 + Double(band) * 0.37) + Double(band))))
            }
            let peak = bypassed ? 0.0 : 0.35 + 0.2 * sin(tick * 2.1)
            return ["controls": ["bypassed": bypassed, "effects": effects, "eq": eq,
                                 "frequencies": frequencies, "spectrum": spectrum],
                    "outputUID": outputUID, "routingActive": !bypassed,
                    "metrics": ["peakLeft": peak, "peakRight": peak * 0.92]]
        case "setBypass":
            bypassed = arguments["value"] as? Bool ?? bypassed
        case "setOutputUID":
            outputUID = arguments["outputUID"] as? String ?? outputUID
        case "setParam":
            apply(arguments["name"] as? String ?? "", (arguments["value"] as? NSNumber)?.doubleValue ?? 0)
        case "setPreset", "savePreset":
            break
        default:
            throw EngineError(message: "unknown command")
        }
        return nil
    }

    private func apply(_ name: String, _ value: Double) {
        if let slot = EffectSlot.allCases.first(where: { $0.parameter == name }) { effects[slot.rawValue] = value }
        if name.hasPrefix("eq"), let band = Int(name.dropFirst(2)), (1...eq.count).contains(band) { eq[band - 1] = value }
    }
}
