#pragma once
#include "pch.h"
#include <filesystem>
#include <regex>
#include <Shlwapi.h>


#pragma comment(lib, "Shlwapi.lib")

std::wstring findPythonDLL(const std::wstring& path);

DWORD WINAPI run_python(LPVOID lpParam);