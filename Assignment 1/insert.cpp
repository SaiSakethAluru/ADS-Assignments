#include "RTree.h"
#include <fstream>

void RTree::insert(string filename)
{
    fstream data_file(filename);
    vector<pair<int,int> > new_bounds;
    while(true){
        for(int i=0;i<n;i++){
            data_file >> new_bounds[i].first;
            data_file >> new_bounds[i].second;
        }
        if(this->root == nullptr){
            this->root = new RTreeNode(this->m,this->M,this->n);
        }
        RTreeNode* new_child = insertRect(this->root,new_bounds);
        if(new_child==nullptr){
            // nothing to do?
        }
        else{
            RTreeNode* new_root = new RTreeNode(this->m,this->M,this->n);
            new_root->isLeaf=false;
            new_root->pointers[0] = this->root;
            new_root->pointers[1] = new_child;
            new_root->num_entries = 2;
            for(int i=0;i<this->n;i++){
                new_root->bounds[i].first = min(new_root->pointers[0]->bounds[i].first,new_root->pointers[1]->bounds[i].first);
                new_root->bounds[i].second = max(new_root->pointers[0]->bounds[i].second,new_root->pointers[1]->bounds[i].second);
            }
            this->root = new_root;
        }
        if(data_file.eof())
            break;
    }
    // save_tree();
}

RTreeNode* RTree::insertRect(RTreeNode* node, vector<pair<int,int> > &new_bounds){
    if(node->isLeaf){
        if(node->num_entries < node-> M){
            RTreeNode* new_node = new RTreeNode(this->m,this->M,this->n);
            new_node->bounds = new_bounds;
            node->pointers[node->num_entries] = new_node;
            node->num_entries++;
            return nullptr;
        }    
        else{
            // TODO split node
        }
    }
    else{
        RTreeNode* min_area_node = nullptr;
        double min_diff = INT_MAX;
        double min_area = INT_MAX;
        for(int i=0;i<node->num_entries;i++){
            pair<double,double> area_pair = calculate_area_diff(node->pointers[i]->bounds,new_bounds);
            double log_area_diff = area_pair.first;
            double log_area = area_pair.second;
            if(log_area_diff < min_diff){
                min_area_node = node->pointers[i];
                min_diff = log_area_diff;
                min_area = log_area;
            }
            else if(log_area_diff == min_diff && log_area < min_area ){
                min_area_node = node->pointers[i];
                min_area = log_area;
            } 
        }
        RTreeNode* new_child = insertRect(min_area_node,new_bounds);
        // No splitting of child node
        if(new_child == nullptr){
            // adjust node bounds
            for(int i=0;i<node->n;i++){
                for(int j=0;j<node->num_entries;j++){
                    node->bounds[i].first = min(node->bounds[i].first,node->pointers[j]->bounds[i].first);
                    node->bounds[i].second = max(node->bounds[i].second,node->pointers[j]->bounds[i].second);
                }
            }
            return nullptr;
        }
        // child got split
        else{
            if(node->num_entries < M){
                node->pointers[node->num_entries] = new_child;
                node->num_entries++;
                // adjust node bounds
                for(int i=0;i<node->n;i++){
                    for(int j=0;j<node->num_entries;j++){
                        node->bounds[i].first = min(node->bounds[i].first,node->pointers[j]->bounds[i].first);
                        node->bounds[i].second = max(node->bounds[i].second,node->pointers[j]->bounds[i].second);
                    }
                }
                return nullptr;
            }
            else {
                // TODO quadratic split
                // TODO adjust node bounds and return the new node. 
            }
        }
    }

}

pair<double,double> calculate_area_diff(vector<pair<int,int> > &curr_bounds, vector<pair<int,int> > &new_bounds){
    double old_area = calculate_area(curr_bounds);
    int n = curr_bounds.size();
    vector<pair<int,int> > modified_bounds(n);
    for(int i=0;i<n;i++){
        modified_bounds[i].first = min(curr_bounds[i].first,new_bounds[i].first);
        modified_bounds[i].second = max(curr_bounds[i].second,new_bounds[i].second);
    }
    double new_area = calculate_area(modified_bounds);
    return make_pair(new_area - old_area,old_area);
}

double calculate_area(vector<pair<int,int> > &bounds){
    double log_area = 0;
    int n = bounds.size();
    for(int i=0;i<n;i++){
        log_area += log(bounds[i].second-bounds[i].first); 
    }
    return log_area;
}