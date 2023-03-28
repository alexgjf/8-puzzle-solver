#include <bits/stdc++.h>
#include <iostream>
#include <algorithm>

using namespace std;
#define N 3

map<int, vector<int> > visited;
int key = 0;

struct Node
{
	int mat[N][N];
	Node *parent;
	int x, y;
	int cost;
	int depth;
};

Node* newNode(int mat[N][N], int x, int y, int new_x, int new_y, int depth, Node* parent)
{
	Node* node = new Node;
	node->parent = parent;
	//
	memcpy(node->mat, mat, sizeof node->mat);
	swap(node->mat[x][y], node->mat[new_x][new_y]);
	node->cost = INT_MAX;
	node->depth = depth;
	node->x = new_x;
	node->y = new_y;
	return node;
}

//calculate misplaced tiles
int calculateCost(int initial[N][N])
{
    int count = 0;
    int final[N][N]=
	{
		{1,2,3},
		{4,5,6},
		{7,8,0}
	};
    for (int i = 0; i < N; i++)
      for (int j = 0; j < N; j++)
        if (initial[i][j] && initial[i][j] != final[i][j])
           count++;
    return count;
}

//calulate euclidean distance
int calculateEuc(int initial[N][N])
{
	double count = 0;
    int final[N][N]=
	{
		{1,2,3},
		{4,5,6},
		{7,8,0}
	};
	for (int i = 0; i < N; i++)
      for (int j = 0; j < N; j++)
        if (initial[i][j] && initial[i][j] != final[i][j])
        {
        	for(int k = 0; k < N ; k++)
        	{
        		for(int p = 0; p < N; p++)
        		{
        			if(initial[i][j] == final[k][p])
					{
						count += sqrt(pow(i - k, 2) + pow(j - p, 2));
					}
				}
			}
		}
    return int(ceil(count));
}

void printPuzzle(int mat[N][N])
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			cout << mat[i][j] << " ";
		}
		cout << endl;
	}
}

//for uniform
struct comp
{
	bool operator()(const Node* left, const Node* right) const
	{
		return (left->cost) > (right->cost);
	}
};

//for a*
struct comp_a
{
	bool operator()(const Node* left, const Node* right) const
	{
		return (left->cost + left->depth) > (right->cost + right->depth);
	}
};

bool check_visited(int mat[N][N])
{
	vector<int> temp;
	for(int i = 0; i<N ; i++)
	{
		for(int j = 0; j < N; j++)
		{
			temp.push_back(mat[i][j]);
		}
	}
	
	for(auto v:visited) 
	{
		int check = 0;
		vector<int> data = v.second;
		for(int j = 0; j<N*N; j++)
		{
			if(data[j]==temp[j]) check++;
		}
		if(check == 9) return true;
	}
	return false;
}

void insert_visited(int mat[N][N])
{
	vector<int> temp;
	for(int i = 0; i<N ; i++)
	{
		for(int j = 0; j < N; j++)
		{
			temp.push_back(mat[i][j]);
		}
	}
	
	visited.insert({key, temp});
	key++;
	//cout<<visited.size()<<endl;
}

bool goal_test(int mat[N][N]){
	if(mat[0][0]!= 1||
	mat[0][1]!= 2||
	mat[0][2]!= 3||
	mat[1][0]!= 4||
	mat[1][1]!= 5||
	mat[1][2]!= 6||
	mat[2][0]!= 7||
	mat[2][1]!= 8||
	mat[2][2]!= 0) return false;
	return true;
}

// print path from root node to destination node
void printPath(Node* root)
{
    if (root == NULL)
        return;
    printPath(root->parent);
    printPuzzle(root->mat);
 
    printf("\n");
}

//Uniform cost search
void uniform_cost_search(int initial[N][N], int x, int y)
{
	
	Node* root = newNode(initial, x, y, x, y, 0, NULL);
	root->cost = 0;
	priority_queue<Node*, std::vector<Node*>, comp> pq;
	pq.push(root);
	
	insert_visited(root->mat);
	
	//for operators' check
	int row[] = {1, 0, -1, 0};
	int col[] = {0, -1, 0, 1};
	int step=0;
	size_t most = pq.size();
	
	while(!pq.empty())
	{
		
		if(pq.empty())
		{
			cout<<"Failure"<<endl;
			return;
		}
		Node* min = pq.top();
		pq.pop();
		if(goal_test(min->mat))
		{
			cout<<"Solution path found!"<<endl;
			printPath(min);
			cout<<"Total expanded node(s): " << step << endl;
			cout<<"The maximum number of nodes in the queue: " << most <<endl;
			cout<<"The depth of the goal node is: " << min->cost << endl << "Uniform cost search end.";
			return;
		}
		for(int i = 0; i < 4 ; ++i)
		{
			
			int r = min->x + row[i], c = min->y + col[i];
			Node* child = newNode(min->mat, min->x, min->y, r, c, min->depth+1, min);
			
			//prevent very long running loop
			if(min->cost+1>15) break;
			
			if(r >= 0 && r < N && c >= 0 && c < N && !check_visited(child->mat))
			{
				insert_visited(child->mat);
				child->cost = min->cost + 1;
				if(goal_test(child->mat))
				{
					cout<<"Solution path found!"<<endl;
					printPath(child);
					cout<<"Total expanded node(s): " << step << endl;
					cout<<"The maximum number of nodes in the queue: " << most <<endl;
					cout<<"The depth of the goal node is: " << child->depth << endl << "Uniform cost search end.";
					return;
				}
				pq.push(child);
				most = max(most, pq.size());
				step++;
				//cout path exploring
				printPuzzle(child->mat);
				cout<<endl;
//				printPuzzle(child->mat);
			}
		}
	}
	cout << "Path not found."<<endl;
	cout<<"Total expanded node(s): " << step << endl;
	cout<<"The maximum number of nodes in the queue: " << most <<endl << "Uniform cost search end.";
}

