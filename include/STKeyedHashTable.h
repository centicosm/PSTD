#pragma once

#ifndef STKEYEDPHASHTABLE_H
#define STKEYEDPHASHTABLE_H

#include <vector>
#include <list>



// Hashtable where all the keys are from a string table allowing for no key memory allocations when adding a key
//   and potential performance increases when trying to find a key with a string table reference

#define HT_ALLOC_SIZE	256

namespace PSTD {

	template<typename T>
	struct STKHTNode {
		const char *_Key;
		T _Val;
		STKHTNode *_Next;

		STKHTNode() : _Key(NULL), _Next(NULL) {};
	};

	template<typename T, int N = 128>
	class STKeyedHashTable {
		public:
		STKeyedHashTable(unsigned int allocSize = HT_ALLOC_SIZE) : _TableSize(N), _NumKeys(0), _OverflowCnt(0), _OverflowPos(allocSize), _OverflowAllocSize(allocSize) {
			_Table = new STKHTNode<T>[_TableSize];

		}

		~STKeyedHashTable(void) {
			delete[] _Table;
			for (size_t i = 0; i < _Overflow.size(); i++) {
				delete[] _Overflow[i];
			}
		}

		// set the entry for key to val potentially replacing preexisting values
		//   this is the case where we know key is a pointer to a string table string
		void Set_ST(const char *key, T val, bool replace = true) {
			unsigned int hash = 2166136261;
			for (const char *s = key; *s; s++) hash = (16777619 * hash) ^ (*s);
			hash &= (_TableSize - 1);

			if (!_Table[hash]._Key) {
				_Table[hash]._Key = key;
				_Table[hash]._Val = val;
				return;
			}

			STKHTNode<T> *node = &_Table[hash];

			// check for a matching key, if none exists add it to the string and hash table
			do {

				// replace existing key if it already exists  (if we are using keys that originate from the string table, we can just compare ptrs)
				if (node->_Key == key) {
					if (replace) _Table[hash]._Val = val;
					return;
				}
				if (node->_Next) {
					node = node->_Next;
				}
			} while (node->_Next);

			Place_InOverflow(key, val);
		}


		// set the entry for key to val potentially replacing preexisting values
		//   this is the case where we don't know if key is a pointer to a string table string
		void Set(const char *key, T val) {

			unsigned int hash = 2166136261;
			for (const char *s = key; *s; s++) hash = (16777619 * hash) ^ (*s);
			hash &= (_TableSize - 1);

			if (!_Table[hash]._Key) {
				_Table[hash]._Key = key;
				_Table[hash]._Val = val;
				return;
			}

			STKHTNode<T> *node = &_Table[hash];

			// check for a matching key, if none exists add it to the string and hash table
			do {

				// replace existing key if it already exists  (if we are using keys that originate from the string table, we can just compare ptrs)
				if (!strcmp(node->_Key, key)) {
					_Table[hash]._Val = val;
					return;
				}
				if (node->_Next) {
					node = node->_Next;
				}
			} while (node->_Next);

			Place_InOverflow(key, val);
		}


		bool Get(const char *key, T &val) const {
			unsigned int hash = 2166136261;
			for (const char *s = key; *s; s++) hash = (16777619 * hash) ^ (*s);
			hash &= (_TableSize - 1);

			// check in main bucket
			if (!_Table[hash]._Key) return false;

			STKHTNode<T> *node = &_Table[hash];

			// check in chained bucket
			do {

				// found in chain bucket
				if (!strcmp(node->_Key, key)) {
					val = node->_Val;
					return true;
				}
			} while (node = node->_Next);

			return false;
		}


		std::list<T> *Get_ItemList(void) {
			std::list<T> *items = new std::list<T>();

			for (unsigned int i = 0; i < _TableSize; i++) {
				if (_Table[i]._Key) {
					items->push_back(_Table[i]._Val);
				}
			}
			for (int i = 0; i < _OverflowCnt - 1; i++) {
				for (unsigned int j = 0; j < _OverflowAllocSize; j++) {
					items->push_back(_Overflow[i][j]);
				}
			}
			for (unsigned int i = 0; i < _OverflowPos; i++) {
				items->push_back(_Overflow[_OverflowCnt][j]);
			}
			return items;
		}
		unsigned int Get_NumKeys(void) { return _NumKeys; };


		private:

		void Place_InOverflow(const char *key, T val) {

			// out of overflow block size so make new one
			if (_OverflowPos == _OverflowAllocSize) {
				STKHTNode<T> *block = new STKHTNode<T>[_OverflowAllocSize];
				_Overflow.push_back(block);
				_OverflowPos = 0;
				_OverflowCnt++;
			}


			// add the string to the next available bucket
			_Overflow.back()[_OverflowPos]._Key = key;
			_Overflow.back()[_OverflowPos]._Val = val;
			node->_Next = &_Overflow.back()[_OverflowPos++];
			_NumKeys++;
		}




		STKHTNode<T> *_Table;

		/**************************************************************************************************
		* @brief	Size of the height table.
		*
		* ### summary	@brief	Number of buckets in the hash table (without chaining).
		**************************************************************************************************/
		unsigned int _TableSize;


		unsigned int _OverflowAllocSize;


		/**************************************************************************************************
		* @brief	The height number keys.
		*
		* ### summary	@brief	The number of strings in the hash table.
		**************************************************************************************************/
		unsigned int _NumKeys;


		/**************************************************************************************************
		* @brief	Number of height overflows.
		*
		* ### summary	@brief	Number of overflow blocks allocated.
		**************************************************************************************************/
		unsigned int _OverflowCnt;


		/**************************************************************************************************
		* @brief	The height overflow position.
		*
		* ### summary	@brief	The current location in the current overflow block.
		**************************************************************************************************/
		unsigned int _OverflowPos;


		/**************************************************************************************************
		* @brief	The height overflow.
		*
		* ### summary	@brief	Collection of overflow blocks.
		**************************************************************************************************/
		std::vector<STKHTNode<T> *> _Overflow;
	};
};

#endif
