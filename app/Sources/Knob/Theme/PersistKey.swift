/// Every persisted setting. Views read and write these only through `@AppStorage`;
/// the model and engine helpers use `UserDefaults` with the same keys.
enum PersistKey: String {
    case selectedTab = "knob.ui.selectedTab"
    case lastPreset = "knob.ui.lastPreset"
    /// Knob's volume while it was the output, for outputs without their own volume (HDMI).
    case lastVolume = "knob.audio.lastVolume"
    /// The physical output Knob raised to 100% underneath itself, until it is given back.
    case raisedOutputUID = "knob.audio.raisedOutputUID"
}
