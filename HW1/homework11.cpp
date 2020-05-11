//#include "definition.h"
#include <cstdlib>
#include <vector>
#include <queue>
#include <cmath>
#include <regex>
#include <string>
#include <fstream>
#include <iostream>
#include<sstream>
#include<unordered_map>
using namespace std;

struct targetNode
{
	int r = 0;
	int c = 0;
	int height = 0;
	targetNode* pointer = NULL;
};

struct Node
{
	int r = 0;
	int c = 0;
	long height = 0;
	long cost = 0;
	bool visited = false;
	Node* pointer = NULL;
	bool failure = false;
};

struct costcompare {

	bool operator ()(Node*& n1,Node*& n2) {
		if (n1->cost > n2->cost)
			return true;
		else
		{
			return false;
		}
	}
};

vector<vector<string>> line_split;
int width, row, lc, lr, maxelevation, target_num;
vector<vector<Node*>> nodelist;
vector<targetNode> targetlist;
vector<int> dc = { -1,0,1,1,1,0,-1,-1 };
vector<int> dr = { -1,-1,-1,0,1,1,1,0 };
Node* failure_case=new Node;

void readtxt() {
    ifstream infile("input.txt");
	string line;
	while (getline(infile, line)) {
		vector<string> thisline;
		string split;
		stringstream input_word(line);
		while (input_word >> split) {
			thisline.push_back(split);
		}
		line_split.push_back(thisline);
	}
	infile.close();
}

void value_assign() {
	width = stoi((line_split[1])[0]);
	row = stoi((line_split[1])[1]);
	lc = stoi((line_split[2])[0]);
	lr= stoi((line_split[2])[1]);
	maxelevation = stoi((line_split[3])[0]);
	target_num = stoi((line_split[4])[0]);
	failure_case->failure = true;
	for (int i = 5; i < 5 + target_num; i++) {
		targetNode* temp=new targetNode;
		temp->c = stoi((line_split[i])[0]);
		temp->r = stoi((line_split[i])[1]);
		targetlist.push_back(*temp);
	}	
	int x1 = 0;
	int y1 = 0;
	for (int j = 5 + target_num; j < 5 + target_num + row; j++) {
		vector<Node*> temp1;
		for (int k = 0; k < width; k++)
		{
			Node *temp=new Node;
			temp->c = y1;
			temp->r = x1;
			temp->height=stoi(line_split[j][k]);
			temp->cost = 0;
			temp1.push_back(temp);
			y1++;
		}
		y1 = 0;
		x1++;
		nodelist.push_back(temp1);
	}
	for (int i = 0; i < targetlist.size(); i++) {
		int temp_c = targetlist[i].c;
		int temp_r = targetlist[i].r;
		targetlist[i].height = nodelist[temp_r][temp_c]->height;
	}
}

void bfs_expand(Node *&curnode, queue<Node*> &nodes) {
	for (int i = 0; i < 8; i++)
	{
		if ((curnode->r + dr[i] >= 0) && (curnode->r + dr[i] <= row - 1) && (curnode->c + dc[i] >= 0) && (curnode->c + dc[i] <= width - 1)) {
			int r = curnode->r + dr[i];
			int c = curnode->c + dc[i];
			if ((abs(curnode->height - nodelist[r][c]->height) <= maxelevation) && !nodelist[r][c]->visited) {
				nodes.push(nodelist[r][c]);
				nodelist[r][c]->visited = true;
				nodelist[r][c]->pointer = curnode;				
			}
		}
	}
}

Node* bfs(targetNode targetnode) {
	queue<Node*> nodes;
	nodes.push(nodelist[lr][lc]);
	nodelist[lr][lc]->visited = true;
	while (true) {
		if (nodes.empty()) {
			return failure_case;
		}		
		Node* curnode = nodes.front();
		nodes.pop();
		if (curnode->r == targetnode.r && curnode->c == targetnode.c) {
			return curnode;
		}				
		Node*& c = curnode;
		queue<Node*>& n = nodes;
		bfs_expand(c,n);
	}
	return failure_case;
}

void bfsrestart() {
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < width; j++)
		{
			nodelist[i][j]->pointer = NULL;
			nodelist[i][j]->visited = false;
		}
	}
}

