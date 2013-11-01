#include <iostream>
#include <stdio.h>
#include <climits>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
using namespace std;

struct Value_struct {
		    int score;
		    int i;
		    int j;
};
int skip=-1;

class Board{
	int piece_count[2];
	Value_struct minimax(Board state,int i, int j, int depth,int a, int b, bool maximizingPlayer);
public:
	Board(void);
	Board(const Board &game);
	int Reset_board(void);
	int move_count;
	int current_color;
	int board[8][8];
	// int buff[16];
	bool valid_moves[2][8][8];
	int valid_move_count[2];
	int movelist_i[2][25];
	int movelist_j[2][25];
	int Find_valid_moves(int color);
	int Evaluate_board(int x, int y, int color);
	int Print_board(void);
	int Import_board(string filename);
	bool Check_if_over(void);
	int DFS_timeout;
	bool PlayerFirst;
	bool PlayerSecond; 
	int Make_move();
	int Random_move();
	int AI_move(Board game);
};

Board::Board(void) {
	// Reset_board();
	// move_count=0;
};
Board::Board(const Board &game){
	int i, j, n;
	int cindex=game.current_color-1;
	move_count=game.move_count;
	current_color=game.current_color;
	piece_count[0]=game.piece_count[0];
	piece_count[1]=game.piece_count[1];
	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			board[i][j]=game.board[i][j];
			valid_moves[0][i][j]=game.valid_moves[0][i][j];
			valid_moves[1][i][j]=game.valid_moves[1][i][j];
		}
	}
	valid_move_count[0]=game.valid_move_count[0];
	valid_move_count[1]=game.valid_move_count[1];
	for(n=0; n<valid_move_count[cindex];n++){
		movelist_i[cindex][n]=game.movelist_i[cindex][n];
		movelist_j[cindex][n]=game.movelist_j[cindex][n];
	}

};

int Board::Reset_board(void){
	move_count=0;
	int i,j;
	for(i=0; i<8; i++){
		for(j=0;j<8;j++){
			board[i][j]=0;
		}
	}
	board[3][3]=2;
	board[3][4]=1;
	board[4][3]=1;
	board[4][4]=2;
	piece_count[0]=2;
	piece_count[1]=2;
return 0;
}
int Board::Import_board(string filename){

	return 0;
}
int Board::Print_board(void){
	printf("Move # %i\n\n",move_count);
	int i,j, n=1;
	cout<<"\t1\t2\t3\t4\t5\t6\t7\t8\n\n";
	for(i=0; i<8; i++){
		cout<<i+1<<"\t";
		for(j=0;j<8;j++){
			if(board[i][j]==0){
				if(valid_moves[current_color-1][i][j]){
					cout<<"\e[033;33m"<<n<<"\e[033;39m"<<"\t";
					n++;
				}
					// printf("\e[033;32m%ie[033;39m\t",n);
				else
					cout<<"-\t";
			} 
			else if(board[i][j]==1)
				cout<<"\e[033;31mO\e[033;39m\t"; 
			else if(board[i][j]==2)
				cout<<"\e[033;34mO\e[033;39m\t"; 
			else
				cout<<board[i][j]<<"\t";
		}
		cout<<"\n\n";
	}
	printf("Black (Red) Score: %i\t\t White (Blue) Score: %i\n",piece_count[0], piece_count[1]);
	cout<<"Valid move count:"<<valid_move_count[current_color-1]<<endl;
	// cout<<"Print_board is done"<<endl;
	return 0;
}

