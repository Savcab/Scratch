#ifndef BST_H
#define BST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <utility>


#define DEBUG 0
/**
 * A templated class for a Node in a search tree.
 * The getters for parent/left/right are virtual so
 * that they can be overridden for future kinds of
 * search trees, such as Red Black trees, Splay trees,
 * and AVL trees.
 */
template <typename Key, typename Value>
class Node
{
public:
    Node(const Key& key, const Value& value, Node<Key, Value>* parent);
    virtual ~Node();

    const std::pair<const Key, Value>& getItem() const;
    std::pair<const Key, Value>& getItem();
    const Key& getKey() const;
    const Value& getValue() const;
    Value& getValue();

    virtual Node<Key, Value>* getParent() const;
    virtual Node<Key, Value>* getLeft() const;
    virtual Node<Key, Value>* getRight() const;

    void setParent(Node<Key, Value>* parent);
    void setLeft(Node<Key, Value>* left);
    void setRight(Node<Key, Value>* right);
    void setValue(const Value &value);

protected:
    std::pair<const Key, Value> item_;
    Node<Key, Value>* parent_;
    Node<Key, Value>* left_;
    Node<Key, Value>* right_;
};

/*
  -----------------------------------------
  Begin implementations for the Node class.
  -----------------------------------------
*/

/**
* Explicit constructor for a node.
*/
template<typename Key, typename Value>
Node<Key, Value>::Node(const Key& key, const Value& value, Node<Key, Value>* parent) :
    item_(key, value),
    parent_(parent),
    left_(NULL),
    right_(NULL)
{

}

/**
* Destructor, which does not need to do anything since the pointers inside of a node
* are only used as references to existing nodes. The nodes pointed to by parent/left/right
* are freed within the deleteAll() helper method in the BinarySearchTree.
*/
template<typename Key, typename Value>
Node<Key, Value>::~Node()
{

}

/**
* A const getter for the item.
*/
template<typename Key, typename Value>
const std::pair<const Key, Value>& Node<Key, Value>::getItem() const
{
    return item_;
}

/**
* A non-const getter for the item.
*/
template<typename Key, typename Value>
std::pair<const Key, Value>& Node<Key, Value>::getItem()
{
    return item_;
}

/**
* A const getter for the key.
*/
template<typename Key, typename Value>
const Key& Node<Key, Value>::getKey() const
{
    return item_.first;
}

/**
* A const getter for the value.
*/
template<typename Key, typename Value>
const Value& Node<Key, Value>::getValue() const
{
    return item_.second;
}

/**
* A non-const getter for the value.
*/
template<typename Key, typename Value>
Value& Node<Key, Value>::getValue()
{
    return item_.second;
}

/**
* An implementation of the virtual function for retreiving the parent.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getParent() const
{
    return parent_;
}

/**
* An implementation of the virtual function for retreiving the left child.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getLeft() const
{
    return left_;
}

/**
* An implementation of the virtual function for retreiving the right child.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getRight() const
{
    return right_;
}

/**
* A setter for setting the parent of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setParent(Node<Key, Value>* parent)
{
    parent_ = parent;
}

/**
* A setter for setting the left child of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setLeft(Node<Key, Value>* left)
{
    left_ = left;
}

/**
* A setter for setting the right child of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setRight(Node<Key, Value>* right)
{
    right_ = right;
}

/**
* A setter for the value of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setValue(const Value& value)
{
    item_.second = value;
}

/*
  ---------------------------------------
  End implementations for the Node class.
  ---------------------------------------
*/

/**
* A templated unbalanced binary search tree.
*/
template <typename Key, typename Value>
class BinarySearchTree
{
public:
    BinarySearchTree(); //TODO
    virtual ~BinarySearchTree(); //TODO
    virtual void insert(const std::pair<const Key, Value>& keyValuePair); //TODO
    //added for debugging purposes vv
    virtual void insert(Key k, Value v);
    //added for debugging purposes ^^
    virtual void remove(const Key& key); //TODO
    void clear(); //TODO
    bool isBalanced() const; //TODO
    void print() const;
    bool empty() const;
public:
    /**
    * An internal iterator class for traversing the contents of the BST.
    */
    class iterator  // TODO
    {
    public:
        iterator();