void delete_node(Node*& curnode, priority_queue<Node*, vector<Node*>, costcompare>& children) {
	priority_queue<Node*, vector<Node*>, costcompare> tempqueue;
	while (!children.empty())
	{
		Node* temp = children.top();
		if (temp->c == curnode->c && temp->r == curnode->r) {
			children.pop();
		}
		else {
			children.pop();
			tempqueue.push(temp);
		}
	}
	while (!tempqueue.empty())
	{
		Node* temp1 = tempqueue.top();
		children.push(temp1);
		tempqueue.pop();
	}
}

Node* ucs(targetNode targetnode) {
	priority_queue<Node*, vector<Node*>,costcompare> open;
	unordered_map<string, bool> is_explored;
	vector<vector<bool>> closed_visited(row, vector<bool>(width, false));
	nodelist[lr][lc]->visited = true;
	open.push(nodelist[lr][lc]);
	while (true)
	{
		if (open.empty())
			return failure_case;
		Node* curnode= open.top();
		open.pop();
		if (curnode->c == targetnode.c && curnode->r == targetnode.r)
			return curnode;
		for (int i = 0; i < 8; i++)
		{

			if ((curnode->r + dr[i] >= 0) && (curnode->r + dr[i] <= row - 1) && (curnode->c + dc[i] >= 0) && (curnode->c + dc[i] <= width - 1)) {
				int r = curnode->r + dr[i];
				int c = curnode->c + dc[i];
				if (abs(curnode->height - nodelist[r][c]->height) <= maxelevation) {
					Node* node_expand = nodelist[r][c];					
					long temp = 0;
					if (dr[i] + dc[i] == 0 || dr[i] + dc[i] == -2 || dr[i] + dc[i] == 2) {
						temp = 14 + curnode->cost;
					}
					else {
						temp = 10 + curnode->cost;
					}
					if (closed_visited[node_expand->r][node_expand->c])
						continue;
					if (is_explored[to_string(node_expand->r) + "," + to_string(node_expand->c)]) {
						if (temp < node_expand->cost)
						{
							node_expand->cost = temp;
							node_expand->pointer = curnode;
							priority_queue<Node*, vector<Node*>, costcompare>& open_refer = open;
							Node*& node_expand_refer = node_expand;
							delete_node(node_expand_refer, open_refer);
							open.push(node_expand);
						}
					}
					else {
						node_expand->cost = temp;
						node_expand->pointer = curnode;
						open.push(node_expand);
						is_explored[to_string(node_expand->r) + "," + to_string(node_expand->c)] = true;
					}
				}
			}
		}
		closed_visited[curnode->r][curnode->c] = true;
		is_explored[to_string(curnode->r) + "," + to_string(curnode->c)] = false;
	}
	return failure_case;
}

void ucs_restart() {
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < width; j++)
		{
			nodelist[i][j]->visited = false;
			nodelist[i][j]->cost = 0;
			nodelist[i][j]->pointer = NULL;
		}
	}
}

long heuristic(Node* curnode, targetNode targetnode) {
	int col_dif = abs(curnode->c - targetnode.c);
	int row_dif = abs(curnode->r - targetnode.r);
	int dif = abs(col_dif - row_dif);
	long temp = min(col_dif, row_dif) * 14 + dif * 10 + abs(curnode->height - targetnode.height);
	return min(col_dif, row_dif) * 14 + dif * 10 + abs(curnode->height-targetnode.height);
}

