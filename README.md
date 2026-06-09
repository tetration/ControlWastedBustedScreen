# ControlWastedBustedScreen

A small ASI plugin for **GTA San Andreas classic PC** that lets you customize how long the **WASTED** and **BUSTED** screens/transitions last.

This was built for the classic 32-bit PC version of GTA SA, especially the commonly modded **1.0 US executable**.

## Files

- `ControlWastedBustedScreen.asi` - the plugin.
- `ControlWastedBustedScreen.ini` - configuration file.
- `ControlWastedBustedScreen.log` - generated when the plugin loads.

## Installation

Copy these files into your GTA San Andreas folder, next to `gta_sa.exe`:

```txt
ControlWastedBustedScreen.asi
ControlWastedBustedScreen.ini
```

You also need an ASI loader such as Ultimate ASI Loader or Silent's ASI Loader.

## Configuration

Edit `ControlWastedBustedScreen.ini`:

```ini
[Settings]
BustedTextMs=1000
BustedFadeStartMs=500
BustedFadeWindowMs=500
BustedFinishMs=1000

WastedTextMs=1000
WastedFadeStartMs=500
WastedFadeWindowMs=500
```

All values are in milliseconds.

Examples:

```txt
500  = half a second
1000 = one second
3000 = three seconds
5000 = five seconds
```

## Original approximate timings

```txt
BUSTED text:        5000 ms
BUSTED fade start:  3000 ms
BUSTED fade window: 3000 ms
BUSTED finish:      4000 ms

WASTED text:        4000 ms
WASTED fade start:  3000 ms
WASTED fade window: 3000 ms
```

## Building locally with MSYS2 MinGW32

Install the 32-bit GCC toolchain:

```bash
pacman -S mingw-w64-i686-gcc
```

Build:

```powershell
gcc32 -shared -O2 -o ControlWastedBustedScreen.asi src\ControlWastedBustedScreen.c
```

Or directly:

```powershell
C:\msys64\mingw32\bin\gcc.exe -shared -O2 -o ControlWastedBustedScreen.asi src\ControlWastedBustedScreen.c
```

Verify it is 32-bit:

```powershell
C:\msys64\mingw32\bin\objdump.exe -f ControlWastedBustedScreen.asi
```

Expected:

```txt
file format pei-i386
architecture: i386
```

## GitHub Actions build

This repo includes a workflow that builds the ASI on Windows and uploads a zipped artifact containing:

```txt
ControlWastedBustedScreen.asi
ControlWastedBustedScreen.ini
README.txt
```

## Notes

This plugin patches hardcoded addresses in `gta_sa.exe`. It may not work on every GTA SA executable version.

Known target:

```txt
GTA San Andreas classic PC 1.0 US
```
