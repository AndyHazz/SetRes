#include <windows.h>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: SetRes.exe \"[target app] [optional flags]\" [width] [height]\n";
        return 1;
    }

    // Parse command-line arguments
    std::string targetApp = argv[1];
    int width = std::stoi(argv[2]);
    int height = std::stoi(argv[3]);

    // Get the original display resolution
    DEVMODE dm;
    dm.dmSize = sizeof(DEVMODE);
    EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm);
    int origWidth = dm.dmPelsWidth;
    int origHeight = dm.dmPelsHeight;

    // Launch the target app
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    std::wstring wideTargetApp(targetApp.begin(), targetApp.end());
    CreateProcess(NULL, const_cast<LPWSTR>(wideTargetApp.c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

    // Change screen resolution
    DEVMODE dm2;
    dm2.dmSize = sizeof(DEVMODE);
    dm2.dmPelsWidth = width;
    dm2.dmPelsHeight = height;
    dm2.dmBitsPerPel = dm.dmBitsPerPel;
    dm2.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
    LONG result = ChangeDisplaySettings(&dm2, CDS_FULLSCREEN);
    if (result != DISP_CHANGE_SUCCESSFUL) {
        std::cerr << "Failed to change display settings: " << result << '\n';
        return 1;
    }

    // Wait for the target app to exit
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Restore original display resolution
    result = ChangeDisplaySettings(&dm, 0);
    if (result != DISP_CHANGE_SUCCESSFUL) {
        std::cerr << "Failed to restore display settings: " << result << '\n';
        return 1;
    }

    // Close process and thread handles.
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}
