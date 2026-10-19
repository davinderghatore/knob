import SwiftUI

/// Live spectrum bars with dB grid lines, drawn like an analyser screen. `bands` are 0...1 from
/// low to high frequency; they are smoothly interpolated into `barCount` thin bars.
struct SpectrumView: View {
    var bands: [Double]
    var barCount = 46
    var showsGrid = true
    var intensity = 1.0

    var body: some View {
        Canvas { context, size in
            if showsGrid {
                for index in 1...3 {
                    let y = size.height * CGFloat(index) / 4
                    var line = Path()
                    line.move(to: CGPoint(x: 0, y: y))
                    line.addLine(to: CGPoint(x: size.width, y: y))
                    context.stroke(line, with: .color(Screen.grid), style: StrokeStyle(lineWidth: 0.5, dash: [2, 3]))
                }
            }
            guard bands.count > 1 else { return }
            let left: CGFloat = 0
            let width = size.width
            let step = width / CGFloat(barCount)
            let barWidth = max(1.2, step * 0.55)
            for index in 0..<barCount {
                let level = sample(at: Double(index) / Double(barCount - 1)) * intensity
                let height = max(1.5, CGFloat(level) * size.height * 0.92)
                let rect = CGRect(x: left + CGFloat(index) * step, y: size.height - height, width: barWidth, height: height)
                context.fill(Path(rect), with: .linearGradient(
                    Gradient(colors: [Screen.bar.opacity(0.95), Screen.bar.opacity(0.25)]),
                    startPoint: CGPoint(x: rect.midX, y: rect.minY), endPoint: CGPoint(x: rect.midX, y: rect.maxY)))
            }
        }
        .allowsHitTesting(false)
        .accessibilityHidden(true)
    }

    /// Smooth (cosine) interpolation between neighbouring bands, plus a fixed ripple so bars
    /// inside one band are not identical. No randomness: the picture only moves with the audio.
    private func sample(at position: Double) -> Double {
        let scaled = position * Double(bands.count - 1)
        let i = min(Int(scaled), bands.count - 2)
        let t = scaled - Double(i)
        let eased = (1 - cos(t * .pi)) / 2
        let base = bands[i] * (1 - eased) + bands[i + 1] * eased
        let ripple = 0.86 + 0.14 * sin(position * 57.0) * sin(position * 13.0)
        return min(max(base * ripple, 0), 1)
    }
}
