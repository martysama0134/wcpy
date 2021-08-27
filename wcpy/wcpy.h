#ifndef __WCPY__WCPY_H__
#define __WCPY__WCPY_H__
#pragma once

#include "wcpy_link.h"

namespace wcpy
{
class App
{
public:
	App(bool init = true) {
		// embedded python3 modules should be imported before the initialization
		if (init)
			Init();
	}
	~App() {
		Clear();
	}

	static void Init() {
		Py_Initialize();
	}
	static void Clear() {
		Py_Finalize();
	}

	static PyObject * GetModule(const char * str) {
		return PyImport_ImportModule(str);
	}
	static PyObject * GetModule(const std::string & str) {
		return GetModule(str.c_str());
	}

#if PY_MAJOR_VERSION >= 3
	static int InitModule(const char * name, PyObject* (*initfunc)(void)) {
		return PyImport_AppendInittab(name, initfunc);
	}
#else
	static PyObject* InitModule(const char* name, PyMethodDef* methods) {
		return Py_InitModule(name, methods);
	}
#endif

	static int AddObject(PyObject * module, const char * name, PyObject * value) {
		return PyModule_AddObject(module, name, value);
	}
	static int AddObject(const char * module, const char * name, PyObject * value) {
		return AddObject(GetModule(module), name, value);
	}

	static int AddString(PyObject * module, const char * name, const char * value) {
		return PyModule_AddStringConstant(module, name, value);
	}
	static int AddString(PyObject * module, const char * name, const std::string value) {
		return PyModule_AddStringConstant(module, name, value.c_str());
	}
	static int AddString(const char * module, const char * name, const char * value) {
		return AddString(GetModule(module), name, value);
	}
	static int AddString(const char * module, const char * name, const std::string & value) {
		return AddString(GetModule(module), name, value);
	}

	static int AddInt(PyObject * module, const char * name, long value) {
		return PyModule_AddIntConstant(module, name, value);
	}
	static int AddInt(const char * module, const char * name, long value) {
		return AddInt(GetModule(module), name, value);
	}

	static void RunString(const char* str) {
		PyRun_SimpleString(str);
	}
	static void RunString(const std::string & str) {
		RunString(str.c_str());
	}
};
} // namespace wcpy


#endif
