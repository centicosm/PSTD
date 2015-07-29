// SLListPooled.cpp : Base implementation for a singlely linked list class
#include <assert.h>
#include <stdio.h>
#include <fstream>

#include "SLListPooled.h"
#include "PooledMemManager.hpp"

using namespace PSTD;

/********************************************************************************************
 * +CM+ SLListPooled(): Constructor for linked list ]
 ********************************************************************************************/
template <typename T>
SLListPooled<T>::SLListPooled(unsigned int poolSize) :
_MemManager(poolSize),
_ListSize(0),
_CurListPos(-1),
_Head(NULL),
_Rear(NULL),
_NextNode(NULL),
_PrevNode(NULL),
_CurNode(NULL)
{

}

/********************************************************************************************
 * +CM+ SLListPooled(^Fixed_MemManager *mm): Constructor for linked list using a pool node allocator ]
 * Parameters: mm (^Fixed_MemManager): Memory manager to use for pooled node allocation ]
 ********************************************************************************************/
template <typename T>
SLListPooled<T>::SLListPooled(T newData, unsigned int poolSize) :
_MemManager(poolSize)
{
	Insert_First(newData);
}





/********************************************************************************************
 * +CM+ ~SLListPooled(): Deconstructor for linked list ]
 ********************************************************************************************/
template <typename T>
SLListPooled<T>::~SLListPooled(void) {
	int cnt;

	for (cnt = 0; cnt < _ListSize; cnt++) {
		Delete_Head();
	}
}


/********************************************************************************************
 * +CM+ void Reset_List(void): Resets list to head position ]
 ********************************************************************************************/
template <typename T>
void SLListPooled<T>::Reset_List(void) {
	_CurNode = _Head;

	// check the case that we are reseting the empty list
	if (_ListSize == 0) {
		_NextNode = NULL;
		_CurListPos = -1;
	}
	else {
		_NextNode = _CurNode->NextNode;
		_CurListPos = 0;
	}
	_PrevNode = NULL;
}



/********************************************************************************************
 * +CM+ void Insert_Head(void *newdata): Inserts data into front of list]
 * Parameters: newdata (void *): data to add to list ]
 ********************************************************************************************/
template <typename T>
void SLListPooled<T>::Insert_Head(T newdata) {
	;

	// check the case that this is the first element being inserted
	if (_ListSize == 0) {
		Insert_First(newdata);
	}

	// otherwise we handle it normally 
	else {

		SLLNode<T> *tempnode = _MemManager.Allocate_Object();

		tempnode->_Data = newdata;
		tempnode->_NextNode = _Head;
		_Head = tempnode;
		_ListSize++;

		// handle the case where curnode = head
		if (_CurListPos == 0) {
			_PrevNode = _Head;
			_NextNode = _CurNode->_NextNode;
		}

		// and increment CurListPos as a node it is one position to the right of
		//   where is used to be
		_CurListPos++;
	}
}


/********************************************************************************************
 * +CM+ void Insert_Rear(void *newdata): Inserts data into rear of list]
 * Parameters: newdata (void *): data to add to list ]
 ********************************************************************************************/
template <typename T>
void SLListPooled<T>::Insert_Rear(T newdata) {


	// handle 1st element case
	if (_ListSize == 0) {
		Insert_First(newdata);
	}
	else {

		// handle allocation appropriately

		SLLNode<T> *tempnode; = _MemManager->Allocate_Object();

		tempnode->_Data = newdata;
		tempnode->_NextNode = NULL;
		_Rear->_NextNode = tempnode;
		_Rear = tempnode;
		_ListSize++;

		// handle the case that CurNode was at the rear when we entered method
		if (_CurListPos == (_ListSize - 2)) {
			_NextNode = _Rear;
		}
	}
}


/********************************************************************************************
 * +CM+ void Insert_CurrentR(void *newdata): Inserts data after current node ]
 * Parameters: newdata (void *): data to add to list ]
 ********************************************************************************************/
template <typename T>
void SLListPooled<T>::Insert_CurrentR(T newdata) {


	// handle 1st element case
	if (_ListSize == 0) {
		Insert_First(newdata);
	}

	else {
		SLLNode<T> *tempnode = _MemManager->Allocate_Object();

		tempnode->_Data = newdata;
		tempnode->_NextNode = NextNode;
		_CurNode->_NextNode = tempnode;
		_NextNode = tempnode;

		// handle the case that CurNode is at the rear
		if (_CurNode == _Rear) {
			_Rear = tempnode;
		}
		_ListSize++;
	}
}


/********************************************************************************************
 * +CM+ void Insert_CurrentF(void *newdata): Inserts data before current node ]
 * Parameters: newdata (void *): data to add to list ]
 ********************************************************************************************/
template <typename T>
void SLListPooled<T>::Insert_CurrentF(T newdata) {


	// handle 1st element case
	if (_ListSize == 0) {
		Insert_First(newdata);
	}

	// handle the case the current node is at the head
	else if (!PrevNode) {
		Insert_Head(newdata);
		_CurListPos = 1;
	}

	// handle all other cases
	else {

		SLLNode<T> *tempnode = _MemManager->Allocate_Object();

		tempnode->_Data = newdata;
		tempnode->_NextNode = _CurNode;
		_PrevNode->_NextNode = tempnode;
		_CurListPos++;
		_ListSize++;
	}
}



/********************************************************************************************
 * +CM+ void Insert_First(void *newdata): Inserts first element into the list ]
 * Parameters: newdata (void *): data to add to list ]
 ********************************************************************************************/
