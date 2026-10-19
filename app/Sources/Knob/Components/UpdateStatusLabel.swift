import SwiftUI

/// What the updater is doing, in the header next to the version: a pulsing orange dot while it
/// checks, downloads or installs, a tick when Knob is up to date, a warning when it failed.
/// Shows nothing when idle.
struct UpdateStatusLabel: View {
    let status: UpdateStatus
    @Environment(\.colorScheme) private var scheme
    @State private var pulse = false

    var body: some View {
        if let full = status.label {
            let p = Palette.of(scheme)
            HStack(spacing: 4) {
                icon
                Text(short ?? full)
                    .font(.system(size: 9.5, weight: .semibold))
                    .foregroundStyle(isFailure ? Palette.accent : p.muted)
                    .lineLimit(1)
                    .truncationMode(.tail)
            }
            .transition(.opacity)
            .help(full)
            .accessibilityElement(children: .ignore)
            .accessibilityLabel(full)
        }
    }

    /// Short forms that fit the header beside the version; the full sentence is the tooltip.
    private var short: String? {
        switch status {
        case .checking: "Checking…"
        case .downloading: "Downloading…"
        case .installing: "Installing…"
        case .upToDate: "Up to date"
        case .failed: "Update failed"
        case .idle: nil
        }
    }

    private var isFailure: Bool {
        if case .failed = status { return true }
        return false
    }

    @ViewBuilder private var icon: some View {
        switch status {
        case .upToDate:
            Image(systemName: "checkmark.circle.fill")
                .font(.system(size: 9, weight: .bold))
                .foregroundStyle(Palette.accent)
        case .failed:
            Image(systemName: "exclamationmark.triangle.fill")
                .font(.system(size: 9, weight: .bold))
                .foregroundStyle(Palette.accent)
        default:
            Circle()
                .fill(Palette.accent)
                .frame(width: 6, height: 6)
                .shadow(color: Palette.accentGlow.opacity(0.9), radius: 3)
                .opacity(pulse ? 1 : 0.35)
                .animation(.easeInOut(duration: 0.7).repeatForever(autoreverses: true), value: pulse)
                .onAppear { pulse = true }
        }
    }
}
