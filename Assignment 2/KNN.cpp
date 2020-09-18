#include "RTree.h"
#include <iostream>

using namespace std;

void NNDistance(RTreeNode* node, vector<pair<int,int> > &point, vector<double> &distances, int k, stack<RTreeNode*> &st, int &count){
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
		for(int i=0;i<node->num_entries){
			if(distances.size()==k && min_dist(point,node->child_bounds[i])>distances.front()){

			}
			else st.push(node->pointers[i]);
		}
		RTreeNode* next_node = st.top();
		st.pop();
		NNDistance(next_node,point,distances,k,st,count);
	}
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
		else if(point[i]>rectangle[i].second) r = rectangle[i].second;
		out+=((point[i].first-r)*(point[i].first-r))
	}
	return sqrt(out);
}

// double min_max_dist(vector<pair<int,int> > &point, vector<pair<int,int> > &rectangle){

// }