        std::pair<const Key,Value>& operator*() const;
        std::pair<const Key,Value>* operator->() const;

        bool operator==(const iterator& rhs) const;
        bool operator!=(const iterator& rhs) const;

        iterator& operator++();

    protected:
        friend class BinarySearchTree<Key, Value>;
        iterator(Node<Key,Value>* ptr);
        Node<Key, Value> *current_;
    };

public:
    iterator begin() const;
    iterator end() const;
    iterator find(const Key& key) const;

protected:
    // Mandatory helper functions
    Node<Key, Value>* internalFind(const Key& k) const; // TODO
    Node<Key, Value> *getSmallestNode() const;  // TODO
    static Node<Key, Value>* predecessor(Node<Key, Value>* current); // TODO
    // Note:  static means these functions don't have a "this" pointer
    //        and instead just use the input argument.

    // Provided helper functions
    virtual void printRoot (Node<Key, Value> *r) const;
    virtual void nodeSwap( Node<Key,Value>* n1, Node<Key,Value>* n2) ;

    // Add helper functions here
    void clearNodeFromTree(Node<Key, Value>* current, std::string mode);
    int balanceHelper(Node<Key, Value>* current) const; 
    static Node<Key, Value>* successor(Node<Key, Value>* current);
    Node<Key, Value>* closestFind(const Key& k) const;
    //has this so that it can be used by floorplan.cpp
public:
    virtual Value& operator[](Key k);
    virtual void erase(iterator it);

protected:
    Node<Key, Value>* root_;
    // You should not need other data members
};

/*
--------------------------------------------------------------
Begin implementations for the BinarySearchTree::iterator class.
---------------------------------------------------------------
*/

/**
* Explicit constructor that initializes an iterator with a given node pointer.
*/
template<class Key, class Value>
BinarySearchTree<Key, Value>::iterator::iterator(Node<Key,Value> *ptr)
{
    current_ = ptr;
}

/**
* A default constructor that initializes the iterator to NULL.
*/
template<class Key, class Value>
BinarySearchTree<Key, Value>::iterator::iterator()
{
    current_ = NULL;
}

/**
* Provides access to the item.
*/
template<class Key, class Value>
std::pair<const Key,Value> &
BinarySearchTree<Key, Value>::iterator::operator*() const
{
    return current_->getItem();
}

/**
* Provides access to the address of the item.
*/
template<class Key, class Value>
std::pair<const Key,Value> *
BinarySearchTree<Key, Value>::iterator::operator->() const
{
    return &(current_->getItem());
}

/**
* Checks if 'this' iterator's internals have the same value
* as 'rhs'
*/
template<class Key, class Value>
bool
BinarySearchTree<Key, Value>::iterator::operator==(
    const BinarySearchTree<Key, Value>::iterator& rhs) const
{
    // TODO
    return current_ == rhs.current_;
}

/**
* Checks if 'this' iterator's internals have a different value
* as 'rhs'
*/
template<class Key, class Value>
bool
BinarySearchTree<Key, Value>::iterator::operator!=(
    const BinarySearchTree<Key, Value>::iterator& rhs) const
{
    return !(current_ == rhs.current_);

}


/**
* Advances the iterator's location using an in-order sequencing
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator&
BinarySearchTree<Key, Value>::iterator::operator++()
{
    // TODO
    current_ = successor(current_);
    return *this;
}


/*
-------------------------------------------------------------
End implementations for the BinarySearchTree::iterator class.
-------------------------------------------------------------
*/

/*
-----------------------------------------------------
Begin implementations for the BinarySearchTree class.
-----------------------------------------------------
*/

/**
* Default constructor for a BinarySearchTree, which sets the root to NULL.
*/
template<class Key, class Value>
BinarySearchTree<Key, Value>::BinarySearchTree()
{
    root_ = NULL;
}

template<typename Key, typename Value>
BinarySearchTree<Key, Value>::~BinarySearchTree()
{
    clear();
}

/**
 * Returns true if tree is empty
*/
template<class Key, class Value>
bool BinarySearchTree<Key, Value>::empty() const
{
    return root_ == NULL;
}

template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::print() const
{
    printRoot(root_);
    std::cout << "\n";
}

