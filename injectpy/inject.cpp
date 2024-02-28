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

bool AddDirToSysPath(PyExportFuncs* dllFunctions, wchar_t* subPath) {
	wchar_t packagePath[MAX_PATH];
	PathCombineW(packagePath, pythonPath, subPath);
	void* sys_module = dllFunctions->PyImport_ImportModule("sys");
	if (sys_module == nullptr) {
		dllFunctions->PyErr_Print();
		return false; 
	}
	void* sys_path = dllFunctions->PyObject_GetAttrString(sys_module, "path");
	if (sys_path == nullptr) {
		dllFunctions->PyErr_Print();
		dllFunctions->Py_DECREF(sys_module);
		return false; 
	}
	void* py_site_packages_path = dllFunctions->PyUnicode_FromWideChar(packagePath, wcslen(packagePath));
	if (py_site_packages_path == nullptr) {
		dllFunctions->PyErr_Print();
		dllFunctions->Py_DECREF(sys_path);
		dllFunctions->Py_DECREF(sys_module);
		return false;
	}
	if (dllFunctions->PyList_Append(sys_path, py_site_packages_path) != 0) {
		dllFunctions->PyErr_Print();
	}
	//dllFunctions->Py_DECREF(py_site_packages_path);
	//dllFunctions->Py_DECREF(sys_path);
	//dllFunctions->Py_DECREF(sys_module);
	return true;
}


void AddSitePackagesToPath(PyExportFuncs* dllFunctions) {
	wchar_t* path1 = (wchar_t*)L"lib\\site-packages";
	wchar_t* path2 = (wchar_t*)L"lib\\site-packages\\win32";
	wchar_t* path3 = (wchar_t*)L"lib\\site-packages\\win32\\lib";
	wchar_t* path4 = (wchar_t*)L"lib\\site-packages\\Pythonwin";

	AddDirToSysPath(dllFunctions, path1);
	AddDirToSysPath(dllFunctions, path2);
	AddDirToSysPath(dllFunctions, path3);
	AddDirToSysPath(dllFunctions, path4);
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
	AddSitePackagesToPath(&dllFunctions);
	dllFunctions.PyRun_SimpleString("print('load python success!')");
	dllFunctions.PyRun_SimpleString("import sys;print(sys.path)");
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
	AddSitePackagesToPath(&dllFunctions);
	dllFunctions.PyRun_SimpleString("print('load python success!')");
	//dllFunctions.PyRun_SimpleString("import sys;print(sys.path)");
	dllFunctions.Py_Main(argc, argv);
}

void RunPythonFileWithPyRun(char* pyCodePath) {
	PyExportFuncs dllFunctions;

	initPython(pythonPath, &dllFunctions, isOpenConsole);

	wchar_t exePath[MAX_PATH];
	GetModuleFileNameW(NULL, exePath, MAX_PATH);
	dllFunctions.Py_SetPythonHome(pythonPath);
	dllFunctions.Py_SetProgramName(exePath);
	dllFunctions.Py_InitializeEx(1);
	AddSitePackagesToPath(&dllFunctions);
	dllFunctions.PyRun_SimpleString("print('load python success!')");

	FILE* fp = fopen(pyCodePath, "rb");
	dllFunctions.PyRun_SimpleFileExFlags(fp, pyCodePath, 1, NULL);
	//fclose(fp);
}
