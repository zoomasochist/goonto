# Goonto

Install Goonto.

## Features

| **Feature**         | **Edgeware** | **Goonto** |
| ------------------- | ------------ | ---------- |
| Linux/macOS support |      ❌      |     ✅†    |
| Ezpz to install     |      ❌      |     ✅     |
| Blazing fast 🚀🚀🚀 |      ❌      |     ✅     |
| Popups follow cursor|      ❌      |     ✅     |
| Notification spam   |      ❌      |     ✅     |
| Clipboard replace   |      ❌      |     ✅     |
| Auto-type           |      ❌      |     ✅     |
| Website opener      |      ✅      |     ✅     |
| Popup opacity       |      ✅      |     ✅     |
| Popups              |      ✅      |     ✅     |
| Video popups        |      ✅      |     ✅     |
| Popup mitosis       |      ✅      |     ✅     |
| Popup censoring     |      ✅      |     ✅     |
| Audio               |      ✅      |     ✅     |
| Prompts             |      ✅      |     ❌     |
| "Hibernate" mode    |      ✅      |     ❌     |
| Panic button        |      ✅      |     ❌     |
| Lowkey mode         |      ✅      |     ❌     |
| Booru downloader    |      ✅      |     ❌     |
| GUI configuration   |      ✅      |     ❌     |
| Discord activity    |      ✅      |     ❌     |
| Drive filler        |      ✅      |     ❌*    |
| Wallpaper changer   |      ✅      |     ❌*    |
| Walltaker support   |      ❌      |     ❌     |
| Background hypno    |      ❌      |     ❌     |
| Block opening Task Manager |      ❌      |     ❌     |

† = "Linux support" means X11. Wayland does not allow programs to control the position
of their windows, so Goonto etc. will never work.

\* = Unlikely to be implemented. Presume everything else marked ❌ is in the pipeline.

## Building

- Install dependencies
- `make`
- ???
- Profit!!!

### Dependencies

- wxWidgets
- libvlc
- nlohmann_json

#### Linux

- wxGTK3
- gtk3
- xtst
- libx11