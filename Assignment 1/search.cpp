#include "RTree.h"

RTreeNode* read_node(fstream& f, int m, int M, int n)
{
    int node_id,num_entries,isLeaf;
    f>>node_id>>num_entries>>isLeaf;
    RTreeNode* node = new RTreeNode(m,M,n);
    node->num_entries = num_entries;
    node->node_id = node_id;
    node->isLeaf = isLeaf;
    for(int i=0;i<n;i++){
        f>>node->bounds[i].first>>node->bounds[i].second;
    }
    return node;

}
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
    vector<RTreeNode*> nodes;
    for(int i=0;i<num_nodes;i++){
        nodes.push_back(read_node(f,m,M,n));
    }
    for(int i=0;i<num_nodes;i++){
        if(parent[i]!=-1){
            nodes[parent[i]]->pointers[nodes[parent[i]]->num_entries] = nodes[i];
            nodes[parent[i]]->num_entries++;
        }
        else tree->root = nodes[i];
    }
    return tree;
}
bool check_overlap(vector<pair<int,int> > &bounds, vector<pair<int,int> > &region)
{
    int i;
    for(i=0;i<bounds.size();i++)
    {
        pair<int,int> p1 = bounds[i];
        pair<int,int> p2 = region[i];
        if(p1.first>=p2.first && p1.first<p2.second || p1.second>p2.first && p1.second<=p2.second)
        {

        }
        else
        {
            return false;
        }   
    }
    return true;
}
RTreeNode* search(RTreeNode *root,vector<pair<int,int>> &region,int M)
{
    bool isLeaf = root->isLeaf;
    vector<RTreeNode*> root_node = root->pointers;
    vector<pair<int,int> > bounds = root->bounds;
    RTreeNode* node1=NULL;
    if(isLeaf)
    {
        if(check_overlap(bounds,region))
        {
            return root;
        }       
    }
    else
    {
        int i = 0;
        if(check_overlap(bounds,region))
        {
            while(i<M && root_node[i]!=NULL)
            {   
                RTreeNode* node2 = search(root_node[i],region,M);
                if(node2!=NULL)
                    node1 = node2;    
                i++;   
            }
            return node1;   
        }        
    }   
    return NULL;
}

int main()
{
    // Read File and make the tree
    RTree* Tree = load_tree("RTree.txt"); // The tree made after reading from input
    int M = Tree->M;
    vector<pair<int,int>> region; // The region to be searched
    RTreeNode* node = search(Tree->root,region,M);
    return 0;
}