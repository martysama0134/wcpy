#ifndef __WCPY__WCPY_H__
#define __WCPY__WCPY_H__
#pragma once

#include "wcpy_link.h"

namespace wcpy
{
	
class Data {
private:
	PyObject* m_v{};

public:
	Data() = delete;
	Data(int v) : m_v((PyObject*)v) {};
	Data(PyObject* v) : m_v(v) {};
	~Data() {
		Reset();
	}

	// copy
	Data(const Data&) = delete;
	Data& operator=(const Data&) = delete;

	// move
	Data(Data&& data) {
		this->m_v = data.m_v;
		data.m_v = nullptr;
	};
	Data& operator=(Data& data) {
		Reset();
		this->m_v = data.m_v;
		data.m_v = nullptr;
	};

	void Reset() {
		if (m_v) {
			Py_DECREF(m_v);
			m_v = nullptr;
		}
	}

}; // class Data

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

	static int RunString(const char* str) {
		return PyRun_SimpleString(str);
	}
	static int RunString(const std::string & str) {
		return RunString(str.c_str());
	}
}; // class App

} // namespace wcpy


#endif
