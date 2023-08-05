/***********************************************************************
 * Header:
 *    BST
 * Summary:
 *    Our custom implementation of a BST for set and for map
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        BST                 : A class that represents a binary search tree
 *        BST::iterator       : An iterator through BST
 * Author
 *    Ryan Madsen, Nathan Wood, Jared Tart
 ************************************************************************/

#pragma once

#ifdef DEBUG
#define debug(x) x
#else // !DEBUG
#define debug(x)
#endif // !DEBUG

#include <cassert>
#include <utility>
#include <memory>     // for std::allocator
#include <functional> // for std::less
#include <utility>    // for std::pair

class TestBST; // forward declaration for unit tests
class TestSet;
class TestMap;

namespace custom
{

   template <typename TT>
   class set;
   template <typename KK, typename VV>
   class map;

/*****************************************************************
 * BINARY SEARCH TREE
 * Create a Binary Search Tree
 *****************************************************************/
template <typename T>
class BST
{
   friend class ::TestBST; // give unit tests access to the privates
   friend class ::TestSet;
   friend class ::TestMap;

   template <class TT>
   friend class custom::set;

   template <class KK, class VV>
   friend class custom::map;
public:
   //
   // Construct
   //

   BST();
   BST(const BST &  rhs);
   BST(      BST && rhs);
   BST(const std::initializer_list<T>& il);
   ~BST();

   //
   // Assign
   //

   BST & operator = (const BST &  rhs);
   BST & operator = (      BST && rhs);
   BST & operator = (const std::initializer_list<T>& il);
   void swap(BST & rhs);

   //
   // Iterator
   //

   class iterator;
   iterator   begin() const noexcept;
   iterator   end()   const noexcept
   {
      if (root == nullptr)
         return iterator(nullptr);
      BNode* p = root;
      while (p != nullptr)
         p = p->pRight;
      return iterator(p);
   }

   //
   // Access
   //

   iterator find(const T& t);

   // 
   // Insert
   //

   std::pair<iterator, bool> insert(const T&  t, bool keepUnique = false);
   std::pair<iterator, bool> insert(      T&& t, bool keepUnique = false);

   //
   // Remove
   // 

   iterator erase(iterator& it);
   void   clear() noexcept;

   // 
   // Status
   //

   bool   empty() const noexcept { return numElements == 0; }
   size_t size()  const noexcept { return numElements;   }
   
private:
   class BNode;

   void clearNode(BNode*& pThis);
   void assign(BNode*& pDest, const BNode* pSrc);

   BNode * root;              // root node of the binary search tree
   size_t numElements;        // number of elements currently in the tree
};


/*****************************************************************
 * BINARY NODE
 * A single node in a binary tree. Note that the node does not know
 * anything about the properties of the tree so no validation can be done.
 *****************************************************************/
template <typename T>
class BST <T> :: BNode
{
public:
   // 
   // Construct
   //
   BNode() : data(T())
   {
      pLeft = pRight = nullptr;
      pParent = nullptr;
      isRed = true;
   }
   BNode(const T& t) : data(t)
   {
      pLeft = pRight = nullptr;
      pParent = nullptr;
      isRed = true;
   }
   BNode(T&& t) : data(std::move(t))
   {
      pLeft = pRight = nullptr;
      pParent = nullptr;
      isRed = true;
   }

   //
   // Insert
   //
   void addLeft (BNode * pNode);
   void addRight(BNode * pNode);
   void addLeft (const T &  t);
   void addRight(const T &  t);
   void addLeft(       T && t);
   void addRight(      T && t);

   // 
   // Status
   //
   bool isRightChild(BNode * pNode) const { return this->data < pNode->data; }
   bool isLeftChild( BNode * pNode) const { return pNode->data < this->data; }

   // balance the tree
   void balance();

#ifdef DEBUG
   //
   // Verify
   //
   std::pair <T,T> verifyBTree() const;
   int findDepth() const;
   bool verifyRedBlack(int depth) const;
   int computeSize() const;
#endif // DEBUG

