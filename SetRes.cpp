#include <windows.h>
#include <string>
#include <vector>

int wmain(int argc, wchar_t* argv[])
{
    if (argc < 3)
    {
        // The application name or screen resolution is missing
        return 1;
    }

    std::wstring appName = L"";
    std::vector<std::wstring> appArgs;

    // Split the target app name and its arguments
    std::wstring appFullName = argv[1];
    size_t spacePos = appFullName.find_first_of(L' ');
    if (spacePos != std::wstring::npos)
    {
        appName = appFullName.substr(0, spacePos);
        std::wstring appArgsStr = appFullName.substr(spacePos + 1);
        size_t argStart = 0, argEnd = 0;
        while (argEnd != std::wstring::npos)
        {
            argEnd = appArgsStr.find_first_of(L' ', argStart);
            appArgs.push_back(appArgsStr.substr(argStart, argEnd - argStart));
            argStart = appArgsStr.find_first_not_of(L' ', argEnd);
        }
    }
    else
    {
        appName = appFullName;
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
    for (const auto& arg : appArgs)
    {
        commandLine += L" \"" + arg + L"\"";
    }
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
