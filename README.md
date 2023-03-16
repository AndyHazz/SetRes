# SetRes
SetRes is a small application that allows you to launch a target application with the specified resolution. Once the target application is closed, SetRes restores the original display resolution.

Disclaimer:  App written with lots of help from ChatGPT! I don't know how to code C++

This was created as a useful companion app for ScummVM (which runs in fullscreen at native res by default, causing cursor lag and high GPU usage on high dpi screens), but could be useful for other applications ... 

Usage
To use SetRes, put it in the same directory as the target application. Open a command prompt and navigate to the directory containing the SetRes.exe executable. Then run the following command:

```SetRes.exe [target app] [width] [height]```

Replace [target app] with the name of the target application including any necessary options (e.g. "scummvm.exe -f"), and replace [width] [height] with the desired resolution (e.g. 1280 720).

Example
To launch ScummVM in fullscreen mode with a resolution of 1280x720, run the following command:

```SetRes.exe "scummvm.exe -f" 1280 720```
