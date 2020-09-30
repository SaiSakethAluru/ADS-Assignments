/*
Assignment 1 : R-Tree
Team Members : Amshumaan Varma Pericherla, Pruthvi Sampath Chabathula
*/

#include<bits/stdc++.h>
#include "dataStructure.h" // import classes and method declarations 

using namespace std;

// Rectangle class methods

void rectangle::updateMbr(vector<pair<int, int> > &childRect) { 
	if(!isEmpty){

		pair<int, int> temp;
		for (int i = 0; i < dim; i++) {
			if (childRect[i].first < mbr[i].first) mbr[i].first = childRect[i].first;
			if (childRect[i].second > mbr[i].second) mbr[i].second = childRect[i].second;
		}
	}
	else
	{
		isEmpty = 0;
		for (int i = 0; i < dim; i++) {
			mbr[i].first = childRect[i].first;
			mbr[i].second = childRect[i].second;
		}
	}
	return;
}

int rectangle::getArea() {
	if(isEmpty)
		return -1;
	else
		{
			int perimeter = 0;
			for (int i = 0; i < dim; i++)
				perimeter = perimeter + (mbr[i].second - mbr[i].first);
			return perimeter;
		}
}








// Node class Methods

void node::addEntry(entry *newEntry) {
	newEntry->current = this;
	len++;
	curLst.push_back(newEntry);
	return;
}

node* node::chooseChild(entry *record) {
	int ind = 0;
	int inc_min = INT_MAX, inc;
	int dmin, dmax, nDmin, nDmax;
	int Dim = curLst[0]->rect->mbr.size();
	for (int i = 0; i < (int)curLst.size(); i++) { 
		inc = 0;
		for (int j = 0; j < Dim; j++) { //Look
			dmin = curLst[i]->rect->mbr[j].first;
			dmax = curLst[i]->rect->mbr[j].second;
			nDmin = record->rect->mbr[j].first;
			nDmax = record->rect->mbr[j].second;
			if (nDmin >= dmin && nDmax <= dmax) continue;
			inc += (max(dmax, nDmax) - min(dmin, nDmin)) - (dmax - dmin); //Look
		}
		if (inc < inc_min) ind = i;
	}
	return curLst[ind]->child;
}

void node::removeEntry(int index) 
{
	curLst.erase(curLst.begin()+index);
	len--;
}









// RTree class functions

// Quadratic Split Algorithm Implementation

int rTree::calwastage(rectangle* R1,rectangle* R2) 
{
	rectangle R = *R1;
	R.updateMbr(R2->mbr);
	int R_wastage = 0;
	int R1_wastage = 0;
	int R2_wastage = 0;
	for(int i=0; i<R.dim; i++)
	{
		R_wastage = R_wastage + (R.mbr[i].second - R.mbr[i].first);
		R1_wastage = R1_wastage + (R1->mbr[i].second - R1->mbr[i].first);
		R2_wastage = R2_wastage + (R2->mbr[i].second - R2->mbr[i].first);

	}

	return R_wastage - R1_wastage -R2_wastage;
}

void rTree::pickseeds(node* old, node* nw,node* temp) 
{
	
	rectangle* R1;
	rectangle* R2;
	entry* seedE1 = old->curLst[0];
	entry* seedE2 = old->curLst[1];
	R1 = old->curLst[0]->rect;
	R2 = old->curLst[1]->rect;
	int ind1 = 0;
	int ind2 = 1;
	int wastage;

	int maxwastage = calwastage(R1,R2);

	for(int i=0; i < old->len - 1; i++)
	{
		for(int j=i+1; j<old->len;j++)
		{
			R1 = old->curLst[i]->rect;
			R2 = old->curLst[j]->rect;
			wastage = calwastage(R1,R2);

			if(wastage>maxwastage)
			{
				seedE1 = old->curLst[i];
				seedE2 = old->curLst[j];
				ind1 = i;
				ind2 = j;
			}
		}
	}

	nw->addEntry(seedE1);
	nw->parent->rect->updateMbr(seedE1->rect->mbr);
	temp->addEntry(seedE2);
	temp->parent->rect->updateMbr(seedE2->rect->mbr);
	old->removeEntry(ind2); 
	old->removeEntry(ind1);

}

entry* rTree::picknext(node* old,node* temp,node* nw) 
{
	int max_diff = -1;
	int index;
	entry* nxt;
	int d1,d2;
	for(int i=0; i<old->len; i++)
	{
		rectangle R1 = *(nw->parent->rect);
		d1 = R1.getArea();
		rectangle R2 = *(temp->parent->rect);
		d2 = R2.getArea();
		R1.updateMbr(old->curLst[i]->rect->mbr);
		R2.updateMbr(old->curLst[i]->rect->mbr);

		d1 = d1-R1.getArea();
		d2 = d2-R2.getArea();

		if(abs(d1-d2)>max_diff)
		{
			max_diff = abs(d1-d2);
			nxt = old->curLst[i];
			index = i;
		
		}
	}

	old->removeEntry(index);
	return nxt;
}

