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

    // Load the RTree already created previously for this dimensions
    string rTree_save = "rTree_" + to_string(n) + ".txt"; 
    rTree *db = new rTree();
    db->loadTree(rTree_save);
    cout << "Tree restored from : " + rTree_save << endl;

	fstream search_file; 
    string searchfile_name = "dataset_" + to_string(n) + ".txt"; 
    search_file.open(searchfile_name.c_str(), ios::in); 
    vector<pair<int, int>> box(n, make_pair(0, 0));
    int result;
    for (int i = 0; i < 100000; i++) {
        for (int j = 0; j < n; j++) search_file >> box[j].first >> box[j].second;
        result = 0;
        db->searchTree(db->root, box, &result);
        cout << result << endl;
    }
    search_file.close();

    cout << "Tree Height : " << db->height << endl;

    delete db;

	return 0;
}