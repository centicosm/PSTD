/** \file poolmemmanager.h
 *  \brief A pooled memory manager
 *
 * The class provides pooled memory management and ensures proper memory alignment of the objects being managed
 */

#ifndef POOL_MEMMANAGER_H
#define POOL_MEMMANAGER_H

#ifdef LINUX
#include <stdint.h>
#endif

#include <type_traits> 
#include "pmath.h"

namespace PSTD {

   
   /** \brief Pooled memory manager
    *
    * A class used to handle the quick allocation of fix size memory chunks initializing additional memory as needed
    * \tparam T Type of object to manage
    */
   template <typename T>
      class PoolMemManager {

        public:

      /** \brief Constructor
       * @param initialObjCnt The initial size of the pool
       * @param growPool Flag indicating if the pool should grow if it runs out of memory
       */
          PoolMemManager(int initialObjCnt, bool growPool = false) :
         _GrowPool(growPool)
         {

			int typeAlignment = std::alignment_of<T>::value;
            int typeSize = sizeof(T);
            int charPtrAlignment = std::alignment_of<char *>::value;
            int charPtrSize = sizeof(char *);
            
            std::vector<unsigned int> memSizes;
            memSizes.push_back(typeAlignment);
            memSizes.push_back(typeSize);
            memSizes.push_back(charPtrAlignment);
            memSizes.push_back(charPtrSize);

            // initialize the pool values and allocate the first block of memory used for pooling
            _ObjPerBlock = initialObjCnt;
            _ChunkSize = PSTD::PMath::LCM(memSizes);
            _NumFreeObj = _ObjPerBlock;
            _NumAllocatedObj = 0;
         
            char *block = new char[(_ObjPerBlock + 1) * _ChunkSize];
            _BlockList.push_back(block); 

            // compute the point where we should start in the buffer so that memory is aligned
             unsigned int offset = Get_AlignmentOffset(block);
            _AlignedBlock.push_back(&block[offset]);

            // link all nodes together
            Init_Memory(0);
            _HeadIndex = _AlignedBlock[0];
         };

     
      //! Deconstructor
         ~PoolMemManager(void) {
            for (unsigned int i = 0; i < _BlockList.size(); i++) {
               delete []_BlockList[i];
            }
         };

		 /**  Get the size of the chunks used for the object
		  *   @return Chunk size
		  */
		 unsigned int Get_ChunkSize(void) { return _ChunkSize; };


         /** Allocate an object out of the pool
          * @return Allocated object
          */
         T *Allocate_Object(void) {
            char *nextObj;
            
            // if we are out of objects in the pool, check if we should create a new block
            if (_HeadIndex == NULL) {
               if (!_GrowPool) {
                  return NULL;
               }
             
               nextObj = Add_Block();
            }
            else {
               nextObj = *((char **)_HeadIndex);
            }

            // then create our object in the pool's memory
            T *obj = new (_HeadIndex) T();
            _NumFreeObj--;
            _NumAllocatedObj++;

            _HeadIndex = nextObj;
            return obj;
         };


      /** \brief Return an object back to the pool
       * @param obj Object to return
       */
      void Free_Object(T *obj) {
         // the returned object is not the front of the free list and the it points to the old head
         *(char **)obj = _HeadIndex;
            _HeadIndex = (char *)obj;
            _NumFreeObj++;
            _NumAllocatedObj--;
         }

                
      /** \brief Reset pool to initialized state 
       *
       *  Reset keeps any extra allocated blocks
       */
      void Reset(void) {

         // re-init all blocks
         for (unsigned int i = 0; i < _BlockList.size(); i++) {
            Init_Memory(i);
            }
            
            // and link the blocks together
            for (unsigned int i = 0; i < _BlockList.size() - 1; i++) {
               char *blockALastPtr =  &(_AlignedBlock[i][(_ObjPerBlock - 1) * _ChunkSize]);         
               char *blockBFirstPtr = _AlignedBlock[i + 1];         
               *(char **)(blockALastPtr) = blockBFirstPtr;
            }

            _NumFreeObj = _ObjPerBlock * _BlockList.size();
            _NumAllocatedObj = 0;
         };

      /** \brief Get the number of free objects remaining in the pool
       * @return Number of free objects remaining
       */
      unsigned int Get_NumFreeObj(void) { return _NumFreeObj; };

   private:
         // determine the offset from the start of a given memory block necessary to make sure the object is properly aligned
         unsigned int Get_AlignmentOffset(char *block) {
            int offset = 0;
            while (((uintptr_t)(&block[offset])) & (_ChunkSize - 1)) {
               offset++;
            }
            return offset;
         };


         // initialize a block of memory and interleave the next object pointers
         void Init_Memory(size_t block) {

            // get the memory aligned starting point of the block and a pointer to the next block if there is one
            char *blockAStart = _AlignedBlock[block];
            char *blockBStart = NULL;

            // interleave pointers to the next object in memory
            for (unsigned int i = 0; i < _ObjPerBlock - 1; i++) {
               *(char **)(&blockAStart[i * _ChunkSize]) = &blockAStart[(i + 1) * _ChunkSize];
            }
            *(char **)(&blockAStart[_ChunkSize * (_ObjPerBlock - 1)]) = blockBStart;
         };


         
         // grow the pool by another block of memory capable of holding _ObjPerBlock onjects
        char *Add_Block(void) {
            char *block = new char[_ObjPerBlock * (_ChunkSize + 1)];
            _BlockList.push_back(block); 
            
            // compute the point where we should start in the buffer so that memory is aligned
            unsigned int offset = Get_AlignmentOffset(block);
            _AlignedBlock.push_back(&block[offset]);

            Init_Memory(_BlockList.size() - 1);
            return &block[offset];
         };                          

         // +ECM+
      bool _GrowPool;                     /*!< Flag indicating if memory pool should grow when it runs out of free chunks */
      
         unsigned int _NumAllocatedObj;
         unsigned int _NumFreeObj;           // +CV+ _FreeObj (unsigned int): Number of free memory chunks ]
         unsigned int _ObjPerBlock;          // +CV+ _ObjPerBlock (unsigned int): Number of pooled objects per block ]
         unsigned int _ChunkSize;              // +CV+ _ChunkSize (unsigned int): Size of the properly aligned chunk of memory necessary 
                                             //                               to hold our object and pointer to the next available object ]
         std::vector<char *> _BlockList;     // +CV+ _BlockList (vector<char *>): The collection of memory blooks used to allocate object ]
         std::vector<char *> _AlignedBlock;  // +CV+ _BlockStart (vector<char *>): The address of the start of the block so that objects are properly alligned in memory ]
         char *_HeadIndex;                   // +CV+ Head_Index (unsigned char *): The next available chunk ]
   };

   // +EndClass+ Pool_MemManager
}


#endif
