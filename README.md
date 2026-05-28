# Zephyr Course Workspace

This is the **west workspace root** for the Zephyr RTOS course targeting the ESP32-DevKitC board.

---

## Workspace Layout

```
C:\Users\hazem\Desktop\Zephyr\          ← west workspace root (.west/config lives here)
├── zephyr-course\                       ← west manifest repo  ← run west commands from here
│   ├── west.yml
│   ├── build\                           ← CMake build output (generated, not in Git)
│   └── .venv\                           ← Python virtual environment
├── deps\
│   ├── zephyr\                          ← Zephyr kernel (4.2.0)
│   └── modules\hal\espressif\           ← ESP32 HAL + esptool
├── walkthrough\                         ← lecture demo apps (source, under Git)
└── Homework\                            ← homework apps (source, under Git)
```

**SDK location:** `C:\Users\hazem\zephyr-sdk-0.17.2`  
**Target board:** `esp32_devkitc/esp32/procpu`

---

## One-time Environment Setup

All commands below are run in **PowerShell** from `C:\Users\hazem\Desktop\Zephyr\zephyr-course`.

### 1 — Activate the Python virtual environment

```powershell
cd C:\Users\hazem\Desktop\Zephyr\zephyr-course
.venv\Scripts\Activate.ps1
```

> If activation is blocked by execution policy, run once:
> ```powershell
> Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
> ```

### 2 — Verify west is available

```powershell
west --version   # should print west version 1.5.0
```

---

## Building an App

All builds are run **from the `zephyr-course` directory** with the venv active.

### General build command

```powershell
west build -p always -b esp32_devkitc/esp32/procpu "<path-to-app>" `
  -- -DDTC_OVERLAY_FILE="<path-to-app>\esp32_led.overlay"
```

- `-p always` cleans the build directory before building (pristine build).
- `-b esp32_devkitc/esp32/procpu` selects the ESP32 board target.
- `-- -DDTC_OVERLAY_FILE=...` applies the custom LED pin overlay (only needed when the app has one).

Build output is written to `zephyr-course\build\`.

### Flash to the board

```powershell
west flash --esp-device COM7 --esp-tool "C:/Users/hazem/Desktop/Zephyr/zephyre_workspace/esptool_noreset.py"
```

### Monitor serial output

```powershell
west espressif monitor -p COM7
```

---

## App Directories

| Directory | Purpose |
|---|---|
| [`../walkthrough/`](../walkthrough/README.md) | Lecture demo apps — safe sandbox for trying things out |
| [`../Homework/`](../Homework/README.md) | Homework tasks — one folder per lecture |

---

## Reference

- [Zephyr Getting Started Guide](https://docs.zephyrproject.org/latest/develop/getting_started/index.html)
- [ESP32-DevKitC board page](https://docs.zephyrproject.org/latest/boards/espressif/esp32_devkitc/doc/index.html)
- [west build reference](https://docs.zephyrproject.org/latest/develop/west/build-flash-debug.html)
