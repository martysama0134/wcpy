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

static PyMethodDef CosModuleMethods[] =
{
	 {"CosFunc", CosModule_CosFunc, METH_VARARGS, "evaluate the cosine"},
	 {NULL, NULL, 0, NULL}
};

WCPY_GENERATE_PYTHON3_MODULE(CosModule);

int main(int argc, char* argv[])
{
	puts("start");
	{
		// testing wcpy::Mem
		auto programName = wcpy::App::SetProgramName(argv[0]);
		std::wcout << Py_GetProgramName() << std::endl;

		// testing wcpy::App
		#if PY_MAJOR_VERSION >= 3
		auto app = wcpy::App(false); //should not be initialized if there are custom modules to load
		WCPY_INIT_PYTHON_MODULE(CosModule); //app.InitModule("CosModule", &PyInit_CosModule);
		app.Init(); //first called after custom module
		app.RunString("import CosModule\nprint(CosModule.CosFunc(1.0))");
		#else
		auto app = wcpy::App();
		WCPY_INIT_PYTHON_MODULE(CosModule); //app.InitModule("CosModule", CosModuleMethods);
		app.RunString("import CosModule\nprint(CosModule.CosFunc(1.0))");
		#endif

		app.RunString("from time import time,ctime");
		app.RunString("print('Today is', ctime(time()))");

		// testing wcpy::Data
		wcpy::Data d = PyTuple_New(3);
		wcpy::Data d2 = std::move(d);
		wcpy::Data d3;
		d3.Assign(PyTuple_New(3));
	}
	puts("stop");
}
