#pragma once

#ifndef PHASHTABLE_H
#define PHASHTABLE_H

#include <vector>
#include <list>

// basic string keyed hashtable with bucket chaining

#define HT_ALLOC_SIZE	256

namespace PSTD {

	template<typename T>
	struct HTNode {
		char *_Key;
		T _Val;
		HTNode *_Next;

		HTNode() : _Key(NULL), _Next(NULL) {};
		~HTNode() { if (_Key) free(_Key); }
	};

	template<typename T>
	class PHashTable {
		public:
		PHashTable(unsigned int maxSize) : _TableSize(maxSize), _NumKeys(0), _OverflowCnt(0), _OverflowPos(0) {
			_Table = new HTNode[_TableSize];

		}


		~PHashTable(void) {
			delete[] _Table;
			for (size_t i = 0; i < _Overflow.size(); i++) {
				delete[] _Overflow[i];
			}
		}


		void Set(const char *key, T val) {

			unsigned int hash = 2166136261;
			for (const char *s = key; *s; s++) hash = (16777619 * hash) ^ (*s);
			hash &= (_TableSize - 1)

			if (!_Table[hash]._Key) {
				_Table[hash]._Key = strdup(key);
				_Table[hash]._Val = val;
				return;
			}

			HTNode *node = &_HashTable[key];

			// check for a matching key, if none exists add it to the string and hash table
			do {

				// replace existing key if it already exists
				if (!strcmp(node->_Key, key)) {
					_Table[hash]._Val = val;
					return;
				}
				if (node->_Next) {
					node = node->_Next;
				}
			} while (node->_Next);

			// out of overflow block size so make new one
			if (_OverflowPos == HT_ALLOC_SIZE) {
				HTNode *block = new HTNode[HT_ALLOC_SIZE];
				_Overflow.push_back(block);
				_OverflowPos = 0;
				_OverflowCnt++;
			}

			_Table[hash]._Key = new char[strlen(key) + 1];
			strdup(_Table[hash]._Key, key);
			_Table[hash]._Val = val;

			// add the string to the next available bucket
			_Overflow.back()[_OverflowPos]._Key = strdup(key); 
			_Overflow.back()[_OverflowPos]._Val = val;
			node->_Next = &_Overflow.back()[_OverflowPos++];
			_NumKeys++;
		}


		bool Get(const char *key, T &val) const {
			unsigned int hash = 2166136261;
			for (const char *s = key; *s; s++) hash = (16777619 * hash) ^ (*s);
			hash &= (_TableSize - 1)

				// check in main bucket
			if (!_Table[hash]._Key) return false;

			HTNode *node = &_HashTable[key];

			// check in chained bucket
			do {

				// found in chain bucket
				if (!strcmp(node->_Key, key)) {
					return node->_Val;
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
				for (unsigned int j = 0; j < HT_ALLOC_SIZE; j++) {
					items->push_back(_Overflow[i][j]);
				}
			}
			for (unsigned int i = 0; i < _OverflowPos; i++) {
				items->push_back(_Overflow[_OverflowCnt][j]);
			}
			return items;
		}
		unsigned int Get_NumKeys(void) { return _NumKeys; };


		HTNode<T> *_Table;

		/**************************************************************************************************
		* @brief	Size of the height table.
		*
		* ### summary	@brief	Number of buckets in the hash table (without chaining).
		**************************************************************************************************/
		unsigned int _TableSize;



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
		std::vector<HTNode<T> *> _Overflow;
	};
};

#endif
