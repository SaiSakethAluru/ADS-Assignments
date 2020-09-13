#include "RTree.h"
#include <iostream>

using namespace std;

RTreeNode::RTreeNode(int m, int M, int n)
{
    // static int id = 0;
    this->m = m;
    this->M = M;
    this->n = n;
    this->pointers = vector<RTreeNode*>(M,nullptr);
    this->isLeaf = true;
    this->num_entries = 0;
    // this->node_id = id++;
    this->bounds = vector<pair<int,int> > (n);
}

RTreeNode::~RTreeNode()
{
}

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

int main()
{
    int n = 2;
    RTree* tree = new RTree(n);
    tree->insert("data.txt","n2Tree.txt");
    return 0;
}