void rTree::quadSplit(node* old,node* nw){ 
	node* temp = new node();
	entry* new_par = new entry(dim);
	temp->parent = new_par;
	new_par->child = temp;

	temp->isLeaf = old->isLeaf;
	nw->isLeaf = old->isLeaf;

	entry* E;
	rectangle* R1 = new rectangle(dim);
	rectangle* R2 = new rectangle(dim);
	int area1,area2,tem1,tem2;

	pickseeds(old,nw,temp);

	while(old->len > 0)
	{	

		int x, y;
		x = m-nw->len;
		y = m-temp->len;
		if(old->len == x)
		{
			for(int i=0; i < x; i++) {
				nw->addEntry(old->curLst[i]);
				nw->parent->rect->updateMbr(old->curLst[i]->rect->mbr);
			}
				

			for(int i=0; i < x; i++)
				old->removeEntry(0);
		}

		else if(old->len == y)
		{
			for(int i=0; i < y; i++) {
				temp->addEntry(old->curLst[i]);
				temp->parent->rect->updateMbr(old->curLst[i]->rect->mbr);
			}
			

			for(int i=0; i < y; i++)
				old->removeEntry(0);
		}
		else
		{
			entry* nxt = picknext(old,temp,nw);
			*R1 = *(nw->parent->rect);
			*R2 = *(temp->parent->rect);
			area1 = R1->getArea();
			area2 = R2->getArea();

			R1->updateMbr(nxt->rect->mbr);
			R2->updateMbr(nxt->rect->mbr);

			tem1 = area1;
			tem2 = area2;
			area1 = R1->getArea()-area1;
			area2 = R1->getArea()-area2;

			if(area1>area2)
			{
				temp->addEntry(nxt);
				temp->parent->rect->updateMbr(nxt->rect->mbr);

			}
			else if (area1<area2)
			{
				nw->addEntry(nxt);
				nw->parent->rect->updateMbr(nxt->rect->mbr);

			}
			else
			{
				if(tem1 < tem2)
				{
					nw->addEntry(nxt);
					nw->parent->rect->updateMbr(nxt->rect->mbr);
				}
				else if(tem2 < tem1)
				{
					temp->addEntry(nxt);
					temp->parent->rect->updateMbr(nxt->rect->mbr);
				}
				else
				{
					if(nw->len < temp->len)
					{
						nw->addEntry(nxt);
						nw->parent->rect->updateMbr(nxt->rect->mbr);
					}
					else
					{
						temp->addEntry(nxt);
						temp->parent->rect->updateMbr(nxt->rect->mbr);
					}
				}
			}
		}
	} 
	delete R1;
	delete R2;
	old->curLst.clear();
	for (int i = 0; i < (int) temp->curLst.size(); i++) old->addEntry(temp->curLst[i]);
	if(old->parent!=NULL) 
	{
		delete old->parent->rect;
		old->parent->rect = new rectangle(dim);
		old->parent->rect->updateMbr(temp->parent->rect->mbr);
	}
	else old->parent = temp->parent;
}














// Methods to Insert new record and Adjust the Tree

node* rTree::getLeaf(node* cur, entry *record) {
	if (cur->isLeaf) return cur;
	node *next = cur->chooseChild(record);
	return getLeaf(next, record);
}

void rTree::adjustTree(node* nod,entry* newRecord) 
{
	if(nod->parent != NULL && newRecord != NULL)
	{	
		if (nod->parent->rect == NULL) 
		if (newRecord->rect == NULL) 
		nod->parent->rect->updateMbr(newRecord->rect->mbr);
		adjustTree(nod->parent->current,nod->parent);
	}
}

void rTree::adjustTree(node* nod1, node* nod2, node* pnode) 
{	
	if(pnode == NULL)
	{	
		root = new node();

		root->addEntry(nod1->parent);
		root->addEntry(nod2->parent);
		height++;
	}
	else
	{	

		pnode->addEntry(nod2->parent);
		if(pnode->len > M)
		{	
			node* parentnode;
			if (pnode->parent == NULL) parentnode = NULL;
			else parentnode = pnode->parent->current;
			node* nw = new node();
			entry* new_par = new entry(dim);
			nw->parent = new_par;
			new_par->child = nw;
			quadSplit(pnode,nw);
			adjustTree(pnode,nw,parentnode);
		}
		else
		{ 	
			adjustTree(pnode,nod2->parent);
		}
	}
}

void rTree::insertRecord(entry *newRecord) { 
	if (root == NULL) {
		root = new node();
		root->isLeaf = true;
		root->addEntry(newRecord);
		height++;
		return;
	}

	node* leaf = getLeaf(root, newRecord); 
	leaf->addEntry(newRecord);

	if(leaf->len > M)
	{	
		node* parentnode;
		if (leaf->parent == NULL) parentnode = NULL;
		else parentnode = leaf->parent->current; 
		node* nw = new node();
		entry* new_par = new entry(dim);
		nw->parent = new_par;
		nw->parent->child = nw;
		quadSplit(leaf,nw);
		adjustTree(leaf,nw,parentnode);
	}
	else
	{	
		adjustTree(leaf,newRecord);
	}

	return;
}














