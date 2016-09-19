#include <Python.h>  // FIRST, before any other header!!
#include <time.h>
#include <stdlib.h>

static PyObject *
module_function(PyObject *self, PyObject *args){
    float a, b, c;
    if (!PyArg_ParseTuple(args, "ff", &a, &b))
        return NULL;
    c = a + b;
    return Py_BuildValue("f", c);
}

static PyMethodDef MyMethods[] = {
    {"add",  module_function, METH_VARARGS, "Adds two numbers"},
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC initmymath(void){ // This NAME COMPULSORY
    (void) Py_InitModule3("mymath", MyMethods,
                          "My documentation of the mymath module");
}

//"C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\dumpbin.exe" /EXPORTS mymath.pyd