   //
   // Data
   //
   T data;                  // Actual data stored in the BNode
   BNode* pLeft;          // Left child - smaller
   BNode* pRight;         // Right child - larger
   BNode* pParent;        // Parent
   bool isRed;              // Red-black balancing stuff
};

/**********************************************************
 * BINARY SEARCH TREE ITERATOR
 * Forward and reverse iterator through a BST
 *********************************************************/
template <typename T>
class BST <T> :: iterator
{
   friend class ::TestBST; // give unit tests access to the privates
   friend class ::TestSet;
   friend class ::TestMap;

   template <class KK, class VV>
   friend class custom::map;
public:
   // constructors and assignment
   iterator(BNode * p = nullptr) : pNode(p) {};

   iterator(const iterator & rhs) : pNode(rhs.pNode) {};

   iterator & operator = (const iterator & rhs)
   {
      this->pNode = rhs.pNode;
      return *this;
   }

   // compare
   bool operator == (const iterator & rhs) const
   {
      // If both nodes are nullptr, they are equal.
      if (pNode == nullptr && rhs.pNode == nullptr)
         return true;
      // If one is nullptr and the other is not, they are not equal.
      else if ((pNode == nullptr && rhs.pNode != nullptr) || (pNode != nullptr && rhs.pNode == nullptr))
         return false;
      // If neither are nullptr, compare the data.
      else
         return pNode->data == rhs.pNode->data;
   }
   bool operator != (const iterator & rhs) const
   {
      // If both nodes are nullptr, they are equal.
      if (pNode == nullptr && rhs.pNode == nullptr)
         return false;
      // If one is nullptr and the other is not, they are not equal.
      else if ((pNode == nullptr && rhs.pNode != nullptr) || (pNode != nullptr && rhs.pNode == nullptr))
         return true;
      // If neither are nullptr, compare the data.
      else
         return !(pNode->data == rhs.pNode->data);
   }

   // de-reference. Cannot change because it will invalidate the BST
   const T & operator * () const 
   {
      return pNode->data;
   }

   // increment and decrement
   iterator & operator ++ ();
   iterator   operator ++ (int postfix)
   {
      return *this;
   }
   iterator & operator -- ();
   iterator   operator -- (int postfix)
   {
      return *this;;
   }

   // must give friend status to remove so it can call getNode() from it
   friend BST <T> :: iterator BST <T> :: erase(iterator & it);

private:
   
    // the node
    BNode * pNode;
};


/*********************************************
 *********************************************
 *********************************************
 ******************** BST ********************
 *********************************************
 *********************************************
 *********************************************/


