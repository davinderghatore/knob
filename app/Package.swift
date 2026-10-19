// swift-tools-version:5.9
import PackageDescription

let package = Package(
    name: "Knob",
    platforms: [.macOS(.v14)],
    dependencies: [
        // Auto-updates from GitHub Releases. build-app.sh copies Sparkle.framework into the app.
        .package(url: "https://github.com/sparkle-project/Sparkle", exact: "2.10.0")
    ],
    targets: [
        .executableTarget(
            name: "Knob",
            dependencies: [.product(name: "Sparkle", package: "Sparkle")],
            path: "Sources/Knob",
            linkerSettings: [.unsafeFlags(["-Xlinker", "-rpath", "-Xlinker", "@executable_path/../Frameworks"])]
        )
    ]
)
