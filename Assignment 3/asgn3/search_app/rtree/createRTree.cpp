/*
Assignment 1 : R-Tree
Team Members : Amshumaan Varma Pericherla, Pruthvi Sampath Chabathula
*/

#include "rTreeFunctions.h" // import RTree data structure and functions from other file
#include<bits/stdc++.h>

using namespace std;

int main() {
	int n, m, M;
	cout << "Enter Dimensions : ";
	cin >> n;
	M = 4096 / (4*n + 1);
	m = M / 2;

	fstream data_file; 
    string datafile_name = "dataset_" + to_string(n) + ".txt"; 
    data_file.open(datafile_name.c_str(), ios::in); 
    vector<pair<int, int>> box(n, make_pair(0, 0));
    vector<entry *> Records(100000);
    rTree* db = new rTree(n, m, M);

    for (int i = 0; i < 100000; i++) {
    	cout << i << endl;
    	Records[i] = new entry(n, i);
    	for (int j = 0; j < n; j++) data_file >> box[j].first >> box[j].second;
    	Records[i]->rect->updateMbr(box);
    	db->insertRecord(Records[i]);
    }
    cout << "------------------------" << endl;
	cout << "RTree Height : " << db->height << endl;
    data_file.close();

    // Save the RTree to a file
	string rTree_save = "rTree_" + to_string(n) + ".txt";
	db->saveTree(rTree_save);
    cout << "Tree saved in : " + rTree_save << endl;

    delete db;

	return 0;
}