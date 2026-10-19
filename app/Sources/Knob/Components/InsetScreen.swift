import SwiftUI

/// The dark recessed screen with the orange glow along its top edge. `glow` is 0...1 and follows
/// the live audio level, so the light breathes with the music.
struct InsetScreen<Content: View>: View {
    var glow: Double
    @ViewBuilder var content: Content

    var body: some View {
        let shape = RoundedRectangle(cornerRadius: 14, style: .continuous)
        content
            .padding(10)
            .frame(maxWidth: .infinity)
            .background(shape.fill(LinearGradient(colors: [Screen.top, Screen.bottom], startPoint: .top, endPoint: .bottom)))
            .overlay(shape.stroke(Screen.edge, lineWidth: 1.5))
            .overlay(shape.inset(by: 1.5).stroke(Screen.highlight, lineWidth: 0.5))
            .overlay(shape.stroke(Color.black.opacity(0.8), lineWidth: 4).blur(radius: 4).clipShape(shape))
            .background(alignment: .top) {
                Ellipse()
                    .fill(RadialGradient(colors: [Palette.accentGlow, Palette.accent.opacity(0)],
                                         center: .center, startRadius: 0, endRadius: 60))
                    .frame(width: 150, height: 34)
                    .blur(radius: 6)
                    .opacity(0.25 + 0.75 * min(max(glow, 0), 1))
                    .offset(y: -16)
                    .animation(Motion.gentle, value: glow)
            }
    }
}
