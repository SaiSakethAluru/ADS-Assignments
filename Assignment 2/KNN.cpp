/*
Team details:
Sai Saketh Aluru - 16CS30030
K Sai Surya Teja - 16CS30015
Sasi Bhushan Seelaboyina - 16CS30032
*/

#include "RTree.h"
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

RTreeNode::RTreeNode(int m, int M, int n)
{
    this->isLeaf = true;
    this->num_entries = 0;
    this->bounds = vector<pair<int, int> >(n);
}

// Constructor for RTree class
RTree::RTree(int n)
{
    this->n = n;
    this->root = nullptr;
    this->M = 4096 / (4 * n + 1);
    this->m = this->M / 2;
    this->num_nodes = 0;
}

double calculate_distance(vector<pair<int, int> > &point1, vector<pair<int, int> > &point2)
{
    double out = 0;
    for (int i = 0; i < point2.size(); i++)
    {
        out += ((point1[i].first - point2[i].first) * (point1[i].first - point2[i].first));
    }
    return sqrt(out);
}

double min_dist(vector<pair<int, int> > &point, vector<pair<int, int> > &rectangle)
{
    double out = 0;
    for (int i = 0; i < point.size(); i++)
    {
        int r = point[i].first;
        if (point[i].first < rectangle[i].first)
            r = rectangle[i].first;
        else if (point[i].second > rectangle[i].second)
            r = rectangle[i].second;
        out += ((point[i].first - r) * (point[i].first - r));
    }
    return sqrt(out);
}

void NNDistance(RTreeNode *node, vector<pair<int, int> > &point, vector<double> &distances, int k, stack<RTreeNode *> &st, long long &count)
{
    count++;
    // If node is leaf
    if (node->isLeaf == true)
    {
        int i;
        for (int i = 0; i < node->num_entries; i++)
        {
            // If heap has space
            if (distances.size() < k)
            {
                distances.push_back(calculate_distance(point, node->child_bounds[i]));
                push_heap(distances.begin(), distances.end());
            }
            // Else heap is full
            else
            {
                double dist = calculate_distance(point, node->child_bounds[i]);
                if (dist < distances.front())
                {
                    pop_heap(distances.begin(), distances.end());
                    distances.pop_back();
                    distances.push_back(dist);
                    push_heap(distances.begin(), distances.end());
                }
            }
        }
        if (!st.empty())
        {
            RTreeNode *next_node = st.top();
            st.pop();
            NNDistance(next_node, point, distances, k, st, count);
        }
    }
    // If node is not a leaf
    else
    {
        for (int i = 0; i < node->num_entries; i++)
        {
            if (distances.size() == k && min_dist(point, node->child_bounds[i]) > distances.front())
            {
            }
            else
                st.push(node->pointers[i]);
        }
        if (!st.empty())
        {
            RTreeNode *next_node = st.top();
            st.pop();
            NNDistance(next_node, point, distances, k, st, count);
        }
    }
}

// Read the data of a single RTreeNode from file stream and return the constructed node object
RTreeNode *read_node(fstream &f, int m, int M, int n)
{
    int node_id, num_entries, isLeaf;
    f >> node_id >> num_entries >> isLeaf;
    RTreeNode *node = new RTreeNode(m, M, n);
    node->num_entries = 0;
    node->node_id = node_id;
    node->isLeaf = isLeaf;
    for (int i = 0; i < n; i++)
    {
        f >> node->bounds[i].first >> node->bounds[i].second;
    }
    if (node->isLeaf)
    {
        node->num_entries = num_entries;
        node->child_bounds = vector<vector<pair<int, int> > >(num_entries, vector<pair<int, int> >(n));
        vector<pair<int, int> > v;
        for (int i = 0; i < num_entries; i++)
        {
            for (int j = 0; j < n; j++)
            {
                int a, b;
                f >> a >> b;
                node->child_bounds[i][j] = make_pair(a, b);
            }
        }
    }
    return node;
}

// Function to read data from passed filename and return RTree object
RTree *load_tree(string filename)
{
    fstream f(filename);
    int num_nodes;
    f >> num_nodes;
    vector<int> parent(num_nodes);
    int m, M, n;
    f >> m >> M >> n;
    RTree *tree = new RTree(n);
    tree->num_nodes = num_nodes;
    for (int i = 0; i < num_nodes; i++)
    {
        f >> parent[i];
    }
    vector<RTreeNode *> nodes(num_nodes);
    for (int i = 0; i < num_nodes; i++)
    {
        RTreeNode *temp = read_node(f, m, M, n);
        nodes[temp->node_id] = temp;
    }
    for (int i = 0; i < num_nodes; i++)
    {
        if (parent[i] == -1)
        {
            tree->root = nodes[i];
        }
        else
        {
            nodes[parent[i]]->pointers.push_back(nodes[i]);
            nodes[parent[i]]->child_bounds.push_back(nodes[i]->bounds);
            nodes[parent[i]]->num_entries++;
        }
    }
    return tree;
}

int main()
{
    int n = 100;
    RTree *tree = load_tree("n" + to_string(n) + "Tree.txt");
    srand(time(0));
    long long avg_count = 0;
    double avg_time = 0.0;
    for (int i = 0; i < 50; i++)
    {
        vector<pair<int, int> > point;
        for (int j = 0; j < n; j++)
        {
            int rand1 = 30 * (float)rand() / RAND_MAX + 10;
            point.push_back(make_pair(rand1, rand1));
        }
        long long count = 0;
        vector<double> distances;
        stack<RTreeNode *> st;
        clock_t begin = clock();
        NNDistance(tree->root, point, distances, 5, st, count);
        clock_t end = clock();
        double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

        avg_count += count;
        avg_time += elapsed_secs;
        cout << avg_count / (i + 1) << " " << avg_time / (i + 1) << endl;
    }
    cout << "average count: " << avg_count / 50 << endl;
    cout << "average time: " << avg_time / 50 << endl;
}