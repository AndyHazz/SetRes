// SetRes.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <windows.h>
#include <string>

int wmain(int argc, wchar_t* argv[])
{
    if (argc < 3)
    {
        // The application name or screen resolution is missing
        return 1;
    }

    std::wstring appName = L"";
    for (int i = 1; i < argc - 2; i++)
    {
        appName += argv[i];
        if (i < argc - 3)
        {
            appName += L" ";
        }
    }

    // Parse the screen resolution from the command-line arguments
    int screenWidth = _wtoi(argv[argc - 2]);
    int screenHeight = _wtoi(argv[argc - 1]);

    // Save the current display settings
    DEVMODE currentMode;
    EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &currentMode);

    // Set the new display settings
    DEVMODE newMode;
    ZeroMemory(&newMode, sizeof(newMode));
    newMode.dmSize = sizeof(newMode);
    newMode.dmPelsWidth = screenWidth;
    newMode.dmPelsHeight = screenHeight;
    newMode.dmBitsPerPel = 32;
    newMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

    LONG result = ChangeDisplaySettings(&newMode, CDS_FULLSCREEN);
    if (result != DISP_CHANGE_SUCCESSFUL)
    {
        // Handle the error
        return 1;
    }

    // Launch the application
    STARTUPINFOW startupInfo;
    PROCESS_INFORMATION processInfo;
    ZeroMemory(&startupInfo, sizeof(startupInfo));
    startupInfo.cb = sizeof(startupInfo);
    ZeroMemory(&processInfo, sizeof(processInfo));
    std::wstring commandLine = L"\"" + appName + L"\"";
    BOOL success = CreateProcessW(NULL, const_cast<LPWSTR>(commandLine.c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &startupInfo, &processInfo);
    if (!success)
    {
        // Handle the error
        return 1;
    }

    // Wait for the application to exit
    WaitForSingleObject(processInfo.hProcess, INFINITE);

    // Restore the previous display settings
    result = ChangeDisplaySettings(&currentMode, CDS_FULLSCREEN);
    if (result != DISP_CHANGE_SUCCESSFUL)
    {
        // Handle the error
        return 1;
    }

    return 0;
}
