/**************************************************************************************************
 * @file	G:\Development\Projects\PSTD\include\PStringtable.h
 *
 * @brief	Declares the stringtable class.
 **************************************************************************************************/
#pragma once

/** \file PStringtable.h
 *  \brief Data structure for storing and retrieving strings
 *
 * This class is used to provide a data structure which allows a single unique reference to strings
 */

/*****************************************************************************
 * +Class+ StringTable: Class used to store strings so as to facilitate memory
 *                      usage, tracking, and subsequent quick access
 *****************************************************************************/


#ifndef STRINGTABLE_H

/**************************************************************************************************
 * @def	STRINGTABLE_H
 *
 * @brief	A macro that defines stringtable h.
 **************************************************************************************************/
#define STRINGTABLE_H

#include <string>
#include <vector>


/**************************************************************************************************
 * @def	ST_HT_ALLOC_SIZE
 *
 * @brief	When hash table overflow occurs, how many new STNodes should we allocate at a time.
 **************************************************************************************************/
#define ST_HT_ALLOC_SIZE	256

namespace PSTD {


	/**************************************************************************************************
	 * @struct	STNode
	 *
	 * @brief	The node used for the hash table in the string table.
	 **************************************************************************************************/
	struct STNode {
		/** @brief	! Represents string in the string table as well as key in hash table. */
		char *_Key;
		/** @brief	! Next bucket for collision chaining. */
		STNode *_Next;

		STNode() : _Key(NULL), _Next(NULL) {};
	};


	/**************************************************************************************************
	 * @class	PStringTable
	 *
	 * @brief	Class used to store strings so as to facilitate memory usage, tracking, and
	 * 			subsequent access  Once the string table is destroyed, any and all references to its
	 * 			strings will be
	 * 			  garbage.
	 **************************************************************************************************/
	class PStringTable {
		public:

		/**************************************************************************************************
		 * @fn	PStringTable::PStringTable(unsigned int maxSize);
		 *
		 * @brief	Constructor.
		 *
		 * @param	maxSize	Maximum size of the string table.
		 **************************************************************************************************/
		PStringTable(unsigned int maxSize);


		/**************************************************************************************************
		 * @fn	PStringTable::~PStringTable(void);
		 *
		 * @brief	Destroy the string table.
		 **************************************************************************************************/
		~PStringTable(void);


		/**********************************************************************************************
		 * @fn	const char PStringTable::*Get_String(const char *sstring);
		 *
		 * @brief	Get the memory address associated with the given string, adding it to the table if it
		 * 		is not already there.
		 *
		 * @param	sstring	String to look for/add to the table.
		 *
		 * @return	Address of the string in the table.
		 **************************************************************************************************/
		const char *Get_String(const char *sstring);


		/**************************************************************************************************
		 * @fn	const char PStringTable::*Get_String(const std::string &sstring);
		 *
		 * @brief	Get the memory address associated with the given string, adding it to the table if it
		 * 			is not already there.
		 *
		 * @param	sstring	String to look for/add to the table.
		 *
		 * @return	Address of the string in the table.
		 **************************************************************************************************/
		const char *Get_String(const std::string &sstring);


		/**************************************************************************************************
		 * @fn	unsigned int PStringTable::Get_StringNum(const char *sstring);
		 *
		 * @brief	Get the table index for a given string.
		 *
		 * @param	sstring	String to get the index of.
		 *
		 * @return	Index into table or -1 if the given string is not in the table.
		 **************************************************************************************************/
		unsigned int Get_StringNum(const char *sstring);



		/**********************************************************************************************
		 * @fn	const char StringTable::*Get_String(unsigned int strnum);
		 *
		 * @brief	Get the memory address associated with a given string index.
		 *
		 * @param	strnum	Index of the string in the table.
		 *
		 * @return	Address of the indexed string.
		 **************************************************************************************************/
		const char *Get_String(unsigned int strnum);


		/**************************************************************************************************
		 * @fn	void StringTable::Display_Table(void);
		 *
		 * @brief	Prints the string table.
		 **************************************************************************************************/
		void Display_Table(void);


		/**************************************************************************************************
		 * @fn	size_t StringTable::Get_TableSize(void)
		 *
		 * @brief	Get the size of the table in bytes.
		 *
		 * @return	Size of table in bytes.
		 **************************************************************************************************/
		size_t Get_TableSize(void) { return _STTableSize; };


		/**************************************************************************************************
		 * @fn	unsigned int PStringTable::Get_NumString(void)
		 *
		 * @brief	Gets the number strings in the table.
		 *
		 * @return	The number of string in the table.
		 **************************************************************************************************/
		unsigned int Get_NumString(void) { return _HTNumKeys; }


		/**************************************************************************************************
		 * @fn	void PStringTable::Add_Buffer(const char *buffer, int size);
		 *
		 * @brief	Add a buffer of null terminated strings into a string table.
		 *
		 * @param	buffer	Buffer containing null terminated strings.
		 * @param	size  	Size of the buffer.
		 **************************************************************************************************/
		void Add_Buffer(const char *buffer, int size);

		private:

		/**************************************************************************************************
		 * @fn	size_t PStringTable::Hash(const char *key) const;
		 *
		 * @brief	Hashes the given key.
		 *
		 * @param	key	The key.
		 *
		 * @return	The hash.
		 **************************************************************************************************/
		size_t Hash(const char *key) const;


		/** @brief	The hash table. */
		STNode *_HashTable;


		/**************************************************************************************************
		 * @brief	The buffer.
		 *
		 * ### summary	@brief	The buffer used to store string.
		 **************************************************************************************************/
		char *_StrBuffer;


		/**************************************************************************************************
		 * @brief	Size of the table.
		 *
		 * ### summary	@brief	Size of the string table buffer.
		 **************************************************************************************************/
		size_t _STTableSize;


		/**************************************************************************************************
		 * @brief	The current index.
		 *
		 * ### summary	@brief	The current buffer index for additions into the string table.
		 **************************************************************************************************/
		size_t _STCurrentIndex;


		/**************************************************************************************************
		 * @brief	Size of the height table.
		 *
		 * ### summary	@brief	Number of buckets in the hash table (without chaining).
		 **************************************************************************************************/
		unsigned int _HTTableSize;


		/**************************************************************************************************
		 * @brief	Size of the height allocate block.
		 *
		 * ### summary	@brief	Size of the height allocate block.
		 **************************************************************************************************/
		//unsigned int _HTAllocBlockSize;


		/**************************************************************************************************
		 * @brief	The height number keys.
		 *
		 * ### summary	@brief	The number of strings in the hash table.
		 **************************************************************************************************/
		unsigned int _HTNumKeys;


		/**************************************************************************************************
		 * @brief	Number of height overflows.
		 *
		 * ### summary	@brief	Number of overflow blocks allocated.
		 **************************************************************************************************/
		unsigned int _HTOverflowCnt;


		/**************************************************************************************************
		 * @brief	The height overflow position.
		 *
		 * ### summary	@brief	The current location in the current overflow block.
		 **************************************************************************************************/
		unsigned int _HTOverflowPos;


		/**************************************************************************************************
		 * @brief	The height overflow.
		 *
		 * ### summary	@brief	Collection of overflow blocks.
		 **************************************************************************************************/
		std::vector<STNode *> _HTOverflow;

	};


	// +EndClass+ StringTable
}

#endif
