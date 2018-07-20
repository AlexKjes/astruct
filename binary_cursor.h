#ifndef BINARYCURSORHEADER
#define BINARYCURSORHEADER

#include <Python.h>
#include "avln.h"
#include "avl.h"

typedef struct {

	AVLNode* currentNode;
	int direction;
	int rev;

} BinaryCursor;


static int bc_moveToNextNode_minMax(BinaryCursor* bc, PyObject* max, PyObject* min, int maxOp, int minOp);
static int bc_moveToNextNode(BinaryCursor* bc);
static AVLNode* bc_getCurrentNode(BinaryCursor* bc);
static BinaryCursor* bc_init(AVLNode* root, int reverse);
static void bc_dealloc(BinaryCursor* bc);

#endif
