// Lütfi Canberk ILISAL

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include "Stack.cpp"

using namespace std;

// creates 2D vector that stores information about cells
void MockMazeCreator(vector<vector<Cell>>& mockMaze, int rows, int cols) {

	for (int i = 0; i < rows; i++) {
		vector<Cell> row;
		for (int j = 0; j < cols; j++) {
			row.push_back(Cell(j, i));
		}
		mockMaze.push_back(row);
	}
}
// randomly selects a wall from a cell
int WallFinder(Cell &x, int row, int col) {

	int wall[4] = {0,0,0,0};

	// checks two conditions and marks valid wall
	// 1- on specified direction is there a wall?
	// 2- specified direction is out of the maze?
	if (x.left == 1 && x.x_cord != 0) { wall[0] = 1; }
	if (x.right == 1 && x.x_cord != col-1) { wall[1] = 1; }
	if (x.up == 1 && x.y_cord != row-1) { wall[2] = 1; }
	if (x.down == 1 && x.y_cord != 0) { wall[3] = 1; }

	while(true) {

		unsigned seed = chrono::system_clock::now().time_since_epoch().count();
		mt19937 gen(seed);
		uniform_int_distribution<int> distribution(0, 3);
		int randomNum = distribution(gen);

		if (wall[randomNum] == 1) {
			return randomNum;
		}
	}
}

// checks all neighbour cells are visited
bool allVisited (const vector<vector<Cell>>& mockMaze, const Cell& x) {

	int row = mockMaze.size();
	int col = mockMaze[0].size();
	int x_pos = x.x_cord;
	int y_pos = x.y_cord;

	// left bottom corner
	if (x_pos == 0 && y_pos == 0) {
		if (mockMaze[1][0].visited && mockMaze[0][1].visited) { return true; }
	}
	// left upper corner
	else if (x_pos == 0 && y_pos == row-1) {
		if (mockMaze[row - 2][0].visited && mockMaze[row - 1][1].visited) { return true; }
	}
	// right upper corner
	else if (x_pos == col - 1 && y_pos == row - 1) {
		if (mockMaze[row-1][col-2].visited && mockMaze[row-2][col-1].visited) { return true; }
	}
	// right bottom corner
	else if (x_pos == col-1 && y_pos == 0) {
		if (mockMaze[0][col-2].visited && mockMaze[1][col-1].visited) { return true; }
	}
	// left most wall
	else if (x_pos == 0) {
		if (mockMaze[y_pos-1][0].visited && mockMaze[y_pos+1][0].visited && mockMaze[y_pos][1].visited) { return true; }
	}
	// right most wall
	else if (x_pos == col - 1) {
		if (mockMaze[y_pos-1][col-1].visited && mockMaze[y_pos+1][col-1].visited && mockMaze[y_pos][col-2].visited) { return true; }

	}
	// bottom wall
	else if (y_pos == 0) {
		if (mockMaze[0][x_pos+1].visited && mockMaze[0][x_pos-1].visited && mockMaze[1][x_pos].visited) { return true; }

	}
	// upper wall
	else if (y_pos == row - 1) {
		if (mockMaze[y_pos][x_pos+1].visited && mockMaze[y_pos][x_pos-1].visited && mockMaze[row-2][x_pos].visited) { return true; }
	}
	// inner cells
	else {
		if (mockMaze[y_pos][x_pos+1].visited && mockMaze[y_pos][x_pos-1].visited && mockMaze[y_pos+1][x_pos].visited && mockMaze[y_pos-1][x_pos].visited) { return true; }
	}
	return false;
}

