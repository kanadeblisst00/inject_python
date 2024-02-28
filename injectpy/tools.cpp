#include "tools.h"


std::wstring findPythonDLL(const std::wstring& path) {
	std::wregex pattern(L"python3[0-9]+\\.dll");

	for (const auto & entry : std::filesystem::directory_iterator(path)) {
		std::wstring fileName = entry.path().filename().wstring();
		if (std::regex_match(fileName, pattern)) {
			return fileName;
		}
	}
	return L"";
}

DWORD initPython(wchar_t* DllPath, PyExportFuncs* functions, bool isOpen) {
	// 打开控制台
	if (isOpen) {
		AllocConsole();
		freopen("CONIN$", "r+t", stdin);
		freopen("CONOUT$", "w+t", stdout);
		freopen("CONOUT$", "w+t", stderr);
	}
	// 查找并拼接python dll的路径
	wchar_t pyDllPath[MAX_PATH];
	std::wstring pyDllName = findPythonDLL(DllPath);
	PathCombine(pyDllPath, DllPath, pyDllName.c_str());
	// 加载Python dll
	HMODULE pyModule = LoadLibraryW(pyDllPath);
	if (pyModule == nullptr) {
		return 0;
	}
	// 定义导出函数
	functions->Py_Main = (Py_Main_Type)GetProcAddress(pyModule, "Py_Main");
	functions->PyRun_SimpleString = (PyRun_SimpleString_Type)GetProcAddress(pyModule, "PyRun_SimpleString");
	functions->Py_InitializeEx = (Py_InitializeEx_Type)GetProcAddress(pyModule, "Py_InitializeEx");
	functions->Py_SetProgramName = (Py_SetProgramName_Type)GetProcAddress(pyModule, "Py_SetProgramName");
	functions->Py_SetPythonHome = (Py_SetPythonHome_Type)GetProcAddress(pyModule, "Py_SetPythonHome");
	functions->PyImport_ImportModule = (PyImport_ImportModule_Type)GetProcAddress(pyModule, "PyImport_ImportModule");
	functions->PyObject_GetAttrString = (PyObject_GetAttrString_Type)GetProcAddress(pyModule, "PyObject_GetAttrString");
	functions->PyUnicode_FromWideChar = (PyUnicode_FromWideChar_Type)GetProcAddress(pyModule, "PyUnicode_FromWideChar");
	functions->PyList_Insert = (PyList_Insert_Type)GetProcAddress(pyModule, "PyList_Insert");
	functions->PyList_Append = (PyList_Append_Type)GetProcAddress(pyModule, "PyList_Append");
	functions->Py_DECREF = (Py_DECREF_Type)GetProcAddress(pyModule, "Py_DECREF");
	functions->PyErr_Print = (PyErr_Print_Type)GetProcAddress(pyModule, "PyErr_Print");
	functions->PyRun_SimpleFileExFlags = (PyRun_SimpleFileExFlags_Type)GetProcAddress(pyModule, "PyRun_SimpleFileExFlags");
	return 1;
}