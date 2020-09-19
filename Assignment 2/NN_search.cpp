#include "RTree.h"
#include <ctime>
#include<cstdlib>
using namespace std;

class Nearest {
public:
	int dist;
	vector<pair<int,int> > bounds;
	Nearest()
	{
		dist = INT_MAX;
	}
};

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

int mindist(vector<int> &point, vector<pair<int,int> > &bounds)
{
	int n = point.size();
	int dist = 0;
	int i;
	for(i=0;i<n;i++)
	{
		if(point[i] < bounds[i].first) {
			int val = point[i] - bounds[i].first;
			dist += val*val;
		}
		else if(point[i] > bounds[i].second) {
			int val = point[i] - bounds[i].second;
			dist += val*val;
		}

	}
	return dist;
}

int minmaxdist(vector<int> &point, vector<pair<int,int> > &bounds)
{
	int n = point.size();
	int dist = 0;
	int i,j;
	int mmdist=INT_MAX;
	for(i=0;i<n;i++)
	{
		dist = 0;
		for(j=0;j<n;j++)
		{
			float mid = float(bounds[i].first+bounds[i].second)/2.0;
			if(j!=i)
			{
				if(point[i] >=mid) {
					int val = point[i] - bounds[i].first;
					dist += val*val;
				}
				else {
					int val = point[i] - bounds[i].second;
					dist += val*val;
				}
			}
			else
			{
				if(point[i] <=mid) {
					int val = point[i] - bounds[i].first;
					dist += val*val;
				}
				else {
					int val = point[i] - bounds[i].second;
					dist += val*val;
				}
			}
		}
		if(dist<mmdist)
			 mmdist = dist;
	}
	return mmdist;

}

void merge(vector<RTreeNode *> &branchlist, vector<int> &mindist_list, vector<int> &minmaxdist_list,int l,int m,int r)
{
	int i = l;
    int j = m+1;
    int k = 0;
    int n = r-l+1;
    vector<int> L(m-l+1);
    vector<int> R(r-m);
    vector<int> l1(m-l+1);
    vector<int> r1(r-m);
    vector<RTreeNode *> l2(m-l+1);
    vector<RTreeNode *> r2(r-m);
    for(i=l;i<m+1;i++)
    {
        L[i-l] = mindist_list[i];
        l1[i-l] = minmaxdist_list[i];
        l2[i-l] = branchlist[i];
    }
    for(i=m+1;i<r+1;i++)
    {
        R[i-(m+1)] = mindist_list[i];
        r1[i-(m+1)] = minmaxdist_list[i];
        r2[i-(m+1)] = branchlist[i];
    }
    i = 0;
    j = 0;
    k = l;
    while(i<(m-l+1) && j<r-m)
    {
      if(L[i] < R[j])
      {
          mindist_list[k] = L[i];
          minmaxdist_list[k] = l1[i];
          branchlist[k] = l2[i];
          k=k+1;
          i=i+1;
          
      }
      else
      {
          mindist_list[k] = R[j];
          minmaxdist_list[k] = r1[j];
          branchlist[k] = r2[j];
          k=k+1;
          j=j+1;
      }
    }
    while(i<(m-l+1))
    {
        mindist_list[k] = L[i];
        minmaxdist_list[k] = l1[i];
        branchlist[k] = l2[i];
          k=k+1;
          i=i+1;
    }
    while(j<r-m)
    {
        mindist_list[k] = R[j];
        minmaxdist_list[k] = r1[j];
        branchlist[k] = r2[j];
          k=k+1;
          j=j+1;
        
    }
    
}
//  Sorting branchlist, mindist_list, minmaxdist_list wrt mindist_list
void sortBranchList(vector<RTreeNode *> &branchlist, vector<int> &mindist_list, vector<int> &minmaxdist_list,int l,int r)
{
	int mid = (l+r)/2;
	if(l<r)
	{
		sortBranchList(branchlist,mindist_list,minmaxdist_list,l,mid);
		sortBranchList(branchlist,mindist_list,minmaxdist_list,mid+1,r);
		merge(branchlist,mindist_list,minmaxdist_list,l,mid,r);
	}

}

