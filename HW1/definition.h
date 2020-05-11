#ifndef DEFINITION_H
#define DEFINITION_H

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

	bool operator() (Node*& n1, Node*& n2) {
		return n1->cost > n2->cost;
	}
};

#endif 