//A* with misplaced tile
void astar_misplaced(int initial[N][N], int x, int y)
{
	
	Node* root = newNode(initial, x, y, x, y, 0, NULL);
	root->cost = calculateCost(initial);
	priority_queue<Node*, std::vector<Node*>, comp_a> pq;
	pq.push(root);
	
	insert_visited(root->mat);
	
	//for operators' check
	int row[] = {1, 0, -1, 0};
	int col[] = {0, -1, 0, 1};
	int step=1;
	size_t most = pq.size();
	
	while(!pq.empty())
	{
		
		if(pq.empty())
		{
			cout<<"Failure"<<endl;
			return;
		}
		Node* min = pq.top();
		pq.pop();
		for(int i = 0; i < 4 ; ++i)
		{
			
			int r = min->x + row[i], c = min->y + col[i];
			Node* child = newNode(min->mat, min->x, min->y, r, c, min->depth+1, min);
			
			//prevent very long running loop
			if(step>30000) break;
			
			if(r >= 0 && r < N && c >= 0 && c < N && !check_visited(child->mat))
			{
				insert_visited(child->mat);
				child->cost = calculateCost(child->mat);
				
				if(goal_test(child->mat))
				{
					cout<<"Solution path found!"<<endl;
					printPath(child);
					cout<<"Total expanded node(s): " << step << endl;
					cout<<"The maximum number of nodes in the queue: " << most <<endl;
					cout<<"The depth of the goal node is: " << child->depth << endl<< "A* with misplaced tile search end.";
					return;
				}
				pq.push(child);
				most = max(most, pq.size());
				step++;
				//cout path exploring
				printPuzzle(child->mat);
				cout<<endl;
//				printPuzzle(child->mat);
			}
		}
	}
	cout << "Path not found."<<endl;
	cout<<"Total expanded node(s): " << step << endl;
	cout<<"The maximum number of nodes in the queue: " << most <<endl<< "A* with misplaced tile search end.";
}

//A* with the Euclidean Distance
void astar_distance(int initial[N][N], int x, int y)
{
	
	Node* root = newNode(initial, x, y, x, y, 0, NULL);
	root->cost = calculateEuc(initial);
	priority_queue<Node*, std::vector<Node*>, comp_a> pq;
	pq.push(root);
	
	insert_visited(root->mat);
	
	//for operators' check
	int row[] = {1, 0, -1, 0};
	int col[] = {0, -1, 0, 1};
	int step=1;
	size_t most = pq.size();
	
	while(!pq.empty())
	{
		
		if(pq.empty())
		{
			cout<<"Failure"<<endl;
			return;
		}
		Node* min = pq.top();
		pq.pop();
		for(int i = 0; i < 4 ; ++i)
		{
			
			int r = min->x + row[i], c = min->y + col[i];
			Node* child = newNode(min->mat, min->x, min->y, r, c, min->depth+1, min);
			
			//prevent very long running loop
			if(step>30000) break;
			
			if(r >= 0 && r < N && c >= 0 && c < N && !check_visited(child->mat))
			{
				insert_visited(child->mat);
				child->cost = calculateEuc(child->mat);
				if(goal_test(child->mat))
				{
					cout<<"Solution path found!"<<endl;
					printPath(child);
					cout<<"Total expanded node(s): " << step << endl;
					cout<<"The maximum number of nodes in the queue: " << most <<endl;
					cout<<"The depth of the goal node is: " << child->depth << endl<< "A* with euclidean distance search end.";
					return;
				}
				pq.push(child);
				most = max(most, pq.size());
				step++;
				//cout path exploring
				printPuzzle(child->mat);
				cout<<endl;
//				printPuzzle(child->mat);
			}
		}
	}
	cout << "Path not found."<<endl;
	cout<<"Total expanded node(s): " << step << endl;
	cout<<"The maximum number of nodes in the queue: " << most <<endl<< "A* with euclidean distance search end.";
}

int main()
{
	
    cout << "Welcome to jguan050 8 puzzle solver."<<endl;
    int choice=1;
    cout << "Print puzzle"<<endl<<"Type '1' to use a default puzzle, or '2' to enter your own puzzle."<<endl;
    cin>>choice;
    int initial[N][N] =
    	{
        	{1, 2, 3},
        	{4, 5, 6},
        	{7, 0, 8}
    	};
    if(choice == 2)
    {
    	bool sure = false;
    	int temp;
    	while(!sure){
			for (int i = 0; i < N; i++)
			{
				for (int j = 0; j < N; j++)
				{
					cout<<"Enter number for the puzzle at " <<i<< ", "<<j<<endl;
					cin>>temp;
					initial[i][j]=temp;
				}
			}
			printPuzzle(initial);
			cout<<"Run with this puzzle? (y/n)\n";
			char s;
			cin>>s;
			if(s=='y') sure = true;
		}
	}
	
	//choice of algorithm
	int alg;
	cout<<"Choose a algorithm, enter 1 for uniform cost search, 2 for A* with the Misplaced Tile heuristic, 3 for A* with the Euclidean Distanceheuristic.\n";
	cin>>alg;
	
	//position of blank
	int x, y;
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if(initial[i][j]==0)
			{
				x = i;
				y = j;
			}
		}
	}
	
//	cout<<endl<<x<< ' '<< y <<endl;
    printPuzzle(initial);
    cout<<endl;
    if(alg == 1){
    	uniform_cost_search(initial, x, y);
	}
	else if (alg==2){
		astar_misplaced(initial, x, y);
	}
    else
    {
    	astar_distance(initial, x, y);
	}
    return 0;
}
