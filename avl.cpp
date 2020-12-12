#include "avl.h"
#include <cstdlib>
#include <vector> 
#include <algorithm>
#include <string>
#include <cstring>
#include <stack>
#include <iostream>

using namespace std;

// initializer for AVL class
AVL :: AVL() {
    root = NULL;
}

// dummy function for insert
void AVL :: insert(string val) {

    Node *to_insert = new Node(val);
    if (root == NULL) // if the root is null, we set the inserted value to the root
        root = to_insert;
    else // otherwise, we find a place to insert the node
        insert(root, to_insert);
    
}

// recursive function for insert
void AVL :: insert(Node* start, Node* to_insert) {
    if (start == NULL) // base case. should never happen
        return;
    
    char *poss = strdup(to_insert->key.c_str());
    char *curr = strdup(start->key.c_str());

    if (strcmp(poss, curr) < 0) // inserted node has smaller (or equal) key, so go left
    {
        if(start->left == NULL)
        {
            start->left = to_insert; // make this node the left child
            to_insert->parent = start; // set the parent pointer
        }
        else // need to make recursive call
        {
            insert(start->left, to_insert);
        }
    }
    else if (strcmp(poss, curr) > 0)// inserted node has larger key, so go right
    {
        if(start->right == NULL)
        {
            start->right = to_insert; // make this node the right child
            to_insert->parent = start; // set the parent pointer
        }
        else // need to make recursive call
        {
            insert(start->right, to_insert);
        }
    }
  
    updateHeight(start); // update the height
    updateSubsize(start); // update the size of the subtree rooted at start

    // check for rotation.
    // if the balance factor is not within [-1, 1], we rotate nodes
    int l, r, ll, rr;
    l = (start->left == NULL) ? 0 : start->left->height;
    r = (start->right == NULL) ? 0 : start->right->height;
    if (l - r >= 2) { // a right rotation needs to occur
        ll = (start->left->left == NULL) ? 0 : start->left->left->height;
        rr = (start->left->right == NULL) ? 0 : start->left->right->height;
        if (ll > rr) {
        rotateRight(start);
        } else { // if the subtree is imbalanced on the inside, we rotate left then right
        rotateLeft(start->left);
        rotateRight(start);
        }
    } else if (l - r <= -2) { //  aleft rotation is needed
        ll = (start->right->left == NULL) ? 0 : start->right->left->height;
        rr = (start->right->right == NULL) ? 0 : start->right->right->height;
        if (ll > rr) { // if the subtree is imbalanced on the inside, we rotate right first then left
        rotateRight(start->right);
        rotateLeft(start);
        } else {
        rotateLeft(start);
        }
    }
  
    // free strings defined for comparison
    free(poss);
    free(curr);
}

void AVL :: updateHeight(Node* start) {

    if (start == NULL) // should never happen
    return;

    // we know we arent at the root and start is a valid node
    // we only update if the height of the parent is less than (which should be impossible)
    // or equal to the child's height
    int rchild, lchild, greater;
    rchild = (start->right == NULL) ? 0 : start->right->height;
    lchild = (start->left == NULL) ? 0 : start->left->height;
    greater = (rchild > lchild) ? rchild : lchild;
    start->height = 1 + greater;

    return;
}

void AVL :: updateSubsize(Node* start) {

    if (start == NULL) // should never happen
    return;

    // we know we arent at the root and start is a valid node
    // we only update if the subtree of the parent is less than (which should be impossible)
    // or equal to the child's height
    int rchild, lchild, sum;
    rchild = (start->right == NULL) ? 0 : start->right->subsize;
    lchild = (start->left == NULL) ? 0 : start->left->subsize;
    sum = rchild + lchild;
    start->subsize = 1 + sum;

    return;
}

void AVL :: rotateLeft(Node* start) {

    Node *tempParent = (start == root) ? NULL : start->parent; // sets a temp node depending on whether or not start is the root
    Node *leftChild = (start->right->left == NULL) ? NULL : start->right->left; // same thing as above but for the possible left child of the node right of start
    Node *right = start->right;


    start->right = leftChild;
    if (leftChild != NULL) // should never happen but here just in case
        leftChild->parent = start;

    right->left = start;
    start->parent = right;

    if (tempParent != NULL) { // if start is not the root we set the parent
        bool isLeft = (tempParent->left == NULL) ? false : (tempParent->left == start);
        right->parent = tempParent;
        if (isLeft)
            tempParent->left = start->parent;
        else
            tempParent->right = start->parent;
        
    }
    else // otherwise we modify the root
        root = start->parent;

    // we now fix the height of start and its parent
    updateHeight(start);
    updateHeight(start->parent);

    updateSubsize(start);
    updateSubsize(start->parent);
  
}

void AVL :: rotateRight(Node* start) {

    Node *tempParent = (start == root) ? NULL : start->parent; // sets a temp node depending on whether or not start is the root
    Node *rightChild = (start->left->right == NULL) ? NULL : start->left->right; // same thing as above but for the possible left child of the node right of start
    Node *left = start->left;

    start->left = rightChild;
    if (rightChild != NULL) // should never happen but here just in case
        rightChild->parent = start;

    left->right = start;
    start->parent = left;

    if (tempParent != NULL) { // if start is not the root we set the parent
        bool isLeft = (tempParent->left == NULL) ? false : (tempParent->left == start);
        left->parent = tempParent;
        if (isLeft)
            tempParent->left = start->parent;
        else
            tempParent->right = start->parent;
        
    }
    else // otherwise we modify the root
        root = start->parent;

    // we now fix the height of start and its parent
    updateHeight(start);
    updateHeight(start->parent);

    updateSubsize(start);
    updateSubsize(start->parent);

}

