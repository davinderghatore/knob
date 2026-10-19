import SwiftUI

/// Knob's one dropdown: a native `Menu` dressed as a recessed hardware pill.
/// Use it for every choice list (presets, outputs). Never build another dropdown.
struct PillMenu<Item: Hashable>: View {
    let items: [Item]
    let selection: Item?
    let title: (Item) -> String
    let placeholder: String
    let onSelect: (Item) -> Void

    @Environment(\.colorScheme) private var scheme

    var body: some View {
        let p = Palette.of(scheme)
        Menu {
            ForEach(items, id: \.self) { item in
                Button { onSelect(item) } label: {
                    if item == selection { Label(title(item), systemImage: "checkmark") } else { Text(title(item)) }
                }
            }
        } label: {
            HStack(spacing: 6) {
                Image(systemName: "chevron.down").font(.system(size: 8, weight: .bold))
                Text(selection.map(title) ?? placeholder)
                    .font(.system(size: 11, weight: .medium))
                    .lineLimit(1)
                    .truncationMode(.tail)
                Spacer(minLength: 0)
            }
            .foregroundStyle(p.label)
            .padding(.horizontal, 10)
            .frame(height: 24)
            .background(RecessedPill())
            .contentShape(Rectangle())
        }
        .menuStyle(.button)
        .buttonStyle(.plain)
        .menuIndicator(.hidden)
    }
}

/// The recessed rounded-rectangle used behind pills and small hardware buttons.
struct RecessedPill: View {
    var cornerRadius: CGFloat = 7
    @Environment(\.colorScheme) private var scheme

    var body: some View {
        let p = Palette.of(scheme)
        let shape = RoundedRectangle(cornerRadius: cornerRadius, style: .continuous)
        shape.fill(p.pillFill)
            .overlay(shape.stroke(p.pillStroke, lineWidth: 1))
            .overlay(shape.stroke(LinearGradient(colors: [Color.black.opacity(scheme == .dark ? 0.35 : 0.08), .clear],
                                                 startPoint: .top, endPoint: .center), lineWidth: 1))
    }
}

/// A small square hardware button with an SF Symbol, matching the pills.
struct HardwareIconButton: View {
    let systemImage: String
    let label: String
    let action: () -> Void
    @Environment(\.colorScheme) private var scheme

    var body: some View {
        Button(action: action) {
            Image(systemName: systemImage)
                .font(.system(size: 10, weight: .semibold))
                .foregroundStyle(Palette.of(scheme).label)
                .frame(width: 24, height: 24)
                .background(RecessedPill())
                .contentShape(Rectangle())
        }
        .buttonStyle(.plain)
        .accessibilityLabel(label)
    }
}
