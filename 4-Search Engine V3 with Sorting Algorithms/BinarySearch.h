#ifndef _BS_H
#define _BS_H

#include<vector>
#include <iostream>
#include <random>

using namespace std;


struct DocumentItem {

	string documentName;
	int count;

	DocumentItem() {}

	DocumentItem(const string& n, int c) : documentName(n), count(c) {}

	DocumentItem(const DocumentItem& rhs) : documentName(rhs.documentName), count(rhs.count) {}
};

struct WordItem {

	string word;
	vector<DocumentItem> info;

	WordItem() {}

	WordItem(const string& w) : word(w) {}

	WordItem(const WordItem& rhs) : word(rhs.word), info(rhs.info.size()) {
		for (int i = 0; i < rhs.info.size(); i++) {

			info[i] = rhs.info[i];
		}
	}
};


int isExists(const vector<WordItem> &v, string s) {

	for (int i = 0; i < v.size(); i++) {
		if (v[i].word == s) { return i; }
	}

	return -1;

}

int generateRandomInt(int left, int right) {
	random_device rd;  
	mt19937 gen(rd()); 
	uniform_int_distribution<int> dis(left, right); 

	return dis(gen);
}

inline void swapMe(WordItem & obj1, WordItem & obj2) {
	
	WordItem tmp = obj1;
	obj1 = obj2;
	obj2 = tmp;
}

const WordItem & median3(vector<WordItem> & sortVector, int left, int right) {
	
	int center = (left + right) / 2;
	if (sortVector[center].word < sortVector[left].word)
		swapMe(sortVector[left], sortVector[center]);
	if (sortVector[right].word < sortVector[left].word)
		swapMe(sortVector[left], sortVector[right]);
	if (sortVector[right].word < sortVector[center].word)
		swapMe(sortVector[center], sortVector[right]);

	swapMe(sortVector[center], sortVector[right - 1]);
	return sortVector[right - 1];
}

void insertionSort(vector<WordItem> & sortVector, int left, int right) {
	
	for (int p = left + 1; p <= right; p++)
	{
		WordItem tmp = sortVector[p];
		int j;

		for (j = p; j > left && tmp.word < sortVector[j - 1].word; j--)
			sortVector[j] = sortVector[j - 1];
		sortVector[j] = tmp;
	}
}

void quickSortRandom(vector<WordItem> & sortVector, int left, int right) {
	
	if (left + 10 <= right)
	{
		int randomNum = generateRandomInt(left, right);
		WordItem pivot = sortVector[randomNum];
		swapMe(sortVector[randomNum], sortVector[right]);
		int i = left, j = right - 1;
		for (; ; )
		{
			while (sortVector[i].word < pivot.word) {
				
				i++;
			}

			while (pivot.word < sortVector[j].word) {
			
				j--;
			}

			if (i < j)
				swapMe(sortVector[i], sortVector[j]);
			else
				break;
		}
		swapMe(sortVector[i], sortVector[right]);
		quickSortRandom(sortVector, left, i - 1);
		quickSortRandom(sortVector, i + 1, right);
	}
	else
		insertionSort(sortVector, left, right);
}

void quickSortRandom(vector<WordItem> & sortVector) {

	quickSortRandom(sortVector, 0, sortVector.size() - 1);
}

void quickSortMedian(vector<WordItem> & sortVector, int left, int right) {
	
	if (left + 10 <= right)
	{
		WordItem pivot = median3(sortVector, left, right);
		int i = left, j = right - 1;
		for (; ; )
		{
			while (sortVector[++i].word < pivot.word) {}

			while (pivot.word < sortVector[--j].word) {}

			if (i < j)
				swapMe(sortVector[i], sortVector[j]);
			else
				break;
		}
		swapMe(sortVector[i], sortVector[right - 1]);
		quickSortMedian(sortVector, left, i - 1);
		quickSortMedian(sortVector, i + 1, right);
	}
	else 
		insertionSort(sortVector, left, right);
}

void quickSortMedian(vector<WordItem> & sortVector) {

	quickSortMedian(sortVector, 0, sortVector.size() - 1);
}

void quickSortFirst(vector<WordItem> & sortVector, int left, int right) {

	if (left + 10 <= right)
	{
		WordItem pivot = sortVector[left];

		int i = left + 1, j = right;
		for (; ; )
		{
			while (sortVector[++i].word < pivot.word) {}

			while (pivot.word < sortVector[--j].word) {}

			if (i < j)
				swapMe(sortVector[i], sortVector[j]);
			else
				break;
		}
		swapMe(sortVector[left], sortVector[j]);
		quickSortRandom(sortVector, left, j - 1);
		quickSortRandom(sortVector, j + 1, right);
	}
	else
		insertionSort(sortVector, left, right);
}

void quickSortFirst(vector<WordItem> & sortVector) {

	quickSortFirst(sortVector, 0, sortVector.size() - 1);
}

void insertionSort(vector<WordItem> & sortVector) {

	int j;
	for (int p = 1; p < sortVector.size(); p++)
	{
		WordItem tmp(sortVector[p]);
		for (j = p; j > 0 && tmp.word < sortVector[j-1].word; j--)
			sortVector[j] = sortVector[j-1];
		sortVector[j] = tmp;
	}
}

void merge(vector<WordItem> & sortVector, int left, int center, int right) {

	int start = center + 1;
	if (sortVector[center].word <= sortVector[start].word)
		return;

	while (left <= center && start <= right)
	{
		if (sortVector[left].word <= sortVector[start].word) {
			left++;
		}
		else
		{
			WordItem info = sortVector[start];
			int idx = start;

			while (idx != left) {
				sortVector[idx] = sortVector[idx - 1];
				idx--;
			}
			sortVector[left] = info;
			left++;
			center++;
			start++;
		}
	}
}

void mergeSort(vector<WordItem> & sortVector, int left, int right) {

	if (left < right)
	{
		int center = (left + right) / 2;
		mergeSort(sortVector, left, center);
		mergeSort(sortVector, center + 1, right);
		merge(sortVector, left, center, right);
	}
}

void mergeSort(vector<WordItem> & sortVector) {


	mergeSort(sortVector, 0, sortVector.size() - 1);
}

inline int leftChild(int i) {

	return 2 * i + 1;
}

void percDown(vector<WordItem> & sortVector, int i, int n) {

	int child;
	WordItem tmp;

	for (tmp = sortVector[i]; leftChild(i) < n; i = child)
	{
		child = leftChild(i);
		if (child != n - 1 && sortVector[child].word < sortVector[child + 1].word)
			child++;
		if (sortVector[child].word > tmp.word)
			sortVector[i] = sortVector[child];
		else
			break;
	}
	sortVector[i] = tmp;
}

void heapSort(vector<WordItem> & sortVector) {

	for (int i = sortVector.size() / 2; i >= 0; i--)
		percDown(sortVector, i, sortVector.size());

	for (int j = sortVector.size()-1; j > 0; j--)
	{
		swap(sortVector[0], sortVector[j]);
		percDown(sortVector, 0, j);
	}
}

int binarySearch(const vector<WordItem>& v, const string & s) {
	
	int low = 0;
	int high = v.size() - 1;

	while (low <= high) {
		
		int mid = (low + high) / 2;

		if (v[mid].word < s)
			low = mid + 1;
		else if ( v[mid].word > s )
			high = mid - 1;
		else 
			return mid;
	}

	return -1;
}


#endif _BST_H