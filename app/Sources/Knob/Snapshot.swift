import AppKit
import SwiftUI

/// Renders the panel offscreen with the demo engine, in light and dark, to PNG files.
/// Used for visual checks: `Knob --snapshot DIR` writes panel-light.png and panel-dark.png.
@MainActor
enum Snapshot {
    static func run(into folder: URL) {
        let app = NSApplication.shared
        app.setActivationPolicy(.prohibited)
        try? FileManager.default.createDirectory(at: folder, withIntermediateDirectories: true)

        let model = KnobModel(engine: DemoEngine(), volume: DemoVolume())
        model.start()
        model.select(Preset.factory().first { $0.name == "Music" } ?? Preset.factory().first!)
        Task { @MainActor in
            await model.refresh()
            model.panelOpened()
            try? await Task.sleep(for: .milliseconds(900))
            for (name, scheme) in [("light", ColorScheme.light), ("dark", ColorScheme.dark)] {
                for tab in ScreenSection.Tab.allCases {
                    UserDefaults.standard.set(tab.rawValue, forKey: PersistKey.selectedTab.rawValue)
                    render(PanelView(model: model).environment(\.colorScheme, scheme), scheme: scheme,
                           to: folder.appendingPathComponent("panel-\(name)-\(tab.rawValue).png"))
                }
            }
            model.panelClosed()
            exit(0)
        }
        app.run()
    }

    private static func render<V: View>(_ view: V, scheme: ColorScheme, to url: URL) {
        let host = NSHostingView(rootView: view)
        host.frame.size = host.fittingSize
        let window = NSWindow(contentRect: host.frame, styleMask: .borderless, backing: .buffered, defer: false)
        window.appearance = NSAppearance(named: scheme == .dark ? .darkAqua : .aqua)
        window.contentView = host
        host.layoutSubtreeIfNeeded()
        RunLoop.main.run(until: Date().addingTimeInterval(0.4))
        guard let bitmap = host.bitmapImageRepForCachingDisplay(in: host.bounds) else { return }
        host.cacheDisplay(in: host.bounds, to: bitmap)
        try? bitmap.representation(using: .png, properties: [:])?.write(to: url)
        print("wrote \(url.path)")
    }
}
