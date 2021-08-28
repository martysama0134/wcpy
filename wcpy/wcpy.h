#ifndef __WCPY__WCPY_H__
#define __WCPY__WCPY_H__
#pragma once

#include "wcpy_link.h"
#include "wcpy_macro.h"

namespace wcpy
{

class Data {

public:
	// aliases
	using Type = PyObject*;

	// constructor
	Data() = default;
	Data(int obj) { Assign(obj);  };
	Data(Data::Type obj) { Assign(obj); };

	// destructor
	~Data() {
		Reset();
	}

	// copy
	Data(const Data&) = delete;
	Data& operator=(const Data&) = delete;

	// move
	Data(Data&& data) {
		this->mObj = data.mObj;
		data.mObj = nullptr;
	};
	Data& operator=(Data& data) {
		Reset();
		this->mObj = data.mObj;
		data.mObj = nullptr;
	};

	// methods
	void Reset() {
		if (mObj) {
			Py_DECREF(mObj);
			mObj = nullptr;
		}
	}

	void Assign(Data::Type obj) {
		Reset();
		mObj = obj;
	}

	void Assign(int obj) {
		Reset();
		mObj = (Data::Type)obj;
	}

	Data::Type Get() {
		return mObj;
	}

	// access
	//! @brief *ptr
	Data::Type operator*() const { return mObj; }
	//! @brief &ptr
	Data::Type * operator&() { return &mObj; }
	//! @brief if (!ptr)
	bool operator!() const { return !mObj; }
	//! @brief ptr->elem
	Data::Type operator->() const { return mObj; }
	//! @brief if (ptr)
	explicit operator bool() const { return mObj; }
	#ifdef WCPY_DATA_ENABLE_IMPLICIT_CONVERSION
	//! @brief implicit std::FILE ptr conversion
	operator Data::Type () const { return mObj; }
	#endif

private:
	Data::Type mObj{nullptr};
}; // class Data

#if PY_MAJOR_VERSION >= 3
class Mem
{
public:
	// aliases
	using Type = void*;

	// constructors
	Mem(Mem::Type ptr) { Assign(ptr); };
	~Mem() { Reset(); };

	// copy
	Mem(const Mem&) = delete;
	Mem& operator=(const Mem&) = delete;

	// move
	Mem(Mem&& mem) {
		this->mPtr = mem.mPtr;
		mem.mPtr = nullptr;
	};
	Mem& operator=(Mem& mem) {
		Reset();
		this->mPtr = mem.mPtr;
		mem.mPtr = nullptr;
	};

	// methods
	void Reset() {
		if (mPtr) {
			PyMem_RawFree(mPtr);
			mPtr = nullptr;
		}
	}

	void Assign(Mem::Type obj) {
		Reset();
		mPtr = obj;
	}

	Mem::Type Get() {
		return mPtr;
	}

private:
	Mem::Type mPtr{nullptr};
};
#endif

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

	static inline void Init() {
		Py_Initialize();
	}
	static inline void Clear() {
		Py_Finalize();
	}

#if PY_MAJOR_VERSION >= 3
	static inline wcpy::Mem SetProgramName(char * name) {
		auto v = Py_DecodeLocale(name, nullptr);
		Py_SetProgramName(v);
		return v;
	}
#else
	static inline bool SetProgramName(char * name) {
		Py_SetProgramName(name);
		return (name);
	}
#endif

	static inline PyObject * GetModule(const char * str) {
		return PyImport_ImportModule(str);
	}
	static inline PyObject * GetModule(const std::string & str) {
		return GetModule(str.c_str());
	}

#if PY_MAJOR_VERSION >= 3
	static inline int InitModule(const char * name, PyObject* (*initfunc)(void)) {
		return PyImport_AppendInittab(name, initfunc);
	}
#else
	static inline PyObject* InitModule(const char* name, PyMethodDef* methods) {
		return Py_InitModule(name, methods);
	}
#endif

	static inline bool StringCheck(PyObject * v) {
		#if PY_MAJOR_VERSION >= 3
		return PyBytes_Check(v);
		#else
		return PyString_Check(v);
		#endif
	}

	static inline char * StringAsString(PyObject * v) {
		#if PY_MAJOR_VERSION >= 3
		return PyBytes_AS_STRING(v);
		#else
		return PyString_AS_STRING(v);
		#endif
	}

	static inline int AddObject(PyObject * module, const char * name, PyObject * value) {
		return PyModule_AddObject(module, name, value);
	}
	static inline int AddObject(const char * module, const char * name, PyObject * value) {
		return AddObject(GetModule(module), name, value);
	}

	static inline int AddString(PyObject * module, const char * name, const char * value) {
		return PyModule_AddStringConstant(module, name, value);
	}
	static inline int AddString(PyObject * module, const char * name, const std::string value) {
		return PyModule_AddStringConstant(module, name, value.c_str());
	}
	static inline int AddString(const char * module, const char * name, const char * value) {
		return AddString(GetModule(module), name, value);
	}
	static inline int AddString(const char * module, const char * name, const std::string & value) {
		return AddString(GetModule(module), name, value);
	}

	static inline int AddInt(PyObject * module, const char * name, long value) {
		return PyModule_AddIntConstant(module, name, value);
	}
	static inline int AddInt(const char * module, const char * name, long value) {
		return AddInt(GetModule(module), name, value);
	}

	static inline int RunString(const char* str) {
		return PyRun_SimpleString(str);
	}
	static inline int RunString(const std::string & str) {
		return RunString(str.c_str());
	}
}; // class App

} // namespace wcpy


#endif
