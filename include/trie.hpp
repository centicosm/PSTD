/** \file trie.hpp
 *  \brief Trie class
 *
 * Class imlementing a Trie datastructure
 */

#ifndef PTRIE_HPP
#define PTRIE_HPP

#include <string.h> 
#include <list>
#include <vector>
#include <string>

namespace PSTD {
   
   /** Trie class with templateable alphabet size
    * \tparam T Data type
    * \tparam A Alphabet size
    */
   template <typename T, unsigned int A>
   class Trie {
      
   public:
      static const unsigned char Uppercase = 65;    //!< ASCII offset to use when trie alphabet will consist of uppercase characters
      static const unsigned char Lowercase = 97;    //!< ASCII offset to use when trie alphabet will consist of lowercase characters
      static const unsigned char AlphaNumeric = 48; //!< ASCII offset to use when trie alphabet will consist of alpha-numeric characters

   private:

      /** Class representing internal nodes used by the Trie data structure
       * \tparam T Data type
       * \tparam A Alphabet size
       */
      class TrieNode {
      public:
		  TrieNode(T *data = NULL) : _Parent(NULL), _Data(data) { memset(_Children, NULL, A * sizeof(TrieNode *)); };
         TrieNode *_Parent;                  //!< Parent of node in the trie
         T *_Data;                           //!< Data associated with the trie node
         TrieNode *_Children[A];             //!< The potential children of the node (one potential child for each symbol in the trie's alphabet
      };
      
      

   public:
      /** \brief Trie constructor
       * @param asciiOffset Offset of asscii value used to map the trie alphabet in ascii to 0 - Alphabet_Size
       */
      Trie(unsigned char asciiOffset) :
         _ASCIIOffset(asciiOffset)
      { 
         _Root = new TrieNode(); 
      };


      //! Deconstructor
      ~Trie(void) { 
         Delete_Subtree(_Root);
      };
         

      /** \brief Get the data associated with a key
       * @param key String key associated with the data
       * @return Data associated with the key or NULL if no data found
       */
      T *Get_Data(const char *key) {
         TrieNode *keyNode = Find_KeyNode(key, _Root);
         if ((keyNode == NULL) || (keyNode->_Data == NULL)) return NULL;
         return keyNode->_Data;
      }

      /** \brief Insert data into the tree using the given key
       * @param key String key associated with the data
       * @param data Data to insert
       * @return true on success, false on failure
       */
      bool Insert_Key(const char *key, T *data) {
         int len = strlen(key);
         int index = 0;
         TrieNode *curNode = _Root;

         while(len != 0) {

            unsigned char c = key[index];
            c -= _ASCIIOffset;
            if (c >= A) return false;
               
            if (curNode->_Children[c] == NULL) {
               curNode->_Children[c] = new TrieNode();
               curNode->_Children[c]->_Parent = curNode;
            }
            curNode = curNode->_Children[c];
                  
            index++;
            len--;
         }

         if (curNode->_Data == NULL) {
            curNode->_Data = data;
         }
            
         return true;
      }            
      
      /** \brief Delete the given key within the trie.  Application is responsible for handling the data associated with the key.
       * @param key String key associated with the data
       * @return true on success, false on failure
       */
      bool Delete_Key(const char *key) { 
         std::list<TrieNode *> nodeList;

         int len = strlen(key);
         int index = 0;
         TrieNode *curNode = _Root;

         while(len != 0) {
               
            unsigned char c = key[index];
            c -= _ASCIIOffset;
            if (c >= A) return NULL;
               
            // the key does not exist to just return
            if (curNode->_Children[c] == NULL) {
               return false;
            }
               
            curNode = curNode->_Children[c];
            nodeList.push_back(curNode);
                  
            index++;
            len--;
         }

         // make sure there is data associated with this prefix
         if (!curNode->_Data) return false;

         curNode->_Data = NULL;
                  
         while(index != 0) {
            curNode = nodeList.back();
            nodeList.pop_back();
               
            // if there are children, we cannot condense the trie
            if (Has_Children(curNode)) {
               break;
            }
               
            TrieNode *prevNode = nodeList.back();
            unsigned char c = key[index - 1];
            c -= _ASCIIOffset;
            prevNode[c] = NULL;
            delete curNode;

            index--;
         }
         return true;
      };


#define MAX_STR_SIZE 256

	  // Get a list of all the strings beginning with key
	  std::vector<std::string> Get_PostSubString(const char *key) {
		  std::vector<std::string> postSub;

		  // we are assuming no string will be longer than 256 characters
		  char buf[MAX_STR_SIZE];

		  int len = strlen(key);
		  int index = 0;
		  TrieNode *curNode = _Root;

		  while (len != 0) {

			  unsigned char c = key[index];
			  c -= _ASCIIOffset;
			  if (c >= A) return postSub;

			  if (curNode->_Children[c] == NULL) {
				  return postSub;
			  }
			  curNode = curNode->_Children[c];

			  buf[index] = c + _ASCIIOffset;
			  index++;
			  len--;
		  }

			// at this point we have reached the substring, now we do a depth first search and find all the other strings
			//   that have this as a sub string
		  Get_SubString(curNode, buf, index, postSub);

		  return postSub;
	  }



   private:

   void Get_SubString(TrieNode *node, char *str, int index, std::vector<std::string> &strings) {
	   if (index == MAX_STR_SIZE) return;

	   if (node->_Data) {
		   str[index] = 0;
		   strings.push_back(std::string(str));
	   }

	   for (unsigned int i = 0; i < A; i++) {
		   if (node->_Children[i]) {
			   str[index] = i + _ASCIIOffset;
			   Get_SubString(node->_Children[i], str, index + 1, strings);
		   }
	   }
   }


      /** Check if a given trie node has any children
       * @param node Node to check for children
       */
      bool Has_Children(TrieNode *node) {
         for (unsigned int i = 0; i < A; i++) {
            if (node->_Children[i]) return true;
         }
         return false;
      }


      /** Delete all children of the given node
       * @param node node to delete the children of
       */
      void Delete_Subtree(TrieNode *node) {
         for (unsigned int i = 0; i < A; i++) {
            if (node->_Children[i]) {
               Delete_Subtree(node->_Children[i]);
            }
         }
         delete node;
      };
         
         
      /** Given the key and starting tree node, find the trie node associated with that key within the node's children
       *    or NULL if the key is not found among the given nodes children
       * @param key Key to look for
       * @param curNode Node to start the search at
       * @return Node associated with the key or NULL of there is no node for the key
       */
      TrieNode *Find_KeyNode(const char *key, TrieNode *curNode)  {
         if (curNode == NULL) {
            return NULL;
         }
            
         unsigned char c = key[0];
            
         // if a node that could hold the key exists, return that node so it can be checked for a valid key
         if (c == 0) return curNode;
            
         c -= _ASCIIOffset;
         if (c >= A) return NULL;

         // if there are no more child nodes the key does not exist
         if (curNode->_Children[c] == NULL) return NULL;
            
         // otherwise recurse
         return Find_KeyNode(&key[1], curNode->_Children[c]);
      };
         
      TrieNode *_Root;                  //!< Root of the trie
      int _ASCIIOffset;                 //!< Offset used to map ASCII rep of trie alphabet to start at 0
         
   };
};



#endif