// creates a maze using stack and strores cells information
void MazeCreator(vector<vector<Cell>> &mockMaze, Stack<Cell> &maze) {

	/*
	left=0
	right=1
	up=2
	down=3
	*/
	
	int row = mockMaze.size();
	int col = mockMaze[0].size();
	int x = 0, y = 0;
	int direction;
	int count = 0;

	// destroys walls M*N-1 times
	while (count != (row*col) - 1) {

		if (!allVisited(mockMaze, mockMaze[y][x])) {

			direction = WallFinder(mockMaze[y][x], row, col);
			
			// move left
			if (direction == 0) {
				if (!mockMaze[y][x-1].visited) {
					mockMaze[y][x].visited = true;
					mockMaze[y][x].left = 0;
					maze.push(mockMaze[y][x]);
					x -= 1;
					mockMaze[y][x].right = 0;
					count++;
				}
			}
			// move right
			else if (direction == 1) {
				if (!mockMaze[y][x+1].visited) {
					mockMaze[y][x].visited = true;
					mockMaze[y][x].right = 0;
					maze.push(mockMaze[y][x]);
					x += 1;
					mockMaze[y][x].left = 0;
					count++;
				}
			}
			// move up
			else if (direction == 2) {
				if (!mockMaze[y + 1][x].visited) {
					mockMaze[y][x].visited = true;
					mockMaze[y][x].up = 0;
					maze.push(mockMaze[y][x]);
					y += 1;
					mockMaze[y][x].down = 0;
					count++;
				}
			}
			// move down
			else {
				if (!mockMaze[y - 1][x].visited) {
					mockMaze[y][x].visited = true;
					mockMaze[y][x].down = 0;
					maze.push(mockMaze[y][x]);
					y -= 1;
					mockMaze[y][x].up = 0;
					count++;
				}
			}
		}
		// if cell's neighbours are visited
		else {

			mockMaze[y][x].visited = true;
			maze.push(mockMaze[y][x]);

			Stack<Cell> tempMaze(maze);

			tempMaze.pop();
			Cell tempCell = tempMaze.top();
			x = tempCell.x_cord;
			y = tempCell.y_cord;

			// checks until finding cell that have unvisited neighbours
			while (allVisited(mockMaze, tempCell)) { 
					
				tempMaze.pop();
				tempCell = tempMaze.top();
				x = tempCell.x_cord;
				y = tempCell.y_cord;
			}
		}
	}

	mockMaze[y][x].visited = true;
	maze.push(mockMaze[y][x]);

}

// randomly selects a open wall from cell
int PathFinder(const Cell &x) {

	int wall[4] = { 0,0,0,0 };

	// checks on specified direction is there a open wall?
	if (x.left == 0) { wall[0] = 1; }
	if (x.right == 0) { wall[1] = 1; }
	if (x.up == 0) { wall[2] = 1; }
	if (x.down == 0) { wall[3] = 1; }

	while (true) {

		unsigned seed = chrono::system_clock::now().time_since_epoch().count();
		mt19937 gen(seed);
		uniform_int_distribution<int> distribution(0, 3);
		int randomNum = distribution(gen);


		if (wall[randomNum] == 1) {
			return randomNum;
		}
	}
}

// checks for are there unvisited cells to move
bool CanMove(const vector<vector<Cell>>& mockMaze, const Cell& cell) {

	int x = cell.x_cord, y = cell.y_cord;

	if (cell.left == 0 && mockMaze[y][x-1].visited == true) { return true; }
	if (cell.right == 0 && mockMaze[y][x+1].visited == true) { return true; }
	if (cell.up == 0 && mockMaze[y+1][x].visited == true) { return true; }
	if (cell.down == 0 && mockMaze[y-1][x].visited == true) { return true; }

	return false;
}

