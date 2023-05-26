# Goonto

Install Goonto.

## Features

| **Feature**         | **Edgeware** | **Goonto** |
| Linux/macOS support |      ❌†     |     ✅     |
| Blazing fast 🚀🚀🚀 |      ❌      |     ✅     |
| Website opener      |      ✅      |     ✅     |
| Popups              |      ✅      |     ✅     |
| Popup mitosis       |      ✅      |     ❌     |
| Audio               |      ✅      |     ❌     |
| Wallpaper changer   |      ✅      |     ❌     |
| Prompts             |      ✅      |     ❌     |
| Drive filler        |      ✅      |     ❌*    |
| "Hibernate" mode    |      ✅      |     ❌     |
| Panic button        |      ✅      |     ❌     |
| Booru downloader    |      ✅      |     ❌     |
| GUI configuration   |      ✅      |     ❌     |
| Popup opacity       |      ✅      |     ❌     |
| Discord activity    |      ✅      |     ❌     |
| Popup censoring     |      ✅      |     ❌     |
| Lowkey mode         |      ❌      |     ❌     |
| Walltaker support   |      ❌      |     ❌     |
| Notification spam   |      ❌      |     ❌     |
| Auto-type           |      ❌      |     ❌     |

† = "Linux support" means X11. Wayland does not allow programs to control the position
of their windows, so Goonto etc. will never work.
\* = Unlikely to be implemented. Presume everything else
marked ❌ is in the pipeline.

## Issues

- Not multi-monitor aware
    - Should be pretty simple to implement

## Building

- Install wxWidgets and nlohmann_json
- `make`
- ???
- Profit!!!