#include <iostream>
#include <iterator>
#include <set>
#include <vector>

using namespace std;

struct board_result {
    int result;
    vector<vector<int> > board;
};

vector<vector<int> > divider;

// returns 1 if the given number fits in that spot
int work(vector<set<int> > board, int row, int col, int number) {
    if (board[row].count(number) != 0) {
        return 0;
    } 
    if (board[9 + col].count(number) != 0) {
        return 0;
    }
    if (board[17 + divider[row][col]].count(number) != 0) {
        return 0;
    }
    return 1;
}

// returns set of possible numbers that fit in the particular spot
set<int> fill(vector<set<int> > board, int row, int col) {
    set<int> answers;
    for (int i = 1; i < 10; i++) {
        if (work(board, row, col, i)) {
            answers.insert(i);
        } 
    }
    return answers;
}

// you have a board, find first empty spot, find all available numbers there and try them one-by-one. For each possible
// answer, do recursion on that new board state to see if it eventually leads to the answer
board_result do_this(vector<vector<int> > board, vector<set<int> > board2) {
    board_result item;
    board_result item2;
    item.result = 0;
    item.board = board;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (board[i][j] != 0) {
                continue;
            } else {
                set<int> result = fill(board2, i, j);
                if (result.empty()) {
                    return item;
                } else {
                    set<int>::iterator itr;
                    for (itr = result.begin(); itr != result.end(); itr++) {
                        board[i][j] = *itr;
                        board2[i].insert(*itr);
                        board2[9 + j].insert(*itr);
                        board2[17 + divider[i][j]].insert(*itr);
                        item2 = do_this(board, board2);
                      // this board state will eventually lead to the answer
                        if (item2.result != 0) {
                            return item2;
                        } else {
                          // this board state will not, so roll the board2 back
                            board2[i].erase(*itr);
                            board2[9 + j].erase(*itr);
                            board2[17 + divider[i][j]].erase(*itr);
                        }
                    }
                  // tried all available answers and none lead to successful board state. Therefore, there were already problems with the board as given
                    if (itr == result.end()) {
                        item.result = 0;
                        return item;
                    }
                }
            }
        }
    }
  // reach here if no empty spots, therefore this is the answer
    item.result = 1;
    return item;
}

int main(int argc, char* argv[]) {
  // board = the actual sudoku board
	// board2 = a vector of sets where each set represents a row, column or subgrid
  vector<vector<int> > board;
	vector<set<int> > board2;
	if (argc == 1) {
    // hard code a board
		board = {
			{9, 0, 0, 0, 1, 4, 0, 6, 8}, 
			{0, 4, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 7, 9, 3, 0, 5},
			{2, 0, 4, 9, 8, 1, 0, 3, 0},
			{0, 0, 9, 0, 0, 0, 4, 0, 0},
			{0, 8, 0, 4, 6, 2, 9, 0, 1},
			{1, 0, 8, 7, 4, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 5, 0},
			{7, 6, 0, 1, 2, 0, 0, 0, 4},
		};
	}

  // tells us which subgrid each square belongs to
	divider = {
	    {1, 1, 1, 2, 2, 2, 3, 3, 3},
	    {1, 1, 1, 2, 2, 2, 3, 3, 3},
	    {1, 1, 1, 2, 2, 2, 3, 3, 3},
	    {4, 4, 4, 5, 5, 5, 6, 6, 6},
	    {4, 4, 4, 5, 5, 5, 6, 6, 6},
	    {4, 4, 4, 5, 5, 5, 6, 6, 6},
	    {7, 7, 7, 8, 8, 8, 9, 9, 9},
	    {7, 7, 7, 8, 8, 8, 9, 9, 9},
	    {7, 7, 7, 8, 8, 8, 9, 9, 9},
	};

	// create our board2
	for (int i = 0; i < 9; i++) {
	    set<int> new_set;
	    for (int j = 0; j < 9; j++) {
	        new_set.insert(board[i][j]);
	    }
	    board2.push_back(new_set);
	}
	
	for (int i = 0; i < 9; i++) {
	    set<int> new_set;
	    for (int j = 0; j < 9; j++) {
	        new_set.insert(board[j][i]);
	    }
	    board2.push_back(new_set);
	}
	
	for (int i = 0; i < 9; i++) {
	    set<int> new_set;
	    board2.push_back(new_set);
	}
	
	for (int i = 0; i < 9; i++) {
	    for (int j = 0; j < 9; j++) {
	        board2[17 + divider[i][j]].insert(board[i][j]);
	    }
	}

    // calls this, gets the answer board, then print it out
    board_result answer = do_this(board, board2);
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            cout << (answer.board)[i][j] << " ";
        }
        cout << "\n";
    }
    
}

