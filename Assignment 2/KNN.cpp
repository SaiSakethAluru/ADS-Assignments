#include "RTree.h"
#include <ctime>
#include<cstdlib>
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

RTreeNode::RTreeNode(int m, int M, int n)
{
    // this->pointers = vector<RTreeNode*>(M,nullptr);
    // this->child_bounds = vector<vector<pair<int,int> > >(M);
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
    // this->M = 1024/(4*n+1);
    this->m = this->M/2;
    this->num_nodes = 0;
}

double calculate_distance(vector<pair<int,int> > &point1, vector<pair<int,int> > &point2){
	double out=0;
	for(int i=0;i<point1.size();i++){
		out+=((point1[i].first-point2[i].first)*(point1[i].first-point2[i].first));
	}
	return sqrt(out);
}

double min_dist(vector<pair<int,int> > &point, vector<pair<int,int> > &rectangle){
	double out = 0;
	for(int i=0;i<point.size();i++){
		int r = point[i].first;
		if(point[i].first<rectangle[i].first) r = rectangle[i].first;
		else if(point[i].second>rectangle[i].second) r = rectangle[i].second;
		out+=((point[i].first-r)*(point[i].first-r));
	}
	return sqrt(out);
}

void NNDistance(RTreeNode* node, vector<pair<int,int> > &point, vector<double> &distances, int k, stack<RTreeNode*> &st, long long &count){
	count++;
	// If node is leaf
	if(node->isLeaf == true){
		int i;
		for(int i=0;i<node->num_entries;i++){
			// If heap has space
			if(distances.size()<k){
				distances.push_back(calculate_distance(point,node->child_bounds[i]));
				push_heap(distances.begin(), distances.end());
			}
			// Else heap is full
			else{
				double dist = calculate_distance(point,node->child_bounds[i]);
				if(dist<distances.front()){
					pop_heap(distances.begin(), distances.end());
					distances.pop_back();
					distances.push_back(dist);
					push_heap(distances.begin(), distances.end());
				}
			}
		}
		if(!st.empty()){
			RTreeNode* next_node = st.top();
			st.pop();
			NNDistance(next_node, point, distances,k,st,count);
		}
	}
	// If node is not a leaf
	else{
		for(int i=0;i<node->num_entries;i++){
			if(distances.size()==k && min_dist(point,node->child_bounds[i])>distances.front()){

			}
			else st.push(node->pointers[i]);
		}
		RTreeNode* next_node = st.top();
		st.pop();
		NNDistance(next_node,point,distances,k,st,count);
	}
}

// double min_max_dist(vector<pair<int,int> > &point, vector<pair<int,int> > &rectangle){

// }



// Read the data of a single RTreeNode from file stream and return the constructed node object
RTreeNode* read_node(fstream& f, int m, int M, int n)
{
	int node_id,num_entries,isLeaf;
	f>>node_id>>num_entries>>isLeaf;
	RTreeNode* node = new RTreeNode(m,M,n);
	// node->num_entries = num_entries;
	node->num_entries = 0;
	node->node_id = node_id;
	node->isLeaf = isLeaf;
	for(int i=0;i<n;i++){
		f>>node->bounds[i].first>>node->bounds[i].second;
	}
	if(node->isLeaf){
		node->child_bounds = vector<vector<pair<int,int> > > (num_entries,vector<pair<int,int> > (n));
		for(int i=0;i<num_entries;i++){
			for(int j=0;j<n;j++){
				f>>node->child_bounds[i][j].first>>node->child_bounds[i][j].second;
			}
		}
	}
	return node;
}

// Function to read data from passed filename and return RTree object
RTree* load_tree(string filename)
{
	fstream f(filename);
	int num_nodes;
	f>>num_nodes;
	vector<int> parent(num_nodes);
	int m, M, n;
	f>>m>>M>>n;
	RTree* tree = new RTree(n);
	tree->num_nodes = num_nodes;
	for(int i=0;i<num_nodes;i++){
		f>>parent[i];
	}
	vector<RTreeNode*> nodes(num_nodes);
	for(int i=0;i<num_nodes;i++){
		RTreeNode* temp = read_node(f,m,M,n);
		nodes[temp->node_id] = temp;
	}
	for(int i=0;i<num_nodes;i++){
		if(parent[i]==-1){
			tree->root = nodes[i];
		}
		else{
			nodes[parent[i]]->pointers.push_back(nodes[i]);
			nodes[parent[i]]->child_bounds.push_back(nodes[i]->bounds);
			nodes[parent[i]]->num_entries++;
		}
	}
	return tree;
}

int main(){
	RTree* tree = load_tree("n2Tree.txt");
	cout<<tree->root->num_entries<<endl;
	vector<pair<int,int> > point;
	point.push_back(make_pair(1,1));
	// point.push_back(make_pair(9,9));
	point.push_back(make_pair(40,40));
	long long count = 0;
	vector<double> distances;
	stack<RTreeNode*> st;
	NNDistance(tree->root,point,distances,5,st,count);
	cout<<count<<endl;
}
