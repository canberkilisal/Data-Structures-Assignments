// Lütfi Canberk Ilýsal

#include "BinarySearchTree.h"
#include "HashTable.h"
#include "BinarySearch.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#include <chrono>

using namespace std;


void StrOrganizer(string & str) {

	int len = str.length();
	for (int i = 0; i < len; i++) {
		if (!isalpha(str[i])) {
			str[i] = ' ';
		}
		else {
			str[i] = tolower(str[i]);
		}
	}
}


int main() {

	int num;
	vector<string> filenames;

	vector<WordItem> mainVector;

	BinarySearchTree<string, WordItem*> myTree;
	HashTable<string, WordItem*> myTable;


	cout << "Enter number of input files: ";
	cin >> num;

	for (int i = 0; i < num; i++) {

		string filename;

		cout << "Enter " << i + 1 << ". file name : ";
		cin >> filename;

		filenames.push_back(filename);
	}

	for (int i = 0; i < filenames.size(); i++) {

		ifstream inputFile(filenames[i]);

		stringstream buffer;
		buffer << inputFile.rdbuf();
		string fileContent = buffer.str();
		StrOrganizer(fileContent);

		istringstream iss(fileContent);

		string word;
		while (iss >> word) {

			// adding words to BST
			if (myTree.contains(word)) {

				bool isFileExist = false;
				WordItem * element = myTree.retrieve(word);

				for (int j = 0; j<element->info.size(); j++) {

					if (filenames[i] == element->info[j].documentName) {

						element->info[j].count += 1;
						isFileExist = true;
					}
				}

				if (!isFileExist) {

					DocumentItem docWord(filenames[i], 1);
					element->info.push_back(docWord);
				}

			}

			else {

				DocumentItem docWord(filenames[i], 1);
				WordItem * element = new WordItem(word);
				element->info.push_back(docWord);
				myTree.insert(word, element);
			}

			// adding words to HashTable
			if (myTable.contains(word)) {

				bool isFileExist = false;
				WordItem * element = myTable.find(word);

				for (int k = 0; k<element->info.size(); k++) {

					if (filenames[i] == element->info[k].documentName) {

						element->info[k].count += 1;
						isFileExist = true;
					}
				}

				if (!isFileExist) {

					DocumentItem docWord(filenames[i], 1);
					element->info.push_back(docWord);
				}

			}

			else {

				DocumentItem docWord(filenames[i], 1);
				WordItem * element = new WordItem(word);
				element->info.push_back(docWord);
				myTable.insert(word, element);
			}

			// adding words to vector
			int vectorPos = isExists(mainVector, word);
			if (vectorPos != -1) {

				bool isFileExist = false;
				WordItem element = mainVector[vectorPos];

				for (int k = 0; k<element.info.size(); k++) {

					if (filenames[i] == element.info[k].documentName) {

						mainVector[vectorPos].info[k].count += 1;
						isFileExist = true;
					}
				}

				if (!isFileExist) {

					DocumentItem docWord(filenames[i], 1);
					mainVector[vectorPos].info.push_back(docWord);
				}

			}

			else {
				
				DocumentItem docWord(filenames[i], 1);
				WordItem element(word);
				element.info.push_back(docWord);
				mainVector.push_back(element);

			}
		}
	}

	int k = 100;
	
	// sorting vector with quick sort algorithm with random element as pivot
	auto start = std::chrono::high_resolution_clock::now();
	vector<WordItem> quickSortRandomVector = mainVector;
	quickSortRandom(quickSortRandomVector);
	auto endQuickSortRandom = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start);

	// sorting vector with quick sort algorithm with median3 pivot
	start = std::chrono::high_resolution_clock::now();
	vector<WordItem> quickSortMedianVector = mainVector;
	quickSortMedian(quickSortMedianVector);
	auto endQuickSortMedian = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start);
	
	// sorting vector with quick sort algorithm with first element as pivot
	start = std::chrono::high_resolution_clock::now();
	vector<WordItem> quickSortFirstVector = mainVector;
	quickSortFirst(quickSortFirstVector);
	auto endQuickSortFirst = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start);

	// sorting vector with insertion sort algorithm
	start = std::chrono::high_resolution_clock::now();
	vector<WordItem> insertionSortVector = mainVector;
	insertionSort(insertionSortVector);
	auto endInsertionSort = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start);

	// sorting vector with merge sort algorithm
	start = std::chrono::high_resolution_clock::now();
	vector<WordItem> mergeSortVector = mainVector;
	mergeSort(mergeSortVector);
	auto endMergeSort = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start);

	// sorting vector with heap sort algorithm
	start = std::chrono::high_resolution_clock::now();
	vector<WordItem> heapSortVector = mainVector;
	heapSort(heapSortVector);
	auto endHeapSort = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start);

	myTable.printInfo();

	string querryWords;

	cout << "Enter queried words in one line: ";

	getline(cin >> ws, querryWords);

	StrOrganizer(querryWords);

	// searching with BST
	start = std::chrono::high_resolution_clock::now();
	for (int m = 0; m < k; m++) {

		int fileCount = 0;

		for (int i = 0; i < filenames.size(); i++) {

			string outBST;
			int count = 0;
			int countCheckBST = 0;

			string word;
			istringstream issBST(querryWords);

			while (issBST >> word) {

				count += 1;

				WordItem * elementBST = myTree.retrieve(word);

				if (elementBST != nullptr) {
					for (int k = 0; k < elementBST->info.size(); k++) {
						if (filenames[i] == elementBST->info[k].documentName) {

							string wordName = elementBST->word;
							int wordCount = elementBST->info[k].count;
							outBST = outBST + ", " + wordName + " found " + to_string(wordCount) + " times";
							countCheckBST += 1;
						}

					}
				}
			}
			if (count == countCheckBST && m == 0) {

				cout << "in Document " << filenames[i] << outBST << "." << endl;
				fileCount++;
			}

		}
		if (fileCount == 0 && m == 0) {

			cout << "No document contains the given query" << endl;
		}
	}
	auto endBST = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start);


	// searching with HashTable
	start = std::chrono::high_resolution_clock::now();
	for (int m = 0; m < k; m++) {

		int fileCount = 0;

		for (int i = 0; i < filenames.size(); i++) {

			string outHash;
			int count = 0;
			int countCheckHash = 0;

			string word;
			istringstream issHash(querryWords);

			while (issHash >> word) {

				count += 1;

				WordItem * elementHash = myTable.find(word);

				if (elementHash != nullptr) {
					for (int k = 0; k < elementHash->info.size(); k++) {
						if (filenames[i] == elementHash->info[k].documentName) {

							string wordName = elementHash->word;
							int wordCount = elementHash->info[k].count;
							outHash = outHash + ", " + wordName + " found " + to_string(wordCount) + " times";
							countCheckHash += 1;
						}

					}
				}
			}
			if (count == countCheckHash && m == 0) {

				cout << "in Document " << filenames[i] << outHash << "." << endl;
				fileCount++;
			}
		}
		if (fileCount == 0 && m == 0) {

			cout << "No document contains the given query" << endl;
		}
	}
	auto endHT = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start);
	
	// search with BinarySearch
	start = std::chrono::high_resolution_clock::now();
	for (int m = 0; m < k; m++) {
	
		int fileCount = 0;

		for (int i = 0; i < filenames.size(); i++) {

			string outBS;
			int count = 0;
			int countCheckBS = 0;

			string word;
			istringstream issHash(querryWords);

			while (issHash >> word) {

				count += 1;

				int pos = binarySearch(quickSortMedianVector, word);
				

				if (pos != -1) {

					WordItem elementBS = quickSortMedianVector[pos];

					for (int k = 0; k < elementBS.info.size(); k++) {
						if (filenames[i] == elementBS.info[k].documentName) {

							string wordName = elementBS.word;
							int wordCount = elementBS.info[k].count;
							outBS = outBS + ", " + wordName + " found " + to_string(wordCount) + " times";
							countCheckBS += 1;
						}

					}
				}
			}
			if (count == countCheckBS && m == 0) {

				cout << "in Document " << filenames[i] << outBS << "." << endl;
				fileCount++;
			}
		}
		if (fileCount == 0 && m == 0) {

			cout << "No document contains the given query" << endl;
		}
	}
	auto endBS = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start);

	cout << "\nBinary Search Tree Time: " << endBST.count() / k << endl;
	cout << "Hash Table Time: " << endHT.count() / k << endl;
	cout << "Binary Search Time: " << endBS.count() / k << endl;

	cout << "\nQuick Sort(median) Time: " << endQuickSortMedian.count() / k << endl;
	cout << "Quick Sort(random) Time: " << endQuickSortRandom.count() / k << endl;
	cout << "Quick Sort(first) Time: " << endQuickSortFirst.count() / k << endl;
	cout << "Merge Sort Time: " << endMergeSort.count() / k << endl;
	cout << "Heap Sort Time: " << endHeapSort.count() / k << endl;
	cout << "Insertion Sort Time: " << endInsertionSort.count() / k << endl;

	cout << "\nSpeedUp BST/HST: " << double(endBST.count()) / endHT.count() << endl;
	cout << "Speed Up Merge Sort/Quick Sort(Median): " << double(endMergeSort.count()) / endQuickSortMedian.count() << endl;
	cout << "Heap Sort/Quick Sort(Median): " << double(endHeapSort.count()) / endQuickSortMedian.count() << endl;
	cout << "Insertion Sort/Quick Sort(Median): " << double(endInsertionSort.count()) / endQuickSortMedian.count() << endl;

	cout << "\nSpeed Up Binary Search Tree Time / Binary Search: " << double(endBST.count()) / endBS.count()  << endl;
	cout << "Speed Up Hash Table Time / Binary Search: " << double(endHT.count()) / endBS.count() << endl;

	return 0;
}