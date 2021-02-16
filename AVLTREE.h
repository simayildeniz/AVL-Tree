#ifndef AVLTREE_H
#define AVLTREE_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

template <class Comparable>
class AvlTree;

template <class Comparable>
class AvlNode
{
	Comparable 	element;
	AvlNode   		*left;
	AvlNode   		*right;
	int        		height;

	AvlNode( const Comparable & theElement, 
		AvlNode *lt, AvlNode *rt, int h = 0 )
		: element( theElement ), left( lt ), right( rt ), height( h ) { }

	friend class AvlTree<Comparable>;
};

template <class Comparable>
class AvlTree
{
public:
	explicit AvlTree( const Comparable & notFound );
	AvlTree( const AvlTree & rhs );
	~AvlTree( );

	const Comparable & findMin( ) ;
	const Comparable & findMax( ) ;
	Comparable & find( Comparable & x ) ;
	bool isEmpty( ) ;
	void printTree( ) ;

	void makeEmpty( );
	void insert( Comparable & x );
	void remove( Comparable & x );

	const AvlTree & operator=( const AvlTree & rhs );

private:
	AvlNode<Comparable> *root;
	Comparable ITEM_NOT_FOUND;

	Comparable & elementAt( AvlNode<Comparable> *t ) ;

	void insert( Comparable & x, AvlNode<Comparable> * & t ) ;
	void remove(const Comparable & x, AvlNode<Comparable> * & t ) ;

	AvlNode<Comparable> * findMin( AvlNode<Comparable> *t ) ;
	AvlNode<Comparable> * findMax( AvlNode<Comparable> *t ) ;
	AvlNode<Comparable> * find( Comparable & x, AvlNode<Comparable> *t ) ;
	void makeEmpty( AvlNode<Comparable> * & t ) ;
	void printTree( AvlNode<Comparable> *t ) ;
	AvlNode<Comparable> * clone( AvlNode<Comparable> *t ) ;

	// Avl manipulations
	int height( AvlNode<Comparable> *t ) ;
	int max( int lhs, int rhs ) ;
	void rotateWithLeftChild( AvlNode<Comparable> * & k2 ) ;
	void rotateWithRightChild( AvlNode<Comparable> * & k1 ) ;
	void doubleWithLeftChild( AvlNode<Comparable> * & k3 ) ;
	void doubleWithRightChild( AvlNode<Comparable> * & k1 ) ;
};

template <class Comparable>
AvlTree<Comparable>::AvlTree( const Comparable & notFound ) 
	: ITEM_NOT_FOUND( notFound ), root( NULL )
{ }

//destructor
template <class Comparable>
AvlTree<Comparable>::~AvlTree( )
{
	makeEmpty( );
}

//public make empty
template <class Comparable>
void AvlTree<Comparable>::makeEmpty( )
{
	makeEmpty( root );
}

//private make empty
template <class Comparable>
void AvlTree<Comparable>::makeEmpty( AvlNode<Comparable> * & t ) 
{
	if( t != NULL )
	{
		makeEmpty( t->left );
		makeEmpty( t->right );
		delete t;
	}
	t = NULL;
}

template <class Comparable>
Comparable & AvlTree<Comparable>::elementAt( AvlNode<Comparable> *t ) 
{
	return t == NULL ? ITEM_NOT_FOUND : t->element;
}

//public find
template <class Comparable>
Comparable & AvlTree<Comparable>::find( Comparable & x ) 
{
	return elementAt( find( x, root ) );
}

//private find
template <class Comparable>
AvlNode<Comparable> * AvlTree<Comparable>::find( Comparable & x, AvlNode<Comparable> * t ) 
{
	if ( t == NULL )
		return NULL;
	else if( x < t->element )
		return find( x, t->left );
	else if( t->element < x )
		return find( x, t->right );
	else
		return t;    // Match    
}

//public findmin
template <class Comparable>
const Comparable & AvlTree<Comparable>::findMin( ) 
{
	return elementAt( findMin( root ) );
}

//private findmin
template <class Comparable>
AvlNode<Comparable> * AvlTree<Comparable>::findMin( AvlNode<Comparable> *t ) 																	
{
	if( t == NULL )
		return NULL;
	if( t->left == NULL )
		return t;
	return findMin( t->left );
}

//public findmax
template <class Comparable>
const Comparable & AvlTree<Comparable>::findMax( ) 
{
	return elementAt( findMax( root ) );
}

//private findmax
template <class Comparable> 
AvlNode<Comparable> * AvlTree<Comparable>::findMax( AvlNode<Comparable> *t ) 														
{
	if( t != NULL )
		while( t->right != NULL )
			t = t->right;
	return t;
}

//public insert
template <class Comparable>
void AvlTree<Comparable>::insert( Comparable & x )
{
	insert( x, root );
}