int Board::Find_valid_moves(int color){
	int cindex=color-1, i, j, opcolor, n;
	for(i=0; i<8; i++){
		for(j=0;j<8;j++){
			valid_moves[cindex][i][j]=false;
		}
	}
	valid_move_count[cindex]=0;
	if(color==1)
		opcolor=2;
	else
		opcolor=1;
	for(i=0; i<8; i++){
		for(j=0;j<8;j++){
			//topleft
			if(board[i][j]==0 && i>1 && j>1 && board[i-1][j-1]==opcolor){			
				for(n=2;n<=i && n<=j;n++){
					if(board[i-n][j-n]==color){
						valid_moves[cindex][i][j]=true;
						movelist_i[cindex][valid_move_count[cindex]]=i;
						movelist_j[cindex][valid_move_count[cindex]]=j;								
						valid_move_count[cindex]++;
						break;
					}
					else if(board[i-n][j-n]==opcolor){}
					else
						break;
				}
			}

			//top
			if(!valid_moves[cindex][i][j] && board[i][j]==0 && i>1 && board[i-1][j]==opcolor){
				for(n=2;n<=i;n++){
					if(board[i-n][j]==color){
						valid_moves[cindex][i][j]=true;
						movelist_i[cindex][valid_move_count[cindex]]=i;
						movelist_j[cindex][valid_move_count[cindex]]=j;								
						valid_move_count[cindex]++;
						break;
					}
					else if(board[i-n][j]==opcolor){}
					else
						break;
				}
			}

			//topright
			if(!valid_moves[cindex][i][j] && board[i][j]==0 && i>1 && j<6 && board[i-1][j+1]==opcolor){
				for(n=2;n<=i && n<(8-j);n++){
					if(board[i-n][j+n]==color){
						valid_moves[cindex][i][j]=true;
						movelist_i[cindex][valid_move_count[cindex]]=i;
						movelist_j[cindex][valid_move_count[cindex]]=j;								
						valid_move_count[cindex]++;
						break;
					}
					else if(board[i-n][j+n]==opcolor){}
					else
						break;
				}
			}
			//left
			if(!valid_moves[cindex][i][j] && board[i][j]==0 && j>1 && board[i][j-1]==opcolor){
				for(n=2;n<=j;n++){
					if(board[i][j-n]==color){
						valid_moves[cindex][i][j]=true;
						movelist_i[cindex][valid_move_count[cindex]]=i;
						movelist_j[cindex][valid_move_count[cindex]]=j;								
						valid_move_count[cindex]++;
						break;
					}
					else if(board[i][j-n]==opcolor){}
					else
						break;
				}
			}
			//right
			if(!valid_moves[cindex][i][j] && board[i][j]==0 && j<6 && board[i][j+1]==opcolor){
				for(n=2;n<(8-j);n++){
					if(board[i][j+n]==color){
						valid_moves[cindex][i][j]=true;
						movelist_i[cindex][valid_move_count[cindex]]=i;
						movelist_j[cindex][valid_move_count[cindex]]=j;								
						valid_move_count[cindex]++;
						break;
					}
					else if(board[i][j+n]==opcolor){}
					else
						break;
				}
			}

			//bottom left
			if(!valid_moves[cindex][i][j] && board[i][j]==0 && i<6 &&j>1 && board[i+1][j-1]==opcolor){
				for(n=2;n<(8-i)&&n<=j;n++){
					if(board[i+n][j-n]==color){
						valid_moves[cindex][i][j]=true;
						movelist_i[cindex][valid_move_count[cindex]]=i;
						movelist_j[cindex][valid_move_count[cindex]]=j;								
						valid_move_count[cindex]++;
						break;
					}
					else if(board[i+n][j-n]==opcolor){}
					else
						break;
				}
			}
			//bottom
			if(!valid_moves[cindex][i][j] && board[i][j]==0 && i<6 && board[i+1][j]==opcolor){
				for(n=2;n<(8-i);n++){
					if(board[i+n][j]==color){
						valid_moves[cindex][i][j]=true;
						movelist_i[cindex][valid_move_count[cindex]]=i;
						movelist_j[cindex][valid_move_count[cindex]]=j;								
						valid_move_count[cindex]++;
						break;
					}
					else if(board[i+n][j]==opcolor){}
					else
						break;
				}
			}
			//bottom right
			if(!valid_moves[cindex][i][j] && board[i][j]==0 && i<6 && j<6 && board[i+1][j+1]==opcolor){
				for(n=2;n<(8-i)&&n<(8-j);n++){
					if(board[i+n][j+n]==color){
						valid_moves[cindex][i][j]=true;
						movelist_i[cindex][valid_move_count[cindex]]=i;
						movelist_j[cindex][valid_move_count[cindex]]=j;								
						valid_move_count[cindex]++;
						break;
					}
					else if(board[i+n][j+n]==opcolor){}
					else
						break;
				}
			}



		}
		
	}




	return 1;
}
int Board::Evaluate_board(int i, int j, int color){
	if(!valid_moves[color-1][i][j]){
		cout<<"in elvaluate, not valid\n";
		return 1;
	}
	else{
		int opcolor,n, cindex=color-1, opindex;
		board[i][j]=color;
		piece_count[cindex]++;
		if(color==1)
			opcolor=2;
		else
			opcolor=1;
		opindex=opcolor-1;
		//topleft
		if(i>1 && j>1 && board[i-1][j-1]==opcolor){			
			for(n=2;n<=i&&n<=j;n++){
				if(board[i-n][j-n]==color){
					while(n>1){
						--n;
						board[i-n][j-n]=color;
						piece_count[cindex]++;
						piece_count[opindex]--;
					}
					break;
				}
				else if(board[i-n][j-n]==opcolor){}
				else
					break;
			}
		}

		//top
		if(i>1 && board[i-1][j]==opcolor){			
			for(n=2;n<=i;n++){
				if(board[i-n][j]==color){
					while(n>1){
						--n;
						board[i-n][j]=color;
						piece_count[cindex]++;
						piece_count[opindex]--;
					}
					break;
				}
				else if(board[i-n][j]==opcolor){}
				else
					break;
			}
		}

		//topright
		if(i>1 && j<6 && board[i-1][j+1]==opcolor){			
			for(n=2;n<=i && n<(8-j);n++){
				if(board[i-n][j+n]==color){
					while(n>1){
						--n;
						board[i-n][j+n]=color;
						piece_count[cindex]++;
						piece_count[opindex]--;
					}
					break;
				}
				else if(board[i-n][j+n]==opcolor){}
				else
					break;
			}
		}


		//left
		if(j>1 && board[i][j-1]==opcolor){			
			for(n=2;n<=j;n++){
				if(board[i][j-n]==color){
					while(n>1){
						--n;
						board[i][j-n]=color;
						piece_count[cindex]++;
						piece_count[opindex]--;
					}
					break;
				}
				else if(board[i][j-n]==opcolor){}
				else
					break;
			}
		}
		//right
		if(j<6 && board[i][j+1]==opcolor){			
			for(n=2;n<(8-j);n++){
				if(board[i][j+n]==color){
					while(n>1){
						--n;
						board[i][j+n]=color;
						piece_count[cindex]++;
						piece_count[opindex]--;
					}
					break;
				}
				else if(board[i][j+n]==opcolor){}
				else
					break;
			}
		}

		//bottom left
		if(i<6 && j>1 && board[i+1][j-1]==opcolor){			
			for(n=2;n<(8-i)&&n<=j;n++){
				if(board[i+n][j-n]==color){
					while(n>1){
						--n;
						board[i+n][j-n]=color;
						piece_count[cindex]++;
						piece_count[opindex]--;
					}
					break;
				}
				else if(board[i+n][j-n]==opcolor){}
				else
					break;
			}
		}
		//bottom
		if(i<6 && board[i+1][j]==opcolor){			
			for(n=2;n<(8-i);n++){
				if(board[i+n][j]==color){
					while(n>1){
						--n;
						board[i+n][j]=color;
						piece_count[cindex]++;
						piece_count[opindex]--;
					}
					break;
				}
				else if(board[i+n][j]==opcolor){}
				else
					break;
			}
		}
		//bottom right
		if(i<6 && j<6 && board[i+1][j+1]==opcolor){			
			for(n=2;n<(8-i)&&n<(8-j);n++){
				if(board[i+n][j+n]==color){
					while(n>1){
						--n;
						board[i+n][j+n]=color;
						piece_count[cindex]++;
						piece_count[opindex]--;
					}
					break;
				}
				else if(board[i+n][j+n]==opcolor){}
				else
					break;
			}
		}
	current_color=opcolor;
	// cout<<"finding valid moves for color: "<<opcolor<<endl;
	Find_valid_moves(opcolor);
	return 0;
	}
}
int Board::Make_move(){
	 // board[i][j]=color;
	int cindex=current_color-1, a=128, i,j;
	// Print_board();
	// cout<<"Choose valid move"<<endl;
	// for(int n=0; n<valid_move_count[cindex];n++){
	// 	printf("%i:\t%i, %i\n",n+1,movelist_j[cindex][n]+1,movelist_i[cindex][n]+1);
	// }
	// if(valid_move_count[cindex]!=0){
		while(a>=valid_move_count[cindex]){
			cout<< "Enter cooresponding valid move number";
			cin>>a;
			a=a-1;
			// cout<<a<<endl;
			i=movelist_i[cindex][a];
			j=movelist_j[cindex][a];
		}

		cout<< "Evaluating board\n\n";
		if (!Evaluate_board(i,j, current_color)){
			cout<<"Valid Move\t Board is now:\n";
			Print_board();
			return 0;
		} else{
			cout<<"Invalid move\t Try Again Something is wrong\n";
			board[i][j]=0;
			Print_board();
			return 1;
		}
	// }
	// else{
	// 	cout<<"skipping move\n";
	// 	current_color=1+!(current_color-1);
	// 	Find_valid_moves(current_color);
	// 	Print_board();
	// 	return 0;
	// }
}
int Board::Random_move(){
	int cindex=current_color-1;
	// if(valid_move_count[cindex]!=0){
		srand (time(NULL));
		int rmove=rand() % valid_move_count[cindex];
		// cout<<"valid_move_count:"<<valid_move_count[cindex]<<endl;
		// cout<<"random move:"<<rmove<<endl;
		int i= movelist_i[cindex][rmove];
		int j= movelist_j[cindex][rmove];
		board[i][j]=current_color;
		// Prisnt_board();
		// cout<< "Evaluating board\n\n";
		if (!Evaluate_board(i,j, current_color)){
			cout<<"Valid Move\t Board is now:\n";
			Print_board();
			return 0;
		} 
		else{
			cout<<"Invalid move\t Try Again\n";
			board[i][j]=0;
			// Print_board();
			return 1;
		}
	// }
	// else{
	// 	cout<<"skipping move\n";
	// 	current_color=1+!(current_color-1);
	// 	Find_valid_moves(current_color);
	// 	Print_board();
	// 	return 0;
	// }
}
int Board::AI_move(Board game){
	// cout<<"got here in ai\n";
	int cindex=current_color-1;
	Board state(game);
	// Print_board();
	// if(valid_move_count[cindex]!=0){
		if(valid_move_count[cindex]==1){
			if(!Evaluate_board(movelist_i[cindex][0],movelist_j[cindex][0],current_color)){
				cout<<"Valid Move\t Board is now:\n";
				Print_board();
				return 0;
			}
			else{
				cout<<"you facked up with one valid move\n";
				printf("ai i=%i \t ai j= %i\n",movelist_i[cindex][0],movelist_j[cindex][0]);
				exit(1);
			}
		}
		Value_struct val=minimax(state,9,10, 6,INT_MIN,INT_MAX, true);
		printf("ai i=%i \t ai j= %i\n",val.i,val.j);
		if (!Evaluate_board(val.i,val.j, current_color)){
			cout<<"Valid Move\t Board is now:\n";
			Print_board();
			return 0;
		}
		else{
			cout<<"you facked up\n";
			printf("ai i=%i \t ai j= %i\n",val.i,val.j);
			exit(1);
		}
	// }
	// else {
	// 	cout<<"skiping move\n";
	// 	current_color=1+!(current_color-1);
	// 	Find_valid_moves(current_color);
	// 	Print_board();
	// }
	return 0;
}

