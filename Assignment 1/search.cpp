#include "RTree.h"

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
    RTree* Tree; // The tree made after reading from input
    int M = Tree->M;
    vector<pair<int,int>> region; // The region to be searched
    RTreeNode* node = search(Tree->root,region,M);
    return 0;
}