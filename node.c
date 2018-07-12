#include <python.h>



// Node 

// constructor

static PyObject* Node_init(PyObject* self, PyObject* args) {

	Py_INCREF(Py_None);
	return Py_None;

}

static PyMethodDef NodeMethods[] = {

	{"__init__", Node_init, MTH_VARARGS, "tree node"},
	{0, 0}

}

static PyMethodDef ModuleMethods[] = {{0, 0}}


#ifdef __cplusplus
extern "C"
#endif

void initFoo(  )
{
    PyMethodDef *def;

    /* create new module and class objects */
    PyObject *module = Py_InitModule("astruct", ModuleMethods);
    PyObject *moduleDict = PyModule_GetDict(module);
    PyObject *classDict = PyDict_New();
    PyObject *className = PyString_FromString("TreeNode");
    PyObject *fooClass = PyClass_New(NULL, classDict, className);
    PyDict_SetItemString(moduleDict, "astruct", fooClass);
    Py_DECREF(classDict);
    Py_DECREF(className);
    Py_DECREF(fooClass);

    /* add methods to class */
    for (def = FooMethods; def->ml_name != NULL; def++) {
        PyObject *func = PyCFunction_New(def, NULL);
        PyObject *method = PyMethod_New(func, NULL, fooClass);
        PyDict_SetItemString(classDict, def->ml_name, method);
        Py_DECREF(func);
        Py_DECREF(method);
    }
}