Value_struct Board::minimax(Board state,int i, int j, int depth, int a, int b, bool maximizingPlayer){
	if(i!=9){
		// cout<<"Evaluate_board in minimax\n";
		// printf("i=%i\tj=%i\n",i, j);
		state.Evaluate_board(i, j, state.current_color);

	}
	// cout<<"depth: "<<depth<<endl;
	int cindex=state.current_color-1;
    if(depth==0 || state.valid_move_count[cindex]==0){
		int opcolor;
		cindex=state.current_color-1;
		int opindex=!cindex;
		opcolor=opindex+1;
		state.Find_valid_moves(opcolor);
    	int corners=0, edges=0;
    	if(state.board[0][0]==state.current_color)
    		corners++;
    	if(state.board[0][7]==state.current_color)
    		corners++;
    	if(state.board[7][0]==state.current_color)
    		corners++;
    	if(state.board[7][7]==state.current_color)
    		corners++;
    	if(state.board[0][0]==opcolor)
    		corners--;
    	if(state.board[0][7]==opcolor)
    		corners--;
    	if(state.board[7][0]==opcolor)
    		corners--;
    	if(state.board[7][7]==opcolor)
    		corners--;
    	for(int n=1;n<7;n++){
    		if(state.board[0][n]==state.current_color)
    			edges++;
    		if(state.board[7][n]==state.current_color)
    			edges++;
    		if(state.board[n][0]==state.current_color)
    			edges++;
    		if(state.board[n][7]==state.current_color)
    			edges++;
    		if(state.board[0][n]==opcolor)
    			edges++;
    		if(state.board[7][n]==opcolor)
    			edges++;
    		if(state.board[n][0]==opcolor)
    			edges++;
    		if(state.board[n][7]==opcolor)
    			edges++;
    	}
		Value_struct val;
		val.i=i;
		val.j=j;
		// val.score=corners;
		val.score=corners*300+edges*100+state.piece_count[cindex]-state.piece_count[opindex]+35*(state.valid_move_count[cindex]-state.valid_move_count[opindex]);
    	return val;
        // return the heuristic value of node;  also remeber to account for move skipping
	}
	int n=0;
    if(maximizingPlayer){
    	// cout<<"maximizingPlayer\n";
    	Value_struct bestValue;
        bestValue.score=a;
        for(n=0; n<state.valid_move_count[state.current_color-1];n++){
        	Value_struct val;
        	// cout<<"before recursion\n";
			// printf("i=%i\tj=%i\n",state.movelist_i[cindex][n], state.movelist_j[cindex][n]);
            val = minimax(state, state.movelist_i[cindex][n], state.movelist_j[cindex][n], depth - 1,a,b, false);

            if(val.score>bestValue.score){
            	bestValue.score=val.score;
            	bestValue.i=state.movelist_i[cindex][n];
            	bestValue.j=state.movelist_j[cindex][n];

            }
            if(b<=a){
            	break;
            }
            // bestValue = max(bestValue, val);
        }
        return bestValue;
    }
    else{
    	Value_struct bestValue;
    	// cout<<"minimizingPlayer\n";
        bestValue.score=b;
        for(n=0; n<state.valid_move_count[state.current_color-1];n++){
        	Value_struct val;
            val = minimax(state, state.movelist_i[cindex][n], state.movelist_j[cindex][n], depth - 1,a,b, false);
            if(val.score<bestValue.score){
            	bestValue.score=val.score;
            	bestValue.i=state.movelist_i[cindex][n];
            	bestValue.j=state.movelist_j[cindex][n];
            }
            if(b<=a){
            	break;
            }
            // bestValue =min(bestValue, val);
        }
        return bestValue;
    }
}

