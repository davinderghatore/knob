import AppKit
import SwiftUI

/// Wordmark, the power switch and the app menu.
struct HeaderBar: View {
    @ObservedObject var model: KnobModel
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
                Button("About Knob", action: showAbout)
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
        }
    }

    private func showAbout() {
        let credits = NSAttributedString(
            string: "Free software under the GNU Affero General Public License v3. It comes with no warranty. "
                + "The complete source code is included with the installer.",
            attributes: [.font: NSFont.systemFont(ofSize: 11), .foregroundColor: NSColor.secondaryLabelColor])
        NSApp.activate(ignoringOtherApps: true)
        NSApp.orderFrontStandardAboutPanel(options: [.credits: credits])
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
