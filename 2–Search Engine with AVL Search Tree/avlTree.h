#ifndef _AVLTREE_H
#define _AVLTREE_H

#include<string>

using namespace std;


template <class Key, class Value>
class AVLSearchTree
{

public:

	AVLSearchTree();
	AVLSearchTree( const AVLSearchTree & );
	~AVLSearchTree();

	bool contains( const Key & ) const;
	bool isEmpty() const;
	const Value & retrieve( const Key & ) const;

	void makeEmpty();
	void insert( const Key&, const Value & );
	void remove( const Key & );

	AVLSearchTree & operator=( const AVLSearchTree & rhs );

private:

	struct AVLNode {
		
		Key name;
		Value element;
		AVLNode *left;
		AVLNode *right;
		int height;

		AVLNode(const Key& theName, const Value& theElement, AVLNode* lt, AVLNode* rt, int h = 0) : 
			name(theName), element(theElement), left(lt), right(rt), height(h) {}
	
	};
	
	AVLNode *root;

	void insert(const Key&, const Value&, AVLNode *& );
	void remove( const Key&, AVLNode *& );
	AVLNode * findMin(AVLNode *t) const;
	AVLNode * findMax(AVLNode *t) const;
	bool contains( const Key& , AVLNode * ) const;
	const Value & retrieve(const Key&, AVLNode *) const;
	void makeEmpty( AVLNode *& );
	AVLNode * copy( AVLNode *& ) const;
	int height( AVLNode * ) const;
	void balance( AVLNode *& );
	void rotateWithLeftChild( AVLNode *& ) const;
	void rotateWithRightChild( AVLNode *& ) const;
	void doubleWithLeftChild( AVLNode *& ) const;
	void doubleWithRightChild( AVLNode *& ) const;

};

#endif _AVLTREE_H
