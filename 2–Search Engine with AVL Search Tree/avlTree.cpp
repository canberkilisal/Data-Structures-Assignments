#include "avlTree.h"
#include <vector>
#include <algorithm>

using namespace std;

static const int ALLOWED_BALANCE = 1;


struct DocumentItem {

	string documentName;
	int count;

	DocumentItem(const string& n, int c) : documentName(n), count(c) {}
};


struct WordItem {

	string word;
	vector<DocumentItem> info;

	WordItem(const string& w) : word(w) {}
};

template <class Key, class Value>
AVLSearchTree<Key, Value>::AVLSearchTree() {

	root = nullptr;
}

template <class Key, class Value>
AVLSearchTree<Key, Value>::AVLSearchTree( const AVLSearchTree & rhs ) {

	root = nullptr;
	*this = rhs;
}

template <class Key, class Value>
AVLSearchTree<Key, Value>::~AVLSearchTree() {

	makeEmpty();
}

template <class Key, class Value>
bool AVLSearchTree<Key, Value>::contains(const Key & val) const {

	return contains(val, root);
}

template <class Key, class Value>
bool AVLSearchTree<Key, Value>::isEmpty() const {

	return root == nullptr ? true : false;
}

template<class Key, class Value>
const Value & AVLSearchTree<Key, Value>::retrieve(const Key & val) const {
	
	return retrieve(val, root);
}

template<class Key, class Value>
void AVLSearchTree<Key, Value>::makeEmpty() {

	makeEmpty(root);
}

template<class Key, class Value>
void AVLSearchTree<Key, Value>::insert(const Key & x, const Value & val) {

	insert(x, val, root);
}

template<class Key, class Value>
void AVLSearchTree<Key, Value>::remove(const Key & val) {

	remove(val, root);
}

template<class Key, class Value>
AVLSearchTree<Key, Value> & AVLSearchTree<Key, Value>::operator=(const AVLSearchTree & rhs) {

	if ( rhs == nullptr ) {
		return nullptr;
	}

	makeEmpty(root);

	root = copy(rhs.root);

	return *this;
}

template<class Key, class Value>
void AVLSearchTree<Key, Value>::insert(const Key& y,const Value& x, AVLNode *& t) {

	if ( t == nullptr ) {
		t = new AVLNode(y, x, nullptr, nullptr);
	}

	else if ( y < t->name ) {
		insert(y, x, t->left);
	}

	else if ( x > t->element ) {
		insert(y, x, t->right);
	}

	balance(t);
}

template<class Key, class Value>
void AVLSearchTree<Key, Value>::remove(const Key & x, AVLNode *& t) {

	if ( t == nullptr ) {
		return;
	}
	
	if ( x < t->name ) {
		remove( x, t->left );
	}

	else if ( x > t->name ) {
		remove(x, t->right);
	}

	else if ( t->left != nullptr && t->right != nullptr) {
		t->name = findMin(t->right)->name;
		remove(t->name, t->right);
	}
	
	else {
		AVLNode *temp = t;
		t = ( t->left != nullptr ) ? t->left : t->right;
		delete temp;
	}

	balance(t);
}

template<class Key, class Value>
typename AVLSearchTree<Key, Value>::AVLNode * AVLSearchTree<Key, Value>::findMin(AVLNode * t) const
{
	if (t == nullptr) {
		
		return nullptr;
	}
	if (t->left == nullptr) {
		return t;
	 }
		 
	return findMin(t->left);
}

template<class Key, class Value>
typename AVLSearchTree<Key, Value>::AVLNode * AVLSearchTree<Key, Value>::findMax(AVLNode * t) const
{
	if (t == nullptr) {

		return nullptr;
	}
	if (t->right == nullptr) {
		return t;
	}

	return findMin(t->right);
}

template<class Key, class Value>
bool AVLSearchTree<Key, Value>::contains(const Key & x, AVLNode * t) const {

	if (t == nullptr) {
		return false;
	}

	else if (x < t->name) {
		return contains(x, t->left);
	}

	else if (x > t->name) {
		return contains(x, t->right);
	}

	else {
		return true;
	}
}

template<class Key, class Value>
const Value & AVLSearchTree<Key, Value>::retrieve(const Key & x, AVLNode * t) const {

	if (t == nullptr) {
		return nullptr;
	}

	else if (x < t->name) {
		return retrieve(x, t->left);
	}

	else if (x > t->name) {
		return retrieve(x, t->right);
	}

	else {
		return t->element;
	}
}


template<class Key, class Value>
void AVLSearchTree<Key, Value>::makeEmpty(AVLNode *& t) {

	if ( t != nullptr ) {
		makeEmpty( t->left );
		makeEmpty( t->right );
		delete t;
	}

	t = nullptr;
}

template<class Key, class Value>
typename AVLSearchTree<Key, Value>::AVLNode * AVLSearchTree<Key, Value>::copy(AVLNode *& t) const {

	if ( t == nullptr ) {
		return nullptr;
	}

	else {
		return new AVLNode{ t->element, copy( t->left ), copy( t->right ) };
	}
}

template<class Key, class Value>
int AVLSearchTree<Key, Value>::height(AVLNode * t) const {

	return t == nullptr ? -1 : t->height;
}

template<class Key, class Value>
void AVLSearchTree<Key, Value>::balance(AVLNode *& t) {

	if (t == nullptr) {
		return;
	}

	if ( height( t->left ) - height( t->right ) > ALLOWED_BALANCE ) {
		if ( height( t->left->left ) >= height( t->left->right ) ) {
			rotateWithLeftChild(t);
		}
		else {
			doubleWithLeftChild(t);
		}
	}

	else if ( height(t->right) - height(t->left) > ALLOWED_BALANCE ){
		if (height(t->right->right) >= height(t->right->left)) {
			rotateWithRightChild(t);
		}
		else {
			doubleWithRightChild(t);
		}
	}

	t->height = max( height( t->left ), height( t->right ) ) + 1;
}

template<class Key, class Value>
void AVLSearchTree<Key, Value>::rotateWithLeftChild(AVLNode *& k2) const {

	AVLNode *k1 = k2->left;
	k2->left = k1->right;
	k1->right = k2;
	k2->height = max( height( k2->left ), height( k2->right ) ) + 1;
	k1->height = max (height( k1->left ), k2->height ) + 1;
	k2 = k1;
}

template<class Key, class Value>
void AVLSearchTree<Key, Value>::rotateWithRightChild(AVLNode *& k1) const {

	AVLNode *k2 = k1->right;
	k1->right = k2->left;
	k2->left = k1;
	k1->height = max( height( k1->left ), height( k1->right ) ) + 1;
	k2->height = max( height( k2->right ), k1->height ) + 1;
	k1 = k2;
}

template<class Key, class Value>
void AVLSearchTree<Key, Value>::doubleWithLeftChild(AVLNode *& k3) const {

	rotateWithRightChild(k3->left);
	rotateWithLeftChild(k3);
}

template<class Key, class Value>
void AVLSearchTree<Key, Value>::doubleWithRightChild(AVLNode *& k1) const {

	rotateWithLeftChild(k1->right);
	rotateWithRightChild(k1);
}


