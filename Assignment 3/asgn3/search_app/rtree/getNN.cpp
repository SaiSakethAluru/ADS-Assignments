#include "NNSearch.h" // import RTree data structure and functions from other file
#include<bits/stdc++.h>

using namespace std;

int main() {
	int n, m, M;
    n=64;
    M = 4096 / (4*n + 1);
    m = M / 2;

    cout << "Restoring R-Tree" << endl;
    // Load the RTree of image histograms
    string rTree_save = "rTree_Histogram.txt"; //Change
    rTreeNew *db = new rTreeNew();
    db->loadTree(rTree_save);
    cout << "Tree restored from : rTree_Histogram.txt" << endl;
    int ttl_nodes = 0;
    db->totalNodes(db->root, &ttl_nodes);
    cout << "Total Nodes : " << ttl_nodes << endl;

    int k;
    int box_temp[64];
    vector<pair<int, int>> box(n, make_pair(0, 0));
    set<pair<int,int>,greater<pair<int, int>>> result;
    result.insert(make_pair(INT_MAX,-1));
    
    std::ifstream filein("input.txt");
    std::string str;
    int i=0;
    while (std::getline(filein, str)) {

        if(i==0){
        stringstream dum(str);
        dum >> k;
        }

        if(i==2){

            int j=0;
            
            stringstream check1(str);
            string intermediate;

            while(getline(check1,intermediate,' '))
            {
                stringstream dum2(intermediate);
                dum2 >> box_temp[j++];
            }

        }
        i++;
    }
    k=500;
    for(i=0;i<64;i++)
    {
        box[i].first = box_temp[i];
        box[i].second =  box_temp[i];
    }
    
    db->topk.clear();
    db->full = false;
    db->topk.insert(INT_MAX);
    db->k = k;
    if (k == 1) db->full = true;
    db->getkNN(db->root, box, result);
    cout << "hello" << db->k << result.size() << endl;
    int nn_id[result.size()];
    i = result.size()-1;
    for (auto const &var : result) {
        
        nn_id[i--] = var.second;
    }

    ofstream myfile("output.txt");

    if(myfile.is_open())
    {
        for(i =0;i<k;i++)
        {
            string str1 = to_string(nn_id[i]);
            myfile<<str1<< endl;

        }
        myfile.close();
    }
    else cerr<<"Unable to open file";

    delete db;

	return 0;
}