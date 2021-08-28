#ifndef __WCPY__WCPY_MACRO_H__
#define __WCPY__WCPY_MACRO_H__
#pragma once

#if PY_MAJOR_VERSION >= 3
#define WCPY_GENERATE_PYTHON3_MODULE(name)			\
	static PyModuleDef mod##name## = {				\
		PyModuleDef_HEAD_INIT, #name,				\
		#name " DOC", -1, ##name##Methods			\
	};												\
	PyMODINIT_FUNC PyInit_##name##(void) {			\
		return PyModule_Create(&mod##name##);		\
	}
#else
#define WCPY_GENERATE_PYTHON3_MODULE(name) //empty
#endif

#if PY_MAJOR_VERSION >= 3
#define WCPY_INIT_PYTHON_MODULE(name) wcpy::App::InitModule(#name, &PyInit_##name##);
#else
#define WCPY_INIT_PYTHON_MODULE(name) wcpy::App::InitModule(#name, ##name##Methods);
#endif

#endif
