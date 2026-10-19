import SwiftUI

/// Fine speckle like a powder-coated hardware panel. Deterministic, so it never flickers.
struct Grain: View {
    var opacity: Double

    var body: some View {
        Canvas { context, size in
            guard opacity > 0 else { return }
            var seed: UInt64 = 0x9E3779B97F4A7C15
            func next() -> Double {
                seed = seed &* 6364136223846793005 &+ 1442695040888963407
                return Double(seed >> 11) / Double(1 << 53)
            }
            let count = Int(size.width * size.height / 38)
            for _ in 0..<count {
                let x = next() * size.width, y = next() * size.height
                let dot = 0.5 + next() * 0.9
                let dark = next() > 0.35
                context.fill(Path(ellipseIn: CGRect(x: x, y: y, width: dot, height: dot)),
                             with: .color((dark ? Color.black : Color.white).opacity(opacity * (0.4 + next() * 0.6))))
            }
        }
        .allowsHitTesting(false)
    }
}
