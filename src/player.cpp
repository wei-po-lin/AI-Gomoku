#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <array>
#include <utility>
#include <set>
#include <cmath>
#include <vector>
#include <map>
#define INF 2147483647

using namespace std;

enum SPOT_STATE {
    EMPTY = 0,
    BLACK = 1,
    WHITE = 2
};

// map<int,int> depth_mul = {{3,3},{2,2},{3,1}};

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

class chess_move{
    public:
        Point pos;
        int value;
        chess_move(int x, int y , int value) : pos(Point(x, y)),value(value) {}
};


int d_x[8] = {-1,-1,-1,0,0,1,1,1};
int d_y[8] = {-1,0,1,-1,1,-1,0,1};

int player;
const int SIZE = 15;
int x , y;
std::array<std::array<int, SIZE>, SIZE> board;


void read_board(std::ifstream& fin) {
    fin >> player;
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

int calculate_value(int block , int cont ,  bool self){
    switch(block){
        case 0:
            switch(cont){
                case 1:
                    return 1;
                case 2:
                    return self ? 7 : 5;
                case 3:
                    return self ? 50000 : 32000;
                case 4:
                    return 80000000;
                case 5:
                    return 100000000;
            }
            break;
        case 1:
            switch(cont){
                case 1:
                    return 1;
                case 2:
                    return 3;
                case 3:
                    return self ? 10 : 5;
                case 4:
                    return self ? 1000000 : 200;
                case 5:
                    return 100000000;
            }
            break;
        case 2:
            if(cont >= 5)
                return 100000000;
            else return 0;
            break;
    }
}

int state_value_func(){
    int value = 0;
    int block = 2;
    int cont = 0;
    int cur_color = 0;

    for(int i = 0 ; i<SIZE ; i++){
        for(int j = 0 ; j<SIZE ; j++){
            if(board[i][j] == 0 && cont == 0) block = 1;
            if(cur_color > 0 && board[i][j] == cur_color) cont++;

            else if(cur_color == 0 && board[i][j] >0){
                block = 1;
                cont = 1;
                cur_color = board[i][j];
            }
            else if(cur_color > 0 && board[i][j] != cur_color && board[i][j] > 0){
                int tmp_value = calculate_value(block , cont , cur_color == player);
                if(cur_color == player) value += tmp_value;
                else value -= tmp_value;

                block = 2;
                cont = 1;
                cur_color = board[i][j];
            }
            else if(cur_color >0 && board[i][j] == 0){
                int tmp_value = calculate_value(block-1 , cont , cur_color == player);
                if(cur_color == player) value += tmp_value;
                else value -= tmp_value;

                block = 1;
                cont = 0;
                cur_color = 0;
            }        
        }
        if(cont>0){
            int tmp_value = calculate_value(block , cont , cur_color == player);
            if(cur_color==player) value += tmp_value;
            else value -= tmp_value; 
        }
        block = 2;
        cont = 0;
        cur_color = 0;
    }


    for(int j = 0 ; j<SIZE ; j++){
        for(int i = 0 ; i<SIZE ; i++){
            if(board[i][j] == 0 && cont == 0) block = 1;
            if(cur_color > 0 && board[i][j] == cur_color) cont++;

            else if(cur_color == 0 && board[i][j] >0){
                cont = 1;
                block = 1;
                cur_color = board[i][j];
            }
            else if(cur_color > 0 && board[i][j] != cur_color && board[i][j] > 0){
                int tmp_value = calculate_value(block , cont , cur_color == player);
                if(cur_color == player) value += tmp_value;
                else value -= tmp_value;

                block = 2;
                cont = 1;
                cur_color = board[i][j];
            }
            else if(cur_color >0 && board[i][j] == 0){
                int tmp_value = calculate_value(block-1 , cont , cur_color == player);
                if(cur_color == player) value += tmp_value;
                else value -= tmp_value;

                block = 1;
                cont = 0;
                cur_color = 0;
            }        
        }
        if(cont>0){
            int tmp_value = calculate_value(block , cont , cur_color == player);
            if(cur_color==player) value += tmp_value;
            else value -= tmp_value; 
        }
        block = 2;
        cont = 0;
        cur_color = 0;
    }
    for(int sum = 4 ; sum<=14 ; sum++){ //右上到左下 左上半部 i遞減 從下往上
        for(int i = sum ; i>=0 ; i--){
            int j = sum-i;
            if(board[i][j] == 0 && cont == 0) block = 1;           
            if(cur_color > 0 && board[i][j] == cur_color) cont++;

            else if(cur_color == 0 && board[i][j] >0){
                cont = 1;
                block = 1;
                cur_color = board[i][j];
            }
            else if(cur_color > 0 && board[i][j] != cur_color && board[i][j] > 0){
                int tmp_value = calculate_value(block , cont , cur_color == player);
                if(cur_color == player) value += tmp_value;
                else value -= tmp_value;

                block = 2;
                cont = 1;
                cur_color = board[i][j];
            }
            else if(cur_color >0 && board[i][j] == 0){
                int tmp_value = calculate_value(block-1 , cont , cur_color == player);
                if(cur_color == player) value += tmp_value;
                else value -= tmp_value;

                block = 1;
                cont = 0;
                cur_color = 0;
            }               
        }
        if(cont>0){
            int tmp_value = calculate_value(block , cont , cur_color == player);
            if(cur_color==player) value += tmp_value;
            else value -= tmp_value; 
        }        
        block = 2;
        cont = 0;
        cur_color = 0;
    }


    for(int sum = 15 ; sum<=24 ; sum++){ //右上到左下 右下半部 i遞增 從上往下
        for(int i = sum-14 ; i<15 ; i++){
            int j = sum-i;
            if(board[i][j] == 0 && cont == 0) block = 1;
            if(cur_color > 0 && board[i][j] == cur_color) cont++;

            else if(cur_color == 0 && board[i][j] >0){
                block = 1;
                cont = 1;
                cur_color = board[i][j];
            }
            else if(cur_color > 0 && board[i][j] != cur_color && board[i][j] > 0){
                int tmp_value = calculate_value(block , cont , cur_color == player);
                if(cur_color == player) value += tmp_value;
                else value -= tmp_value;

                block = 2;
                cont = 1;
                cur_color = board[i][j];
            }
            else if(cur_color >0 && board[i][j] == 0){
                int tmp_value = calculate_value(block-1 , cont , cur_color == player);
                if(cur_color == player) value += tmp_value;
                else value -= tmp_value;

                block = 1;
                cont = 0;
                cur_color = 0;
            }        
        }
        if(cont>0){
            int tmp_value = calculate_value(block , cont , cur_color == player);
            if(cur_color==player) value += tmp_value;
            else value -= tmp_value; 
        }
        block = 2;
        cont = 0;
        cur_color = 0; 
    }
    
   
    for(int minus = 10 ; minus>=0 ; minus--){ //左上到右下 左下半部 i遞增 從上往下
        for(int i = minus ; i<15 ; i++){
            int j = i - minus;
            if(board[i][j] == 0 && cont == 0) block = 1;
            if(cur_color > 0 && board[i][j] == cur_color) cont++;

            else if(cur_color == 0 && board[i][j] >0){
                block = 1;
                cont = 1;
                cur_color = board[i][j];
            }
            else if(cur_color > 0 && board[i][j] != cur_color && board[i][j] > 0){
                int tmp_value = calculate_value(block , cont , cur_color == player);
                if(cur_color == player) value += tmp_value;
                else value -= tmp_value;

                block = 2;
                cont = 1;
                cur_color = board[i][j];
            }
            else if(cur_color >0 && board[i][j] == 0){
                int tmp_value = calculate_value(block-1 , cont , cur_color == player);
                if(cur_color == player) value += tmp_value;
                else value -= tmp_value;

                block = 1;
                cont = 0;
                cur_color = 0;
            }        
        }
        if(cont>0){
            int tmp_value = calculate_value(block , cont , cur_color == player);
            if(cur_color==player) value += tmp_value;
            else value -= tmp_value; 
        }
        block = 2;
        cont = 0;
        cur_color = 0;
    }
    
    for(int minus = -1 ; minus>=-10 ; minus--){ //從左上到右下 右上半部 i遞增 從上往下
        for(int j = -minus ; j<15 ; j++){
            int i = minus+j;
            if(board[i][j] == 0 && cont == 0) block = 1;
            if(cur_color > 0 && board[i][j] == cur_color) cont++;

            else if(cur_color == 0 && board[i][j] >0){
                cont = 1;
                block = 1;
                cur_color = board[i][j];
            }
            else if(cur_color > 0 && board[i][j] != cur_color && board[i][j] > 0){
                int tmp_value = calculate_value(block , cont , cur_color == player);
                if(cur_color == player) value += tmp_value;
                else value -= tmp_value;

                block = 2;
                cont = 1;
                cur_color = board[i][j];
            }
            else if(cur_color >0 && board[i][j] == 0){
                int tmp_value = calculate_value(block-1 , cont , cur_color == player);
                if(cur_color == player) value += tmp_value;
                else value -= tmp_value;

                block = 1;
                cont = 0;
                cur_color = 0;
            }        
        }
        if(cont>0){
            int tmp_value = calculate_value(block , cont , cur_color == player);
            if(cur_color==player) value += tmp_value;
            else value -= tmp_value; 
        }
        block = 2;
        cont = 0;
        cur_color = 0;
    }

    return value;
}


//將putx,puty下到棋盤上後產生新的盤面，對於新盤面打分數
//目前產生隨機的分數以測試minimax功能

// int state_value_func(void){
//     int value = rand() % 100;
//     return value;
// }

bool empty(void){
    for(int i = 0 ; i<SIZE ; i++)
        for(int j = 0 ; j<SIZE ; j++)
            if(board[i][j] != 0)    return false;
    
    return true;
}


pair<int,int> before_alphabeta(void){
    vector<Point> possible = find_position();
    int opponent = player == 1 ? 2 : 1;

    for(auto pos : possible){
        board[pos.x][pos.y] = player;
        if(state_value_func() >= 997000000){
            cout<<"before_alphabeta~~~~~~~~~~~~~~~~~~~~~~~\n";
            board[pos.x][pos.y] = 0;
            return pair<int,int>(pos.x,pos.y);
        }
        board[pos.x][pos.y] = 0;
    }
    for(auto pos : possible){
        board[pos.x][pos.y] = opponent;
        if(state_value_func() <= -997000000){
            cout<<"before_alphabeta~~~~~~~~~~~~~~~~~~~~~~~\n";
            board[pos.x][pos.y] = 0;
            return pair<int,int>(pos.x,pos.y);
        }
        board[pos.x][pos.y] = 0;
    }
    return pair<int,int>(-1,-1);
}


chess_move alpha_beta(int depth , int alpha , int beta , bool maximizingPlayer){
    if(depth == 0){
        return chess_move(-1,-1,state_value_func());
        // return state_value_func();
        // return rand()%100;
    }
    cout<<"&&&&&&\n";
    for(int i = 0 ; i<15 ; i++){
        for(int j = 0 ; j<15 ; j++){
            int p = board[i][j];
            char c;
            switch(p){
                case 0: 
                    c = '.';
                    break;
                case 1:
                    c = 'O';
                    break;
                 case 2:
                    c = 'X';
                    break;
            }
            cout<<c<<" ";
        }
        cout<<"\n";
    }
    cout<<"&&&&&&\n";

    if(maximizingPlayer == true){
        chess_move best_move(-1,-1,-INF);
        chess_move new_alpha_beta(-1,-1,-INF);

        vector<Point> possibility = find_position();
        cout<<"-----------------player:"<<depth<<endl;
        for(auto pos : possibility){
            board[pos.x][pos.y] = player;
            // int new_alpha_beta = alpha_beta(depth-1 , alpha , beta , false , fout);
            new_alpha_beta = alpha_beta(depth-1 , alpha , beta , false);
            board[pos.x][pos.y] = 0;
            cout<<new_alpha_beta.value<<" "<<endl;
            
            alpha = max(alpha , new_alpha_beta.value);
            if(alpha >= beta) return new_alpha_beta;
            if( new_alpha_beta.value > best_move.value){
                best_move = new_alpha_beta;
                best_move.value = new_alpha_beta.value;
                best_move.pos.x = pos.x;
                best_move.pos.y = pos.y;
            }
            
            // alpha = alpha>value ? alpha : value;
            // if(alpha >= beta)
            //     break;
        }
        cout<<"\n player: "<<depth <<" choose : "<<best_move.value<<endl;
        // cout<<x<<" "<<y<<endl;
        // fout << x << " " << y << std::endl;
        // fout.flush();
        return best_move;
    }
    else{
        chess_move best_move(-1,-1, INF);
        chess_move new_alpha_beta(-1,-1, INF);

        vector<Point> possibility = find_position(); 
        cout<<"---------------opponent:"<<depth<<endl;
        for(auto pos : possibility){
            board[pos.x][pos.y] = (1 == player ? 2 : 1);
            // int new_alpha_beta = alpha_beta(depth-1 , alpha , beta , true , fout);
            new_alpha_beta = alpha_beta(depth-1 , alpha , beta , true);
            board[pos.x][pos.y] = 0;
            cout<<new_alpha_beta.value<<" ";

            beta = min(beta , new_alpha_beta.value);
            if(beta <= alpha) return new_alpha_beta;
            if(new_alpha_beta.value < best_move.value){
                best_move = new_alpha_beta;
                best_move.value = new_alpha_beta.value;
                best_move.pos.x = pos.x;
                best_move.pos.y = pos.y;
            }
            // if(beta <= alpha)
            //     break;
        }
        cout<<"\n opponent: "<<depth<<" choose : "<<best_move.value<<endl;
        // cout<<x<<" "<<y<<endl;
        // fout << x << " " << y << std::endl;
        // fout.flush();
        return best_move;
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
            fout<<SIZE/2<<" "<<SIZE/2<<endl;
            fout.flush();
            return;
        }        
        else{
            pair<int,int> b = before_alphabeta();
            if(b.first != -1 && b.second != -1){
                fout << b.first << " " << b.second <<endl;
                fout.flush();
                return;               
            }
            chess_move p = alpha_beta(3 , (-1)*INF , INF , true);
            fout << p.pos.x << " " << p.pos.y <<endl;
            fout.flush();
            return;
        }
        // cout<<"choose: "<<x<<" "<<y<<endl;
        
        // fout << x << " " << y << std::endl;
        // fout.flush();
        // return;
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