void downwardPruning(vector<RTreeNode *> &branchlist, vector<int> &mindist_list, vector<int> &minmaxdist_list,Nearest* N)
{
	vector<RTreeNode *> list1;
	vector<int> list2;
	vector<int> list3;
	int i,j;
	int c;
	for(i=0;i<branchlist.size();i++)
	{
		c = 0;
		for(j=0;j<branchlist.size();j++)
		{
			if(j!=i)
			{
				if(mindist_list[i]>minmaxdist_list[j])
				{
					c++;
				}
			}
		}
		if(c==0)
		{
			list1.push_back(branchlist[i]);
			list2.push_back(mindist_list[i]);
			list3.push_back(minmaxdist_list[i]);
		}
	}
	branchlist = list1;
	mindist_list = list2;
	minmaxdist_list = list3;
	list1.clear();
	list2.clear();
	list3.clear();
	for(i=0;i<branchlist.size();i++)
	{
		if(mindist_list[i]<=N->dist)
		{
			list1.push_back(branchlist[i]);
			list2.push_back(mindist_list[i]);
			list3.push_back(minmaxdist_list[i]);
		}
	}
	branchlist = list1;
	mindist_list = list2;
	minmaxdist_list = list3;
}


void NNsearch(RTreeNode *root, vector<int> &point,Nearest* N,int &count)
{
	int i;
	// cout<<"sasi0"<<endl;
	if(root->isLeaf)
	{
		// cout<<"sasi1"<<endl;
		count++;
		
		int dist1;
		// cout<<root->bounds.size()<<endl;
		// cout<<root->bounds[0].first<<" "<<root->bounds[0].second<<" "<<root->bounds[1].first<<" "<<root->bounds[0].second<<endl;
		// for(i=0;i<root->num_entries;i++)
		// {
			dist1 = mindist(point,root->bounds);
			// cout<<"sasi2"<<endl;
			if(dist1<N->dist)
			{
				N->dist = dist1;
				N->bounds = root->bounds;
			}
		// }
	}
	else
	{
		
		count++;
		// cout<<"sasi3"<<endl;
		// cout<<root->num_entries<<endl;
		vector<RTreeNode *> branchlist;
		vector<int> mindist_list;
		vector<int> minmaxdist_list;
		for(i=0;i<root->num_entries;i++)
		{
			
			branchlist.push_back(root->pointers[i]);
			mindist_list.push_back(mindist(point,root->child_bounds[i]));
			minmaxdist_list.push_back(minmaxdist(point,root->child_bounds[i]));
		}
		// cout<<"sasi5"<<endl;
		sortBranchList(branchlist,mindist_list,minmaxdist_list,0,i-1);
		// cout<<"sasi6"<<endl;
		// cout<<branchlist.size()<<endl;
		// downwardPruning(branchlist,mindist_list,minmaxdist_list,N);
		// cout<<"sasi7"<<endl;
		// cout<<branchlist.size()<<endl;
		if(branchlist.size() > 0)
		{
			int flag = 1;
			// cout<<"sasi8"<<endl;
			for(i=0;i < branchlist.size();i++)
			{	
				// upward pruning
				if(mindist_list[i] > N->dist)
					flag = 0;
				else
					flag = 1;	

				if(flag)
				{
					NNsearch(branchlist[i],point,N,count);
				}

				
			}
		}
	}
}

int main()
{
	RTree* Tree = load_tree("n2Tree.txt"); 
	int n = 2;
	// cout<<"Give Number of Dimensions: "<<endl;
	// cin>>n;
	
	int rand1,rand2;
	srand(time(0));
	int count = 0;
	double avg_time = 0.0;
	for(int j =0;j<50;j++)
	{
		vector<int> point;  // The region to be searched
		Nearest* N = new Nearest();
		for(int i=0;i<n;i++)
		{
			rand1=30*(float)rand()/RAND_MAX;
			point.push_back(10+rand1); 
		}
		int visited = 0;
		// cout<<point[0]<<" "<<point[1]<<endl;
		clock_t begin = clock();  
		NNsearch(Tree->root,point,N,visited);
		clock_t end = clock();
		double elapsed_secs = double(end - begin) /(CLOCKS_PER_SEC/1000);
		count += visited;
		avg_time += elapsed_secs;
		// cout<<N->bounds.size()<<endl;
		// cout<<N->bounds[0].first<<" "<<N->bounds[0].second<<" "<<N->bounds[1].first<<" "<<N->bounds[0].second<<endl;
		
		// cout<<N->dist<<endl;
		// cout<<visited<<endl;
	}
	cout<<"Average number of visited nodes: "<<int(count/50)<<endl;
	
	cout << "Time taken is : " << fixed 
		 << (avg_time/50.0) << setprecision(6); 
	cout << " msec " << endl;
	
	return 0;
}
