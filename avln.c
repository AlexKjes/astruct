#include <Python.h>
#include "structmember.h"
#include "avln.h"



////////////////////
//	BinaryTreeNode
////////////////////

#ifndef AVLNODE
#define AVLNODE





static void avln_dealloc(AVLNode_O *self)
{
    Py_XDECREF(self->key);
    Py_XDECREF(self->value);
	if (self->parent != NULL){
		Py_XDECREF(self->parent);
	}
	if (self->left != NULL){
		Py_XDECREF(self->left);
	}
	if (self->right != NULL){
		Py_XDECREF(self->right);
	}
    Py_TYPE(self)->tp_free((PyObject *) self);
}


static PyObject* avln_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    AVLNode_O* self = (AVLNode_O*)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->key = Py_BuildValue("");
		self->value = Py_BuildValue("");
		self->parent = Py_BuildValue("");
		self->left = Py_BuildValue("");
		self->right = Py_BuildValue("");
		self->height = 1;
    }
    return (PyObject *) self;
}

static int avln_init(AVLNode_O* self, PyObject *args, PyObject *kwds)
{
    static char *kwlist[] = {"key", "value", "parent", "left", "right", NULL};
    PyObject *key = NULL, *value = NULL, *parent = NULL, *left = NULL, *right = NULL, *tmp;

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|OOOOO", kwlist,
                                     &key, &value, &parent, &left, &right)){
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

	if (parent){
		tmp = self->parent;
        Py_INCREF(parent);
        self->parent = parent;
        Py_XDECREF(parent);
	} 

	if (left){
		tmp = self->left;
		Py_INCREF(left);
		self->left = left;
		Py_XDECREF(left);
	}

	if (right){
		tmp = self->right;
		Py_INCREF(right);
		self->right = right;
		Py_XDECREF(right);
	}

    return 0;
};

static PyMemberDef btn_members[] = {
    {"key", T_OBJECT_EX, offsetof(AVLNode_O, key), 0,
     "key by which entry is ordered"},
    {"value", T_OBJECT_EX, offsetof(AVLNode_O, value), 0,
     "value to be stored in the entry"},
    {"parent", T_OBJECT_EX, offsetof(AVLNode_O, parent), 0,
     "this nodes parent"},
    {"left", T_OBJECT_EX, offsetof(AVLNode_O, left), 0,
     "this nodes left child"},
    {"right", T_OBJECT_EX, offsetof(AVLNode_O, right), 0,
     "this nodes right child"},
    {NULL}  /* Sentinel */
};



/*
static PyMethodDef btn_methods[] = {
    {"getBalanceFactor", (PyCFunction) getBalanceFactor, METH_NOARGS,
     "Return balance factor of node"
    },
	{"getSubTreeHeight", (PyCFunction) getSubTreeHeight, METH_NOARGS,
     "Return height of subtree with node as root"
    },
    {NULL}  
};
*/
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


#endif


