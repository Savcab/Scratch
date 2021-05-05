#ifndef RBBST_H
#define RBBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the height as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int getHeight () const;
    void setHeight (int height);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int height_;
};


/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), height_(1)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the height of a AVLNode.
*/
template<class Key, class Value>
int AVLNode<Key, Value>::getHeight() const
{
    return height_;
}

/**
* A setter for the height of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setHeight(int height)
{
    height_ = height;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); 

    virtual void remove(const Key& key);  
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    
    // Add helper functions here
    //updates heights in the tree given the node changed
    void updateAndBalance(AVLNode<Key, Value>* changed);
    //updates a single node's height
    bool updateNodeHeight(AVLNode<Key, Value>* curr);
    void leftRotate(AVLNode<Key, Value>* curr);
    void rightRotate(AVLNode<Key, Value>* curr);
    void rebalance(AVLNode<Key, Value>* curr);
public:
    virtual void erase(Key k);

};

/**
 * Added for compatability with map implementation
 * */
template<class Key, class Value>
void AVLTree<Key, Value>::erase(Key k){
    remove(k);
}

/**
 * The insert function
 * */
template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value> &keyValuePair)
{
    //case when tree is empty
    if(BinarySearchTree<Key, Value>::empty()){
        Node<Key, Value>* first = new AVLNode<Key, Value>(keyValuePair.first, keyValuePair.second, NULL);
        BinarySearchTree<Key, Value>::root_ = first;
        return;
    }

    Node<Key, Value>* curr = BinarySearchTree<Key, Value>::root_;
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
    Node<Key, Value>* ans = new AVLNode<Key, Value>(keyValuePair.first, keyValuePair.second, static_cast<AVLNode<Key, Value>*>(curr));
    if(curr->getKey() < keyValuePair.first){
        curr->setRight(ans);
    } else {
        curr->setLeft(ans);
    }
    if(DEBUG){
        std::cout << "This is the tree after inserting and before balancing: " << std::endl;
        BinarySearchTree<Key, Value>::print();
    }
    updateAndBalance(static_cast<AVLNode<Key, Value>*>(ans));
    return;
}

template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    BinarySearchTree<Key, Value>::remove(key);
    AVLNode<Key, Value>* parent = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::closestFind(key));
    if(parent == NULL){ return;}
    if(DEBUG){
        std::cout << "removed: " << key << std::endl << "this is the tree after the remove and before rebalancing" << std::endl;
        BinarySearchTree<Key, Value>::print();
    }
    if(!updateNodeHeight(parent)){
        rebalance(parent);
    }
    updateAndBalance(parent);
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int tempH = n1->getHeight();
    n1->setHeight(n2->getHeight());
    n2->setHeight(tempH);
}


/**
 * Updates a single node's height by using the height of it's children
 * return true is the node is balaced, false if it is unbalanced
 * */
template<class Key, class Value>
bool AVLTree<Key, Value>::updateNodeHeight(AVLNode<Key, Value>* curr){
    int lsth = 0;
    int rsth = 0;
    if(curr->getLeft() != NULL){
        lsth = curr->getLeft()->getHeight();
    }
    if(curr->getRight() != NULL){
        rsth = curr->getRight()->getHeight();
    }
    if(lsth - rsth > 1 || lsth - rsth < -1){
        return false;
    }
    curr->setHeight(std::max(lsth, rsth) + 1);
    return true;
}


/**
 * updates heights in the tree given the node that was changed
 * Also balances tree while doing so
 * */
template<class Key, class Value>
void AVLTree<Key, Value>::updateAndBalance(AVLNode<Key, Value>* changed){
    if(changed->getParent() == NULL){
        return;
    }
    int ph = changed->getParent()->getHeight();
    //update parent height & if parent is still balanced
    if(updateNodeHeight(changed->getParent())){
        //if parents height didn't changed vs changed
        if(ph != changed->getParent()->getHeight()){
            updateAndBalance(changed->getParent());
        }
    //parent becomes unbalanced, balance
    } else {
        rebalance(changed->getParent());
        updateAndBalance(changed);
    }
}

/**
 * rebalances the tree by finding zig-zigs, and zig-zags, and using left and right rotate
 * GIVEN - curr is the node that is unbalanced
 * */