Node* astar(targetNode targetnode) {
	priority_queue<Node*, vector<Node*>, costcompare> open;
	unordered_map<string, bool> is_explored;
	vector<vector<bool>> closed_visited(row, vector<bool>(width, false));
	nodelist[lr][lc]->cost = heuristic(nodelist[lr][lc], targetnode);
	open.push(nodelist[lr][lc]);
	while (true)
	{
		if (open.empty())
			return failure_case;
		Node* curnode = open.top();
		open.pop();
		if (curnode->c == targetnode.c && curnode->r == targetnode.r)
			return curnode;
		for (int i = 0; i < 8; i++)
		{
			if ((curnode->r + dr[i] >= 0) && (curnode->r + dr[i] <= row - 1) && (curnode->c + dc[i] >= 0) && (curnode->c + dc[i] <= width - 1)) {
				int r = curnode->r + dr[i];
				int c = curnode->c + dc[i];
				if (abs(curnode->height - nodelist[r][c]->height) <= maxelevation) {
					Node* node_expand = nodelist[r][c];
					long temp = 0;
					if (dr[i] + dc[i] == 0 || dr[i] + dc[i] == -2 || dr[i] + dc[i] == 2) {
						temp = 14 + (curnode->cost - heuristic(curnode, targetnode)) + abs(curnode->height - nodelist[r][c]->height);
						temp += heuristic(node_expand, targetnode);
					}
					else {
						temp = 10 + (curnode->cost - heuristic(curnode, targetnode)) + abs(curnode->height - nodelist[r][c]->height);
						temp += heuristic(node_expand, targetnode);
					}					
					if (closed_visited[node_expand->r][node_expand->c])
						continue;					
					if (is_explored[to_string(node_expand->r) + "," + to_string(node_expand->c)]) {
						if (temp < node_expand->cost)
						{
							node_expand->cost = temp;
							node_expand->pointer = curnode;
							priority_queue<Node*, vector<Node*>, costcompare>& open_refer = open;
							Node*& node_expand_refer = node_expand;
							delete_node(node_expand_refer, open_refer);
							open.push(node_expand);
						}
					}
					else {
						node_expand->cost = temp;
						node_expand->pointer = curnode;
						open.push(node_expand);
						is_explored[to_string(node_expand->r) + "," + to_string(node_expand->c)] = true;
					}
				}
			}
		}
		closed_visited[curnode->r][curnode->c] = true;
	    is_explored[to_string(curnode->r) + "," + to_string(curnode->c)] = false;
	}
	return failure_case;
}

void astar_restart() {
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < width; j++)
		{
			nodelist[i][j]->visited = false;
			nodelist[i][j]->cost = 0;
			nodelist[i][j]->pointer = NULL;
		}
	}
}


int main()
{
	readtxt();
	value_assign();
	if (line_split[0][0] == "BFS") {
		Node *result;
		ofstream savefile("output.txt");
		for (int i = 0; i < targetlist.size(); i++)
		{
			result = bfs(targetlist[i]);
			if (result->failure) {
				savefile << "FAIL";
			}
			else {
				vector<Node*> paths;
				while (true)
				{
					Node* temp;
					temp = result;
					paths.push_back(result);
					if (temp->c == lc && temp->r == lr)
						break;
					result = result->pointer;
				}
				for (int i = paths.size() - 1; i >= 0; i--)
				{
					savefile << paths[i]->c << ',' << paths[i]->r << ' ';
				}
			}
			if(i<targetlist.size()-1)
				savefile << '\n';
			bfsrestart();
		}
		savefile.close();
	}
	else if (line_split[0][0] == "UCS") {
		Node* result_ucs;
		ofstream savefile("output.txt");
		for (int i = 0; i < targetlist.size(); i++)
		{
			result_ucs = ucs(targetlist[i]);
			if (result_ucs->failure) {
				savefile << "FAIL";
			}
			else
			{
				vector<Node*> paths;
				while (true)
				{
					Node* temp;
					temp = result_ucs;
					paths.push_back(result_ucs);
					if (temp->c == lc && temp->r == lr)
						break;
					result_ucs = result_ucs->pointer;
				}
				for (int i = paths.size() - 1; i >= 0; i--)
				{
					savefile << paths[i]->c << ',' << paths[i]->r << ' ';
				}
			}
			if (i < targetlist.size() - 1)
				savefile << '\n';
			ucs_restart();
		}
		savefile.close();
	}
	else if (line_split[0][0]=="A*")
	{
		Node* result_Astar;
		ofstream savefile("output.txt");
		for (int i = 0; i < targetlist.size(); i++)
		{
			result_Astar = astar(targetlist[i]);
			if (result_Astar->failure) {
				savefile << "FAIL";
			}
			else
			{
				vector<Node*> paths;
				while (true)
				{
					Node* temp;
					temp = result_Astar;
					paths.push_back(result_Astar);
					if (temp->c == lc && temp->r == lr)
						break;
					result_Astar = result_Astar->pointer;
				}
				for (int i = paths.size() - 1; i >= 0; i--)
				{
					savefile << paths[i]->c << ',' << paths[i]->r << ' ';
				}
			}
			if (i < targetlist.size() - 1)
				savefile << '\n';
			astar_restart();
		}
		savefile.close();
	}
}
