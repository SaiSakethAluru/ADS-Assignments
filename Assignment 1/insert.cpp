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

            // PickSeeds

            RTreeNode* inserted_node = new RTreeNode(this->m,this->M,this->n);
            inserted_node->bounds = new_bounds;
            inserted_node->isLeaf = true;
            node->pointers.push_back(inserted_node);
            vector<RTreeNode*> pointers1;
            vector<RTreeNode*> pointers2;
            vector<pair<int,int> > bounds1;
            vector<pair<int,int> > bounds2;
            double max_empty_area = INT_MIN;
            int max_empty_area_node1, max_empty_area_node2;

            for(int i=0;i<node->M;i++){
                for(int j=i+1;j<node->M+1;j++){
                    vector<pair<int,int> > mbr(node->n);
                    for(int k=0;k<node->n;k++){
                        mbr[k].first = min(node->pointers[i]->bounds[k].first,node->pointers[j]->bounds[k].first);
                        mbr[k].second = max(node->pointers[i]->bounds[k].second,node->pointers[j]->bounds[k].second);
                    }
                    double empty_area = calculate_area(mbr) - calculate_area(node->pointers[i]->bounds) - calculate_area(node->pointers[j]->bounds);
                    if(empty_area>max_empty_area){
                        max_empty_area = empty_area;
                        max_empty_area_node1 = i;
                        max_empty_area_node2 = j;
                    }
                }
            }
            vector<int> visited(node->pointers.size(),0);
            pointers1.push_back(node->pointers[max_empty_area_node1]);
            pointers2.push_back(node->pointers[max_empty_area_node2]);
            bounds1 = node->pointers[max_empty_area_node1]->bounds;
            bounds2 = node->pointers[max_empty_area_node2]->bounds;
            visited[max_empty_area_node1]=1;
            visited[max_empty_area_node2]=1;
            
            // PickNext
            
            while(1){
                // If neither splits are full
                if(pointers1.size()<node->m+1+(M%2==1?1:0) && pointers2.size()<node->m+1+(M%2==1?1:0)){
                    double max_area_diff = INT_MIN;
                    int max_area_diff_node,favored_split;
                    for(int i=0;i<node->pointers.size();i++){
                        if(visited[i]==0){
                            pair<double,double> first_split_diff = calculate_area_diff(bounds1,node->pointers[i]->bounds);
                            pair<double,double> second_split_diff = calculate_area_diff(bounds2,node->pointers[i]->bounds);
                            if(abs(first_split_diff.first - second_split_diff.first) > max_area_diff){
                                max_area_diff = abs(first_split_diff.first - second_split_diff.first);
                                max_area_diff_node = i;
                                favored_split = (first_split_diff.first > second_split_diff.first?2:1)
                            }
                        }
                    }
                    if(favored_split==1){
                        pointers1.push_back(node->pointers[max_area_diff_node]);
                        for(int i=0;i<node->n;i++){
                            bounds1[i].first = min(bounds1.first, node->pointers[max_area_diff_node]->bounds[i].first);
                            bounds1[i].second = max(bounds1.second, node->pointers[max_area_diff_node]->bounds[i].second);
                        }
                    }
                    else{
                        pointers2.push_back(node->pointers[max_area_diff_node]);
                        for(int i=0;i<node->n;i++){
                            bounds2[i].first = min(bounds2.first, node->pointers[max_area_diff_node]->bounds[i].first);
                            bounds2[i].second = max(bounds2.second, node->pointers[max_area_diff_node]->bounds[i].second);
                        }
                    }
                    visited[max_area_diff_node]=1;
                    // node->pointers.erase(max_area_diff_node);
                }
                // one of the split is full
                else{
                    if(pointers1.size()==node->m+1){
                        for(int i=0;i<node->pointers.size();i++){
                            if(visited[i]==0){
                                pointers2.push_back(node->pointers[i]);
                                for(int j=0;j<node->n;j++){
                                    bounds2[j].first = min(bounds2.first, node->pointers[i]->bounds[j].first);
                                    bounds2[j].second = max(bounds2.second, node->pointers[i]->bounds[j].second);
                                }
                            }
                        }
                    }
                    else if(pointers2.size()==node->m+1){
                        for(int i=0;i<node->pointers.size();i++){
                            if(visited[i]==0){
                                pointers1.push_back(node->pointers[i]);
                                for(int j=0;j<node->n;j++){
                                    bounds1[j].first = min(bounds1.first, node->pointers[i]->bounds[j].first);
                                    bounds1[j].second = max(bounds1.second, node->pointers[i]->bounds[j].second);
                                }
                            }
                        }
                    }
                    node->pointers.clear();
                    break;
                }
            }
            node->pointers = pointers1;
            node->bounds = bounds1;
            node->num_entries = pointers1.size();
            RTreeNode* new_node = new RTreeNode(this->m,this->M,this->n);
            new_node->pointers = pointers2;
            new_node->bounds = bounds2;
            new_node->num_entries = pointers2.size();
            new_node->isLeaf = false;
            return new_node;
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
                node->pointers.push_back(new_child);
                vector<RTreeNode*> pointers1;
                vector<RTreeNode*> pointers2;
                vector<pair<int,int> > bounds1;
                vector<pair<int,int> > bounds2;
                double max_empty_area = INT_MIN;
                int max_empty_area_node1, max_empty_area_node2;

                for(int i=0;i<node->M;i++){
                    for(int j=i+1;j<node->M+1;j++){
                        vector<pair<int,int> > mbr(node->n);
                        for(int k=0;k<node->n;k++){
                            mbr[k].first = min(node->pointers[i]->bounds[k].first,node->pointers[j]->bounds[k].first);
                            mbr[k].second = max(node->pointers[i]->bounds[k].second,node->pointers[j]->bounds[k].second);
                        }
                        double empty_area = calculate_area(mbr) - calculate_area(node->pointers[i]->bounds) - calculate_area(node->pointers[j]->bounds);
                        if(empty_area>max_empty_area){
                            max_empty_area = empty_area;
                            max_empty_area_node1 = i;
                            max_empty_area_node2 = j;
                        }
                    }
                }
                vector<int> visited(node->pointers.size(),0);
                pointers1.push_back(node->pointers[max_empty_area_node1]);
                pointers2.push_back(node->pointers[max_empty_area_node2]);
                bounds1 = node->pointers[max_empty_area_node1]->bounds;
                bounds2 = node->pointers[max_empty_area_node2]->bounds;
                visited[max_empty_area_node1]=1;
                visited[max_empty_area_node2]=1;
                // PickNext
                
                while(1){
                    // If neither splits are full
                    if(pointers1.size()<node->m+1+(M%2==1?1:0) && pointers2.size()<node->m+1+(M%2==1?1:0)){
                        double max_area_diff = INT_MIN;
                        int max_area_diff_node,favored_split;
                        for(int i=0;i<node->pointers.size();i++){
                            if(visited[i]==0){
                                pair<double,double> first_split_diff = calculate_area_diff(bounds1,node->pointers[i]->bounds);
                                pair<double,double> second_split_diff = calculate_area_diff(bounds2,node->pointers[i]->bounds);
                                if(abs(first_split_diff.first - second_split_diff.first) > max_area_diff){
                                    max_area_diff = abs(first_split_diff.first - second_split_diff.first);
                                    max_area_diff_node = i;
                                    favored_split = (first_split_diff.first > second_split_diff.first?2:1)
                                }
                            }
                        }
                        if(favored_split==1){
                            pointers1.push_back(node->pointers[max_area_diff_node]);
                            for(int i=0;i<node->n;i++){
                                bounds1[i].first = min(bounds1.first, node->pointers[max_area_diff_node]->bounds[i].first);
                                bounds1[i].second = max(bounds1.second, node->pointers[max_area_diff_node]->bounds[i].second);
                            }
                        }
                        else{
                            pointers2.push_back(node->pointers[max_area_diff_node]);
                            for(int i=0;i<node->n;i++){
                                bounds2[i].first = min(bounds2.first, node->pointers[max_area_diff_node]->bounds[i].first);
                                bounds2[i].second = max(bounds2.second, node->pointers[max_area_diff_node]->bounds[i].second);
                            }
                        }
                        visited[max_area_diff_node]=1;
                    }
                    // one of the split is full
                    else{
                        if(pointers1.size()==node->m+1){
                            for(int i=0;i<node->pointers.size();i++){
                                if(visited[i]==0){
                                    pointers2.push_back(node->pointers[i]);
                                    for(int j=0;j<node->n;j++){
                                        bounds2[j].first = min(bounds2.first, node->pointers[i]->bounds[j].first);
                                        bounds2[j].second = max(bounds2.second, node->pointers[i]->bounds[j].second);
                                    }
                                }
                            }
                        }
                        else if(pointers2.size()==node->m+1){
                            for(int i=0;i<node->pointers.size();i++){
                                if(visited[i]==0){
                                    pointers1.push_back(node->pointers[i]);
                                    for(int j=0;j<node->n;j++){
                                        bounds1[j].first = min(bounds1.first, node->pointers[i]->bounds[j].first);
                                        bounds1[j].second = max(bounds1.second, node->pointers[i]->bounds[j].second);
                                    }
                                }
                            }
                        }
                        node->pointers.clear();
                        break;
                    }
                }
                node->pointers = pointers1;
                node->bounds = bounds1;
                node->num_entries = pointers1.size();
                RTreeNode* new_node = new RTreeNode(this->m,this->M,this->n);
                new_node->pointers = pointers2;
                new_node->bounds = bounds2;
                new_node->num_entries = pointers2.size();
                new_node->isLeaf = false;
                return new_node;
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