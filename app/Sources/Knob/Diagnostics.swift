import AppKit
import Foundation

/// `Knob --volume-check SECONDS`: exercises the real Mac volume path. Prints every change it
/// hears (volume keys, Control Center, other apps) for SECONDS, then sets the volume from the app
/// and restores the original level. Used to verify volume sync without needing a screenshot.
@MainActor
enum VolumeCheck {
    static func run(seconds: Double) {
        let app = NSApplication.shared
        app.setActivationPolicy(.prohibited)
        let control = SystemVolume()
        var original: Double?
        var latest = 0.0
        control.onChange = { volume, muted, controllable in
            if original == nil { original = volume }
            latest = volume
            print(String(format: "heard volume=%.3f muted=%@ controllable=%@", volume, muted ? "yes" : "no", controllable ? "yes" : "no"))
        }
        control.start()
        Task { @MainActor in
            try? await Task.sleep(for: .seconds(seconds))
            let start = original ?? latest
            let target = start > 0.5 ? start - 0.1 : start + 0.1
            print(String(format: "setting volume from app to %.3f", target))
            control.setVolume(target)
            try? await Task.sleep(for: .milliseconds(400))
            print(String(format: "restoring volume to %.3f", start))
            control.setVolume(start)
            try? await Task.sleep(for: .milliseconds(400))
            exit(0)
        }
        app.run()
    }
}

/// Dev mode only (`KNOB_DEV_OPEN_POPOVER=1`, set by scripts/dev.sh): reopens the menu bar panel
/// right after a relaunch so changes are visible without clicking. SwiftUI has no API for this,
/// so it clicks Knob's own status bar button.
@MainActor
enum DevPopover {
    static func open() {
        let button = NSApp.windows
            .filter { $0.className.contains("StatusBarWindow") }
            .compactMap { $0.contentView.flatMap(findButton) }
            .first
        guard let button else { NSLog("dev: menu bar button not found"); return }
        button.performClick(nil)
        NSLog("dev: opened the Knob panel")
    }

    private static func findButton(in view: NSView) -> NSStatusBarButton? {
        if let button = view as? NSStatusBarButton { return button }
        for subview in view.subviews { if let found = findButton(in: subview) { return found } }
        return nil
    }
}
