#include <Python.h>


#ifndef BINARYTREENODEHEADER
#define BINARYTREENODEHEADER


typedef struct {
	PyObject_HEAD
	PyObject* key;
	PyObject* value;

	PyObject* parent;
	PyObject* left;
	PyObject* right;

} AVLNode_O; 



static void btn_dealloc(AVLNode_O *self);
static int btn_init(AVLNode_O* self, PyObject *args, PyObject *kwds);


//int getBalanceFactor_C(PyObject* self);
//int getSubTreeHeight_C(PyObject* self);

//PyObject* getBalanceFactor(PyObject* self);
//PyObject* getSubTreeHeight(PyObject* self);




#endif
