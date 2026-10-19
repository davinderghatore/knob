import AppKit
import Sparkle

/// Keeps Knob up to date from GitHub Releases (Sparkle). It checks at launch and every few hours,
/// downloads new versions quietly, checks their signature, then installs and relaunches straight
/// away: Knob lives in the menu bar and rarely quits, so waiting for a quit would mean never.
/// Off in dev builds (`npm run dev`) and in builds without a feed URL.
@MainActor
final class Updater: NSObject, SPUUpdaterDelegate, SPUStandardUserDriverDelegate {
    static let shared = Updater()
    private var controller: SPUStandardUpdaterController?

    var isAvailable: Bool { controller != nil }

    func start() {
        let bundle = Bundle.main
        guard controller == nil,
              bundle.object(forInfoDictionaryKey: "SUFeedURL") != nil,
              bundle.bundleIdentifier?.hasSuffix(".dev") == false else { return }
        controller = SPUStandardUpdaterController(startingUpdater: true, updaterDelegate: self,
                                                  userDriverDelegate: self)
    }

    func checkForUpdates() {
        controller?.checkForUpdates(nil)
    }

    /// Any message Sparkle does need to show (a password for a driver update, an error) appears
    /// without jumping in front of what the user is doing.
    nonisolated var supportsGentleScheduledUpdateReminders: Bool { true }

    nonisolated func updater(_ updater: SPUUpdater, willInstallUpdateOnQuit item: SUAppcastItem,
                             immediateInstallationBlock immediateInstallHandler: @escaping () -> Void) -> Bool {
        immediateInstallHandler()
        return true
    }
}
