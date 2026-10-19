import Foundation

/// A `.fac` preset file. Its display name is line 3 of the file, as the preset format defines.
struct Preset: Hashable, Identifiable {
    let name: String
    let url: URL
    var id: URL { url }

    /// Factory presets shipped in the app bundle, in their numbered order (Default first).
    static func factory() -> [Preset] {
        guard let folder = Bundle.main.resourceURL?.appendingPathComponent("presets"),
              let files = try? FileManager.default.contentsOfDirectory(at: folder, includingPropertiesForKeys: nil)
        else { return [] }
        return files.filter { $0.pathExtension == "fac" }
            .sorted { order($0) < order($1) }
            .compactMap(load)
    }

    private static func order(_ url: URL) -> Int {
        let stem = url.deletingPathExtension().lastPathComponent
        return stem == "Default" ? 0 : Int(stem) ?? 1000
    }

    private static func load(_ url: URL) -> Preset? {
        guard let size = try? url.resourceValues(forKeys: [.fileSizeKey]).fileSize, size < 1 << 20,
              let text = try? String(contentsOf: url, encoding: .utf8) else { return nil }
        let lines = text.split(separator: "\n", omittingEmptySubsequences: false)
        guard lines.count > 2, lines[0].hasPrefix("CLASS1") else { return nil }
        let name = lines[2].trimmingCharacters(in: .whitespacesAndNewlines)
        return name.isEmpty ? nil : Preset(name: name, url: url)
    }
}
