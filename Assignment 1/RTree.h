#include <iostream>
#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

#ifndef RTREE_H
#define RTREE_H

class RTreeNode
{
public:
    vector<RTreeNode*> pointers;
    int node_id;
    int num_entries;
    bool isLeaf;
    int m;
    int M;
    int n;
    vector<pair<int,int> > bounds;

// public:
    RTreeNode(int m, int M, int n);
    ~RTreeNode();
};



class RTree
{
public:
    RTreeNode *root;
    int n;
    int M;
    int m;
    int num_nodes;
// public:
    RTree(int n);
    ~RTree();
    void insert(string in_filename,string out_filename);
    RTreeNode* insertRect(RTreeNode* node, vector<pair<int,int> > &new_bounds);
    void save(string filename);
};


#endif