template<class Key, class Value>
void AVLTree<Key, Value>::rebalance(AVLNode<Key, Value>* curr){
    enum direction {left, right};
    //order[0] is the direciton where the bigger first child is (left or right)
    //order[1] is the direction where the bigger second child is (left or right)
    int order [2];

    AVLNode<Key, Value>* temp = curr;
    AVLNode<Key, Value>* bigger;
    //goes down 2 levels
    for(int i = 0 ; i < 2 ; i++){
        int lsth = 0;
        int rsth = 0;
        if(temp->getLeft() != NULL){ lsth = temp->getLeft()->getHeight();}
        if(temp->getRight() != NULL){ rsth = temp->getRight()->getHeight();}

        if(lsth > rsth){
            order[i] = left;
            bigger = temp->getLeft();
        } else {
            order[i] = right;
            bigger = temp->getRight();
        }
        temp = bigger;
    }

    //zig left zig left
    if(order[0] == left && order[1] == left){
        rightRotate(curr);

    //zig left zag right
    } else if(order[0] == left && order[1] == right){
        leftRotate(curr->getLeft());
        rightRotate(curr);

    //zig right zag left
    } else if(order[0] == right && order[1] == left){
        rightRotate(curr->getRight());
        leftRotate(curr);

    //zig right zig right
    } else if(order[0] == right && order[1] == right){
        leftRotate(curr);
    }
}

/**
 * This function does the right rotate on the node given
 * */
template<class Key, class Value>
void AVLTree<Key, Value>::rightRotate(AVLNode<Key, Value>* curr){
    //case wherre right rotate cannot be done
    if(curr->getLeft() == NULL){
        return;
    }
    if(DEBUG){ std::cout << "Doing right rotate now on: " << curr->getKey() <<  std::endl;}
    AVLNode<Key, Value>* lc = curr->getLeft();
    if(curr->getParent() != NULL){
        //parent reroute 
        if(curr->getKey() > curr->getParent()->getKey()){
            curr->getParent()->setRight(lc);
        } else {
            curr->getParent()->setLeft(lc);
        }
        lc->setParent(curr->getParent());
    }
    //curr adopts lc's rc
    curr->setLeft(lc->getRight());
    if(lc->getRight() != NULL){
        lc->getRight()->setParent(curr);
    }
    //lc becomes curr's parent
    lc->setRight(curr);
    curr->setParent(lc);
    //change root if necessary
    if(BinarySearchTree<Key, Value>::root_ == curr){
        BinarySearchTree<Key, Value>::root_ = lc;
        lc->setParent(NULL);
    }
    //update node heights
    updateNodeHeight(curr); //right leg
    updateNodeHeight(curr->getParent()); //parent
    if(DEBUG){
        std::cout << "printing tree now after right rotate: " << std::endl;
        BinarySearchTree<Key, Value>::print();
    }
}

/**
 * This function does the left rotate on the node given
 * */
template<class Key, class Value>
void AVLTree<Key, Value>::leftRotate(AVLNode<Key, Value>* curr){
    //case wherre left rotate cannot be done
    if(curr->getRight() == NULL){
        return;
    }
    if(DEBUG){ std::cout << "Doing left rotate now on: " << curr->getKey() << std::endl;}
    AVLNode<Key, Value>* rc = curr->getRight();
    if(curr->getParent() != NULL){
        //parent reroute 
        if(curr->getKey() > curr->getParent()->getKey()){
            curr->getParent()->setRight(rc);
        } else {
            curr->getParent()->setLeft(rc);
        }
        rc->setParent(curr->getParent());
    }
    //adopt rc's lc
    curr->setRight(rc->getLeft());
    if(rc->getLeft() != NULL){
        rc->getLeft()->setParent(curr);
    }
    //rc becomes curr's parent
    rc->setLeft(curr);
    curr->setParent(rc);
    //change root if necessary
    if(BinarySearchTree<Key, Value>::root_ == curr){
        BinarySearchTree<Key, Value>::root_ = rc;
        rc->setParent(NULL);
    }
    //update node heights
    updateNodeHeight(curr); //left leg
    updateNodeHeight(curr->getParent()); //parent
    if(DEBUG){
        std::cout << "printing tree now after left rotate: " << std::endl;
        BinarySearchTree<Key, Value>::print();
    }

}

#endif
