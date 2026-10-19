import SwiftUI

/// The background of the Knob installer window (the .dmg). Finder draws the Knob and
/// Applications icons on top, centred in the two recessed slots: see packaging/dmg-settings.py
/// for the matching icon positions. Light faceplate so Finder's dark icon labels stay readable.
struct InstallerBackgroundView: View {
    static let size = CGSize(width: 660, height: 440)
    /// Icon centres in the window, shared with the dmg layout.
    static let appCentre = CGPoint(x: 170, y: 212)
    static let applicationsCentre = CGPoint(x: 490, y: 212)

    var body: some View {
        let p = Palette.light
        ZStack(alignment: .topLeading) {
            LinearGradient(colors: [p.panelTop, p.panelBottom], startPoint: .top, endPoint: .bottom)
            Grain(opacity: p.grain)

            HStack(alignment: .firstTextBaseline) {
                Text("knob")
                    .font(.system(size: 26, weight: .heavy, design: .rounded))
                    .foregroundStyle(p.label)
                    .shadow(color: .white, radius: 0, y: 1)
                Spacer()
                Text("INSTALL")
                    .font(.system(size: 10, weight: .bold)).tracking(1.4)
                    .foregroundStyle(Palette.accent)
            }
            .padding(.horizontal, 34)
            .padding(.top, 26)

            VStack(spacing: 4) {
                Text("Drag Knob into Applications")
                    .font(.system(size: 17, weight: .semibold))
                    .foregroundStyle(p.label)
                Text("Studio sound for every output on your Mac.")
                    .font(.system(size: 11.5, weight: .medium))
                    .foregroundStyle(p.muted)
            }
            .frame(width: Self.size.width)
            .offset(y: 70)

            slot(at: Self.appCentre)
            slot(at: Self.applicationsCentre)
            arrow

            nextStep
                .frame(width: 580, height: 58)
                .offset(x: 40, y: 340)

            Text("Designed and built by Simran Dhindsa")
                .font(.system(size: 9.5, weight: .medium))
                .foregroundStyle(p.faint)
                .frame(width: Self.size.width)
                .offset(y: 412)
        }
        .frame(width: Self.size.width, height: Self.size.height)
        .environment(\.colorScheme, .light)
    }

    /// A recessed well the Finder icon and its label sit in.
    private func slot(at centre: CGPoint) -> some View {
        RecessedPill(cornerRadius: 22)
            .frame(width: 168, height: 176)
            .position(x: centre.x, y: centre.y + 16)
    }

    private var arrow: some View {
        let y = Self.appCentre.y - 4
        let start = Self.appCentre.x + 100, end = Self.applicationsCentre.x - 100
        return ZStack {
            Path { path in
                path.move(to: CGPoint(x: start, y: y))
                path.addLine(to: CGPoint(x: end - 4, y: y))
            }
            .stroke(Palette.accent, style: StrokeStyle(lineWidth: 3, lineCap: .round, dash: [2, 9]))
            Path { path in
                path.move(to: CGPoint(x: end - 16, y: y - 12))
                path.addLine(to: CGPoint(x: end, y: y))
                path.addLine(to: CGPoint(x: end - 16, y: y + 12))
            }
            .stroke(Palette.accent, style: StrokeStyle(lineWidth: 3.5, lineCap: .round, lineJoin: .round))
        }
        .shadow(color: Palette.accentGlow.opacity(0.7), radius: 6)
    }

    /// The dark analyser-style strip explaining what happens after the drag.
    private var nextStep: some View {
        let shape = RoundedRectangle(cornerRadius: 14, style: .continuous)
        return HStack(spacing: 12) {
            AppIconView(size: 40)
            VStack(alignment: .leading, spacing: 2) {
                Text("Then open Knob and turn it on.")
                    .font(.system(size: 12, weight: .semibold))
                    .foregroundStyle(Screen.text)
                Text("The first time, it asks for your password once to install its audio device.")
                    .font(.system(size: 10.5))
                    .foregroundStyle(Screen.textSecondary)
            }
            Spacer(minLength: 0)
        }
        .padding(.horizontal, 12)
        .background(shape.fill(LinearGradient(colors: [Screen.top, Screen.bottom], startPoint: .top, endPoint: .bottom)))
        .overlay(shape.stroke(Screen.edge, lineWidth: 1.5))
        .overlay(shape.inset(by: 1.5).stroke(Screen.highlight, lineWidth: 0.5))
    }
}
