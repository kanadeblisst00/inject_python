#pragma once
#include "pch.h"
#include "tools.h"



extern "C" __declspec(dllexport) void SetPythonPath(wchar_t* path);

extern "C" __declspec(dllexport) void SetOpenConsole(bool isOpen);

extern "C" __declspec(dllexport) void RunPythonConsole();

extern "C" __declspec(dllexport) void RunPythonFile(wchar_t* pyCodePath);