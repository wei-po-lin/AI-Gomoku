#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <array>
#include <iostream>
#include <utility>
#include <set>
#include <cmath>
#include <vector>
#define INFINITY 2147483647

using namespace std;

enum SPOT_STATE {
    EMPTY = 0,
    BLACK = 1,
    WHITE = 2
};

struct Point {
    int x, y;
	Point() : Point(0, 0) {}
	Point(float x, float y) : x(x), y(y) {}
	bool operator==(const Point& rhs) const {
		return x == rhs.x && y == rhs.y;
	}
	bool operator!=(const Point& rhs) const {
		return !operator==(rhs);
	}
	Point operator+(const Point& rhs) const {
		return Point(x + rhs.x, y + rhs.y);
	}
	Point operator-(const Point& rhs) const {
		return Point(x - rhs.x, y - rhs.y);
	}
};

int d_x[8] = {-1,-1,-1,0,0,1,1,1};
int d_y[8] = {-1,0,1,-1,1,-1,0,1};

int player;
const int SIZE = 15;
int x , y;
std::array<std::array<int, SIZE>, SIZE> board;


void read_board(std::ifstream& fin) {
    fin >> player;
    // cout<<"player: "<<player;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fin >> board[i][j];
        }
    }
}

vector<Point> find_position(void){
    vector<Point> position_can_place;
    for(int i = 0 ; i<SIZE ; i++){
        for(int j = 0 ; j < SIZE ; j++){
            int xx = i;
            int yy = j;
            bool can_place = false;

            if(board[xx][yy] > 0 ) continue;
            for(int direction = 0 ; direction < 8 ; direction++){
                int check_x = xx + d_x[direction];
                int check_y = yy + d_y[direction];
                if(check_x < 0 || check_x >= SIZE || check_y < 0 || check_y >= SIZE) continue;
                if(board[check_x][check_y] > 0){
                    can_place = true;
                    break;
                }
            }
            if(can_place) position_can_place.emplace_back(Point(xx,yy));
        }
    }
    return position_can_place;
}

//將putx,puty下到棋盤上後產生新的盤面，對於新盤面打分數
//目前產生隨機的分數以測試minimax功能

int state_value_func(void){
    int value = rand() % 100;
    return value;
}

bool empty(void){
    for(int i = 0 ; i<SIZE ; i++)
        for(int j = 0 ; j<SIZE ; j++)
            if(board[i][j] != 0)    return false;
    
    return true;
}

int alpha_beta(int depth , int alpha , int beta , bool maximizingPlayer){
    if(depth == 0){
        return state_value_func();
    }
    if(maximizingPlayer == true){
        int value = (-1)*INFINITY;
        vector<Point> possibility = find_position();
        cout<<"player:"<<depth<<endl;
        for(auto pos : possibility){
            board[pos.x][pos.y] = player;
            int new_alpha_beta = alpha_beta(depth-1 , alpha , beta , false);
            board[pos.x][pos.y] = 0;
            // cout<<new_alpha_beta<<" ";

            if( new_alpha_beta > value){
                value = new_alpha_beta;
                x = pos.x;
                y = pos.y;
            }
            alpha = max(alpha , value);
            
            if(alpha >= beta)
                break;
        }
        // cout<<"\nchoose : "<<value<<endl;
        return value;
    }
    else{
        int value = INFINITY;
        vector<Point> possibility = find_position(); 
        cout<<"opponent:"<<depth<<endl;
        for(auto pos : possibility){
            board[pos.x][pos.y] = (1 == player ? 2 : 1);
            int new_alpha_beta = alpha_beta(depth-1 , alpha , beta , true);
            board[pos.x][pos.y] = 0;
            // cout<<new_alpha_beta<<" ";

            if(new_alpha_beta < value){
                value = new_alpha_beta;
                x = pos.x;
                y = pos.y;
            }
            beta = min(beta , value);
            if(beta <= alpha)
                break;
        }
        // cout<<"\nchoose : "<<value<<endl;
        return value;
    }
}

void write_valid_spot(std::ofstream& fout) {
    srand(time(NULL));
    // int x, y;
    // Keep updating the output until getting killed.
    // while(true) {
        // // Choose a random spot.
        // int x = (rand() % SIZE);
        // int y = (rand() % SIZE);
        // if (board[x][y] == EMPTY) {
        //     fout << x << " " << y << std::endl;
        //     // Remember to flush the output to ensure the last action is written to file.
        //     fout.flush();
        // }
        if(empty()){
            x = SIZE/2;
            y = SIZE/2;
        }        
        else alpha_beta(3 , (-1)*INFINITY , INFINITY , true);
        
        fout << x << " " << y << std::endl;
        fout.flush();
        return;
    // }
}

int main(int, char** argv) {
    std::ifstream fin(argv[1]);
    std::ofstream fout(argv[2]);
    read_board(fin);
    
    write_valid_spot(fout);
    fin.close();
    fout.close();
    return 0;
}
