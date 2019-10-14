#pragma once
#include <Windows.h>
#include <string>

static std::string getExePathA(uint32_t pathLength = MAX_PATH) {
	std::string path(pathLength, '\0');
	GetModuleFileNameA(nullptr, path.data(), pathLength);
	path.resize(path.rfind('\\'));
	path.shrink_to_fit();
	return path;
}
static std::wstring getExePathW(uint32_t pathLength = MAX_PATH) {
	std::wstring path(pathLength, L'\0');
	GetModuleFileNameW(nullptr, path.data(), pathLength);
	path.resize(path.rfind('\\'));
	path.shrink_to_fit();
	return path;
}

#ifdef UNICODE
#define getExePath  getExePathW
#else
#define getExePath  getExePathA
#endif // !UNICODE