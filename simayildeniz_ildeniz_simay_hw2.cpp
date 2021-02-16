# include <iostream>
# include <string>
# include <vector>
# include <algorithm>
# include <fstream>
# include <sstream>
# include "AVLTREE.h"

using namespace std;

WordItem item_NOT_found;

//reads the file word by word, lowercases and removes punction in every word
void readFile_and_UpdateTree(string fileName, AvlTree<WordItem> & searchTree)
{
	vector<string> uniqueWords;
	vector<int> occurances;

	//push dummy words so that they can have miminum size of 1
	uniqueWords.push_back("simay");
	occurances.push_back(0);

	fstream inFile;
	string word;
	inFile.open(fileName.c_str());
	while (inFile >> word)
	{
		//convert all letters of the word into lowercase
		for_each(word.begin(), word.end(), [](char & c) 
		{  c = ::tolower(c); });

		//strip punctuation:
		if (word[0] == '\"')
		{
			word.erase(0,1); //start from 0th index, delete 1 char
		}// word is ready to be pushed to uniqueWords


		bool isUnique = true;
		for (int i = 0; i < uniqueWords.size(); i++)
		{
			if (uniqueWords[i] == word)
			{
				isUnique = false;
				break;
			}
		}
		if (isUnique == true)
		{
			uniqueWords.push_back(word);
			occurances.push_back(1); // it occured once.
		}
		else
		{
			auto it = find(uniqueWords.begin(), uniqueWords.end(), word); 
			//the word exists in unique words, i.e. it is not unique
			int index = distance(uniqueWords.begin(), it); 
			occurances[index]++;
		}

	}
	inFile.clear();
	inFile.seekg(0);
	inFile.close();

	//start from index 1 becasue index 0 has a dummy value in both vectors
	for (int i = 1; i < uniqueWords.size(); i++)
	{
		DocumentItem newDocument;
		newDocument.documentName = fileName;
		newDocument.count = occurances[i];

		//new node created:
		WordItem newNode = WordItem(uniqueWords[i], newDocument);

		//if there is no equal node to newly created WordItem newNode
		//insert new node
		if (searchTree.find(newNode) == item_NOT_found)
		{
			searchTree.insert(newNode);
		}
		//else, find the already existing node
		//update by push_back ing the new document name and corresponding count
		else
		{
			WordItem * alreadyExist = &searchTree.find(newNode);
			alreadyExist->addTo_listOfDocuments(newDocument);

		}
	}
}


void printList( vector<string> & toDisplay)
{
	int doesExist = 0;
	for (int i = 0; i < toDisplay.size(); i++)
	{
		if (toDisplay[i] == "No document contains the given query")
		{
			doesExist += 1;
		}
	}
	if (doesExist == toDisplay.size())
	{
		cout << "No document contains the given query" << endl;
		toDisplay.resize(0);
		return;
	}
	else
	{
		for (int x = 0; x < toDisplay.size(); x++)
		{
			if (toDisplay[x] != "No document contains the given query")
			{
				cout << toDisplay[x] << endl;
			}	
		}
		toDisplay.resize(0);
	}
}


void processQuery( AvlTree<WordItem> & searchTree, string & query, int & numOfFiles, const vector<string> fileNames)
{
	cin.ignore();
	getline(cin,query);

	string element;
	// in order to keep every word inside query, use a vector:
	vector<string> lookFor;
	//split query from white spaces:
	istringstream iss(query);
	while (iss >> element)
	{
		lookFor.push_back(element);
	}
	//convert every element to look for into lowercase:
	for (int i = 0; i < lookFor.size(); i++)
	{
		for_each(lookFor[i].begin(), lookFor[i].end(), [](char & c) 
		{  c = ::tolower(c); });
	}

	vector<WordItem> resultsVectorAVLTree;
	vector<string> displayAVLTree;
	for (int i = 0; i < lookFor.size(); i++)
	{
		WordItem temp;
		temp.changekeyWord_as(lookFor[i]);
		temp = searchTree.find(temp);

		resultsVectorAVLTree.push_back(temp);
	}

	for (int ii = 0; ii < fileNames.size(); ii++)
	{
		bool fileExist = true;
		for (int j = 0; j < resultsVectorAVLTree.size(); j++)
		{
			bool wordExist = false;
			vector<DocumentItem> WItempDIlist = resultsVectorAVLTree[j].access_listOfDocuments();
			for (int k = 0; k < WItempDIlist.size(); k++)
			{
				if (WItempDIlist[k].documentName == fileNames[ii])
				{
					wordExist = true;
					break;
				}
			}
			if (wordExist == false)
			{
				fileExist = false;
				break;
			}
		}
		if (fileExist == true)
		{
			//print
			for (int s = 0; s < resultsVectorAVLTree.size(); s++)
			{
				string toDisplay = "in Document " + fileNames[ii] + ", " + resultsVectorAVLTree[s].access_keyWord() + " found ";
				vector <DocumentItem> myList = resultsVectorAVLTree[s].access_listOfDocuments();
				for (int ss = 0; ss < myList.size() ; ss++)
				{
					if (myList[ss].documentName == fileNames[ii])
					{
						toDisplay += to_string(myList[ss].count) + " times, ";
					}
				}

				toDisplay[ toDisplay.size() - 2 ] = '.';

				displayAVLTree.push_back(toDisplay);
				//cout <<toDisplay << endl;
			}
		}
		else
		{
			//not found
			string toDisplay = "No document contains the given query";
			//cout <<  "No document contains the given query" << endl;
			displayAVLTree.push_back(toDisplay);
		}
	}
	printList(displayAVLTree);
}	

int main()
{
	AvlTree<WordItem> searchTree(item_NOT_found); //initial root is item not found

	int numOfFiles;
	cout << "Enter number of input files: ";
	cin >> numOfFiles;

	vector<string> fileNames;

	for (int i = 0; i < numOfFiles; i++)
	{
		string fName;
		cout << "Enter " << to_string(i+1) << ". file name: ";
		cin >> fName;
		fileNames.push_back(fName);

		readFile_and_UpdateTree(fName, searchTree );
		//after this function, I will have been done with the tree construction adn updates
	}

	// Query Part
	string query;
	cout << "Enter queried words in one line: ";

	processQuery(searchTree, query, numOfFiles, fileNames);

	return 0;
}