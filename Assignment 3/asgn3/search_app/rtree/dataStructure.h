/*
Assignment 1 : R-Tree
Team Members : Amshumaan Varma Pericherla, Pruthvi Sampath Chabathula
*/

#include<bits/stdc++.h>

using namespace std;

fstream file, file1;
void printRect(vector<pair<int, int>> rect);

// underlying data structures for RTree Implementation
class rectangle;
class entry;
class node;
class rTree;

class rectangle {
public:
	int dim;
	vector<pair<int, int> > mbr;
	bool isEmpty = 1;

	rectangle(int n) {
		dim = n;
		mbr.resize(dim, make_pair(0, 0)); 
	}

	void updateMbr(vector<pair<int, int> > &childRect); // Modify MBR
	int getArea(); // get Rectangle area (perimeter used for high dimentions to avoid overflow)
};
	
// Entry in each node to point to children and store their mbr
class entry {
public:
	rectangle *rect;
	node *current = NULL; // points to current node in wich the entry is
	node *child = NULL;
	int url_id = -1;
	entry(int dim) {
		this->rect = new rectangle(dim);
	}
	entry(int dim, int url) {
		this->rect = new rectangle(dim);
		url_id = url;
	}
};

class node {
public:
	int len = 0;
	vector<entry*> curLst; 
	entry *parent = NULL;
	bool isLeaf = false;
	void addEntry(entry *newEntry);
	node* chooseChild(entry *record); // chose child based on overlap for searching
	void removeEntry(int index);
};

// RTree Data Structure
class rTree {
public:
	int dim, m, M, height;
	node *root;

	rTree() {
		this->dim = 0;
		this->m = 0;
		this->M = 0;
		this->root = NULL;
		height = 0;	
	}

	rTree(int dim, int m, int M) {
		this->dim = dim;
		this->m = m;
		this->M = M;
		this->root = NULL;
		height = 0;
	}

	int calwastage(rectangle* R1,rectangle* R2); // Helping function for quadratic split
	void pickseeds(node* old, node* nw,node* temp); // Inital spilt in quadratic split
	entry* picknext(node* old,node* temp,node* nw); // Helping function for quadratic split
	node* getLeaf(node* cur, entry *record); // Where to add new entry, get leaf with least enlargement 
	void quadSplit(node* old,node* nw); // to split the node if size > M
	void adjustTree(node* nod,entry* newRecord); // To update parent Mbr	
	void adjustTree(node* nod1, node* nod2, node* pnode); // After node split
	void searchTree(node* cur, vector<pair<int, int>> find, int *result); // Search the tree for given query 
	bool exists(node* cur, rectangle *find); // to fing if a given rectangle exists in the records
	void insertRecord(entry *newRecord); // To insert New Record
	void printTree(node *cur);
	void loadTree(string filename); // Load an Rtree from a file
	void readInto(node *cur);
	void saveTree(string filename); // Save constructre RTree to a file
	
};

// All functions writen in rTreeFunctions.h file