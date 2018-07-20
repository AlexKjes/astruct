#include <Python.h>
#include "avl.c"





static PyModuleDef astruct = {
    PyModuleDef_HEAD_INIT,
    .m_name = "astruct",
    .m_doc = "Example module that creates an extension type.",
    .m_size = -1,
};

PyMODINIT_FUNC PyInit_astruct(void) {
    PyObject *m;

	if (PyType_Ready(&AVLTree_T) < 0) { return NULL; }

    m = PyModule_Create(&astruct);
    if (m == NULL)
        return NULL;

	Py_INCREF(&AVLTree_T);
	PyModule_AddObject(m, "AVLTree", (PyObject *) &AVLTree_T);
    return m;
};

