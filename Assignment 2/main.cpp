/*
Team details:
Sai Saketh Aluru - 16CS30030
K Sai Surya Teja - 16CS30015
Sasi Bhushan Seelaboyina - 16CS30032
*/

#include "RTree.h"
#include <iostream>

using namespace std;

// Constructor for RTreeNode class
RTreeNode::RTreeNode(int m, int M, int n)
{
    this->isLeaf = true;
    this->num_entries = 0;
    this->bounds = vector<pair<int,int> > (n);
}

// Constructor for RTree class
RTree::RTree(int n)
{
    this->n = n;
    this->root = nullptr;
    this->M = 4096/(4*n+1);
    this->m = this->M/2;
    this->num_nodes = 0;
}

// main function for creating RTree and storing in file
int main()
{
    int n;
    cout<<"Enter number of dimensions: ";
    cin>>n;
    RTree* tree = new RTree(n);
    tree->insert("data.txt","n50Tree.txt");
    return 0;
}