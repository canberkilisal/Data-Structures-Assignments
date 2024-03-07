// Lütfi Canberk Ilýsal

#include "BinarySearchTree.h"
#include "HashTable.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#include <chrono>

using namespace std;


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

		}

	}

	myTable.printInfo();

	string querryWords;

	cout << "Enter queried words in one line: ";
	
	getline(cin >> ws, querryWords);

	StrOrganizer(querryWords);

	int k = 20;

	// searching with BST
	auto start = std::chrono::high_resolution_clock::now();
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
	auto BSTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
		(std::chrono::high_resolution_clock::now() - start);


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
	auto HTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
		(std::chrono::high_resolution_clock::now() - start);

	cout << "\nTime: " << BSTTime.count() / k << "\n";
	cout << "\nTime: " << HTTime.count() / k << "\n";
	cout << "SpeedUp: " << double(BSTTime.count()) / HTTime.count() << endl;

	return 0;
}