#pragma once
#include <Windows.h>
#include <urlmon.h>
//#include <iostream>

#pragma comment(lib, "urlmon.lib")

namespace Download
{
	const char* url = "https://raw.githubusercontent.com/a2x/cs2-dumper/main/output/offsets.json";
	const char* outputPath = "offsets.json";

	HRESULT DownloadOffsets()
	{
		HRESULT hr = URLDownloadToFileA(nullptr, url, outputPath, 0, nullptr);
		if (SUCCEEDED(hr)) {
			//std::cout << "File downloaded successfully." << std::endl;
		}
		else {
			//std::cerr << "Failed to download file. Error code: " << hr << std::endl;
		}
		return hr;
	}
}