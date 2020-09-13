#ifndef RTREE_H
#define RTREE_H

#include <iostream>
#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

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
    void calculate_bounds(vector<pair<int,int> > &new_bounds);
};

RTreeNode::RTreeNode(int m, int M, int n)
{
    static int id = 0;
    this->m = m;
    this->M = M;
    this->n = n;
    pointers = vector<RTreeNode*>(M,nullptr);
    isLeaf = true;
    num_entries = 0;
    node_id = id++;
}

RTreeNode::~RTreeNode()
{
}

void RTreeNode::calculate_bounds(vector<pair<int,int> > &new_bounds){
    if((this->bounds).size() == 0){
        this->bounds=  new_bounds;
    }
    else{
        for(int i=0;i<this->n;i++){
            this->bounds[i].first = min(this->bounds[i].first,new_bounds[i].first);
            this->bounds[i].second = max(this->bounds[i].second,new_bounds[i].second);
        }
    }
}

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

RTree::RTree(int n)
{
    this->n = n;
    this->root = nullptr;
    this->M = 4096/(4*n+1);
    this->m = this->M/2;
    this->num_nodes = 0;
}

RTree::~RTree()
{
}
#endif