/**
* Returns an iterator to the "smallest" item in the tree
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::begin() const
{
    BinarySearchTree<Key, Value>::iterator begin(getSmallestNode());
    return begin;
}

/**
* Returns an iterator whose value means INVALID
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::end() const
{
    BinarySearchTree<Key, Value>::iterator end(NULL);
    return end;
}

/**
* Returns an iterator to the item with the given key, k
* or the end iterator if k does not exist in the tree
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::find(const Key & k) const
{
    Node<Key, Value> *curr = internalFind(k);
    BinarySearchTree<Key, Value>::iterator it(curr);
    return it;
}

/**
 * For compatibility with map implementation
 * */
template<class Key, class Value>
Value& BinarySearchTree<Key, Value>::operator[](Key k){
    iterator result = find(k);
    return result.current_->getValue();
}

/**
 * For compatibility with map implementation
 * */
template<class Key, class Value>
void BinarySearchTree<Key, Value>::erase(iterator it){
    remove(it.current_->getKey());
}



/**
* An insert method to insert into a Binary Search Tree.
* The tree will not remain balanced when inserting.
*/
template<class Key, class Value>
void BinarySearchTree<Key, Value>::insert(const std::pair<const Key, Value> &keyValuePair)
{
    //case when tree is empty
    if(empty()){
        Node<Key, Value>* first = new Node<Key, Value>(keyValuePair.first, keyValuePair.second, NULL);
        root_ = first;
        return;
    }

    Node<Key, Value>* curr = root_;
    while(1){
        if(curr->getKey() < keyValuePair.first){
            if(curr->getRight() == NULL){
                break;
            }
            curr = curr->getRight();
        } else if(curr->getKey() > keyValuePair.first){
            if(curr->getLeft() == NULL){
                break;
            }
            curr = curr->getLeft();
        //if equal, then overwrite the previous value with new
        } else{
            curr->setValue(keyValuePair.second);
            return;
        }
    }
    Node<Key, Value>* ans = new Node<Key, Value>(keyValuePair.first, keyValuePair.second, curr);
    if(curr->getKey() < keyValuePair.first){
        curr->setRight(ans);
    } else {
        curr->setLeft(ans);
    }
    return;
}

/**
 * Added for debugging convienience
 * */
template <class Key, class Value>
void BinarySearchTree<Key, Value>::insert(Key k, Value v){
    std::pair<Key, Value> p = std::pair<Key, Value>(k, v);
    insert(p);
}


/**
* A remove method to remove a specific key from a Binary Search Tree.
* The tree may not remain balanced after removal.
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::remove(const Key& key)
{
    Node<Key, Value>* pos = internalFind(key);
    //case where the element doesn't exist
    if(pos == NULL){
        return;
    } else
    //case where it is a leaf node
    if(pos->getLeft() == NULL && pos->getRight() == NULL){
        //case where it is root node
        if(pos->getParent() == NULL){
            delete pos;
            root_ = NULL;
        } else {
        clearNodeFromTree(pos, "leaf");
        }
    //case where it has 1 child
    } else if(!(pos->getRight() != NULL && pos->getLeft() != NULL)){
        clearNodeFromTree(pos, "oneChild");
    //case where it has 2 children
    } else{
        Node<Key, Value>* pred = predecessor(pos);
        nodeSwap(pos, pred);
        if(DEBUG){ std::cout << "Swapped with predecessor: " << pred->getKey() << std::endl;}
        //delete the pos, which is now in pred's place
        if(pos->getRight() == NULL && pos->getLeft() == NULL){
            clearNodeFromTree(pos, "leaf");
        } else {
            clearNodeFromTree(pos, "oneChild");
        }
    }
}

/**
 * Helper function for remove()
 * clearNodeFromParent() clears a node from parent and deletes the node
 * current's parent CANNOT be NULL
 * mode is to select the mode which to delete the node:
 *  "leaf" is when the node to be deleted is a leaf node
 *  "oneChild" is when the node to be deleted has one child
 *  any other input does nothing
 * */
