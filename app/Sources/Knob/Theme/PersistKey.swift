/// Every persisted UI setting. Views read and write these only through `@AppStorage`.
enum PersistKey: String {
    case selectedTab = "knob.ui.selectedTab"
}
