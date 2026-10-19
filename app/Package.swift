// swift-tools-version:5.9
import PackageDescription

let package = Package(
    name: "Knob",
    platforms: [.macOS(.v14)],
    targets: [
        .executableTarget(name: "Knob", path: "Sources/Knob")
    ]
)
