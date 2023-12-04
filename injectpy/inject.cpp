#include "inject.h"

// Python所在路径
wchar_t pythonPath[MAX_PATH];
// 是否打开控制台
bool isOpenConsole = false;


void SetPythonPath(wchar_t* path) {
	swprintf_s(pythonPath, MAX_PATH, path);
}

void SetOpenConsole(bool isOpen) {
	isOpenConsole = isOpen;
}

void RunPythonConsole() {
	PyExportFuncs dllFunctions;

	initPython(pythonPath, &dllFunctions, isOpenConsole);

	wchar_t exePath[MAX_PATH];
	GetModuleFileNameW(NULL, exePath, MAX_PATH);

	int argc = 1;
	wchar_t* argv[] = { exePath, nullptr };

	dllFunctions.Py_SetPythonHome(pythonPath);
	dllFunctions.Py_SetProgramName(exePath);
	dllFunctions.Py_InitializeEx(1);
	dllFunctions.PyRun_SimpleString("print('load python success!')");
	dllFunctions.Py_Main(argc, argv);
}

void RunPythonFile(wchar_t* pyCodePath) {
	PyExportFuncs dllFunctions;

	initPython(pythonPath, &dllFunctions, isOpenConsole);

	wchar_t exePath[MAX_PATH];
	GetModuleFileNameW(NULL, exePath, MAX_PATH);

	int argc = 2;
	wchar_t* argv[] = { exePath, pyCodePath, nullptr };

	dllFunctions.Py_SetPythonHome(pythonPath);
	dllFunctions.Py_SetProgramName(exePath);
	dllFunctions.Py_InitializeEx(1);

	dllFunctions.PyRun_SimpleString("print('load python success!')");
	dllFunctions.Py_Main(argc, argv);
}