# wcpy
Wrapper for embedded CPython for both Py2 and Py3






---------
This is an example of a possible wcpy_link.h:
```cpp
#ifndef __WCPY__WCPY_LINK_H__
#define __WCPY__WCPY_LINK_H__
#pragma once

// Some projects may have different include paths, edit them here.

//#include <Python27/Python.h>
#include <Python39/Python.h>

#if PY_MAJOR_VERSION >= 3
#include <Python39/frameobject.h>
#include <Python39/longintrepr.h>
#include <Python39/marshal.h>
#ifndef PyInt_FromLong
#define PyInt_FromLong PyLong_FromLong
#endif

#else
#include <Python27/frameobject.h>
#include <Python27/longintrepr.h>
#include <Python27/marshal.h>
#undef SHIFT //fmt conflict
#endif

#endif
```
