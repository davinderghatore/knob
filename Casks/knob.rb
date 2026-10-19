# Homebrew install for Knob, served straight from this repository:
#   brew tap simranxdhindsa/knob https://github.com/simranxdhindsa/knob
#   brew install --cask simranxdhindsa/knob/knob
# It always installs the newest release, and Knob updates itself after that, so this file
# never needs a version bump.
cask "knob" do
  version :latest
  sha256 :no_check

  url "https://github.com/simranxdhindsa/knob/releases/latest/download/Knob-Install.pkg"
  name "Knob"
  desc "System-wide volume, presets, effects and EQ, including for HDMI outputs"
  homepage "https://github.com/simranxdhindsa/knob"

  auto_updates true
  depends_on macos: :sonoma

  pkg "Knob-Install.pkg"

  # Same steps as Uninstall-Knob.command: refuse while Knob's device is the active output,
  # remove the app and the driver, then restart the Mac's audio service.
  uninstall quit:   "io.github.simranxdhindsa.knob",
            script: {
              executable: "/bin/sh",
              args:       ["-c", "/Applications/Knob.app/Contents/Helpers/audio-install-guard && " \
                                 "/bin/rm -rf /Library/Audio/Plug-Ins/HAL/Knob.driver /Applications/Knob.app && " \
                                 "/usr/sbin/pkgutil --forget io.github.simranxdhindsa.knob; " \
                                 "/usr/bin/killall coreaudiod || true"],
              sudo:       true,
            }

  zap trash: [
    "~/Library/Application Support/io.github.simranxdhindsa.knob",
    "~/Library/Caches/io.github.simranxdhindsa.knob",
    "~/Library/HTTPStorages/io.github.simranxdhindsa.knob",
    "~/Library/Preferences/io.github.simranxdhindsa.knob.plist",
  ]
end
