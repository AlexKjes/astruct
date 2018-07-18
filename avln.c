#include "structmember.h"
#include "avln.h"



////////////////////
//	BinaryTreeNode
////////////////////

#ifndef AVLNODE
#define AVLNODE


static PyMemberDef btn_members[] = {
    {"key", T_OBJECT_EX, offsetof(AVLNode_O, key), 0,
     "key by which entry is ordered"},
    {"value", T_OBJECT_EX, offsetof(AVLNode_O, value), 0,
     "value to be stored in the entry"},
    {"parent", T_OBJECT_EX, offsetof(AVLNode_O, parent), 0,
     "this nodes parent"},
    {"left", T_OBJECT_EX, offsetof(AVLNode_O, children[0]), 0,
     "this nodes left child"},
    {"right", T_OBJECT_EX, offsetof(AVLNode_O, children[1]), 0,
     "this nodes right child"},
    {NULL}  /* Sentinel */
};


static PyTypeObject AVLNode_T = {

    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "astruct.BinaryTreeNode",
    .tp_doc = "BinaryTreeNode for populating a binary tree",
    .tp_basicsize = sizeof(AVLNode_O),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = avln_new,
	.tp_init = (initproc) avln_init,
	.tp_dealloc = (destructor) avln_dealloc,
	.tp_members = btn_members,
	//.tp_methods = btn_methods,


};


static void avln_dealloc(AVLNode_O *self)
{
    Py_XDECREF(self->key);
    Py_XDECREF(self->value);
	if (self->parent != NULL){
		Py_XDECREF(self->parent);
	}
	if (self->children[0] != NULL){
		Py_XDECREF(self->children[0]);
	}
	if (self->children[1] != NULL){
		Py_XDECREF(self->children[1]);
	}
    Py_TYPE(self)->tp_free((PyObject *) self);
}


static PyObject* avln_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    AVLNode_O* self = (AVLNode_O*)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->key = Py_BuildValue("");
		self->value = Py_BuildValue("");
		self->parent = Py_BuildValue("");
		self->children[0] = Py_BuildValue("");
		self->children[1] = Py_BuildValue("");
		self->height = 1;
    }
    return (PyObject *) self;
}

static int avln_init(AVLNode_O* self, PyObject *args)
{
    
    PyObject *key = NULL, *value = NULL, *tmp = NULL;

    if (!PyArg_ParseTuple(args, "|OO", &key, &value)){
        return -1;
	}

	if (!key){
		PyErr_SetString(PyExc_ValueError, "Key is mandatory");
	}

    if (key) {
        tmp = self->key;
        Py_INCREF(key);
        self->key = key;
        Py_XDECREF(tmp);
    }
    if (value) {
        tmp = self->value;
        Py_INCREF(value);
        self->value = value;
        Py_XDECREF(value);
    } 

    return 0;
};

#endif