template <typename T>
void SLListPooled<T>::Insert_First(T newdata) {

	_CurNode = _MemManager->Allocate_Object();

	_CurNode->_Data = newdata;
	_CurNode->_NextNode = NULL;
	_Head = _Rear = _CurNode;
	_PrevNode = _NextNode = NULL;
	_CurListPos = 0;
	_ListSize = 1;
}


/********************************************************************************************
 * +CM+ void Delete_Head(void): Deletes first element in list ]
 * Returns: (void *): data to add to list ]
 ********************************************************************************************/
template <typename T>
void SLListPooled<T>::Delete_Head(void) {

	// handle  empty list
	if (_ListSize == 0) {
		return;
	}


	// handle the case of more then 1 element
	if (_ListSize != 1) {
		SLLNode<T> *tempnode = _Head->_NextNode;

		_MemManager->Free_Object(_Head);

		_Head = tempnode;
		_ListSize--;
		if (_CurListPos == 0) {
			Reset_List();
		}
		else {
			_CurListPos--;
		}
	}

	// handle the case of one element
	else {
		_MemManager->Free_Object(_Head);
		_ListSize--;
		_CurListPos = -1;
		_Head = _Rear = _NextNode = _PrevNode = _CurNode = NULL;
	}

}


/********************************************************************************************
 * +CM+ void Delete_Rear(void): Deletes last element in list ]
 * Returns: (void *): data to add to list ]
 ********************************************************************************************/
template <typename T>
void SLListPooled<T>::Delete_Rear(void) {
	void *tempdata;
	int curpos, cnt;

	if (_ListSize == 0) {
		return NULL;
	}


	// save the state of the current node
	curpos = _CurListPos;
	if (_CurNode == _Rear) {
		curpos--;
	}

	Reset_List();

	// go to the last position in the list
	while (Next_Node());

	// do this stuff if there is more then one element in the list
	if (_PrevNode != NULL) {
		_PrevNode->NextNode = NULL;
		_MemManager->Free_Object(_Rear);

		_Rear = _PrevNode;
	}
	_ListSize--;

	// now go back to the current position
	Reset_List();
	for (cnt = 0; cnt < curpos; cnt++) {
		Next_Node();
	}

	return tempdata;
}


/********************************************************************************************
 * +CM+ void *Delete_Current(void): Deletes current element in list ]
 * Returns: (void *): data to add to list ]
 ********************************************************************************************/
template <typename T>
void SLListPooled<T>::Delete_Current(void) {

	// handle case where there is an empty list
	if (_ListSize == 0) {
		return NULL;
	}

	// handle deletion from front of list
	if (_CurListPos == 0) {
		return Delete_Head();
	}

	// handle deletion from rear of list
	if (_CurListPos == _ListSize - 1) {
		return Delete_Rear();
	}

	_PrevNode->_NextNode = _CurNode - _ > NextNode;

	_MemManager->Free_Object(_CurNode);

	_ListSize--;
	_CurNode = _NextNode;
	_NextNode = NextNode->_NextNode;
}

/********************************************************************************************
 * +CM+ void *Get_Data(void): Returns current element in list ]
 * Returns: (void *): data at current node ]
 ********************************************************************************************/
template <typename T>
T PSTD::SLListPooled<T>::Get_Data(void) {
	if (_ListSize == 0) {
		return NULL;
	}
	return _CurNode->_Data;
}


template <typename T>
T SLListPooled<T>::Get_Head(void) {
	if (_ListSize == 0) {
		return NULL;
	}
	return _Head->_Data;
}


/********************************************************************************************
 * +CM+ bool Next_Node(void): Traverses the list by one node ]
 * Returns: (bool): Success of traversal ]
 ********************************************************************************************/
template <typename T>
bool SLListPooled<T>::Next_Node(void) {
	if (NextNode == NULL) {
		return false;
	}
	_PrevNode = _CurNode;
	_CurNode = NextNode;
	_NextNode = _CurNode->NextNode;
	_CurListPos++;
	return true;
}


/********************************************************************************************
 * +CM+ bool Goto_Node(int seekpos): Traverses the list by seekpos nodes ]
 * Parameters: seekpos (int): Position in the list to traverse to
 * Returns: (bool): Success of traversal ]
 ********************************************************************************************/
template <typename T>
bool SLListPooled<T>::Goto_Node(int seekpos) {

	// check for valid positions
	if ((seekpos < 0) || (seekpos > _ListSize - 1)) {
		return false;
	}

	// check to see if we have to back up
	if (seekpos < _CurListPos) {
		Reset_List();
	}

	// look through the list until we reach proper position
	while (_CurListPos != seekpos) {
		Next_Node();
	}
	return true;
}




/********************************************************************************************
 * +CM+ void Insert_List(SLListPooled *mlist): Adds a list to the current list (Non-destructive) ]
 * Parameters: mlist (^SLListPooled *): List to add ]
 ********************************************************************************************/
template <typename T>
void SLListPooled<T>::Insert_ListHead(SLListPooled<T> *mlist) {
	;
	unsigned int listsize = mlist->Get_ListSize();
	mlist->Reset_List();

	for (unsigned int cnt = 0; cnt < listsize; cnt++) {
		Insert_Head(mlist->Get_Data());
		mlist->Next_Node();
	}
}



/********************************************************************************************
 * +CM+ void Insert_ListRear(SLListPooled *mlist): Adds a list to the rear of current list
 * Parameters: mlist (^SLListPooled *): List to add ]
 ********************************************************************************************/
template <typename T>
void SLListPooled<T>::Insert_ListRear(SLListPooled *mlist) {

	unsigned listsize = mlist->Get_ListSize();
	mlist->Reset_List();

	for (unsigned int cnt = 0; cnt < listsize; cnt++) {
		Insert_Rear(mlist->Get_Data());
		mlist->Next_Node();
	}

}



