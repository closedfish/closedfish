
#include <vector>
#include <array>
#include <iostream>
#include "../../lib/board_implementation/CFBoard.h"
#include "DFS1p.h"
#include <string>

using namespace std;


//evaluates the position might be different from stockfish. to be implemented later on
int eval()
{
	//gives the evaluation
	return 0;
}

//returns number of possible moves by every piece
int numMoves(int n)
{
	//input piece id
	// output total possible moves
	if (n % 2 == 1)
		n--;
	if (n == 0)
		return 6;
	if (n == 2)
		return 8;
	if (n == 4)
		return 28;
	if (n == 6)
		return 28;
	if (n == 8)
		return 56;
	return 10;
}

int newTile(int piece, int move)
{
	//input the piece id and the move id
	//output the change in square

	if (numMoves(piece) < move)
		return 0;
	else
	{
		switch (piece)
		{
			if (piece % 2 == 1)
				return (-1 * newTile(piece - 1, move));
			int k = 0;
			case 0:
				if (move == 1)
					k= - 8;
				else if (move == 2)
					k= - 16;
				else if (move == 3 || move == 5)
					k= - 7;
				else if (move == 4|| move ==6)
					k= - 9;

			case 2:
				if (move == 1)
					k = 17;
				else if (move == 2)
					k = 10;
				else if (move == 3)
					k = -6;
				else if (move == 4)
					k = -15;
				else
					k = -1 * k;

			case 4:
				if ( move < 7)
					k = 7 * k;
				else if (move < 14)
					k = -7 * k;
				else if (move < 21)
					k = -9 * k;
				else
					k = 9 * k;

			case 6:
				if (move < 7)
					k = -1 * k;
				else if (move < 14)
					k = -8 * k;
				else if (move < 21)
					k = 1 * k;
				else
					k = 8 * k;

			case 8:
				int d = (int)move / 7;
				if (d % 2 == 0)
					return(newTile(4, move - 7 * d / 2));
				else
					move -= 7;
				return (newTile(6, move - 7 * d / 2));

			case 10:
				if (move == 1)
					k = 7;
				else if (move == 2)
					k = -1;
				else if (move == 3)
					k = -7;
				else if (move == 4)
					k = -8;
				else if (move == 5)
					k = -9;
				else if (move == 6)
					k = 1;
				else if (move == 7)
					k = 9;
				else if (move == 8)
					k = 8;
				else
					k = 2;
			return  k;

		}
	}
}

bool isSquareInBoard(int sq)
{
	//input a square. 
	//outputs true if its in the board
	if (sq >= 0 && sq <= 63)
		return true;
	else
		return false;
}

//checks if a move is stupid or not
// a move is stupid 
bool is_stupid(CFBoard& board)
{

}

/*
* the main idea has been optimized a lot more than last week and to that end,
* the DFS has been changed quite a lot
* 
* the main thing to do is to take a position, and keep on making moves for only one side.
* After every 5 moves we can use a naive evaluation criteria and discard all but the top line
* 
* essentially we can represent every legal move as a node and every node will give rise to a new set of nodes and hence our tree
* 
*/

int DFSHelp(CFBoard board, Node n, int depth, string fen, int top, string fenTop)
{
	if (depth==5 && eval()>top)
	{
			top = eval();
			fenTop = fen;
	}
	else if (depth < 5)
	{

	}
}

void Dfs_main(CFBoard& board)
{
	//Input: a board. 
	//Output: the top line from the 1 person BFS
	//for now its only white moving

	//makes a vector of all possible moves
	vector<Node> moves;
	for (int i = 0; i < 64; i++)
	{
		int piece = board.getBit(i);
		if (piece >= 0)
		{
			int tot = numMoves(piece);
			for (int j = 0; j < tot; j++)
			{
				if (board.isLegal(j))
				{
					Node n = Node(j, i, 0, NULL, { Node()});
					moves.push_back(n);
				}
			}
		}

	}
	
	// the actual DFS will start
	for (Node n : moves)
	{

	}
	
}

// Tree structure for the DFS

Node::Node()
{
	move = 0;
	square = 0;
	eval = 0;
	parent = NULL;
	children = {Node()};
}

Node::Node(int mov, int squar, int e, Node* paren, vector<Node> childre)
{
	move = mov;
	squar = square;
	eval = e;
	parent = parent;
	children = childre;
}

int Node::getMove()
{
	return move;
}

void Node::setMove(int mo)
{
	move = mo;
}

int Node::getSquare()
{
	return square;
}

void Node::setSquare(int s)
{
	square = s;
}

int Node::getEval()
{
	return eval;
}

void Node::setEval(int e)
{
	eval = e;
}

Node* Node::getParent()
{
	return parent;
}

void Node::setParent(Node* paren)
{
	parent = paren;
}

vector<Node> Node::getChildren()
{
	return children;
}

void Node::setChildren(vector<Node> child)
{
	children = child;
}

Tree::Tree()
{
	root = NULL;
}

Tree::Tree(Node* roo)
{
	root = roo;
}

Node* Tree::getRoot()
{
	return root;
}

void Tree::setRoot(Node* n)
{
	root = n;
}
