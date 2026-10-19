import SwiftUI

/// Port of the 21st.dev "Switch" (serafimcloud): a pill-shaped thumb over a track whose
/// gradient slides from muted to accent, inset track shadows, and 250ms on / 125ms off timing.
struct PowerSwitch: View {
    @Binding var isOn: Bool
    var label: String

    @Environment(\.colorScheme) private var scheme
    @Environment(\.isEnabled) private var isEnabled

    private let width: CGFloat = 46
    private let height: CGFloat = 20
    private let thumbWidth: CGFloat = 26
    private let thumbHeight: CGFloat = 18

    var body: some View {
        let p = Palette.of(scheme)
        let muted = p.faint.opacity(0.45)
        // The 21st.dev track paints a gradient 2.78x wider than itself and slides it.
        let gradientWidth = width * 2.78
        HStack(spacing: 8) {
            Text(isOn ? "ON" : "OFF")
                .font(.system(size: 10, weight: .bold))
                .tracking(1)
                .foregroundStyle(isOn ? Palette.accent : p.muted)
                .shadow(color: isOn ? Palette.accentGlow.opacity(0.8) : .clear, radius: 4)
                .frame(width: 26, alignment: .trailing)

            ZStack(alignment: .leading) {
                LinearGradient(stops: [.init(color: Palette.accent, location: 0.35),
                                       .init(color: muted, location: 0.65)],
                               startPoint: .leading, endPoint: .trailing)
                    .frame(width: gradientWidth)
                    .offset(x: isOn ? 0 : -(gradientWidth - width))
                    .frame(width: width, height: height, alignment: .leading)
                    .clipShape(Capsule())
                Capsule().stroke(Color.black.opacity(0.05), lineWidth: 1)
                Capsule()
                    .stroke(LinearGradient(colors: [Color.black.opacity(0.18), .clear],
                                           startPoint: .top, endPoint: .center), lineWidth: 2)
                    .blur(radius: 1)
                    .clipShape(Capsule())
                Capsule()
                    .fill(Color.white)
                    .frame(width: thumbWidth, height: thumbHeight)
                    .shadow(color: .black.opacity(0.08), radius: 0.5)
                    .shadow(color: .black.opacity(0.12), radius: 0.5, y: 1)
                    .shadow(color: .black.opacity(0.18), radius: 2, x: 1, y: 2)
                    .offset(x: isOn ? width - thumbWidth - 1 : 1)
            }
            .frame(width: width, height: height)
            .shadow(color: isOn ? Palette.accentGlow.opacity(0.45) : .clear, radius: 6)
        }
        .opacity(isEnabled ? 1 : 0.5)
        .contentShape(Rectangle())
        .onTapGesture {
            guard isEnabled else { return }
            withAnimation(Motion.switchThumb(turningOn: !isOn)) { isOn.toggle() }
        }
        .accessibilityElement()
        .accessibilityLabel(label)
        .accessibilityValue(isOn ? "On" : "Off")
        .accessibilityAddTraits(.isToggle)
        .accessibilityAction { withAnimation(Motion.switchThumb(turningOn: !isOn)) { isOn.toggle() } }
    }
}