// Search Methods

/*
void rTree::searchTree(node *cur, rectangle *find, vector<rectangle> &result) {
	for (int i = 0; i < (int)cur->curLst.size(); i++) {
		bool intersect = true;
		for (int j = 0; j < dim; j++) {
			if (find->mbr[j].second < cur->curLst[i]->rect->mbr[j].first || find->mbr[j].first > cur->curLst[i]->rect->mbr[j].second) {
				intersect = false;
				break;
			}
		}
		if (intersect) {
			if (cur->isLeaf) result.push_back(*(cur->curLst[i]->rect));
			else searchTree(cur->curLst[i]->child, find, result);
		}
	}
	return;
}
*/

void rTree::searchTree(node *cur, vector<pair<int, int>> find, int* result) {

	if (cur->isLeaf) {
		for (int i = 0; i < (int)cur->curLst.size(); i++) {
			bool intersect = true;
			for (int j = 0; j < dim; j++) {
				if (find[j].second < cur->curLst[i]->rect->mbr[j].first || find[j].first > cur->curLst[i]->rect->mbr[j].second) {
					intersect = false;
					break;
				}
			}
			if (intersect) *result = cur->curLst[i]->url_id;
		}
		return;
	}
	for (int i = 0; i < (int)cur->curLst.size(); i++) {
		bool intersect = true;
		for (int j = 0; j < dim; j++) {
			if (find[j].second < cur->curLst[i]->rect->mbr[j].first || find[j].first > cur->curLst[i]->rect->mbr[j].second) {
				intersect = false;
				break;
			}
		}
		if (intersect) {
			searchTree(cur->curLst[i]->child, find, result);
		}
	}
	return;
}

bool rTree::exists(node *cur, rectangle *find) {
	if (cur->isLeaf) {
		bool present = false;
		for (int i = 0; i < (int)cur->curLst.size(); i++) {
			bool equal = true;
			for (int j = 0; j < dim; j++) {
				if (find->mbr[j] == cur->curLst[i]->rect->mbr[j]) {
					continue;
				}
				equal = false;
			}
			if (equal) {
				present = true;
				break;
			}
		}
		return present;
	}
	bool present = false;
	for (int i = 0; i < (int)cur->curLst.size(); i++) {
		bool intersect = true;
		for (int j = 0; j < dim; j++) {
			if (find->mbr[j].second < cur->curLst[i]->rect->mbr[j].first || find->mbr[j].first > cur->curLst[i]->rect->mbr[j].second) {
				intersect = false;
				break;
			}
		}
		if (intersect) {
			bool pres = exists(cur->curLst[i]->child, find);
			if(pres) {
				present = true;
				break;
			}
		}
		if (present) break;
	}
	return present;
}













// To store the RTree in a File

void printRect(vector<pair<int, int>> rect) {
	for (int i = 0; i < (int)rect.size(); i++) {
		file << rect[i].first << " " << rect[i].second << " ";
	}
	file << endl;
}

void rTree::printTree(node *cur) {
	if (cur->isLeaf) {
		file << 0 << " " << (int) cur->curLst.size() << endl;
		for (int i = 0; i < (int) cur->curLst.size(); i++) {
			printRect(cur->curLst[i]->rect->mbr);
			file << cur->curLst[i]->url_id << endl;
		}
		return;
	}
	file << 1 << " " << (int) cur->curLst.size() << endl;
	for (int i = 0; i < (int) cur->curLst.size(); i++) {
		printRect(cur->curLst[i]->rect->mbr);
	}
	for (int i = 0; i < (int) cur->curLst.size(); i++) {
		printTree(cur->curLst[i]->child);
	}
	return;
}

void rTree::saveTree(string filename) {
	file.open(filename.c_str(), ios::out | ios::trunc);
	file << dim << " " << m << " " << M << " " << height << endl;
	printTree(root);
	file.close();
	return;
}















// To restore the RTree from a file

void rTree::readInto(node *cur) {
	int leaf_val, lst_size;
	file1 >> leaf_val >> lst_size;
	if (leaf_val == 0) cur->isLeaf = true;
	cur->curLst.resize(lst_size);
	cur->len = lst_size;
	vector<pair<int, int>> box(dim, make_pair(0, 0));
	for (int i = 0; i < lst_size; i++) {
		for (int j = 0; j < dim; j++) file1 >> box[j].first >> box[j].second;
		cur->curLst[i] = new entry(dim);
		cur->curLst[i]->current = cur;
		cur->curLst[i]->rect->updateMbr(box);
		if (leaf_val == 0) {
			int url;
			file1 >> url;
			cur->curLst[i]->url_id = url;
		}
	}
	if (leaf_val == 1) {
		for (int i = 0; i < lst_size; i++) {
			cur->curLst[i]->child = new node();
			cur->curLst[i]->child->parent = cur->curLst[i];
			readInto(cur->curLst[i]->child);
		}
	}
	return;
}

void rTree::loadTree(string filename) {
	file1.open(filename.c_str(), ios::in);
	file1 >> dim >> m >> M >> height;
	root = new node();
	readInto(root);
	file1.close();
}

