/*
Team details:
Sai Saketh Aluru - 16CS30030
K Sai Surya Teja - 16CS30015
Sasi Bhushan Seelaboyina - 16CS30032
*/

// Includes and function declarations
#include "RTree.h"
#include <fstream>

pair<double,double> calculate_area_diff(vector<pair<int,int> > &curr_bounds, vector<pair<int,int> > &new_bounds);
double calculate_area(vector<pair<int,int> > &bounds);
void traverse_tree(RTreeNode* node, vector<int> &parent, int par_id);
void print_node(RTreeNode* node, fstream &f);
void print_rec(RTreeNode* node, fstream &f);

/* 
Function to read data from input file, 
Insert into a RTree and finally save the RTree into output file
*/
void RTree::insert(string in_filename,string out_filename)
{
    cout<<"Reading data"<<endl;
    fstream data_file(in_filename);
    vector<pair<int,int> > new_bounds(this->n);
    while(true){
        for(int i=0;i<this->n;i++){
            data_file >> new_bounds[i].first;
            data_file >> new_bounds[i].second;
        }
        // Create root node initially.
        if(this->root == nullptr){
            this->root = new RTreeNode(this->m,this->M,this->n);
            this->root->node_id = this->num_nodes;
            this->num_nodes++;
        }
        // Call to recursive function to insert the new datapoint.
        RTreeNode* new_child = insertRect(this->root,new_bounds);
        if(new_child==nullptr){
            // Nothing to do
        }
        // If previous insertion caused root to split
        else{
            // Create a new root node 
            RTreeNode* new_root = new RTreeNode(this->m,this->M,this->n);
            new_root->node_id = this->num_nodes;
            this->num_nodes++;
            new_root->isLeaf=false;
            // Add the previous root and new node obtained after split as children to this new root
            new_root->pointers[0] = this->root;
            new_root->pointers[1] = new_child;
            new_root->num_entries = 2;
            // Modify the MBR bounds of the new root.
            for(int i=0;i<this->n;i++){
                new_root->bounds[i].first = min(new_root->pointers[0]->bounds[i].first,new_root->pointers[1]->bounds[i].first);
                new_root->bounds[i].second = max(new_root->pointers[0]->bounds[i].second,new_root->pointers[1]->bounds[i].second);
            }
            this->root = new_root;
        }
        // Stop after reaching end of loop
        if(data_file.eof())
            break;
    }
    cout<<"Tree created"<<endl;
    // Save the tree
    this->save(out_filename);

}

