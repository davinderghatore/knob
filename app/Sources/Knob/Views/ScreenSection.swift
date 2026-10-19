import SwiftUI

/// The dark analyser screen: Effects and Equalizer tabs over the live spectrum.
struct ScreenSection: View {
    @ObservedObject var model: KnobModel
    @AppStorage(PersistKey.selectedTab.rawValue) private var tab = Tab.effects

    enum Tab: String, CaseIterable { case effects, equalizer }

    var body: some View {
        InsetScreen(glow: model.powerOn ? LevelMeter.position(max(model.peakLeft, model.peakRight)) : 0) {
            VStack(spacing: 10) {
                SegmentedTabs(options: Tab.allCases, selection: $tab) { $0 == .effects ? "EFFECTS" : "EQUALIZER" }
                ZStack {
                    SpectrumView(bands: model.spectrum, showsGrid: tab == .equalizer,
                                 intensity: tab == .effects ? 0.45 : 0.8)
                        .opacity((model.powerOn ? 1 : 0.35) * (tab == .equalizer ? 0.45 : 1))
                    if tab == .effects {
                        EffectsGrid(model: model)
                    } else {
                        EqualizerBank(model: model)
                    }
                }
                .frame(height: 176)
                .disabled(model.connection != .online)
                if let notice = model.notice {
                    statusNote(notice, attention: !model.starting)
                } else if model.connection == .offline {
                    statusNote("Sound engine is off. Turn Knob on to start it.", attention: false)
                }
            }
        }
        .environment(\.colorScheme, .dark)
    }

    /// Shown under the controls (never instead of them): engine status, or why it couldn't start.
    private func statusNote(_ text: String, attention: Bool) -> some View {
        HStack(alignment: .firstTextBaseline, spacing: 6) {
            Circle().fill(attention ? Palette.accent : Screen.textTertiary).frame(width: 5, height: 5)
            Text(text)
                .font(.system(size: 10, weight: .medium))
                .fixedSize(horizontal: false, vertical: true)
        }
        .foregroundStyle(attention ? Screen.text : Screen.textSecondary)
        .frame(maxWidth: .infinity, alignment: .leading)
        .accessibilityElement(children: .combine)
    }
}

/// Five small knobs, one per effect.
struct EffectsGrid: View {
    @ObservedObject var model: KnobModel

    var body: some View {
        HStack(spacing: 0) {
            ForEach(EffectSlot.allCases) { slot in
                VStack(spacing: 2) {
                    Text(slot.title.uppercased())
                        .font(.system(size: 8.5, weight: .semibold)).tracking(0.6)
                        .foregroundStyle(Screen.text)
                        .multilineTextAlignment(.center)
                        .lineLimit(2)
                        .frame(width: 62, height: 22, alignment: .bottom)
                    RealisticKnob(value: Binding(get: { model.effects[slot.rawValue] }, set: { model.setEffect(slot, $0) }),
                                  size: 54, defaultValue: 0, showsMinMax: false,
                                  accessibilityLabel: slot.title,
                                  accessibilityValue: "\(Int((model.effects[slot.rawValue] * 10).rounded())) of 10",
                                  onEditingChanged: model.editingChanged)
                    Text("\(Int((model.effects[slot.rawValue] * 10).rounded()))")
                        .font(.system(size: 10, weight: .semibold).monospacedDigit())
                        .foregroundStyle(Palette.accent)
                        .contentTransition(.numericText(value: model.effects[slot.rawValue]))
                }
                .frame(maxWidth: .infinity)
            }
        }
        .padding(.top, 10)
        .frame(maxHeight: .infinity, alignment: .top)
    }
}

/// One fader per EQ band, over the live spectrum.
struct EqualizerBank: View {
    @ObservedObject var model: KnobModel
    private let faderHeight: CGFloat = 128

    /// Placeholder faders shown (disabled) until the engine reports its real bands.
    private static let skeletonBands = 10

    var body: some View {
        if model.eq.isEmpty { skeleton } else { bank }
    }

    private var skeleton: some View {
        HStack(spacing: 0) {
            ForEach(0..<Self.skeletonBands, id: \.self) { _ in
                HardwareFader(value: .constant(0), height: faderHeight, accessibilityLabel: "Equalizer band",
                              accessibilityValue: "Unavailable")
                    .frame(maxWidth: .infinity)
            }
        }
        .padding(.bottom, 17)
        .frame(maxHeight: .infinity, alignment: .bottom)
        .disabled(true)
    }

    private var bank: some View {
        VStack(spacing: 8) {
            HStack(spacing: 0) {
                ForEach(model.eq.indices, id: \.self) { band in
                    HardwareFader(value: Binding(get: { model.eq[band] }, set: { model.setEQ(band: band, $0) }),
                                  height: faderHeight,
                                  accessibilityLabel: "\(Self.label(model.frequencies[safe: band])) hertz",
                                  accessibilityValue: String(format: "%+.1f dB", model.eq[band] * 12),
                                  onEditingChanged: model.editingChanged)
                        .frame(maxWidth: .infinity)
                }
            }
            HStack(spacing: 0) {
                ForEach(model.eq.indices, id: \.self) { band in
                    Text(Self.label(model.frequencies[safe: band]))
                        .font(.system(size: 7.5, weight: .medium).monospacedDigit())
                        .foregroundStyle(Screen.textTertiary)
                        .frame(maxWidth: .infinity)
                }
            }
        }
        .frame(maxHeight: .infinity, alignment: .bottom)
    }

    static func label(_ hertz: Double?) -> String {
        guard let hertz else { return "" }
        return hertz >= 1000 ? "\(Int((hertz / 1000).rounded()))K" : "\(Int(hertz.rounded()))"
    }
}

extension Array {
    subscript(safe index: Int) -> Element? { indices.contains(index) ? self[index] : nil }
}