template<class Key, class Value>
void BinarySearchTree<Key, Value>::clearNodeFromTree(Node<Key, Value>* current, std::string mode){
    //case where current is a leaf node
    if(mode == "leaf"){
        if(DEBUG){std::cout<<"doing leaf node algo" << std::endl;}
        if(current->getParent()->getRight() == current){
            current->getParent()->setRight(NULL);
        } else {
            current->getParent()->setLeft(NULL);
        }
        delete current;
    } else if(mode == "oneChild"){
        if(DEBUG){std::cout<<"doing one child algo" << std::endl;}
        Node<Key, Value>* child;
        if(current->getLeft() != NULL){
            child = current->getLeft();
        } else {
            child = current->getRight();
        }
        //update child's parent
        child->setParent(current->getParent());
        //update current's parent to connect
        if(current->getParent() != NULL){
            if(current->getParent()->getRight() == current){
                current->getParent()->setRight(child);
            } else {
                current->getParent()->setLeft(child);
            }
        }
        //update root if necessary
        if(root_ == current){ root_ = child;}
        delete current;
    }
}


template<class Key, class Value>
Node<Key, Value>*
BinarySearchTree<Key, Value>::predecessor(Node<Key, Value>* current)
{
    //case 0: current is NULL
    if(current == NULL){
        return NULL;
    }
    
    Node<Key, Value>* temp = current;
    //case 1: the node has a LST
    if(current->getLeft() != NULL){
        //go down left, then all the way right
        temp = current->getLeft();
        while(temp->getRight() != NULL){
            temp = temp->getRight();
        }
        return temp;
    } else 
    //case 2: the node doesn't have a LST
    {
        //go up right all the way, then up left once
         while(temp->getParent() != NULL && temp->getParent()->getLeft() == temp){
             temp = temp->getParent();
         }
         if(temp->getParent() != NULL && temp->getParent()->getRight() == temp){
             temp = temp->getParent();
             return temp;
         } else
         //if doesn't have up left, then no predecessor
         {
             return NULL;
         }
    }
}

/**
 * Finds the successor of the current node in the BST
 * Used for iterator operator++ function
 * Returns NULL if no sucessor
 * */
template <class Key, class Value>
Node<Key, Value>*
BinarySearchTree<Key, Value>::successor(Node<Key, Value>* current){
    //case 0: current is NULL
    if(current == NULL){
        return NULL;
    }
    
    Node<Key, Value>* temp = current;
    //case 1: the node has a RST
    if(current->getRight() != NULL){
        //go down right, then all the way left
        temp = current->getRight();
        while(temp->getLeft() != NULL){
            temp = temp->getLeft();
        }
        return temp;
    } else 
    //case 2: the node doesn't have a RST
    {
        //go up left all the way, then up right once
         while(temp->getParent() != NULL && temp->getParent()->getRight() == temp){
             temp = temp->getParent();
         }
         if(temp->getParent() != NULL && temp->getParent()->getLeft() == temp){
             temp = temp->getParent();
             return temp;
         } else
         //if doesn't have up left, then no predecessor
         {
             return NULL;
         }
    }
}


/**
* A method to remove all contents of the tree and
* reset the values in the tree for use again.
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::clear()
{
    while(!empty()){
        if(DEBUG){std::cout << "about to delete: " << getSmallestNode()->getKey() << std::endl;}
        remove(getSmallestNode()->getKey()); 
        if(DEBUG){print();}
    }
    root_ = NULL;
}


/**
* A helper function to find the smallest node in the tree.
*/
template<typename Key, typename Value>
Node<Key, Value>*
BinarySearchTree<Key, Value>::getSmallestNode() const
{
    if(!empty()){
        Node<Key, Value>* temp = root_;
        while(temp->getLeft() != NULL){
            temp = temp->getLeft();
        }
        return temp;
    } else {
        return NULL;
    }
}

/**
* Helper function to find a node with given key, k and
* return a pointer to it or NULL if no item with that key
* exists
*/
template<typename Key, typename Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::internalFind(const Key& key) const
{
    Node<Key, Value>* found = closestFind(key);
    if(found == NULL){
        return NULL;
    }
    if(found->getKey() != key){
        return NULL;
    } else {
        return found;
    }
}

/**
 * Function that finds the closest node to the given key
 * Purpose is to find the parent of a node after the node is deleted
 * */
template<typename Key, typename Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::closestFind(const Key& key) const{
    if(empty()){
        return NULL;
    } else {
        Node<Key, Value>* temp = root_;
        while(temp->getKey() != key){
            if(temp->getKey() > key){
                if(temp->getLeft() == NULL){
                    break;
                }
                temp = temp->getLeft();
            } else {
                if(temp->getRight() == NULL){
                    break;
                }
                temp = temp->getRight();
            }
        }
        return temp;
    }
}

