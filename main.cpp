#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <tchar.h>

#include <exception>

#include "overlay_app.hpp"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dwmapi.lib")

#include "windows_api.hpp"
#include "download.hpp"
#include "offsets.hpp"
#include <avrt.h>
#pragma comment(lib, "Avrt.lib")

extern HANDLE hProcess = nullptr;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int) {
    try {
		// i am ignoring the value of hresult, i dont need it now
		Download::DownloadOffsets();

		if (!Offsets::Load())
		{
			//std::cout << "Failed to load offsets.json\n";
			return 1;
		}


		// HIGH_PRIORITY_CLASS
		SetProcessPriorityBoost(GetCurrentProcess(), FALSE);
		SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS); // risky
		SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);

		DWORD taskIndex = 0;
		HANDLE hTask = AvSetMmThreadCharacteristicsW(L"Playback", &taskIndex);
		if (hTask) {
			AvSetMmThreadPriority(hTask, AVRT_PRIORITY_HIGH);
		}

		SetThreadIdealProcessor(GetCurrentThread(), 3);
		SetPriorityClass(GetCurrentProcess(), PROCESS_MODE_BACKGROUND_END);

        overlay::OverlayApp app{ instance };
        return app.run();
    }
    catch (const std::exception& exception) {
        MessageBoxA(nullptr, exception.what(), "Overlay Error", MB_ICONERROR | MB_OK);
        return 1;
    }
}