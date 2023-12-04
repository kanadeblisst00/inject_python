#pragma once
#include "pch.h"
#include <filesystem>
#include <regex>
#include <Shlwapi.h>



typedef int(*Py_Main_Type)(int, wchar_t**);
typedef void(*PyRun_SimpleString_Type)(const char *);
typedef void(*Py_InitializeEx_Type)(int);
typedef void(*Py_SetProgramName_Type)(const wchar_t *);
typedef void(*Py_SetPythonHome_Type)(const wchar_t *);
typedef void*(*PyImport_ImportModule_Type)(const char *);
typedef void *(*PyObject_GetAttrString_Type)(void *, const char *);
typedef void *(*PyUnicode_FromWideChar_Type)(const wchar_t *, size_t);
typedef int(*PyList_Insert_Type)(void *, size_t, void *);


#define DECLARE_DLL_FUNCTION(module, funcName, returnType, ...) \
    typedef returnType(*funcName##_Type)(__VA_ARGS__); \
    funcName##_Type funcName = (funcName##_Type)GetProcAddress(module, #funcName);


struct PyExportFuncs
{
	Py_Main_Type Py_Main;
	PyRun_SimpleString_Type PyRun_SimpleString;
	Py_InitializeEx_Type Py_InitializeEx;
	Py_SetProgramName_Type Py_SetProgramName;
	Py_SetPythonHome_Type Py_SetPythonHome;
	PyImport_ImportModule_Type PyImport_ImportModule;
	PyObject_GetAttrString_Type PyObject_GetAttrString;
	PyUnicode_FromWideChar_Type PyUnicode_FromWideChar;
	PyList_Insert_Type PyList_Insert;
};


std::wstring findPythonDLL(const std::wstring& path);

DWORD initPython(wchar_t* DllPath, PyExportFuncs* functions, bool isOpen);