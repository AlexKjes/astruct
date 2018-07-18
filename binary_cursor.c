#ifndef BINARYCURSOR
#define BINARYCURSOR

#include "binary_cursor.h"
#include "avl.c"




static AVLNode_O* bc_getCurrentNode(BinaryCursor* bc){
	
	return bc->currentNode;

}

static int bc_moveToNextNode_minMax(BinaryCursor* bc, PyObject* max, 
										PyObject* min, int maxOp, int minOp){

	if ((PyObject*)bc->currentNode == Py_None) return 0;
	while (1){
		// go left til end
		if (bc->direction == 0){ 
			// end of left 
			if (bc->currentNode->children[0] == Py_None){
				bc->direction = 2;
				if (PyObject_RichCompareBool(bc->currentNode->key, min, minOp) &&
					PyObject_RichCompareBool(bc->currentNode->key, max, maxOp))
					return 1;
				continue;
			}
			bc->currentNode = (AVLNode_O*)bc->currentNode->children[0];
			continue;
		// back up
		}else if (bc->direction == 1){
			// end of tree
			if (bc->currentNode->parent == Py_None){
				return 0;
			// back up right
			} else if (isLeftChildOfParent(bc->currentNode)){
				bc->direction = 2;
				bc->currentNode = (AVLNode_O*)bc->currentNode->parent;
				if (PyObject_RichCompareBool(bc->currentNode->key, min, minOp) &&
					PyObject_RichCompareBool(bc->currentNode->key, max, maxOp))
					return 1;
				continue;
			}
			// back up left
			bc->currentNode = (AVLNode_O*)bc->currentNode->parent;
			continue;
		// take step right
		} else if (bc->direction == 2){
			// far right, back up
			if (bc->currentNode->children[1] == Py_None){
				bc->direction = 1;
				continue;
			}
			bc->currentNode = (AVLNode_O*)bc->currentNode->children[1];
			bc->direction = 0;
			continue;
		}

	}

}


static int bc_moveToNextNode(BinaryCursor* bc){
	if ((PyObject*)bc->currentNode == Py_None) return 0;
	while (1){
		// go left til end
		if (bc->direction == 0){ 
			// end of left 
			if (bc->currentNode->children[(bc->rev)? 1 : 0] == Py_None){
				bc->direction = 2;
				return 1;
			}
			bc->currentNode = (AVLNode_O*)bc->currentNode->children[(bc->rev)? 1 : 0];
			continue;
		// back up
		}else if (bc->direction == 1){
			// end of tree
			if (bc->currentNode->parent == Py_None){
				return 0;
			// back up right
			} else if ((bc->rev) ? 1-isLeftChildOfParent(bc->currentNode) : isLeftChildOfParent(bc->currentNode)){
				bc->direction = 2;
				bc->currentNode = (AVLNode_O*)bc->currentNode->parent;
				return 1;
			}
			// back up left
			bc->currentNode = (AVLNode_O*)bc->currentNode->parent;
			continue;
		// take step right
		} else if (bc->direction == 2){
			// far right, back up
			if (bc->currentNode->children[(bc->rev)? 0 : 1] == Py_None){
				bc->direction = 1;
				continue;
			}
			bc->currentNode = (AVLNode_O*)bc->currentNode->children[(bc->rev)? 0 : 1];
			bc->direction = 0;
			continue;
		}

	}

}

static BinaryCursor* bc_init(AVLNode_O* root, int reverse){

	BinaryCursor* bc = (BinaryCursor*)malloc(sizeof(BinaryCursor));
	bc->direction = 0;
	bc->rev = reverse;
	bc->currentNode = root;

	return bc;

}

static void bc_dealloc(BinaryCursor* bc){
	free(bc);
}




#endif
