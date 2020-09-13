
#include "RTree.h"
#include <ctime>
#include<cstdlib>

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
	vector<RTreeNode*> nodes(num_nodes);
	// vector<RTreeNode*> nodes;
	for(int i=0;i<num_nodes;i++){
		// nodes.push_back(read_node(f,m,M,n));
		RTreeNode* temp = read_node(f,m,M,n);
		nodes[temp->node_id] = temp;
	}
	// cerr<<"reading done"<<endl;
	for(int i=0;i<num_nodes;i++){
		if(parent[i]==-1){
			tree->root = nodes[i];
		}
		else{
			nodes[parent[i]]->pointers[nodes[parent[i]]->num_entries] = nodes[i];
			nodes[parent[i]]->num_entries++;
		}
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
		if((p1.first>=p2.first && p1.first<p2.second) || (p1.second>p2.first && p2.first>=p1.first)
			|| (p1.first<=p2.first && p2.second<=p1.second))
		{ }
		else
		{
			return false;
		}   
	}
	return true;
}

RTreeNode* search(RTreeNode* node, vector<pair<int,int> > &region,int &count)
{
	RTreeNode* ans = nullptr;
	// cerr<<"Search "<<count<<endl;
	if(node->isLeaf){
		// cerr<<"leaf"<<endl;
		for(int i=0;i<node->num_entries;i++){
			if(check_overlap(node->pointers[i]->bounds,region)){
				count++;
				// cout<<node->pointers[i]->bounds[0].first<<" "<<node->pointers[i]->bounds[0].second<<" "<<node->pointers[i]->bounds[1].first<<" "<<node->pointers[i]->bounds[1].second<<endl;
				ans = node->pointers[i];
			}
		}
	}
	else{
		if(check_overlap(node->bounds,region)){
			// cerr<<"non leaf"<<node->num_entries<<endl;
			for(int i=0;i<node->num_entries;i++){
				if(check_overlap(node->pointers[i]->bounds,region)){
					count++;
					// cout<<node->pointers[i]->bounds[0].first<<" "<<node->pointers[i]->bounds[0].second<<" "<<node->pointers[i]->bounds[1].first<<" "<<node->pointers[i]->bounds[1].second<<endl;
					RTreeNode* temp = search(node->pointers[i],region,count);
					if(temp!=nullptr)
						ans = temp;
				}
			}
		}
	}
	return ans;
}


int main()
{
	// Read File and make the tree
	RTree* Tree = load_tree("n2Tree.txt"); // The tree made after reading from input
	int n;
	cout<<"Give Number of Dimensions: "<<endl;
	cin>>n;
	int rand1,rand2;
	srand(time(0));
	int count = 0;
	double avg_time = 0.0;
	for(int j =0;j<50;j++)
	{
		vector<pair<int,int> > region;  // The region to be searched
		for(int i=0;i<n;i++)
		{
			rand1=20*(float)rand()/RAND_MAX;
			rand2=20*(float)rand()/RAND_MAX;
			region.push_back({min(rand1,rand2),max(rand1,rand2)}); 
		}
	// cout<<"The region to be checked: ";
	// cout<<region[0].first<<" "<<region[0].second<<" "<<region[1].first<<" "<<region[1].second<<endl;
		int visited = 0;
		clock_t begin = clock();  
		RTreeNode* node = search(Tree->root,region,visited);
		clock_t end = clock();
		double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
		count += visited;
		avg_time += elapsed_secs;
	}
	cout<<"Average number of visited nodes: "<<int(count/50)<<endl;
	
	cout << "Time taken is : " << fixed 
		 << (avg_time/50.0) << setprecision(6); 
	cout << " sec " << endl;
	
	return 0;
}
