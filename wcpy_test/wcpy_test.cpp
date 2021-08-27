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
        // testing wcpy::App
        #if PY_MAJOR_VERSION >= 3
        auto app = wcpy::App(false); //should not be initialized if there are custom modules to load
        app.InitModule("CosModule", &PyInit_CosModule);
        app.Init(); //first called after custom module
        app.RunString("import CosModule\nprint(CosModule.CosFunc(1.0))");
        #else
        auto app = wcpy::App();
        app.InitModule("CosModule", CosMethods);
        app.RunString("import CosModule\nprint(CosModule.CosFunc(1.0))");
        #endif
        app.RunString("from time import time,ctime\n"
            "print('Today is', ctime(time()))\n");

        // testing wcpy::Data
        wcpy::Data d = PyTuple_New(3);
        wcpy::Data d2 = std::move(d);
    }
    puts("stop");
}
