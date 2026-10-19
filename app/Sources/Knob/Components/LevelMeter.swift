import SwiftUI

/// A pair of vertical L/R meters in recessed grooves, orange fill on a dB scale (-48...0 dB),
/// with tick labels like a hardware threshold meter.
struct LevelMeter: View {
    var left: Double
    var right: Double
    var height: CGFloat

    @Environment(\.colorScheme) private var scheme

    var body: some View {
        let p = Palette.of(scheme)
        HStack(alignment: .top, spacing: 5) {
            ZStack(alignment: .topTrailing) {
                ForEach([0, -12, -24, -36, -48], id: \.self) { decibel in
                    Text("\(decibel)").font(.system(size: 7.5, weight: .medium).monospacedDigit())
                        .foregroundStyle(p.muted)
                        .frame(height: 10)
                        .offset(y: height * CGFloat(-Double(decibel) / 48) - 5)
                }
            }
            .frame(width: 18, height: height, alignment: .topTrailing)
            groove(level: left)
            groove(level: right)
        }
        .frame(height: height)
        .accessibilityElement()
        .accessibilityLabel("Output level")
        .accessibilityValue("\(Int(Self.decibels(max(left, right)))) dB")
    }

    private func groove(level: Double) -> some View {
        let fill = Self.position(level)
        return ZStack(alignment: .bottom) {
            Capsule().fill(LinearGradient(colors: [Color.black.opacity(0.85), Color(hex: 0x2A2C30)],
                                          startPoint: .leading, endPoint: .trailing))
            Capsule()
                .fill(LinearGradient(colors: [Palette.accentGlow, Palette.accent], startPoint: .top, endPoint: .bottom))
                .frame(height: max(0, height * CGFloat(fill)))
                .shadow(color: Palette.accentGlow.opacity(0.8), radius: 3)
                .animation(.linear(duration: 0.08), value: fill)
        }
        .frame(width: 5, height: height)
    }

    static func decibels(_ linear: Double) -> Double { linear > 0 ? max(-60, 20 * log10(linear)) : -60 }
    /// Maps -48...0 dB onto 0...1.
    static func position(_ linear: Double) -> Double { min(max((decibels(linear) + 48) / 48, 0), 1) }
}
