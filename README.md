# SetRes
App to set a custom display resolution for full screen apps before launching them

Disclaimer:  App written with lots of help from ChatGPT! I don't know how to code C++

This was created as a useful companion app for ScummVM (which runs in fullscreen at native res by default, causing cursor lag and high GPU usage on high dpi screens), but could be useful for other applications ... 

It switches screen res, runs scumm (or other app passed as a command), and restores native res when the target app closes.

Put the app in the same folder as scummvm.exe and run it with e.g. `.\SetRes.exe "scummvm.exe" 1920 1080`
