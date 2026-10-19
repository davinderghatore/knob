import AppKit
import SwiftUI

/// A hardware-style rotary knob: recessed well, shadowed skirt, lit cap, pointer and a glowing
/// value arc. Drag up/down (or sideways) to turn it. Double-click resets to `defaultValue`.
/// Outside changes (for example the Mac volume keys) glide in with a spring; drags follow 1:1.
struct RealisticKnob: View {
    @Binding var value: Double
    var size: CGFloat
    var defaultValue: Double? = nil
    var showsMinMax = true
    var accessibilityLabel: String
    var accessibilityValue: String
    var onEditingChanged: (Bool) -> Void = { _ in }

    @Environment(\.colorScheme) private var scheme
    @Environment(\.isEnabled) private var isEnabled
    @State private var dragStart: Double?

    private static let sweep = 270.0

    var body: some View {
        let p = Palette.of(scheme)
        let v = min(max(value, 0), 1)
        VStack(spacing: size * 0.01) {
            ZStack {
                arc(p, v)
                well(p)
                skirt(p)
                cap(p)
                pointer(p, v)
            }
            .frame(width: size, height: size)
            .contentShape(Circle())
            .gesture(drag)
            .onTapGesture(count: 2) { if let d = defaultValue { set(d, animated: true) } }
            .animation(dragStart == nil ? Motion.knob : nil, value: value)
            .opacity(isEnabled ? 1 : 0.45)

            if showsMinMax {
                HStack {
                    Text("MIN")
                    Spacer()
                    Text("MAX")
                }
                .font(.system(size: max(7, size * 0.065), weight: .semibold))
                .tracking(0.6)
                .foregroundStyle(p.muted)
                .frame(width: size * 0.92)
                .offset(y: -size * 0.1)
            }
        }
        .accessibilityElement()
        .accessibilityLabel(accessibilityLabel)
        .accessibilityValue(accessibilityValue)
        .accessibilityAdjustableAction { direction in
            set(value + (direction == .increment ? 0.05 : -0.05), animated: true)
        }
    }

    // MARK: Layers

    private func arc(_ p: Palette, _ v: Double) -> some View {
        let fraction = Self.sweep / 360
        let width = max(2, size * 0.028)
        return ZStack {
            Circle().trim(from: 0, to: fraction)
                .stroke(p.faint.opacity(0.35), style: StrokeStyle(lineWidth: width, lineCap: .round))
            Circle().trim(from: 0, to: fraction * v)
                .stroke(Palette.accent, style: StrokeStyle(lineWidth: width, lineCap: .round))
                .shadow(color: Palette.accentGlow.opacity(isEnabled ? 0.9 : 0), radius: width * 1.6)
        }
        .rotationEffect(.degrees(90 + (360 - Self.sweep) / 2))
        .padding(width / 2)
    }

    private func well(_ p: Palette) -> some View {
        Circle()
            .fill(LinearGradient(colors: [p.knobWellTop, p.knobWellBottom], startPoint: .top, endPoint: .bottom))
            .overlay(Circle().stroke(Color.black.opacity(0.35), lineWidth: 1).blur(radius: 1).clipShape(Circle()))
            .frame(width: size * 0.86, height: size * 0.86)
    }

    private func skirt(_ p: Palette) -> some View {
        let d: CGFloat = size * 0.77
        let outline: CGFloat = max(1, size * 0.012)
        let body = LinearGradient(colors: [p.knobBodyTop, p.knobBodyBottom],
                                  startPoint: .topLeading, endPoint: .bottomTrailing)
        let farShadow: CGFloat = size * 0.06
        let nearShadow: CGFloat = size * 0.015
        return Circle()
            .fill(body)
            .overlay(Circle().stroke(p.knobOutline, lineWidth: outline))
            .frame(width: d, height: d)
            .shadow(color: p.dropShadow, radius: farShadow, x: 0, y: size * 0.055)
            .shadow(color: p.dropShadow.opacity(0.5), radius: nearShadow, x: 0, y: size * 0.01)
    }

    private func cap(_ p: Palette) -> some View {
        let d = size * 0.66
        return ZStack {
            Circle().fill(RadialGradient(colors: [p.knobCapTop, p.knobCapBottom],
                                         center: UnitPoint(x: 0.35, y: 0.28),
                                         startRadius: 0, endRadius: d * 0.75))
            if p.knobSheen > 0 {
                Circle().fill(AngularGradient(colors: [.white, .clear, .white, .clear, .white, .clear, .white],
                                              center: .center))
                    .opacity(p.knobSheen)
                    .blendMode(.overlay)
            }
            Grain(opacity: p.grain * 0.8).clipShape(Circle())
            Circle().stroke(LinearGradient(colors: [Color.white.opacity(scheme == .dark ? 0.22 : 0.9), .clear],
                                           startPoint: .top, endPoint: .center), lineWidth: 1)
            Circle().stroke(LinearGradient(colors: [.clear, Color.black.opacity(0.25)],
                                           startPoint: .center, endPoint: .bottom), lineWidth: 1)
        }
        .frame(width: d, height: d)
    }

    private func pointer(_ p: Palette, _ v: Double) -> some View {
        Capsule()
            .fill(p.knobIndicator)
            .frame(width: max(2, size * 0.032), height: size * 0.13)
            .shadow(color: scheme == .dark ? Color.white.opacity(0.35) : .clear, radius: 2)
            .offset(y: -size * 0.215)
            .rotationEffect(.degrees(-Self.sweep / 2 + Self.sweep * v))
    }

    // MARK: Interaction

    private var drag: some Gesture {
        DragGesture(minimumDistance: 0)
            .onChanged { g in
                if dragStart == nil { dragStart = value; onEditingChanged(true) }
                let travel = (-g.translation.height + g.translation.width * 0.6) / max(160, size * 1.8)
                set((dragStart ?? value) + travel, animated: false)
            }
            .onEnded { _ in dragStart = nil; onEditingChanged(false) }
    }

    private func set(_ newValue: Double, animated: Bool) {
        let clamped = min(max(newValue, 0), 1)
        if (value > 0 && clamped == 0) || (value < 1 && clamped == 1) {
            NSHapticFeedbackManager.defaultPerformer.perform(.alignment, performanceTime: .now)
        }
        var transaction = Transaction(animation: animated ? Motion.knob : nil)
        transaction.disablesAnimations = !animated
        withTransaction(transaction) { value = clamped }
    }
}
