#pragma once
#include <vector>

/*
* Tree implementation that will be used for the BFS
*/
using namespace std;
class Node
{
public:
	Node();
	Node(int mov, int squar, int e, Node* paren, vector<Node> childre);

	int getMove();
	void setMove(int mo);

	int getSquare();
	void setSquare(int s);

	int getEval();
	void setEval(int e);

	Node* getParent();
	void setParent(Node* paren);

	vector<Node> getChildren();
	void setChildren(vector<Node> child);

private:
	int move;
	int square;
	int eval;
	Node* parent;
	vector<Node> children;
};

class Tree
{
public:
	Tree();
	Tree(Node* roo);

	Node* getRoot();
	void setRoot(Node* n);

private:
	Node* root;
};