// finds a path for created maze using stack, entry and exit points
void MazeSolver(vector<vector<Cell>> &mockMaze, Stack<Cell> &path, int entry_x, int entry_y, int exit_x, int exit_y) {

	int x = entry_x, y = entry_y;
	int direction;

	while (!(x == exit_x && y== exit_y)) {

		if (CanMove(mockMaze, mockMaze[y][x])) {

			direction = PathFinder(mockMaze[y][x]);

			// move left
			if (direction == 0) {
				if (mockMaze[y][x - 1].visited) {
					mockMaze[y][x].visited = false;
					path.push(mockMaze[y][x]);
					x -= 1;
				}
			}
			// move right
			else if (direction == 1) {
				if (mockMaze[y][x + 1].visited) {
					mockMaze[y][x].visited = false;
					path.push(mockMaze[y][x]);
					x += 1;
				}
			}
			// move up
			else if (direction == 2) {
				if (mockMaze[y + 1][x].visited) {
					mockMaze[y][x].visited = false;
					path.push(mockMaze[y][x]);
					y += 1;
				}
			}
			// move down
			else {
				if (mockMaze[y - 1][x].visited) {
					mockMaze[y][x].visited = false;
					path.push(mockMaze[y][x]);
					y -= 1;
				}
			}
		}
		else {

			mockMaze[y][x].visited = false;

			Cell tempCell = path.top();
			x = tempCell.x_cord;
			y = tempCell.y_cord;

			while (!CanMove(mockMaze, tempCell)) {

				path.pop();
				tempCell = path.top();
				x = tempCell.x_cord;
				y = tempCell.y_cord;
					
				
			}

			path.pop();
		}
	}

	mockMaze[y][x].visited = false;
	path.push(mockMaze[y][x]);

}

void PrintMaze(const vector<vector<Cell>> &maze, int k) {

	int row = maze.size();
	int col = maze[0].size();

	string filename = "maze_" + to_string(k+1) + ".txt";

	ofstream outputFile(filename);

	outputFile << row << " " << col << endl;

	for (unsigned int i=0; i<row; i++){
		for (unsigned int j=0; j<col; j++) {	
			outputFile << "x=" << maze[i][j].x_cord << " " << "y=" << maze[i][j].y_cord << " " << "l=" << maze[i][j].left << " " << "r="
				<< maze[i][j].right << " " << "u=" << maze[i][j].up << " " << "d=" << maze[i][j].down << endl;
		}
	}

	outputFile.close();
}

void PrintPath(const Stack<Cell> &maze, int mazeID, int entry_x, int entry_y, int exit_x, int exit_y) {

	string filename = "maze_" + to_string(mazeID) + "_path_" + to_string(entry_x) + "_" + to_string(entry_y) + "_" + to_string(exit_x) + "_" + to_string(exit_y) + ".txt";
	string pathInfo;

	ofstream outputFile(filename);

	Stack<Cell> tempMaze(maze);

	while (!tempMaze.isEmpty()) {

		Cell tempCell(tempMaze.topAndPop());
		pathInfo = to_string(tempCell.x_cord) + " " + to_string(tempCell.y_cord) + "\n" + pathInfo;
	}

	outputFile << pathInfo;

	outputFile.close();
}



int main(){

	
	int k, m, n, mazeID, entry_x, entry_y, exit_x, exit_y;
	
	cout << "Enter the number of mazes: ";
	cin >> k;

	cout << "Enter the number of rows and columns (M and N): ";
	cin >> m >> n;

	vector<vector<vector<Cell>>> vectorMockMaze;

	for (unsigned int i=0; i<k; i++) {

		vector<vector<Cell>> mockMaze;
		Stack<Cell> maze;
	
		MockMazeCreator(mockMaze, m, n);
		MazeCreator(mockMaze, maze);

		vectorMockMaze.push_back(mockMaze);

		PrintMaze(mockMaze, i);
	}

	cout << "All mazes are created." << endl << endl;

	cout << "Enter a maze ID between 1 to " << k << " inclusive to find a path: ";
	cin >> mazeID;

	cout << "Enter x and y coordinates of the entry points (x,y) or (column,row): ";
	cin >> entry_x >> entry_y;

	cout << "Enter x and y coordinates of the exit points (x,y) or (column,row): ";
	cin >> exit_x >> exit_y;

	
	Stack<Cell> path;

	MazeSolver(vectorMockMaze[mazeID-1], path, entry_x, entry_y, exit_x, exit_y);

	PrintPath(path, mazeID, entry_x, entry_y, exit_x, exit_y);



	return 0;
}