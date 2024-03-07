#ifndef _HASH_H
#define _HASH_H

#include<vector>
#include <iostream>

using namespace std;


template <class Key, class Value>
class HashTable
{

public:

	HashTable(int size = 53);

	HashTable(const HashTable & rhs) :
		hashArray(rhs.hashArray), currentSize(rhs.currentSize) {}

	bool contains(const Key &) const;
	const Value & find(const Key &) const;

	void makeEmpty();
	void insert(const Key &, const Value &);
	void remove(const Key &);

	enum EntryType { ACTIVE, EMPTY, DELETED };

	void printInfo() const;

private:

	struct HashEntry {

		Key name;
		Value element;
		EntryType status;

		HashEntry(const Key & k = Key{}, const Value & e = Value{}, EntryType i = EMPTY) :
			name{ k }, element{ e }, status{ i } {}
	};

	vector<HashEntry> hashArray;
	int currentSize;

	bool isActive(int) const;
	int findPos(const Key &) const;
	void rehash();

};

bool isPrime(int n) {

	if (n <= 1) {
		return false;
	}
	for (int i = 2; i * i <= n; ++i) {
		if (n % i == 0) {
			return false;
		}
	}
	return true;
}

int nextPrime(int n) {

	while (!isPrime(n)) {
		n++;
	}

	return n;
}

long long int hashh(const string & key, int tableSize) {

	long long int hashVal = 0;

	for (char ch : key) {
		hashVal = 37 * hashVal + ch;
	}

	hashVal = hashVal % tableSize;

	if (hashVal < 0) {
		hashVal = hashVal + tableSize;
	}

	return hashVal;
}

template <class Key, class Value>
HashTable<Key, Value>::HashTable(int size)
{
	hashArray.resize(nextPrime(size));
	makeEmpty();
}

template<class Key, class Value>
bool HashTable<Key, Value>::contains(const Key & x) const
{
	return isActive(findPos(x));
}

template<class Key, class Value>
const Value & HashTable<Key, Value>::find(const Key & x) const
{
	int currentPos = findPos(x);

	if (isActive(currentPos)) {
		return hashArray[currentPos].element;
	}

	return nullptr;
}

template<class Key, class Value>
void HashTable<Key, Value>::makeEmpty()
{
	currentSize = 0;

	for (int i = 0; i < hashArray.size(); i++) {
		hashArray[i].status = EMPTY;
	}
}

template<class Key, class Value>
void HashTable<Key, Value>::insert(const Key & x, const Value & val)
{
	int currentPos = findPos(x);

	if (isActive(currentPos)) {
		return;
	}

	hashArray[currentPos] = HashEntry(x, val, ACTIVE);
	currentSize++;

	double load = double(currentSize) / hashArray.size();

	if (load >= 0.9) {
		rehash();
	}
}

template<class Key, class Value>
void HashTable<Key, Value>::remove(const Key & x)
{
	int currentPos = findPos(x);

	if (isActive(currentPos)) {
		hashArray[currentPos].info = DELETED;
	}
}

template<class Key, class Value>
void HashTable<Key, Value>::printInfo() const
{
	float arraySize = hashArray.size();
	float loadRatio = currentSize / arraySize;

	cout << "After preprocessing, the unique word count is " << currentSize << ". Current load ratio is " << loadRatio << endl;
}

template<class Key, class Value>
bool HashTable<Key, Value>::isActive(int currentPos) const
{
	return hashArray[currentPos].status == ACTIVE;
}

template<class Key, class Value>
int HashTable<Key, Value>::findPos(const Key & x) const
{
	int collisionNum = 0;
	int currentPos = hashh(x, hashArray.size());

	while (hashArray[currentPos].status != EMPTY && hashArray[currentPos].name != x)
	{
		currentPos += pow(++collisionNum, 2);

		while (currentPos >= hashArray.size()) {
			currentPos -= hashArray.size();
		}
	}
	return currentPos;
}

template<class Key, class Value>
void HashTable<Key, Value>::rehash()
{
	vector<HashEntry> oldArray = hashArray;

	hashArray.resize(nextPrime(2 * oldArray.size()));
	for (int j = 0; j < hashArray.size(); j++) {
		hashArray[j].status = EMPTY;
	}

	currentSize = 0;
	for (int i = 0; i < oldArray.size(); i++) {
		if (oldArray[i].status == ACTIVE) {
			insert(oldArray[i].name, oldArray[i].element);
		}
	}
}


#endif _HASH_H


