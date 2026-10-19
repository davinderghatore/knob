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

/// Preset picker with previous/next buttons, like the preset strip on studio gear, plus a save
/// button that turns the strip into a name field.
struct PresetBar: View {
    @ObservedObject var model: KnobModel
    @State private var naming = false
    @State private var name = ""
    @FocusState private var nameFocused: Bool
    @Environment(\.colorScheme) private var scheme

    var body: some View {
        HStack(spacing: 6) {
            if naming {
                TextField("Name your preset", text: $name)
                    .textFieldStyle(.plain)
                    .font(.system(size: 11, weight: .medium))
                    .foregroundStyle(Palette.of(scheme).label)
                    .padding(.horizontal, 10)
                    .frame(height: 24)
                    .background(RecessedPill())
                    .focused($nameFocused)
                    .onSubmit(save)
                    .onExitCommand { naming = false }
                HardwareIconButton(systemImage: "checkmark", label: "Save preset", action: save)
                    .disabled(name.trimmingCharacters(in: .whitespaces).isEmpty)
                HardwareIconButton(systemImage: "xmark", label: "Cancel") { naming = false }
            } else {
                PillMenu(items: model.presets, selection: model.currentPreset, title: { $0.name },
                         placeholder: "Choose a preset") { model.select($0) }
                HardwareIconButton(systemImage: "chevron.left", label: "Previous preset") { model.stepPreset(-1) }
                HardwareIconButton(systemImage: "chevron.right", label: "Next preset") { model.stepPreset(1) }
                HardwareIconButton(systemImage: "square.and.arrow.down", label: "Save as a new preset") {
                    name = ""
                    naming = true
                    nameFocused = true
                }
            }
        }
        .disabled(model.connection != .online)
    }

    private func save() {
        let chosen = name
        Task {
            if await model.savePreset(named: chosen) { naming = false }
        }
    }
}
