// Filename: avl.h
//
// File for the avl class

#ifndef AVL_H
#define AVL_H

#include <string>

using namespace std;

// node structure
class Node
{
	public:
		string key;
    int height, subsize;
		Node *left, *right, *parent;

		Node() // default contstructor
		{
			left = right = parent = NULL;
			height = subsize = 0;
		}

		Node(string val)
		{
			key = val;
			height = 1;
			subsize = 1;
			left = right = parent = NULL;
		}
};

class AVL
{
	private:
		Node *root;
	public:
		AVL();
		void insert(string);
		void insert(Node*, Node*);
    void updateHeight(Node*);
    void updateSubsize(Node*);
    
    void rotateLeft(Node*);
    void rotateRight(Node*);
    
    int range(char*, char*);
    int range(Node*, char*, char*);
    
    void deleteAVL(); // deletes every node to prevent memory leaks, and frees memory
    void deleteAVL(Node* start); // deletes every Node in subtree rooted at start to prevent memory leaks.
    
		string printInOrder();
    string printInOrder(Node* start);
		string printPreOrder();
		string printPreOrder(Node* start);
		string printPostOrder();
		string printPostOrder(Node* start);
};

#endif

