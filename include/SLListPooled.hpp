// sllist.h header for singly linked list class

/*****************************************************************************
 * +Class+ SLList: Singlely linked list class ]
 *****************************************************************************/

#ifndef SLLISTPOOLED_H
#define SLLISTPOOLED_H

#include "PooledMemManager.hpp"

namespace PSTD {

	// A singlely linked list implemented using pooled memory

	template <typename T>
	struct SLLNode {
		T _Data;
		SLLNode *_NextNode;
	};


	template <typename T>
	class SLListPooled {
		public:
		SLListPooled(unsigned int poolSize) :
			_MemManager(poolSize),
			_ListSize(0),
			_CurListPos(-1),
			_Head(NULL),
			_Rear(NULL),
			_NextNode(NULL),
			_PrevNode(NULL),
			_CurNode(NULL) { };

		SLListPooled(T newdata, unsigned int poolSize) : _MemManager(poolSize)	{ Insert_First(newData); };

		~SLListPooled(void) {
			for (unsigned int cnt = 0; cnt < _ListSize; cnt++) {
				Delete_Head();
			}
		};

		void Reset_List(void) {
			_CurNode = _Head;

			// check the case that we are reseting the empty list
			if (_ListSize == 0) {
				_NextNode = NULL;
				_CurListPos = -1;
			}
			else {
				_NextNode = _CurNode->_NextNode;
				_CurListPos = 0;
			}
			_PrevNode = NULL;
		};


		void Insert_Head(T newdata) {

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
		};

		void Insert_Rear(T newdata) {

			// handle 1st element case
			if (_ListSize == 0) {
				Insert_First(newdata);
			}
			else {

				// handle allocation appropriately

				SLLNode<T> *tempnode = _MemManager.Allocate_Object();

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
		};


		void Insert_CurrentR(T newdata)  {

			// handle 1st element case
			if (_ListSize == 0) {
				Insert_First(newdata);
			}

			else {
				SLLNode<T> *tempnode = _MemManager.Allocate_Object();

				tempnode->_Data = newdata;
				tempnode->_NextNode = _NextNode;
				_CurNode->_NextNode = tempnode;
				_NextNode = tempnode;

				// handle the case that CurNode is at the rear
				if (_CurNode == _Rear) {
					_Rear = tempnode;
				}
				_ListSize++;
			}
		};


		void Insert_CurrentF(T newdata)  {

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
		};


		void Delete_Head(void) {

			// handle  empty list
			if (_ListSize == 0) {
				return;
			}


			// handle the case of more then 1 element
			if (_ListSize != 1) {
				SLLNode<T> *tempnode = _Head->_NextNode;

				_MemManager.Free_Object(_Head);

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
				_MemManager.Free_Object(_Head);
				_ListSize--;
				_CurListPos = -1;
				_Head = _Rear = _NextNode = _PrevNode = _CurNode = NULL;
			}
		};


		void Delete_Rear(void)  {
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
		};


		void Delete_Current(void)  {

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
		};


		bool Has_Data(void) { return _ListSize != 0; };


		T Get_Data(void) {
			if (_ListSize == 0) {
				return NULL;
			}
			return _CurNode->_Data;
		};


		T Get_Head(void)  {
			if (_ListSize == 0) {
				return NULL;
			}
			return _Head->_Data;
		};


		bool Next_Node(void) {
			if (_NextNode == NULL) {
				return false;
			}
			_PrevNode = _CurNode;
			_CurNode = _NextNode;
			_NextNode = _CurNode->_NextNode;
			_CurListPos++;
			return true;
		};


		unsigned int Get_ListSize(void) { return _ListSize; };

		bool Goto_Node(int seekpos)  {

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
		};


		void Insert_ListHead(SLListPooled<T> *templist) {
			;
			unsigned int listsize = templist->Get_ListSize();
			templist->Reset_List();

			for (unsigned int cnt = 0; cnt < listsize; cnt++) {
				Insert_Head(mlist->Get_Data());
				templist->Next_Node();
			}
		};


		void Insert_ListRear(SLListPooled<T> *templist)  {

			unsigned listsize = templist->Get_ListSize();
			templist->Reset_List();

			for (unsigned int cnt = 0; cnt < listsize; cnt++) {
				Insert_Rear(mlist->Get_Data());
				templist->Next_Node();
			}

		};


		int Get_CurrentPos(void) { return _CurListPos; };

		protected:
		void Insert_First(T newdata)  {

			_CurNode = _MemManager.Allocate_Object();

			_CurNode->_Data = newdata;
			_CurNode->_NextNode = NULL;
			_Head = _Rear = _CurNode;
			_PrevNode = _NextNode = NULL;
			_CurListPos = 0;
			_ListSize = 1;
		};

		// +ECM+

		SLLNode<T> *_Head;
		SLLNode<T> *_Rear;
		SLLNode<T> *_NextNode;
		SLLNode<T> *_PrevNode;
		SLLNode<T> *_CurNode;
		unsigned int _ListSize;
		int _CurListPos;
		PoolMemManager<SLLNode<T>> _MemManager;

	};

};



#endif

