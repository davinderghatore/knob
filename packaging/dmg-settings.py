# dmgbuild settings for the Knob installer window (drag Knob into Applications).
# Used by packaging/build-dmg.sh. Icon positions match the two slots drawn by
# app/Sources/Knob/Views/InstallerBackgroundView.swift (660 x 440 window).
import os.path

app = defines["app"]

files = [app]
symlinks = {"Applications": "/Applications"}
icon = os.path.join(app, "Contents", "Resources", "AppIcon.icns")
badge_icon = None

background = defines["background"]
window_rect = ((240, 160), (660, 440))
default_view = "icon-view"
show_status_bar = False
show_tab_view = False
show_toolbar = False
show_pathbar = False
show_sidebar = False

icon_size = 104
text_size = 13
icon_locations = {
    os.path.basename(app): (170, 206),
    "Applications": (490, 206),
}
arrange_by = None
grid_offset = (0, 0)
label_pos = "bottom"

format = "UDZO"
filesystem = "HFS+"
