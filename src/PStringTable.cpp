/** \file stringtable.cpp
 *  \brief Datastructure for storing and retrieving strings
 *
 * Implementation of the string table class
 */


#include <string.h>
#include <stdlib.h>
#include <stdio.h> 
#include <cassert>
#include "PStringTable.h"


using namespace std;
using namespace PSTD;

/*****************************************************************************************
 * +CM+ StringTable(size_t maxSize): Constructor which initializes string table ]
 *****************************************************************************************/
PStringTable::PStringTable(unsigned int maxSize) :
_STTableSize(maxSize),
_HTTableSize(maxSize / 6),
_HTOverflowPos(0),
_HTNumKeys(0),
_HTOverflowCnt(0)
{
  _STCurrentIndex = 0;
  _StrBuffer = new char[_STTableSize];
  _HashTable = new STNode[_HTTableSize];
  _HTOverflowPos = ST_HT_ALLOC_SIZE;
}

/****************************************************************************************
 * +CM+ ~StringTable(void): Deconstructor to free up memory used by string table ]
 ***************************************************************************************/
PStringTable::~PStringTable(void) {
	delete []_StrBuffer;

	size_t numOverflow = _HTOverflow.size();
	for (size_t i = 0; i < numOverflow; i++) {
		delete[] _HTOverflow[i];
	}

	delete[]_HashTable;
}

/****************************************************************************************
 * +CM+ char *Get_String(char *string): Get a string address if the string already exists or 
 *                           add it if it does not ]
 * Parameters: string (char *): String to look up ]
 * Returns: (char *): Pointer to string in string table ]
 ***************************************************************************************/
const char *PStringTable::Get_String(const char *sstring) {
	size_t key = Hash(sstring) & (_HTTableSize - 1);

	if (_HashTable[key]._Key == NULL) {
		char *res = &_StrBuffer[_STCurrentIndex];
		_HashTable[key]._Key = res;
		strcpy(res, sstring);
		_STCurrentIndex += (strlen(sstring) + 1);
		_HTNumKeys++;
		return res;
	}

	STNode *node = &_HashTable[key];

	// check for a matching key, if none exists add it to the string and hash table
	do {
		if (!strcmp(node->_Key, sstring)) {
			return node->_Key;
		}
		if (node->_Next) {
			node = node->_Next;
		}
	} while (node->_Next);

	// out of overflow block size so make new one
	if (_HTOverflowPos == ST_HT_ALLOC_SIZE) {
		STNode *block = new STNode[ST_HT_ALLOC_SIZE];
		_HTOverflow.push_back(block);
		_HTOverflowPos = 0;
		_HTOverflowCnt++;
	}

	// add the string to the next available bucket
	char *res = &_StrBuffer[_STCurrentIndex];
	strcpy(res, sstring);
	_STCurrentIndex += (strlen(sstring) + 1);
	_HTOverflow.back()[_HTOverflowPos]._Key = res;
	node->_Next = &_HTOverflow.back()[_HTOverflowPos++];
	_HTNumKeys++;
	return res;
}


const char *PStringTable::Get_String(const std::string &sstring) {
	return Get_String(sstring.c_str());
}


unsigned int PStringTable::Get_StringNum(const char *sstring) {
	size_t key = Hash(sstring) & (_HTTableSize - 1);

	if (_HashTable[key]._Key == NULL) {
		size_t index = _STCurrentIndex;
		char *res = &_StrBuffer[index];
		strcpy(res, sstring);
		_STCurrentIndex += (strlen(sstring) + 1);
		_HTNumKeys++;
		return (unsigned int)index;
	}

	STNode *node = &_HashTable[key];

	// check for a matching key, if none exists add it to the string and hash table
	do {
		if (!strcmp(node->_Key, sstring)) {
			unsigned int index = (unsigned int)(node->_Key - _StrBuffer);
		}
		if (node->_Next) {
			node = node->_Next;
		}
	} while (node->_Next);

	// out of overflow block size so make new one
	if (_HTOverflowPos == ST_HT_ALLOC_SIZE) {
		STNode *block = new STNode[ST_HT_ALLOC_SIZE];
		_HTOverflow.push_back(block);
		_HTOverflowPos = 0;
		_HTOverflowCnt++;
	}

	// add the string to the next available bucket
	size_t index = _STCurrentIndex;
	char *res = &_StrBuffer[index];
	strcpy(res, sstring);
	_STCurrentIndex += (strlen(sstring) + 1);
	_HTOverflow.back()[_HTOverflowPos]._Key = res;
	node->_Next = &_HTOverflow.back()[_HTOverflowPos++];
	_HTNumKeys++;
	return (unsigned int)index;
}


/****************************************************************************************
 * +CM+ char *Get_String(int strnum): Get a string address of string strnum ]
 * Parameters: strnum (int): String number to look up ]
 * Returns: (const char *): Pointer to string in string table ]
 ***************************************************************************************/
const char *PStringTable::Get_String(unsigned int strnum) {
	assert(strnum < _STTableSize);
	return &_StrBuffer[strnum];
}

// Print the table
void PStringTable::Display_Table(void) {
	for (unsigned int i = 0; i < 20; i++) {
		printf("%d\n", _StrBuffer[i]);
	}
	for (size_t i = 0; i < _STTableSize; i++) {
		printf("%u : %s\n", i, &_StrBuffer[i]);
		i += strlen(&_StrBuffer[i]);
		if ((i + 1) >= _STCurrentIndex) {
			i = _STTableSize;
		}
	}
}



void PStringTable::Add_Buffer(const char *strBuf, int size) {
  size_t bufferindex = 0;
  
	// go through each string in the buffer and try to add it
	while (bufferindex != size) {
		Get_String(&strBuf[bufferindex]);
		bufferindex += (strlen(&strBuf[bufferindex]) + 1);
	}
}
      
size_t PStringTable::Hash(const char *key) const {
	unsigned long hashval = 5381;
	int c;

	while ((c = *key++))
		hashval = ((hashval << 5) + hashval) + c;
	return hashval = hashval & (_HTTableSize - 1);
}