bool Board::Check_if_over(void){
	// Find_valid_moves(1+!(current_color-1));
	// printf("valid move count:%i\t%i\n",valid_move_count[current_color-1],valid_move_count[!(current_color-1)]);

	if(valid_move_count[current_color-1]==0){
		Find_valid_moves(1+!(current_color-1));
		if(valid_move_count[!(current_color-1)]==0){
			// cout<<valid_move_count[0]<<'\n'<<valid_move_count[1]<<endl;
			cout<<"Game is over"<<endl;
			return true;
		}
		else{
			// if(skip!=move_count-1){
			// 	cout<<"skipping move"<<endl;
			// 	if(current_color==1)
			// 		current_color=2;
			// 	else
			// 		current_color=1;
			// 	skip=move_count;
			// }
			return false;
		}
	}
	else{
		// cout<<"Continuing game"<<endl;
		return false;
	}
}

int main () {
	// int x, y;
	int a,b, timeout;
	string infilename;
	// bool SecondStartFirst=false;
	cout<<"nothing yet"<<endl;
	Board game;
	game.Reset_board();
	game.current_color=1;
	cout<<"Pick mode of play"<<endl;
	cout<<"1:\tHuman plays first"<<endl;
	cout<<"2:\tComputer plays first"<<endl;
	cout<<"3:\tComputer vs Computer"<<endl;
	cout<<"4:\tInput Board"<<endl;
	cout<<":";
	// game.AI_move(game);
	cin>>a;
	// while(1){
		switch(a){
			case 1:
				game.PlayerFirst=true;
				game.PlayerSecond=false;
				break;
			case 2:
				game.PlayerFirst=false;
				game.PlayerSecond=true;
				break;
			case 3:
				game.PlayerFirst=false;
				game.PlayerSecond=false;
				break;
			case 4:
				cout<<"Enter the relative path of the input file:\t";
				cin>>infilename;
				cout<<"\nPick mode of play"<<endl;
				cout<<"1:\tHuman plays first"<<endl;
				cout<<"2:\tComputer plays first"<<endl;
				cout<<"3:\tComputer vs Computer"<<endl;
				cout<<":";
				cin>>b;
				//Loadboard function;
				switch(b){
					case 1:
						game.PlayerFirst=true;
						game.PlayerSecond=false;
						break;
					case 2:
						game.PlayerFirst=false;
						game.PlayerSecond=true;
						break;
					case 3:
						game.PlayerFirst=false;
						game.PlayerSecond=false;
						break;
					default:
						game.PlayerFirst=true;
						game.PlayerSecond=false;
						break;
				}
				break;
			default:
				game.PlayerFirst=true;
				game.PlayerSecond=false;
				break;
		}
	cout<<"Enter AI timeout:\t";
	cin>>timeout;
	game.DFS_timeout=timeout;
	game.move_count=0;
	game.Find_valid_moves(game.current_color);
	game.Print_board();
	while(1){
		game.move_count++;

		if(game.valid_move_count[game.current_color-1]!=0){
			if(game.PlayerFirst){
				game.Make_move();
			}
			else{
				// game.AI_move(game);
				game.Random_move();
			}
			if(game.Check_if_over()){
				// cout<<"game is over"<<endl;
				break;
			}
		}
		else{
			if(game.Check_if_over()){
				// cout<<"game is over"<<endl;
				break;
			}
			cout<<"skiping move\n";
			game.current_color=1+!(game.current_color-1);
			game.Find_valid_moves(game.current_color);
			game.Print_board();
		}
		game.move_count++;

		if(game.valid_move_count[game.current_color-1]!=0){
			if(game.PlayerSecond){
				game.Make_move();
			}
			else{
				game.AI_move(game);
				// game.Random_move();
			}
			if(game.Check_if_over()){
				// cout<<"game is over"<<endl;
				break;
			}
		}
		else{
			if(game.Check_if_over()){
				// cout<<"game is over"<<endl;
				break;
			}
			cout<<"skiping move\n";
			game.current_color=1+!(game.current_color-1);
			game.Find_valid_moves(game.current_color);
			game.Print_board();
		}
		if(game.move_count>68)
			break;
		
	}
	// game.Print_board();

}




	// }
	// while(game.Check_if_over()==false){
	// 	cout<<"Pick one of the valid moves\n";
	// 	if(game.Make_move()==0){
	// 		if(game.Check_if_over()){
	// 			// cout<<"game is over"<<endl;
	// 			break;
	// 		}
	// 		cout<<"Computer will now move randomly";
	// 		cout<<endl;
	// 		game.Random_move();
	// 		game.move_count++;
	// 	}
	// }
	// cout<<"Game is over"<<endl;