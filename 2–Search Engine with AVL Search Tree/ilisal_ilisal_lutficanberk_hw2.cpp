// Lütfi Canberk ILISAL

#include <iostream>
#include <sstream>
#include <fstream>
#include "avlTree.cpp"

using namespace std;

bool isWord(const string & s) {

	int len = s.length();

	for (int i = 0; i < len; i++) {

		if (!((s[i] >= 65 && s[i] <= 90) || (s[i] >= 97 && s[i] <= 122))) {
			
			return false;
		}
	}

	return true;
}

void ToLower(string & s)   
{
	int len = s.length();
	for (int k = 0; k < len; k++)
	{
		s[k] = tolower(s[k]);
	}
}

void StripPunc(string & word)
{
    int first = 0;
    int len = word.length();
    
    while (first < len && ispunct(word[first]))
    {    
        first++;
    }

    int last = len - 1;
    
    while(last >= 0 && ispunct(word[last]))
    {
        last--;
    }
    word = word.substr(first,last-first+1);
}

int main(){

	int num;
	string word, line;
	vector<string> filenames;
	AVLSearchTree<string, WordItem*> myTree;

	cout << "Enter number of input files: ";
	cin >> num;
	
	for (unsigned int i = 0; i < num; i++) {

		string filename;

		cout << "Enter "<< i+1 <<". file name : ";
		cin >> filename;

		filenames.push_back(filename);

		ifstream inputFile(filename);
		
		while (inputFile >> word) {

			StripPunc(word);
			if (isWord(word)) {
				
				ToLower(word);
				
				if (myTree.contains(word)) {
					
					bool isFileExist = false;
					WordItem * element = myTree.retrieve(word);
					
					for (int i=0; i<element->info.size(); i++) {

						if (filename == element->info[i].documentName) {

							element->info[i].count += 1;
							isFileExist = true;
						}
					}

					if (!isFileExist) {

						DocumentItem docWord(filename, 1);
						element->info.push_back(docWord);
					}

				}

				else {

					DocumentItem docWord(filename, 1);
					WordItem * element = new WordItem(word);
					element->info.push_back(docWord);
					myTree.insert(word, element);
				}
			}
		}
	}

	while (true) {
		
		cout << endl << "Enter queried words in one line: ";

		getline(cin >> ws, line);
		
		istringstream iss(line);
		string firstWord;
		vector<string> words;

		iss >> firstWord;
		
		if (firstWord == "ENDOFINPUT") { break; }

		else if (firstWord == "REMOVE") {

			while (iss >> word) {

				ToLower(word);
				words.push_back(word);
			}

			for (int i = 0; i < words.size(); i++) {

				myTree.remove(words[i]);
				cout << words[i] << " has been REMOVED" << endl;
			}
		}
		
		else {

			ToLower(firstWord);
			words.push_back(firstWord);

			while (iss >> word) {

				ToLower(word);
				words.push_back(word);
			}

			int fileCount = 0;

			for (int i = 0; i < filenames.size(); i++) {

				string out;
				int count = 0;

				for (int j = 0; j < words.size(); j++) {

					WordItem * element = myTree.retrieve(words[j]);

					if (element != nullptr) {

						for (int k = 0; k < element->info.size(); k++) {

							if (filenames[i] == element->info[k].documentName) {

								string wordName = element->word;
								int wordCount = element->info[k].count;
								out = out + ", " + wordName + " found " + char(wordCount + 48) + " times";
								count += 1;
							}

						}

					}
				}

				if (count == words.size()) {

					cout << "in Document " << filenames[i] << out << "." << endl;
					fileCount += 1;
				}
			}

			if (fileCount == 0) {

				cout << "No document contains the given query" << endl;
			}
		}
	}
	return 0;
}