// dummy function for finding range
int AVL :: range(char* val1, char* val2) {
  return range(root, val1, val2);
}

// recursive function for finding range
int AVL :: range(Node* curr, char* val1, char* val2) {
  if (curr == NULL)
    return 0;
  
  char* curr_c = strdup(curr->key.c_str());
  int x, l, r;
  
  // if either val1 or val2 is 0, the lca has already been found
  // we are now trying to figure out where the upper bound ends
  if (val1 == NULL) { // this means we are trying to find all the numbers less than or equal to val2
    if (strcmp(val2, curr_c) < 0) { // if it is less than curr, go left
      free(curr_c);
      return range(curr->left, NULL, val2);
    }
    if (strcmp(val2, curr_c) > 0) { // if it is greater than curr, go right and also return the size of the subtree on the less, since its nodes must be less than val2
      free(curr_c);
      x = (curr->left == NULL) ? 0 : curr->left->subsize;
      r = range(curr->right, NULL, val2);
      return 1 + x + r;
    }
    if (strcmp(val2, curr_c) == 0) { // we have found the upper bound and return its subtree size with all of the nodes more than it removed
      free(curr_c);
      x = (curr->right == NULL) ? 0 : curr->right->subsize;
      return curr->subsize - x;
    }
  }
  
  // if either val1 or val2 is 0, the lca has already been found
  // we are now trying to figure out where the lower bound ends
  if (val2 == NULL) { // this means we are trying to find all the numbers greater than or equal to val1
    if (strcmp(val1, curr_c) > 0) { // if it is greater than curr, go right
      free(curr_c);
      return range(curr->right, val1, NULL);
    }
    if (strcmp(val1, curr_c) < 0) { // if it is less than curr, go left, and also return the size of the subtree on the right, since its nodes must be greater than val1
      free(curr_c);
      x = (curr->right == NULL) ? 0 : curr->right->subsize;
      r = range(curr->left, val1, NULL);
      return 1 + x + r;
    }
    if (strcmp(val1, curr_c) == 0) { // we have found the lower bound and return its subtree size with all of the nodes less than it removed
      free(curr_c);
      x = (curr->left == NULL) ? 0 : curr->left->subsize;
      return curr->subsize - x;
    }
  }
  
  // code that essentially tallies the nodes while looking for the lca
  if (strcmp(val2, curr_c) < 0) {
    free(curr_c);
    return range(curr->left, val1, val2);
  }
  else if (strcmp(val1, curr_c) > 0) {
    free(curr_c);
    return range(curr->right, val1, val2);
  }
  else if (strcmp(val2, curr_c) > 0 && strcmp(val1, curr_c) < 0) {
    free(curr_c);
    l = range(curr->left, val1, NULL);
    r = range(curr->right, NULL, val2);
    return 1 + l + r;
  } else if (strcmp(val2, curr_c) == 0) {
    free(curr_c);
    l = range(curr->left, val1, NULL);
    return 1 + l;
  } else if (strcmp(val1, curr_c) == 0) {
    free(curr_c);
    r = range(curr->right, NULL, val2);
    return 1 + r;
  }
  
  free(curr_c);
  return 0; // should never happen, here just in case tho
  
}

// Deletes every Node to prevent memory leaks.
// Input: None
// Output: Void, just deletes every Node of the list
void AVL :: deleteAVL()
{
    deleteAVL(root);
}

// Deletes every Node in subtree rooted at startto prevent memory leaks.
// Input: Node* start
// Output: Void, just deletes every Node of the list
void AVL :: deleteAVL(Node* start)
{
    if(start == NULL) // tree is already empty
        return; 
    deleteAVL(start->left); // delete left subtree
    deleteAVL(start->right); // delete right subtree
    delete(start); // delete node itself
}

string AVL :: printInOrder()
{
  return printInOrder(root);
}

string AVL :: printPreOrder()
{
  return printPreOrder(root);
}

string AVL :: printPostOrder()
{
  return printPostOrder(root);
}

string AVL :: printInOrder(Node* start)    
{
    if(start == NULL) // base case
        return ""; // return empty string
    string leftpart = printInOrder(start->left);
    string rightpart = printInOrder(start->right);
    string output = start->key;
    if(leftpart.length() != 0) // left part is empty
        output = leftpart + " " + output; // append left part
    if(rightpart.length() != 0) // right part in empty
        output = output + " " + rightpart; // append right part
    return output;
}

string AVL :: printPreOrder(Node* start)    
{
    if(start == NULL) // base case
        return ""; // return empty string
    string leftpart = printPreOrder(start->left);
    string rightpart = printPreOrder(start->right);
    string output = "(" + start->key + " " + to_string(start->height) + " " + to_string(start->subsize) + ")";
    if(leftpart.length() != 0) // left part is empty
        output = output + " " + leftpart; // append left part
    if(rightpart.length() != 0) // right part in empty
        output = output + " " + rightpart; // append right part
    return output;
}

string AVL :: printPostOrder(Node* start)    
{
    if(start == NULL) // base case
        return ""; // return empty string
    string leftpart = printPostOrder(start->left);
    string rightpart = printPostOrder(start->right);
    string output = start->key;
    if(rightpart.length() != 0) // right part is empty
        output = rightpart + " " + output; // append left part
    if(leftpart.length() != 0) // left part in empty
        output = leftpart + " " + output; // append right part
    return output;
}
