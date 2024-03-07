#ifndef _BST_H
#define _BST_H

#include <vector>
#include <algorithm>
#include <string>

using namespace std;


template <class Key, class Value>
class BinarySearchTree
{

public:

	BinarySearchTree();
	BinarySearchTree(const BinarySearchTree &);
	~BinarySearchTree();

	bool contains(const Key &) const;
	bool isEmpty() const;
	const Value & retrieve(const Key &) const;

	void makeEmpty();
	void insert(const Key &, const Value &);
	void remove(const Key &);

	BinarySearchTree & operator=(const BinarySearchTree & rhs);

private:

	struct BinaryNode {

		Key name;
		Value element;
		BinaryNode *left;
		BinaryNode *right;

		BinaryNode(const Key& theName, const Value& theElement, BinaryNode* lt, BinaryNode* rt) :
			name(theName), element(theElement), left(lt), right(rt) {}

	};

	BinaryNode *root;

	void insert(const Key&, const Value&, BinaryNode *&);
	void remove(const Key&, BinaryNode *&);
	BinaryNode * findMin(BinaryNode *t) const;
	BinaryNode * findMax(BinaryNode *t) const;
	bool contains(const Key&, BinaryNode *) const;
	const Value & retrieve(const Key&, BinaryNode *) const;
	void makeEmpty(BinaryNode *&);
	BinaryNode * copy(BinaryNode *&) const;

};


template <class Key, class Value>
BinarySearchTree<Key, Value>::BinarySearchTree() {

	root = nullptr;
}

template <class Key, class Value>
BinarySearchTree<Key, Value>::BinarySearchTree(const BinarySearchTree & rhs) {

	root = nullptr;
	*this = rhs;
}

template <class Key, class Value>
BinarySearchTree<Key, Value>::~BinarySearchTree() {

	makeEmpty();
}

template <class Key, class Value>
bool BinarySearchTree<Key, Value>::contains(const Key & val) const {

	return contains(val, root);
}

template <class Key, class Value>
bool BinarySearchTree<Key, Value>::isEmpty() const {

	return root == nullptr ? true : false;
}

template<class Key, class Value>
const Value & BinarySearchTree<Key, Value>::retrieve(const Key & val) const {

	return retrieve(val, root);
}

template<class Key, class Value>
void BinarySearchTree<Key, Value>::makeEmpty() {

	makeEmpty(root);
}

template<class Key, class Value>
void BinarySearchTree<Key, Value>::insert(const Key & x, const Value & val) {

	insert(x, val, root);
}

template<class Key, class Value>
void BinarySearchTree<Key, Value>::remove(const Key & val) {

	remove(val, root);
}

template<class Key, class Value>
BinarySearchTree<Key, Value> & BinarySearchTree<Key, Value>::operator=(const BinarySearchTree & rhs) {

	if (rhs == nullptr) {
		return nullptr;
	}

	makeEmpty(root);

	root = copy(rhs.root);

	return *this;
}

template<class Key, class Value>
void BinarySearchTree<Key, Value>::insert(const Key& y, const Value& x, BinaryNode *& t) {

	if (t == nullptr) {
		t = new BinaryNode(y, x, nullptr, nullptr);
	}

	else if (y < t->name) {
		insert(y, x, t->left);
	}

	else if (y > t->name) {
		insert(y, x, t->right);
	}
}

template<class Key, class Value>
void BinarySearchTree<Key, Value>::remove(const Key & x, BinaryNode *& t) {

	if (t == nullptr) {
		return;
	}

	if (x < t->name) {
		remove(x, t->left);
	}

	else if (x > t->name) {
		remove(x, t->right);
	}

	else if (t->left != nullptr && t->right != nullptr) {
		t->name = findMin(t->right)->name;
		remove(t->name, t->right);
	}

	else {
		BinaryNode *temp = t;
		t = (t->left != nullptr) ? t->left : t->right;
		delete temp;
	}
}

template<class Key, class Value>
typename BinarySearchTree<Key, Value>::BinaryNode * BinarySearchTree<Key, Value>::findMin(BinaryNode * t) const
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
typename BinarySearchTree<Key, Value>::BinaryNode * BinarySearchTree<Key, Value>::findMax(BinaryNode * t) const
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
bool BinarySearchTree<Key, Value>::contains(const Key & x, BinaryNode * t) const {

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
const Value & BinarySearchTree<Key, Value>::retrieve(const Key & x, BinaryNode * t) const {

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
void BinarySearchTree<Key, Value>::makeEmpty(BinaryNode *& t) {

	if (t != nullptr) {
		makeEmpty(t->left);
		makeEmpty(t->right);
		delete t;
	}

	t = nullptr;
}

template<class Key, class Value>
typename BinarySearchTree<Key, Value>::BinaryNode * BinarySearchTree<Key, Value>::copy(BinaryNode *& t) const {

	if (t == nullptr) {
		return nullptr;
	}

	else {
		return new BinaryNode{ t->element, copy(t->left), copy(t->right) };
	}
}

#endif _BST_H

