import AppKit
import SwiftUI

/// Wordmark, the power switch and the app menu.
struct HeaderBar: View {
    @ObservedObject var model: KnobModel
    @AppStorage(PersistKey.showInDock.rawValue) private var showInDock = true
    @Environment(\.colorScheme) private var scheme

    var body: some View {
        let p = Palette.of(scheme)
        HStack(spacing: 10) {
            Text("knob")
                .font(.system(size: 19, weight: .heavy, design: .rounded))
                .foregroundStyle(p.muted)
                .shadow(color: scheme == .dark ? .black.opacity(0.6) : .white, radius: 0, y: 1)
                .accessibilityAddTraits(.isHeader)
            Spacer()
            PowerSwitch(isOn: Binding(get: { model.powerOn }, set: { model.setPower($0) }), label: "Sound effects")
                .disabled(model.starting)
            Menu {
                Button("About Knob") { AboutWindow.show() }
                if Updater.shared.isAvailable {
                    Button("Check for Updates…") { Updater.shared.checkForUpdates() }
                }
                Toggle("Show in Dock", isOn: $showInDock)
                Divider()
                Button("Quit Knob") { model.quit() }.keyboardShortcut("q")
            } label: {
                Image(systemName: "ellipsis")
                    .font(.system(size: 11, weight: .bold))
                    .foregroundStyle(p.label)
                    .frame(width: 24, height: 24)
                    .background(RecessedPill())
                    .contentShape(Rectangle())
            }
            .menuStyle(.button)
            .buttonStyle(.plain)
            .menuIndicator(.hidden)
            .accessibilityLabel("Knob menu")
            .onChange(of: showInDock) { _, shown in DockPresence.apply(shown) }
        }
    }

}

/// Preset picker with previous/next buttons, like the preset strip on studio gear.
struct PresetBar: View {
    @ObservedObject var model: KnobModel

    var body: some View {
        HStack(spacing: 6) {
            PillMenu(items: model.presets, selection: model.currentPreset, title: { $0.name },
                     placeholder: "Choose a preset") { model.select($0) }
            HardwareIconButton(systemImage: "chevron.left", label: "Previous preset") { model.stepPreset(-1) }
            HardwareIconButton(systemImage: "chevron.right", label: "Next preset") { model.stepPreset(1) }
        }
        .disabled(model.connection != .online)
    }
}