 /*********************************************
  * BST :: DEFAULT CONSTRUCTOR
  ********************************************/
template <typename T>
BST <T> ::BST()
{
   numElements = 0;
   root = nullptr;
}

/*********************************************
 * BST :: COPY CONSTRUCTOR
 * Copy one tree to another
 ********************************************/
template <typename T>
BST <T> :: BST (const BST<T>& rhs)
{
   // Copy nodes from rhs to this tree.
   numElements = 0;
   root = nullptr;
   *this = rhs;
}

/*********************************************
 * BST :: MOVE CONSTRUCTOR
 * Move one tree to another
 ********************************************/
template <typename T>
BST <T> :: BST(BST <T> && rhs) : 
root(std::move(rhs.root)), 
numElements(std::move(rhs.numElements))
{
   rhs.numElements = 0;
   rhs.root = nullptr;
}

/*********************************************
 * BST :: INITIALIZER LIST CONSTRUCTOR
 * Create a BST from an initializer list
 ********************************************/
template <typename T>
BST <T> :: BST(const std::initializer_list<T>& il) : root(nullptr), numElements(0)
{
   // Insert each node from the initializer list.
   for (auto& element : il)
   {
      this->insert(element);
   }
}

/*********************************************
 * BST :: DESTRUCTOR
 ********************************************/
template <typename T>
BST <T> :: ~BST()
{
   this->clear();
}


/*********************************************
 * BST :: ASSIGNMENT OPERATOR
 * Copy one tree to another
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (const BST <T> & rhs)
{
   assign(root, rhs.root);
   this->numElements = rhs.numElements;
   return *this;
}

/*********************************************
 * BST :: ASSIGNMENT OPERATOR with INITIALIZATION LIST
 * Copy nodes onto a BTree
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (const std::initializer_list<T>& il)
{
   this->clear();
   for (auto& element : il)
   {
      this->insert(element);
   }
   numElements = il.size();
   return *this;
}

/*********************************************
 * BST :: ASSIGN-MOVE OPERATOR
 * Move one tree to another
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (BST <T> && rhs)
{
   this->clear();
   this->swap(rhs);
   return *this;
}

/*********************************************
 * BST :: SWAP
 * Swap two trees
 ********************************************/
template <typename T>
void BST <T> :: swap (BST <T>& rhs)
{
   BST<T>::BNode *tempRoot = rhs.root;
   rhs.root = this->root;
   this->root = tempRoot;

   size_t tempElements = rhs.numElements;
   rhs.numElements = this->numElements;
   this->numElements = tempElements;
}

/*****************************************************
 * BST :: INSERT
 * Insert a node at a given location in the tree
 ****************************************************/
template <typename T>
std::pair<typename BST <T> :: iterator, bool> BST <T> :: insert(const T & t, bool keepUnique)
{
   // If keepUnique is true, check if the node already exists.
   // If it does, return the iterator to the node and false.
   if (keepUnique)
   {
      auto it = this->find(t);
      if (it != this->end())
         return std::pair<iterator, bool>(it, false);
   }

   // Create a new node with the given data.
   auto newNode = new BNode(t);

   // If the root is nullptr, set the root to the new node.
   if (this->root == nullptr)
   {
      this->root = newNode;
      this->root->isRed = false;
      this->numElements++;
      return std::pair<iterator, bool>(newNode, true);
   }

   // Set the current node to the root.
   auto current = this->root;
   bool found = false;

   // Find where to insert the new node.
   while (!found)
   {
      // If the new node is less than the current node, go left.
      if (current->isLeftChild(newNode))
      {
         // If we are not at a leaf, go left.
         if (current->pLeft != nullptr)
            current = current->pLeft;
         // If we are at a leaf, add the new node to the left.
         else
         {
            current->addLeft(newNode);
            found = true;
         }
      }
      // If the new node is greater than the current node, go right.
      else
      {
         // If we are not at a leaf, go right.
         if (current->pRight != nullptr)
            current = current->pRight;
         // If we are at a leaf, add the new node to the right.
         else
         {
            current->addRight(newNode);
            found = true;
         }
      }
   }

   // Balance the tree.
   newNode->balance();
   // Reset the root node.
   auto pTemp = newNode;
   while (pTemp->pParent != nullptr)
      pTemp = pTemp->pParent;
   this->root = pTemp;
   // Increment the number of elements.
   this->numElements++;
   return std::pair<iterator, bool>(newNode, true);
}

template <typename T>
std::pair<typename BST <T> ::iterator, bool> BST <T> ::insert(T && t, bool keepUnique)
{
   if (keepUnique)
   {
      auto it = this->find(t);
      if (it != this->end())
         return std::pair<iterator, bool>(it, false);
   }

   auto newNode = new BNode(std::move(t));

   // If the root is nullptr, set the root to the new node.
   if (this->root == nullptr)
   {
      this->root = newNode;
      this->root->isRed = false;
      this->numElements++;
      return std::pair<iterator, bool>(newNode, true);
   }

   // Set the current node to the root.
   auto current = this->root;
   bool found = false;

   while (!found)
   {
      // Find where to insert the new node.
      if (current->isLeftChild(newNode))
      {
         if (current->pLeft != nullptr)
            current = current->pLeft;
         else
         {
            current->addLeft(newNode);
            found = true;
         }
      }
      else
      {
         if (current->pRight != nullptr)
            current = current->pRight;
         else
         {
            current->addRight(newNode);
            found = true;
         }
      }
   }

   newNode->balance(); // balance from inserted node
   if (newNode->pParent == nullptr)
      this->root = newNode;
   else if (newNode->pParent->pParent == nullptr)
      this->root = newNode->pParent;

   this->numElements++;
   std::pair<iterator, bool> pairReturn(newNode, true);
   return pairReturn;
}

/*************************************************
 * BST :: ERASE
 * Remove a given node as specified by the iterator
 ************************************************/
template <typename T>
typename BST <T> ::iterator BST <T> :: erase(iterator & it)
{
   if (it == end())
      return end();

   // Case 1: No children
   if (it.pNode->pLeft == nullptr && it.pNode->pRight == nullptr)
   {
      // Store the parent for return.
      auto pParent = it.pNode->pParent;
      // If the removed node is the root.
      if (it.pNode->pParent == nullptr)
         this->root = nullptr;
      // If the removed node is a left child.
      else if (it.pNode->pParent->pLeft == it.pNode)
         it.pNode->pParent->pLeft = nullptr;
      // If the removed node is a right child.
      else
         it.pNode->pParent->pRight = nullptr;
      // Delete the node and decrement the number of elements.
      delete it.pNode;
      it.pNode = nullptr;
      it.pNode = pParent;
      this->numElements--;
      // Return the parent.
      return it;
   }

   // Case 2: One child
   else if ((it.pNode->pLeft == nullptr && it.pNode->pRight != nullptr)
      || (it.pNode->pLeft != nullptr && it.pNode->pRight == nullptr))
   {
      // If the removed node is the root.
      if (it.pNode->pParent == nullptr)
      {
         // If left child is present.
         if (it.pNode->pLeft != nullptr)
         {
            this->root = it.pNode->pLeft;
         }
         // If right child is present.
         else
         {
            this->root = it.pNode->pRight;
         }
         // Set the child node's parent to
         // nullptr since it will be the new
         // root.
         this->root->pParent = nullptr;
      }
      // If the removed node is a left child.
      else if (it.pNode->pParent->pLeft == it.pNode)
      {
         // If the left child is present.
         if (it.pNode->pLeft != nullptr)
         {
            it.pNode->pParent->pLeft = it.pNode->pLeft;
            it.pNode->pLeft->pParent = it.pNode->pParent;
         }
         // If the right child is present.
         else
         {
            it.pNode->pParent->pLeft = it.pNode->pRight;
            it.pNode->pRight->pParent = it.pNode->pParent;
         }
      }
      // If the removed node is right child.
      else
      {
         // If the left node is present.
         if (it.pNode->pLeft != nullptr)
         {
            it.pNode->pParent->pRight = it.pNode->pLeft;
            it.pNode->pLeft->pParent = it.pNode->pParent;
         }
         // If the right node is present.
         else
         {
            it.pNode->pParent->pRight = it.pNode->pRight;
            it.pNode->pRight->pParent = it.pNode->pParent;
         }
      }
      delete it.pNode;
      it.pNode = nullptr;
      this->numElements--;
      // Return the first element in the tree.
      return begin();
   }

   // Case 3: Two Children
   else
   {
      // Find in order successor.
      auto pTemp = it.pNode->pRight;
      // Stop once we find the leftmost node in the right branch.
      while (pTemp->pLeft != nullptr)
      {
         pTemp = pTemp->pLeft;
      }

      // Check if our in-order successor has a right child.
      if (pTemp->pRight)
      {
         // If the our ios' parent is not the removed node,
         // set the right child's parent to ios' parent and
         // ios' parent's left to ios' right child.
         if (pTemp->pParent != it.pNode)
         {
            pTemp->pRight->pParent = pTemp->pParent;
            pTemp->pParent->pLeft = pTemp->pRight;
         }
      }

      // Place the ios in the removed node's spot.
      pTemp->pParent = it.pNode->pParent;
      if (it.pNode == it.pNode->pParent->pLeft)   // If removed node is a left child.
         it.pNode->pParent->pLeft = pTemp;
      else                                // If removed node is a right child.
         it.pNode->pParent->pLeft = pTemp;

      // Set ios' left child.
      pTemp->pLeft = it.pNode->pLeft;
      // If left child is not nullptr,
      // set it.pNode's parent to ios.
      if (it.pNode->pLeft)
         it.pNode->pLeft->pParent = pTemp;

      // If our right node is not ios,
      // set ios' right child to right node.
      if (it.pNode->pRight != pTemp)
      {
         pTemp->pRight = it.pNode->pRight;
         it.pNode->pRight->pParent = pTemp;
      }

      delete it.pNode;
      it.pNode = nullptr;
      it.pNode = pTemp;
      this->numElements--;
      // Return the ios.
      return it;
   }
}

/*****************************************************
 * BST :: CLEAR
 * Removes all the BNodes from a tree
 ****************************************************/
template <typename T>
void BST <T> ::clear() noexcept
{
   clearNode(root);
   numElements = 0;
}

/*****************************************************
 * BST :: BEGIN
 * Return the first node (left-most) in a binary search tree
 ****************************************************/
template <typename T>
typename BST <T> :: iterator custom :: BST <T> :: begin() const noexcept
{
   if (this->root == nullptr)
      return end();
   auto current = this->root;
   while (current->pLeft != nullptr)
      current = current->pLeft;
   return iterator(current);
}


/****************************************************
 * BST :: FIND
 * Return the node corresponding to a given value
 ****************************************************/
template <typename T>
typename BST <T> :: iterator BST<T> :: find(const T & t)
{
   if (this->root == nullptr)
      return end();

   auto current = this->root;
   while (current != nullptr)
   {
      if (current->data == t)
         return iterator(current);
      else if (current->data < t)
         current = current->pRight;
      else
         current = current->pLeft;
   }

   return end();
}

/******************************************************
 ******************************************************
 ******************************************************
 *********************** B NODE ***********************
 ******************************************************
 ******************************************************
 ******************************************************/

 
/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addLeft (BNode * pNode)
{
   if (pNode != nullptr)
      pNode->pParent = this;
   this->pLeft = pNode;
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addRight (BNode * pNode)
{
   if (pNode != nullptr)
      pNode->pParent = this;
   this->pRight = pNode;
}

/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST<T> :: BNode :: addLeft (const T & t)
{
   BST<T>::BNode* pNode = new BST<T>::BNode(t);
   pNode->pParent = this;
   this->pLeft = pNode;
}

/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST<T> ::BNode::addLeft(T && t)
{
   BST<T>::BNode* pNode = new BST<T>::BNode(std::move(t));
   pNode->pParent = this;
   this->pLeft = pNode;
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addRight (const T & t)
{
   BST<T>::BNode* pNode = new BST<T>::BNode(t);
   pNode->pParent = this;
   this->pRight = pNode;
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> ::BNode::addRight(T && t)
{
   BST<T>::BNode* pNode = new BST<T>::BNode(std::move(t));
   pNode->pParent = this;
   this->pRight = pNode;
}

/*****************************************************
 * DELETE BINARY TREE
 * Delete all the nodes below pThis including pThis
 * using postfix traverse: LRV
 ****************************************************/
template <typename T>
void BST<T>::clearNode(BNode*& pThis)
{
   if (pThis == nullptr)
      return;
   clearNode(pThis->pLeft);
   clearNode(pThis->pRight);
   delete pThis;
   pThis = nullptr;
}

/**********************************************
 * assign
 * copy the values from pSrc onto pDest preserving
 * as many of the nodes as possible.
 *********************************************/
template <typename T>
void BST<T>::assign(BNode*& pDest, const BNode* pSrc)
{
   // If source is nullptr, clear and return.
   if (pSrc == nullptr)
   {
      clearNode(pDest);
      return;
   }

   // If destination is empty, create a new node
   // and copy the rest.
   if (pDest == nullptr && pSrc != nullptr)
   {
      pDest = new BNode(pSrc->data);
      pDest->isRed = pSrc->isRed;

      assign(pDest->pLeft, pSrc->pLeft);
      if (pDest->pLeft != nullptr)
         pDest->pLeft->pParent = pDest;
      assign(pDest->pRight, pSrc->pRight);
      if (pDest->pRight != nullptr)
         pDest->pRight->pParent = pDest;
      return;
   }

   if (pDest != nullptr && pSrc != nullptr)
   {
         pDest->data = pSrc->data;
         pDest->isRed = pSrc->isRed;
         assign(pDest->pRight, pSrc->pRight);
         if (pDest->pRight != nullptr)
            pDest->pRight->pParent = pDest;
         assign(pDest->pLeft, pSrc->pLeft);
         if (pDest->pLeft != nullptr)
            pDest->pLeft->pParent = pDest;
   }
}

#ifdef DEBUG
/****************************************************
 * BINARY NODE :: FIND DEPTH
 * Find the depth of the black nodes. This is useful for
 * verifying that a given red-black tree is valid
 ****************************************************/
template <typename T>
int BST <T> :: BNode :: findDepth() const
{
   // if there are no children, the depth is ourselves
   if (pRight == nullptr && pLeft == nullptr)
      return (isRed ? 0 : 1);

   // if there is a right child, go that way
   if (pRight != nullptr)
      return (isRed ? 0 : 1) + pRight->findDepth();
   else
      return (isRed ? 0 : 1) + pLeft->findDepth();
}

/****************************************************
 * BINARY NODE :: VERIFY RED BLACK
 * Do all four red-black rules work here?
 ***************************************************/
template <typename T>
bool BST <T> :: BNode :: verifyRedBlack(int depth) const
{
   bool fReturn = true;
   depth -= (isRed == false) ? 1 : 0;

   // Rule a) Every node is either red or black
   assert(isRed == true || isRed == false); // this feels silly

   // Rule b) The root is black
   if (pParent == nullptr)
      if (isRed == true)
         fReturn = false;

   // Rule c) Red nodes have black children
   if (isRed == true)
   {
      if (pLeft != nullptr)
         if (pLeft->isRed == true)
            fReturn = false;

      if (pRight != nullptr)
         if (pRight->isRed == true)
            fReturn = false;
   }

   // Rule d) Every path from a leaf to the root has the same # of black nodes
   if (pLeft == nullptr && pRight && nullptr)
      if (depth != 0)
         fReturn = false;
   if (pLeft != nullptr)
      if (!pLeft->verifyRedBlack(depth))
         fReturn = false;
   if (pRight != nullptr)
      if (!pRight->verifyRedBlack(depth))
         fReturn = false;

   return fReturn;
}


/******************************************************
 * VERIFY B TREE
 * Verify that the tree is correctly formed
 ******************************************************/
template <typename T>
std::pair <T, T> BST <T> :: BNode :: verifyBTree() const
{
   // largest and smallest values
   std::pair <T, T> extremes;
   extremes.first = data;
   extremes.second = data;

   // check parent
   if (pParent)
      assert(pParent->pLeft == this || pParent->pRight == this);

   // check left, the smaller sub-tree
   if (pLeft)
   {
      assert(!(data < pLeft->data));
      assert(pLeft->pParent == this);
      pLeft->verifyBTree();
      std::pair <T, T> p = pLeft->verifyBTree();
      assert(!(data < p.second));
      extremes.first = p.first;

   }

   // check right
   if (pRight)
   {
      assert(!(pRight->data < data));
      assert(pRight->pParent == this);
      pRight->verifyBTree();

      std::pair <T, T> p = pRight->verifyBTree();
      assert(!(p.first < data));
      extremes.second = p.second;
   }

   // return answer
   return extremes;
}

/*********************************************
 * COMPUTE SIZE
 * Verify that the BST is as large as we think it is
 ********************************************/
template <typename T>
int BST <T> :: BNode :: computeSize() const
{
   return 1 +
      (pLeft  == nullptr ? 0 : pLeft->computeSize()) +
      (pRight == nullptr ? 0 : pRight->computeSize());
}
#endif // DEBUG

/******************************************************
 * BINARY NODE :: BALANCE
 * Balance the tree from a given location
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: balance()
{
   BNode* pParent = this->pParent;

   // Case 1: if we are the root, then color ourselves black and call it a day.
   if (!this->pParent)
   {
      this->isRed = false;
      return;
   }

   // Case 2: if the parent is black, then there is nothing left to do
   if (!this->pParent->isRed)
      return;

   // Case 3: If parent is red, grandparent is black, and aunt is red and exists
   // then recolor.
   BNode* pGranny = this->pParent->pParent;
   if (!pGranny) return; // If granny is null, none of the other operations will work.
   BNode* pAunt = (pGranny->pLeft == pParent) ? pGranny->pRight : pGranny->pLeft;

   if (pParent->isRed && !pGranny->isRed &&
      pAunt != nullptr && pAunt->isRed)
  {
      pParent->isRed = false;
      pAunt->isRed = false;
      if (pGranny->pParent != nullptr)
         pGranny->isRed = true;

      // Balance granny in case it's parent is red.
      // If it's the root or it's parent is black,
      // this will have no effect.
      pGranny->balance();

      return;
   }

   // Case 4: if the aunt is black or non-existent, then we need to rotate
   if (pParent->isRed && !pGranny->isRed &&
      pAunt == nullptr || !pAunt->isRed)
   {
      // Case 4a: We are mom's left and mom is granny's left
      if (this == pParent->pLeft && pParent == pGranny->pLeft)
      {
         // In this case the sibling is to the right.
         BNode* pSibling = this->pParent->pRight;
         pParent->pRight = pGranny;
         pGranny->pLeft = pSibling;
         pParent->pParent = pGranny->pParent;
         pGranny->pParent = pParent;

         // If parent is not the root, then we need to update the grandparent's
         // child pointer to point to the parent.
         if (pParent->pParent != nullptr)
            if (pParent->pParent->pLeft == pGranny)
               pParent->pParent->pLeft = pParent;
            else
               pParent->pParent->pRight = pParent;

         // Recolor.
         pParent->isRed = false;
         pGranny->isRed = true;
      }
      // case 4b: We are mom's right and mom is granny's right
      if (this == pParent->pRight && pParent == pGranny->pRight)
      {
         // In this case the sibling is to the left.
         BNode* pSibling = this->pParent->pLeft;
         pParent->pLeft = pGranny;
         pGranny->pRight = pSibling;
         pParent->pParent = pGranny->pParent;
         pGranny->pParent = pParent;

         // If parent is not the root, then we need to update the grandparent's
         // child pointer to point to the parent.
         if (pParent->pParent != nullptr)
            if (pParent->pParent->pLeft == pGranny)
               pParent->pParent->pLeft = pParent;
            else
               pParent->pParent->pRight = pParent;

         // Recolor.
         pParent->isRed = false;
         pGranny->isRed = true;
      }
      // Case 4c: We are mom's right and mom is granny's left
      if (this == pParent->pRight && pParent == pGranny->pLeft)
      {
         // Distribute N's children.
         pGranny->pLeft = this->pRight;
         pParent->pRight = this->pLeft;
         // Set N's children's parents to be their new parents
         // if they are not nullptr.
         if (this->pRight != nullptr)
            this->pRight->pParent = pGranny;
         if (this->pLeft != nullptr)
            this->pLeft->pParent = pParent;

         // Set granny to be right of this.
         this->pRight = pGranny;
         // Set parent of this to be granny's parent.
         this->pParent = pGranny->pParent;
         // Set granny's parent to this.
         pGranny->pParent = this;

         // Set parent's parent to this.
         pParent->pParent = this;
         // Set this' left to parent.
         this->pLeft = pParent;

         // Recolor.
         this->isRed = false;
         pGranny->isRed = true;
      }
      // case 4d: We are mom's left and mom is granny's right
      if (this == pParent->pLeft && pParent == pGranny->pRight)
      {
         // Distribute N's children
         pGranny->pRight = this->pLeft;
         pParent->pLeft = this->pRight;
         // Set N's children's parents to be their new parents
         // if they are not nullptr.
         if (this->pLeft != nullptr)
            this->pLeft->pParent = pGranny;
         if (this->pRight != nullptr)
            this->pRight->pParent = pParent;

         // If pGranny is the root, parent's parent needs to be nullptr.
         if (pGranny->pParent == nullptr)
            this->pParent = nullptr;
         // Otherwise, set this' parent to be granny's parent.
         else if (pGranny->pParent->pRight == pGranny)
            pGranny->pParent->pRight = this;
         else
            pGranny->pParent->pLeft = this;

         // Set granny to be left of this.
         this->pLeft = pGranny;
         // Set right of this to be parent.
         this->pRight = pParent;
         // Set granny's parent to this.
         pGranny->pParent = this;
         // Set parent's parent to this.
         pParent->pParent = this;
         // Recolor.
         pGranny->isRed = true;
         this->isRed = false;
      }
   }
}

/*************************************************
 *************************************************
 *************************************************
 ****************** ITERATOR *********************
 *************************************************
 *************************************************
 *************************************************/     

/**************************************************
 * BST ITERATOR :: INCREMENT PREFIX
 * advance by one
 *************************************************/
template <typename T>
typename BST <T> :: iterator & BST <T> :: iterator :: operator ++ ()
{
   // If there is no node, return.
   if (pNode == nullptr)
      return *this;

   // If there is a right node, go right, then left as far as possible.
   if (pNode->pRight != nullptr)
   {
      pNode = pNode->pRight;
      while (pNode->pLeft != nullptr)
         pNode = pNode->pLeft;
      return *this;
   }

   BNode* pSave = pNode;
   pNode = pNode->pParent;
   if (pNode == nullptr)
      return *this;

   if (pSave == pNode->pLeft)
      return *this;

   while (pNode != nullptr && pSave == pNode->pRight)
   {
      pSave = pNode;
      pNode = pNode->pParent;
   }

   return *this;
}

/**************************************************
 * BST ITERATOR :: DECREMENT PREFIX
 * advance by one
 *************************************************/
template <typename T>
typename BST <T> :: iterator & BST <T> :: iterator :: operator -- ()
{
   // If there is no node, return.
   if (pNode == nullptr)
      return *this;

   // If there is a left node, go left, then as far right as possible.
   if (pNode->pLeft != nullptr)
   {
      pNode = pNode->pLeft;
      while (pNode->pRight != nullptr)
         pNode = pNode->pRight;
      return *this;
   }

   BNode* pSave = pNode;
   pNode = pNode->pParent;
   if (pNode == nullptr)
      return *this;

   if (pSave == pNode->pRight)
      return *this;

   while (pNode != nullptr && pSave == pNode->pLeft)
   {
      pSave = pNode;
      pNode = pNode->pParent;
   }

   return *this;
}


} // namespace custom


