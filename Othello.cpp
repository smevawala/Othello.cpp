#include <iostream>
#include <stdio.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
using namespace std;

class Board{
	int board[8][8];
	bool valid_moves[2][8][8];
	int valid_move_count[2];
	int movelist_i[2][25];
	int movelist_j[2][25];
	int piece_count[2];
	int Reset_board(void);
	int Evaluate_board(int x, int y, int color);
	int minimax(int node,int depth,int maximizingPlayer);
public:
	Board(void);
	Board(const Board &game);
	int move_count;
	int current_color;
	int Find_valid_moves(int color);
	int Make_move();
	int Random_move();
	int AI_move();
	int Print_board(void);
	int Import_board(string filename);
	bool Check_if_over(void);
	int DFS_timeout;
	bool PlayerFirst;
	bool PlayerSecond; 
};



Board::Board(void) {
	Reset_board();
	move_count=0;
};
Board::Board(const Board &game){
	

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
	int i,j;
	cout<<"\t1\t2\t3\t4\t5\t6\t7\t8\n\n";
	for(i=0; i<8; i++){
		cout<<i+1<<"\t";
		for(j=0;j<8;j++){
			if(board[i][j]==0){
				if(valid_moves[current_color-1][i][j])
					cout<<"?\t";
				else
					cout<<"-\t";
			} 
			if(board[i][j]==1)
				cout<<"1\t"; 
			if(board[i][j]==2)
				cout<<"2\t"; 
		}
		cout<<"\n\n";
	}
	printf("White Score: %i\t\t Black Score: %i\n",piece_count[0], piece_count[1]);
	cout<<valid_move_count[current_color-1]<<endl;
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
	if(!valid_moves[color-1][i][j])
		return 1;
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
				if(board[i+1][j-n]==color){
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
	Find_valid_moves(current_color);
	return 0;
	}
}
int Board::Make_move(){
	// board[i][j]=color;
	int cindex=current_color-1, a=128, i,j;
	Print_board();
	cout<<"Choose valid move"<<endl;
	for(int n=0; n<valid_move_count[cindex];n++){
		printf("%i:\t%i, %i\n",n+1,movelist_j[cindex][n]+1,movelist_i[cindex][n]+1);
	}
	while(a>=valid_move_count[cindex]){
		cout<< "Enter cooresponding number";
		cin>>a;
		a=a-1;
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
}
int Board::Random_move(){
	int cindex=current_color-1;
		srand (time(NULL));
		int rmove=rand() % valid_move_count[cindex];
		// cout<<"valid_move_count:"<<valid_move_count[cindex]<<endl;
		// cout<<"random move:"<<rmove<<endl;
		int i= movelist_i[cindex][rmove];
		int j= movelist_j[cindex][rmove];
		board[i][j]=current_color;
		Print_board();
		cout<< "Evaluating board\n\n";
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
}
int Board::AI_move(){
	int cindex=current_color-1;
	// minimax(origin, depth, TRUE)

	return 0;
}

// int Board::minimax(int node, depth,int maximizingPlayer){
//     if(depth==0 || valid_move_count[current_color-1]==0){ 
//     	return 0;
//         // return the heuristic value of node;
// 	}
//     if(current_color==maximizingPlayer){
//         int bestValue=-99999;
//         for(int i=0; i<valid_move_count[current_color-1];i++){
//             int val = minimax(node, board, depth - 1, FALSE);
//             bestValue = max(bestValue, val);
//         }
//         return bestValue;
//     }
//     else{
//         int bestValue = 99999;
//         for each child of node{
//             int val = minimax(nodw, depth - 1, TRUE));
//             bestValue =min(bestValue, val);
//         }
//         return bestValue;
//     }
// }

bool Board::Check_if_over(void){
	if(valid_move_count[current_color-1]==0){
		cout<<valid_move_count[0]<<'\n'<<valid_move_count[1]<<endl;
		return true;
	}
	else{
		cout<<"Continuing game"<<endl;
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
	game.current_color=1;
	cout<<"Pick mode of play"<<endl;
	cout<<"1:\tHuman plays first"<<endl;
	cout<<"2:\tComputer plays first"<<endl;
	cout<<"3:\tComputer vs Computer"<<endl;
	cout<<"4:\tInput Board"<<endl;
	cout<<":";
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
	game.Find_valid_moves(game.current_color);

	while(game.Check_if_over()==false){
		// if(game.move_count==0 && SecondStartFirst){
		if(game.PlayerFirst){
			game.Make_move();
		}
		else{
			game.Random_move();
		}
		// }

		if(game.Check_if_over()){
			// cout<<"game is over"<<endl;
			break;
		}

		if(game.PlayerSecond){
			game.Make_move();
		}
		else{
			game.Random_move();
		}

		game.move_count++;
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