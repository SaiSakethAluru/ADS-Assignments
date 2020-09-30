/*
Assignment 2 : R-Tree NN Search
Team Members : Amshumaan Varma Pericherla, Pruthvi Sampath Chabathula

*/

#include<bits/stdc++.h>
#include "rTreeFunctions.h" // From previous Assignment

class rTreeNew : public rTree { // Extends rTree from previous assignment to implement NN and kNN search
public:
	int dist;
	set<int, greater<int>> topk; // fot kNN search
	int k;
	bool full;

	rTreeNew() : rTree() {
		dist = INT_MAX;
		topk.clear();
		k = 1;
		full = false;
	}
	rTreeNew(int dim, int m, int M) : rTree(dim, m, M) {
		dist = INT_MAX;
		topk.clear();
		k = 1;
		full = false;
	}

	int getMinDist(vector<pair<int, int>> point, vector<pair<int, int>> mbr);
	int getMinMaxDist(vector<pair<int, int>> point, vector<pair<int, int>> mbr);
	void getNN(node *cur, vector<pair<int, int>> &point, int *result);
	void getkNN(node *cur, vector<pair<int, int>> &point, set<pair<int, int>, greater<pair<int, int>>> &results);
	void totalNodes(node *cur, int *ttl) {
		(*ttl)++;
		if (cur->isLeaf) return;
		for(int i = 0; i < (int)cur->curLst.size(); i++) totalNodes(cur->curLst[i]->child, ttl);
		return;
	}
};

// To Get Min Distance
int rTreeNew::getMinDist(vector<pair<int, int>> point, vector<pair<int, int>> mbr) {
	long long distance = 0;
	int x, p;
	for (int i = 0; i < (int)mbr.size(); i++) {
		p = point[i].first;
		if (p < mbr[i].first) x = mbr[i].first;
		else if (p > mbr[i].second) x = mbr[i].second;
		else x = p;
		distance += ((long long)(p-x))*((long long)(p-x));
	}
	return (int)sqrt(distance);
} 

// To Get MinMax Distance
int rTreeNew::getMinMaxDist(vector<pair<int, int>> point, vector<pair<int, int>> mbr) {
	int distance = INT_MAX, p, rm, rM; 
	long long total = 0, tmp;
	for (int i = 0; i < (int)mbr.size(); i++) {
		p = point[i].first;
		if (p >= (mbr[i].first + mbr[i].second)/2) rM = mbr[i].first;
		else rM = mbr[i].second;
		total += ((long long)(p-rM))*((long long)(p-rM));
	}

	for (int i = 0; i < (int)mbr.size(); i++) {
		tmp = total;
		p = point[i].first;
		if (p >= (mbr[i].first + mbr[i].second)/2) {
			rm = mbr[i].second;
			rM = mbr[i].first;
		}
		else {
			rm = mbr[i].second;
			rM = mbr[i].second;
		}
		tmp = tmp - ((long long)(p-rM))*((long long)(p-rM)) + ((long long)(p-rm))*((long long)(p-rm));
		distance = min(distance, (int)sqrt(tmp));
	}
	return distance;
}

// Sort function to maintain active branch list
bool custom_sort(pair<int, int> a, pair<int, int> b) {
	return a.first <= b.first;
}


void rTreeNew::getNN(node *cur, vector<pair<int, int>> &point, int *result) {
	if (cur->isLeaf) {
		int distance;
		for(int i = 0; i < (int)cur->curLst.size(); i++) {
			distance = getMinDist(point, cur->curLst[i]->rect->mbr);
			if (distance < dist) {
				dist = distance;
				*result = cur->curLst[i]->url_id;
			}
		}
		return;
	}
	vector<pair<int, int>> abl;
	pair<int, int> tmp;
	int mindist, minmaxdist, down_prune = INT_MAX;
	for(int i = 0; i < (int)cur->curLst.size(); i++) {
		mindist = getMinDist(point, cur->curLst[i]->rect->mbr);
		minmaxdist = getMinMaxDist(point, cur->curLst[i]->rect->mbr);
		abl.push_back(make_pair(mindist, i));
		if (minmaxdist < down_prune) down_prune = minmaxdist; // For Downward Pruning
	}
	sort(abl.begin(), abl.end());
	auto itr = abl.begin();
	while (itr != abl.end() && itr->first < dist && itr->first < down_prune) { // For upward Pruning
		getNN(cur->curLst[itr->second]->child, point, result);
		itr++;
	}
	return;
}


void rTreeNew::getkNN(node *cur, vector<pair<int, int>> &point, set<pair<int, int>, greater<pair<int, int>>> &results) {
	if (cur->isLeaf) {
		int distance;
		for(int i = 0; i < (int)cur->curLst.size(); i++) {
			distance = getMinDist(point, cur->curLst[i]->rect->mbr);
			// untill result set is full
			if (!full) {
				topk.insert(distance);
				results.insert(make_pair(distance, cur->curLst[i]->url_id));
				if (topk.size() == k) full = true;
			} 
			// Compare with farthest result
			else if (distance < *topk.begin()) {
				topk.erase(topk.begin());
				topk.insert(distance);
				results.erase(results.begin());
				results.insert(make_pair(distance, cur->curLst[i]->url_id));
			}
		}
		return;
	}
	vector<pair<int, int>> abl;
	vector<int> helper;
	pair<int, int> tmp;
	int mindist, minmaxdist, down_prune = INT_MAX;
	for(int i = 0; i < (int)cur->curLst.size(); i++) {
		mindist = getMinDist(point, cur->curLst[i]->rect->mbr);
		minmaxdist = getMinMaxDist(point, cur->curLst[i]->rect->mbr);
		abl.push_back(make_pair(mindist, i));
		helper.push_back(minmaxdist);
	}
	sort(abl.begin(), abl.end());
	sort(helper.begin(), helper.end());
	if (helper.size() >= k) down_prune = helper[k-1]; // For Downward Pruning
	auto itr = abl.begin();
	while (itr != abl.end() && itr->first < *topk.begin() && itr->first < down_prune) { // For Upward Pruning
		getkNN(cur->curLst[itr->second]->child, point, results);
		itr++;
	}
	return;
}