/**
 * Return true iff the BST is balanced.
 */
template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::isBalanced() const
{
    int h = balanceHelper(root_);
    if(h == -1){
        return false;
    } else {
        return true;
    }
}

/**
 * Recursive helper function for isBalanced()
 * height calculator with a twist: if tree is unbalanced, will return -1 for height
 * If one of its sub tree returns -1, it will return -1
 * */
template<class Key, class Value>
int BinarySearchTree<Key, Value>::balanceHelper(Node<Key, Value>* current) const{
    //base cases:
    if(current == NULL){
        return 0;
    }
    //recursive step
    int lsth = balanceHelper(current->getLeft());
    int rsth = balanceHelper(current->getRight());
    //if subtree is unbalanced, this is unbalanced too
    if(lsth == -1 || rsth == -1){
        return -1;
    } 
    //current tree is unbalnced
    if(lsth - rsth < -1 || lsth - rsth > 1){
        return -1;
    } else
    //tree is balanced
    {
        if(lsth > rsth){
            return lsth + 1;
        } else {
            return rsth + 1;
        }
    }
}


template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::nodeSwap( Node<Key,Value>* n1, Node<Key,Value>* n2)
{
    if((n1 == n2) || (n1 == NULL) || (n2 == NULL) ) {
        return;
    }
    Node<Key, Value>* n1p = n1->getParent();
    Node<Key, Value>* n1r = n1->getRight();
    Node<Key, Value>* n1lt = n1->getLeft();
    bool n1isLeft = false;
    if(n1p != NULL && (n1 == n1p->getLeft())) n1isLeft = true;
    Node<Key, Value>* n2p = n2->getParent();
    Node<Key, Value>* n2r = n2->getRight();
    Node<Key, Value>* n2lt = n2->getLeft();
    bool n2isLeft = false;
    if(n2p != NULL && (n2 == n2p->getLeft())) n2isLeft = true;


    Node<Key, Value>* temp;
    temp = n1->getParent();
    n1->setParent(n2->getParent());
    n2->setParent(temp);

    temp = n1->getLeft();
    n1->setLeft(n2->getLeft());
    n2->setLeft(temp);

    temp = n1->getRight();
    n1->setRight(n2->getRight());
    n2->setRight(temp);

    if( (n1r != NULL && n1r == n2) ) {
        n2->setRight(n1);
        n1->setParent(n2);
    }
    else if( n2r != NULL && n2r == n1) {
        n1->setRight(n2);
        n2->setParent(n1);

    }
    else if( n1lt != NULL && n1lt == n2) {
        n2->setLeft(n1);
        n1->setParent(n2);

    }
    else if( n2lt != NULL && n2lt == n1) {
        n1->setLeft(n2);
        n2->setParent(n1);

    }


    if(n1p != NULL && n1p != n2) {
        if(n1isLeft) n1p->setLeft(n2);
        else n1p->setRight(n2);
    }
    if(n1r != NULL && n1r != n2) {
        n1r->setParent(n2);
    }
    if(n1lt != NULL && n1lt != n2) {
        n1lt->setParent(n2);
    }

    if(n2p != NULL && n2p != n1) {
        if(n2isLeft) n2p->setLeft(n1);
        else n2p->setRight(n1);
    }
    if(n2r != NULL && n2r != n1) {
        n2r->setParent(n1);
    }
    if(n2lt != NULL && n2lt != n1) {
        n2lt->setParent(n1);
    }


    if(this->root_ == n1) {
        this->root_ = n2;
    }
    else if(this->root_ == n2) {
        this->root_ = n1;
    }

}

/**
 * Lastly, we are providing you with a print function,
   BinarySearchTree::printRoot().
   Just call it with a node to start printing at, e.g:
   this->printRoot(this->root_) // or any other node pointer

   It will print up to 5 levels of the tree rooted at the passed node,
   in ASCII graphics format.
   We hope it will make debugging easier!
  */

// include print function (in its own file because it's fairly long)
#include "print_bst.h"

/*
---------------------------------------------------
End implementations for the BinarySearchTree class.
---------------------------------------------------
*/

#endif