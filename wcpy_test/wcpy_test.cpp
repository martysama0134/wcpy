// wcpy_test.cpp : Questo file contiene la funzione 'main', in cui inizia e termina l'esecuzione del programma.
//

#include <iostream>
#include "../wcpy/wcpy.h"


static PyObject* CosModule_CosFunc(PyObject* self, PyObject* args)
{
    double value;
    double answer;

    /*  parse the input, from python float to c double */
    if (!PyArg_ParseTuple(args, "d", &value))
        return NULL;
    /* if the above function returns -1, an appropriate Python exception will
     * have been set, and the function simply returns NULL
     */

     /* call cos from libm */
    answer = cos(value);

    /*  construct the output from cos, from c double to python float */
    return Py_BuildValue("f", answer);
}

static PyMethodDef CosMethods[] =
{
     {"CosFunc", CosModule_CosFunc, METH_VARARGS, "evaluate the cosine"},
     {NULL, NULL, 0, NULL}
};

#if PY_MAJOR_VERSION >= 3
static struct PyModuleDef cModPyDem =
{
    PyModuleDef_HEAD_INIT,
    "CosModule", "Some documentation",
    -1,
    CosMethods
};

PyMODINIT_FUNC PyInit_CosModule(void)
{
    return PyModule_Create(&cModPyDem);
}
#endif

int main()
{
    puts("start"); 
    {
        auto app = wcpy::App();
        app.RunString("from time import time,ctime\n"
            "print('Today is', ctime(time()))\n");
        //auto * mod = PyModule_New("CosModule");
        //PyModule_AddFunctions(mod, CosMethods);
#if PY_MAJOR_VERSION >= 3
        PyInit_CosModule();
        PyRun_SimpleString(
            "import importlib.abc\n" \
            "import importlib.machinery\n" \
            "import sys\n" \
            "\n" \
            "\n" \
            "class Finder(importlib.abc.MetaPathFinder):\n" \
            "    def find_spec(self, fullname, path, target=None):\n" \
            "        if fullname in sys.builtin_module_names:\n" \
            "            return importlib.machinery.ModuleSpec(\n" \
            "                fullname,\n" \
            "                importlib.machinery.BuiltinImporter,\n" \
            "            )\n" \
            "\n" \
            "\n" \
            "sys.meta_path.append(Finder())\n" \
        );
#else
        //PyModule_AddFunctions(mod, CosMethods);
        Py_InitModule("CosModule", CosMethods);
#endif
        app.RunString("import CosModule\nprint(CosModule.cos_func(1.0))");
    }
    puts("stop");
}
