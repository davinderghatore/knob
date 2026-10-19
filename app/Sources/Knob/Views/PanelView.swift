import SwiftUI

/// The whole menu bar panel: a hardware faceplate holding every section.
struct PanelView: View {
    @ObservedObject var model: KnobModel
    @Environment(\.colorScheme) private var scheme

    static let width: CGFloat = 384

    var body: some View {
        let p = Palette.of(scheme)
        VStack(spacing: 14) {
            HeaderBar(model: model)
            PresetBar(model: model)
            VolumeSection(model: model)
            ScreenSection(model: model)
        }
        .padding(.horizontal, 16)
        .padding(.top, 14)
        .padding(.bottom, 18)
        .frame(width: Self.width)
        .background {
            ZStack {
                LinearGradient(colors: [p.panelTop, p.panelBottom], startPoint: .top, endPoint: .bottom)
                Grain(opacity: p.grain)
                VStack {
                    Rectangle().fill(p.panelHighlight).frame(height: 1)
                    Spacer()
                }
            }
        }
        .redacted(reason: model.connection == .connecting ? .placeholder : [])
        .onAppear { model.panelOpened() }
        .onDisappear { model.panelClosed() }
    }
}

/// Small uppercase engraved label used above every control.
struct EngravedLabel: View {
    let text: String
    var size: CGFloat = 11
    @Environment(\.colorScheme) private var scheme

    var body: some View {
        Text(text)
            .font(.system(size: size, weight: .semibold))
            .tracking(0.9)
            .foregroundStyle(Palette.of(scheme).label)
            .shadow(color: scheme == .dark ? .black.opacity(0.5) : .white.opacity(0.9), radius: 0, y: 1)
    }
}