/*
Recursive function that traverses the tree and inserts the hyperrectangle of dimensions new_bounds
in appropriate location. After insertions returns pointer to new node created in case of split and
null if no new node is created
*/
RTreeNode* RTree::insertRect(RTreeNode* node, vector<pair<int,int> > &new_bounds)
{
    if(node->isLeaf){
        // Case 1 - node is a leaf node and has space for additional entry
        // Insert node and return null
        if(node->num_entries < node-> M){
            RTreeNode* new_node = new RTreeNode(this->m,this->M,this->n);
            new_node->node_id = this->num_nodes;
            this->num_nodes++;
            new_node->bounds = new_bounds;
            node->pointers[node->num_entries] = new_node;
            node->num_entries++;
            return nullptr;
        }    
        // Case 2 - node is leaf node and is full. 
        // Apply quadratic split algorithm
        else{
            // PickSeeds
            RTreeNode* inserted_node = new RTreeNode(this->m,this->M,this->n);
            inserted_node->node_id = this->num_nodes;
            this->num_nodes++;
            inserted_node->bounds = new_bounds;
            node->pointers.push_back(inserted_node);
            vector<RTreeNode*> pointers1;
            vector<RTreeNode*> pointers2;
            vector<pair<int,int> > bounds1;
            vector<pair<int,int> > bounds2;
            double max_empty_area = INT_MIN;
            int max_empty_area_node1, max_empty_area_node2;
            for(int i=0;i<M;i++){
                for(int j=i+1;j<M+1;j++){
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
            pointers1.push_back(node->pointers[max_empty_area_node1]);
            pointers2.push_back(node->pointers[max_empty_area_node2]);
            bounds1 = node->pointers[max_empty_area_node1]->bounds;
            bounds2 = node->pointers[max_empty_area_node2]->bounds;
            vector<bool> erased(node->pointers.size(),false);
            int erase_left = node->pointers.size();
            erased[max_empty_area_node1] = true;
            erased[max_empty_area_node2] = true;
            erase_left -= 2;
            
            // PickNext
            while(erase_left>0){
                // If neither splits are full
                if(pointers1.size() + erase_left > node->m && pointers2.size() + erase_left > node->m){
                    double max_area_diff = INT_MIN;
                    int max_area_diff_node,favored_split;
                    for(int i=0;i<node->pointers.size();i++){
                        if(erased[i])
                            continue;
                        pair<double,double> first_split_diff = calculate_area_diff(bounds1,node->pointers[i]->bounds);
                        pair<double,double> second_split_diff = calculate_area_diff(bounds2,node->pointers[i]->bounds);
                        if(abs(first_split_diff.first - second_split_diff.first) > max_area_diff){
                            max_area_diff = abs(first_split_diff.first - second_split_diff.first);
                            max_area_diff_node = i;
                            favored_split = (first_split_diff.first > second_split_diff.first?2:1);
                        }
                    }
                    if(favored_split==1){
                        pointers1.push_back(node->pointers[max_area_diff_node]);
                        for(int i=0;i<node->n;i++){
                            bounds1[i].first = min(bounds1[i].first, node->pointers[max_area_diff_node]->bounds[i].first);
                            bounds1[i].second = max(bounds1[i].second, node->pointers[max_area_diff_node]->bounds[i].second);
                        }
                    }
                    else{
                        pointers2.push_back(node->pointers[max_area_diff_node]);
                        for(int i=0;i<node->n;i++){
                            bounds2[i].first = min(bounds2[i].first, node->pointers[max_area_diff_node]->bounds[i].first);
                            bounds2[i].second = max(bounds2[i].second, node->pointers[max_area_diff_node]->bounds[i].second);
                        }
                    }
                    erased[max_area_diff_node] = true;
                    erase_left--;
                }
                // one of the split is full
                else{
                    if(pointers1.size()> node->m && pointers2.size() + erase_left ==node->m){
                        for(int i=0;i<node->pointers.size();i++){
                            if(erased[i])
                                continue;
                            pointers2.push_back(node->pointers[i]);
                            for(int j=0;j<node->n;j++){
                                bounds2[j].first = min(bounds2[j].first, node->pointers[i]->bounds[j].first);
                                bounds2[j].second = max(bounds2[j].second, node->pointers[i]->bounds[j].second);
                            }
                        }
                    }
                    else if(pointers2.size() > node->m && pointers1.size() + erase_left ==node->m){
                        for(int i=0;i<node->pointers.size();i++){
                            if(erased[i])
                                continue;
                            pointers1.push_back(node->pointers[i]);
                            for(int j=0;j<node->n;j++){
                                bounds1[j].first = min(bounds1[j].first, node->pointers[i]->bounds[j].first);
                                bounds1[j].second = max(bounds1[j].second, node->pointers[i]->bounds[j].second);
                            }
                        }
                    }
                    node->pointers.clear();
                    break;
                }
            }
            node->pointers = vector<RTreeNode*> (M,nullptr);
            for(int i=0;i<pointers1.size();i++){
                node->pointers[i] = pointers1[i];
            }
            node->bounds = bounds1;
            node->num_entries = pointers1.size();
            RTreeNode* new_node = new RTreeNode(this->m,this->M,this->n);
            new_node->node_id = this->num_nodes;
            this->num_nodes++;
            for(int i=0;i<pointers2.size();i++){
                new_node->pointers[i] = pointers2[i];
            }
            new_node->bounds = bounds2;
            new_node->num_entries = pointers2.size();
            new_node->isLeaf = node->isLeaf;
            return new_node;
        }
    }
    // Current node is internal node
    else{
        // Find child entry that has minimum increase in area upon addition of new node,
        // Break ties by selecting the entry with least area
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
        // Case 3 - No splitting of child node
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
        // Child node got split
        else{
            // Case 4 - child got split and current node has empty space
            if(node->num_entries < M){
                // Add new node
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
            // Case 5 - child got split and parent is full - apply quadratic split algorithm
            else {
                node->pointers.push_back(new_child);
                vector<RTreeNode*> pointers1;
                vector<RTreeNode*> pointers2;
                vector<pair<int,int> > bounds1;
                vector<pair<int,int> > bounds2;
                double max_empty_area = INT_MIN;
                int max_empty_area_node1, max_empty_area_node2;
                // Pick seeds
                for(int i=0;i<M;i++){
                    for(int j=i+1;j<M+1;j++){
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
                pointers1.push_back(node->pointers[max_empty_area_node1]);
                pointers2.push_back(node->pointers[max_empty_area_node2]);
                bounds1 = node->pointers[max_empty_area_node1]->bounds;
                bounds2 = node->pointers[max_empty_area_node2]->bounds;
                vector<bool> erased(node->pointers.size(),false);
                int erase_left = node->pointers.size();
                erased[max_empty_area_node1] = true;
                erased[max_empty_area_node2] = true;
                erase_left -= 2;
                
                // PickNext
                while(erase_left>0){
                    // If neither splits are full
                    if(pointers1.size() + erase_left > node->m && pointers2.size() + erase_left > node->m){
                        double max_area_diff = INT_MIN;
                        int max_area_diff_node,favored_split;
                        for(int i=0;i<node->pointers.size();i++){
                            if(erased[i])
                                continue;
                            pair<double,double> first_split_diff = calculate_area_diff(bounds1,node->pointers[i]->bounds);
                            pair<double,double> second_split_diff = calculate_area_diff(bounds2,node->pointers[i]->bounds);
                            if(abs(first_split_diff.first - second_split_diff.first) > max_area_diff){
                                max_area_diff = abs(first_split_diff.first - second_split_diff.first);
                                max_area_diff_node = i;
                                favored_split = (first_split_diff.first > second_split_diff.first?2:1);
                            }
                        }
                        if(favored_split==1){
                            pointers1.push_back(node->pointers[max_area_diff_node]);
                            for(int i=0;i<node->n;i++){
                                bounds1[i].first = min(bounds1[i].first, node->pointers[max_area_diff_node]->bounds[i].first);
                                bounds1[i].second = max(bounds1[i].second, node->pointers[max_area_diff_node]->bounds[i].second);
                            }
                        }
                        else{
                            pointers2.push_back(node->pointers[max_area_diff_node]);
                            for(int i=0;i<node->n;i++){
                                bounds2[i].first = min(bounds2[i].first, node->pointers[max_area_diff_node]->bounds[i].first);
                                bounds2[i].second = max(bounds2[i].second, node->pointers[max_area_diff_node]->bounds[i].second);
                            }
                        }
                        erased[max_area_diff_node] = true;
                        erase_left--;
                    }
                    // one of the split is full
                    else{
                        if(pointers1.size()> node->m && pointers2.size() + erase_left ==node->m){
                            for(int i=0;i<node->pointers.size();i++){
                                if(erased[i])
                                    continue;
                                pointers2.push_back(node->pointers[i]);
                                for(int j=0;j<node->n;j++){
                                    bounds2[j].first = min(bounds2[j].first, node->pointers[i]->bounds[j].first);
                                    bounds2[j].second = max(bounds2[j].second, node->pointers[i]->bounds[j].second);
                                }
                            }
                        }
                        else if(pointers2.size() > node->m && pointers1.size() + erase_left ==node->m){
                            for(int i=0;i<node->pointers.size();i++){
                                if(erased[i])
                                    continue;
                                pointers1.push_back(node->pointers[i]);
                                for(int j=0;j<node->n;j++){
                                    bounds1[j].first = min(bounds1[j].first, node->pointers[i]->bounds[j].first);
                                    bounds1[j].second = max(bounds1[j].second, node->pointers[i]->bounds[j].second);
                                }
                            }
                        }
                        node->pointers.clear();
                        break;
                    }
                }
                node->pointers = vector<RTreeNode*> (M,nullptr);
                for(int i=0;i<pointers1.size();i++){
                    node->pointers[i] = pointers1[i];
                }
                node->bounds = bounds1;
                node->num_entries = pointers1.size();
                RTreeNode* new_node = new RTreeNode(this->m,this->M,this->n);
                new_node->node_id = this->num_nodes;
                this->num_nodes++;
                for(int i=0;i<pointers2.size();i++){
                    new_node->pointers[i] = pointers2[i];
                }
                new_node->bounds = bounds2;
                new_node->num_entries = pointers2.size();
                new_node->isLeaf = node->isLeaf;
                return new_node;
            }
        }
    }
}

/*
Function to calculate area of a hyperrectangle and the difference in increase of area upon additon of a new entry
The function takes log of the (max-min) values in order to prevent overflow during multiplication
*/
pair<double,double> calculate_area_diff(vector<pair<int,int> > &curr_bounds, vector<pair<int,int> > &new_bounds)
{
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

// Calculate log area of a MBR
double calculate_area(vector<pair<int,int> > &bounds)
{
    double log_area = 0;
    int n = bounds.size();
    for(int i=0;i<n;i++){
        log_area += log(bounds[i].second-bounds[i].first); 
    }
    return log_area;
}

// Traverses the tree recursively and makes note of parent id of each node. Root has id -1
void traverse_tree(RTreeNode* node, vector<int> &parent, int par_id)
{
    parent[node->node_id] = par_id;
    if(!node->isLeaf){
        for(int i=0;i<node->num_entries;i++){
            traverse_tree(node->pointers[i],parent,node->node_id);
        }
    }
    else{
        for(int i=0;i<node->num_entries;i++){
            parent[node->pointers[i]->node_id] = node->node_id;
        }
    }
}

// Prints a node and its associated values to a filestream
void print_node(RTreeNode* node, ofstream &f)
{
    f<<node->node_id<<" ";
    f<<node->num_entries<<" ";
    f<<(int)node->isLeaf<<" ";
    for(int i=0;i<node->bounds.size();i++){
        f<<node->bounds[i].first<<" "<<node->bounds[i].second<<" ";
    }
    f<<endl;
}

// Recursive function to traverse and print each node to filestream
void print_rec(RTreeNode* node, ofstream &f)
{
    print_node(node,f);
    if(!node->isLeaf){
        for(int i=0;i<node->num_entries;i++){
            print_rec(node->pointers[i],f);
        }
    }
    else{
        for(int i=0;i<node->num_entries;i++){
            print_node(node->pointers[i],f);
        }
    }
}

// Function to save a tree to a file. 
void RTree::save(string filename)
{
    ofstream f;
    f.open(filename);
    f<<this->num_nodes<<endl;
    f<<this->m<<" "<<this->M<<" "<<this->n<<endl;
    vector<int> parent(this->num_nodes,-2);
    traverse_tree(this->root,parent,-1);
    for(int i=0;i<parent.size();i++){
        f<<parent[i]<<' ';
    }
    f<<endl;
    print_rec(this->root,f);
}