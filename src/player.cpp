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


int pattern_convert(int p){
    int new_pattern = 0;
    for(int i = 0 ; i<=5 ; i++){
        int n = p%10;
        p = p/10;

        n = (3-n)%3;
        new_pattern += n*pow(10,i); 
    }
    return new_pattern;
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
                    return self ? 50000 : 20000;
                case 4:
                    return self ? 1000000 : 500000;
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
                    return self ? 1000000 : 100000;
                case 5:
                    return 100000000;
            }
            break;
        case 2:
            if(cont == 5)
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

                block = 1;
                cont = 1;
                cur_color = board[i][j];
            }
            else if(cur_color >0 && board[i][j] == 0){
                int tmp_value = calculate_value(block-1 , cont , cur_color == player);
                if(cur_color == player) value += tmp_value;
                else value -= tmp_value;

                block = 2;
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

int alpha_beta(int depth , double alpha , double beta , bool maximizingPlayer , std::ofstream& fout){

    if(depth == 0){
        return state_value_func();
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
        int value = (-1)*INF;
        vector<Point> possibility = find_position();
        cout<<"-----------------player:"<<depth<<endl;
        for(auto pos : possibility){
            board[pos.x][pos.y] = player;
            int new_alpha_beta = alpha_beta(depth-1 , alpha , beta , false , fout);
            board[pos.x][pos.y] = 0;
            cout<<new_alpha_beta<<" "<<endl;

            if( new_alpha_beta > value){
                value = new_alpha_beta;
                x = pos.x;
                y = pos.y;
            }
            // alpha = max(alpha , value);
            alpha = alpha>value ? alpha : value;
            
            if(alpha >= beta)
                break;
        }
        cout<<"\n player: "<<depth <<" choose : "<<value<<endl;
        // cout<<x<<" "<<y<<endl;
        fout << x << " " << y << std::endl;
        fout.flush();
        return value;
    }
    else{
        int value = INF;
        vector<Point> possibility = find_position(); 
        cout<<"---------------opponent:"<<depth<<endl;
        for(auto pos : possibility){
            board[pos.x][pos.y] = (1 == player ? 2 : 1);
            int new_alpha_beta = alpha_beta(depth-1 , alpha , beta , true , fout);
            board[pos.x][pos.y] = 0;
            cout<<new_alpha_beta<<" ";

            if(new_alpha_beta < value){
                value = new_alpha_beta;
                x = pos.x;
                y = pos.y;
            }
            // beta = min(beta , value);
            beta = beta<value ? beta : value;
            if(beta <= alpha)
                break;
        }
        cout<<"\n opponent: "<<depth<<" choose : "<<value<<endl;
        // cout<<x<<" "<<y<<endl;
        fout << x << " " << y << std::endl;
        fout.flush();
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
        else alpha_beta(3 , (-1)*INFINITY , INFINITY , true , fout);
        // cout<<"choose: "<<x<<" "<<y<<endl;
        
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
