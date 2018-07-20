#include "structmember.h"
#include "avln.h"



////////////////////
//	BinaryTreeNode
////////////////////

#ifndef AVLNODE
#define AVLNODE

static void avln_dealloc(AVLNode* self)
{
	
    Py_XDECREF(self->key);
    Py_XDECREF(self->value);
    free(self);
}

static AVLNode* avln_init(PyObject* key, PyObject* value){

	AVLNode* self = (AVLNode*) malloc(sizeof(AVLNode));
	
	if (key == Py_None)
		PyErr_SetString(PyExc_TypeError, "Key can not be None.");

	self->height = 1;
	self->parent = self->children[0] = self->children[1] = NULL;

	//Py_INCREF(key);
    self->key = key;


	//Py_INCREF(value);
	self->value = value;

    return self;
};

#endif


