import SwiftUI

/// Knob's app icon, drawn with the same parts as the panel: a grained charcoal faceplate in the
/// macOS icon shape, a soft orange glow and the realistic knob. `--icons DIR` renders it to PNG.
struct AppIconView: View {
    var size: CGFloat = 1024

    var body: some View {
        let scale = size / 1024
        let plate = RoundedRectangle(cornerRadius: 185 * scale, style: .continuous)
        ZStack {
            plate
                .fill(LinearGradient(colors: [Color(hex: 0x484B51), Color(hex: 0x1C1E21)], startPoint: .top, endPoint: .bottom))
                .overlay(Grain(opacity: 0.05).clipShape(plate))
                .overlay(plate.stroke(LinearGradient(colors: [.white.opacity(0.22), .clear], startPoint: .top, endPoint: .center),
                                      lineWidth: 3 * scale))
                .overlay {
                    RadialGradient(colors: [Palette.accentGlow.opacity(0.34), .clear],
                                   center: UnitPoint(x: 0.5, y: 0.1), startRadius: 0, endRadius: 300 * scale)
                        .scaleEffect(x: 1.6, y: 1, anchor: .top)
                        .clipShape(plate)
                }
                .frame(width: 824 * scale, height: 824 * scale)
                .shadow(color: .black.opacity(0.35), radius: 22 * scale, y: 14 * scale)
            RealisticKnob(value: .constant(0.68), size: 590 * scale, showsMinMax: false,
                          accessibilityLabel: "Knob", accessibilityValue: "")
                .offset(y: 8 * scale)
        }
        .frame(width: size, height: size)
        .environment(\.colorScheme, .dark)
        .accessibilityHidden(true)
    }
}