//private insert
template <class Comparable>
void AvlTree<Comparable>::insert( Comparable & x, AvlNode<Comparable> * & t )
{
	if ( t == NULL )
		t = new AvlNode<Comparable>( x, NULL, NULL );

	else if ( x < t->element ) {
		// X should be inserted to the left tree!
		insert( x, t->left );
		// Check if the left tree is out of balance (left subtree grew in height!)
		if ( height( t->left ) - height( t->right ) == 2 )
			if ( x < t->left->element )  // X was inserted to the left-left subtree!
				rotateWithLeftChild( t );
			else			     // X was inserted to the left-right subtree!
				doubleWithLeftChild( t );
	}

	else if( t->element < x )
	{    // Otherwise X is inserted to the right subtree
		insert( x, t->right );
		if ( height( t->right ) - height( t->left ) == 2 )
			// height of the right subtree increased
				if ( t->right->element < x )
					// X was inserted to right-right subtree
						rotateWithRightChild( t );
				else // X was inserted to right-left subtree
					doubleWithRightChild( t );
	}
	else
		;  // Duplicate; do nothing

	// update the height the node
	t->height = max( height( t->left ), height( t->right ) ) + 1;
}


/**
* Return the height of node t, or -1, if NULL.
*/
template <class Comparable>
int AvlTree<Comparable>::height( AvlNode<Comparable> *t ) 
{
	if (t == NULL)
		return -1;

	return t->height;
}

/**
* Return maximum of lhs and rhs.
*/
template <class Comparable>
int AvlTree<Comparable>::max( int lhs, int rhs ) 
{
	if (lhs > rhs)
		return lhs;

	return rhs;            
}

/**
* Rotate binary tree node with left child.
* For AVL trees, this is a single rotation for case 1.
* Update heights, then set new root.
*/
template <class Comparable>
void AvlTree<Comparable>::rotateWithLeftChild( AvlNode<Comparable> * & k2 ) 
{
	AvlNode<Comparable> *k1 = k2->left;
	k2->left = k1->right;
	k1->right = k2;
	k2->height = max( height( k2->left ), height( k2->right ) ) + 1;
	k1->height = max( height( k1->left ), k2->height ) + 1;
	k2 = k1;
}

/**
* Rotate binary tree node with right child.
* For AVL trees, this is a single rotation for case 4.
* Update heights, then set new root.
*/
template <class Comparable>
void AvlTree<Comparable>::rotateWithRightChild( AvlNode<Comparable> * & k1 ) 
{
	AvlNode<Comparable> *k2 = k1->right;
	k1->right = k2->left;
	k2->left = k1;
	k1->height = max( height( k1->left ), height( k1->right ) ) + 1;
	k2->height = max( height( k2->right ), k1->height ) + 1;
	k1 = k2;
}

/**
* Double rotate binary tree node: first left child.
* with its right child; then node k3 with new left child.
* For AVL trees, this is a double rotation for case 2.
* Update heights, then set new root.
*/
template <class Comparable>
void AvlTree<Comparable>::doubleWithLeftChild( AvlNode<Comparable> * & k3 ) 
{
	rotateWithRightChild( k3->left );
	rotateWithLeftChild( k3 );
}

/**
* Double rotate binary tree node: first right child.
* with its left child; then node k1 with new right child.
* For AVL trees, this is a double rotation for case 3.
* Update heights, then set new root.
*/
template <class Comparable>
void AvlTree<Comparable>::doubleWithRightChild( AvlNode<Comparable> * & k1 ) 
{
	rotateWithLeftChild( k1->right );
	rotateWithRightChild( k1 );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct DocumentItem
{
	string documentName;
	int count;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

class WordItem
{
public:
	//constructors
	WordItem();
	WordItem(string keyword, DocumentItem documentInfo)
	{
		vector <DocumentItem> temp;

		this->keyWord = keyword;

		temp.push_back(documentInfo);
		this->listOfDocuments = temp;
	}

	bool operator < ( const WordItem & r );
	bool operator < ( string & r )
	{
		bool check = ( this->keyWord < r );
		return check;
	}

	bool operator > ( const WordItem & r );
	bool operator > ( string & r )
	{
		bool check = ( this->keyWord > r );
		return check;
	}

	bool operator == ( const WordItem & r );
	bool operator == ( string & r )
	{
		bool check = ( this->keyWord == r );
		return check;
	}

	void addTo_listOfDocuments(DocumentItem item)
	{
		listOfDocuments.push_back(item);
	}

	void changekeyWord_as(string & s)
	{
		this->keyWord = s;
	}

	bool operator != ( const WordItem & r )
	{
	 return !((*this) == r);
	}

	vector<DocumentItem> access_listOfDocuments()
	{
		return (*this).listOfDocuments;
	}

	string access_keyWord()
	{
		return this->keyWord;
	}

private:
	string keyWord;
	vector<DocumentItem> listOfDocuments;

};

WordItem::WordItem()
{
	string emptyString =  "";
	vector <DocumentItem> myList; // initially empty
	this->keyWord = emptyString;
	this->listOfDocuments = myList;
}

bool WordItem::operator < (const WordItem & r )
{
	bool check = false;
	if ((*this).keyWord < r.keyWord)
	{
		check= true;
	}
	return check;
}

bool WordItem::operator > (const WordItem & r )
{
	bool check = false;
	if ((*this).keyWord > r.keyWord)
	{
		check= true;
	}
	return check;
}

bool WordItem::operator == (const WordItem & r )
{
	bool check = false;
	if ((*this).keyWord == r.keyWord)
	{
		check = true;
	}
	return check;
}

#endif