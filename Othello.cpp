#include <iostream>
#include <stdio.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
using namespace std;

class Board{
	int board[8][8];
	bool valid_moves[2][8][8];
	int valid_move_count[2];
	int comp_move_list_i[25];
	int comp_move_list_j[25];
	int count_white;
	int count_black;
	int move_count;
	int Reset_board(void);
	int Evaluate_board(int x, int y, int color);
public:
	Board(void);
	int current_color;
	int Find_valid_moves(int color);
	int Make_move(int x, int y, int color);
	int Random_move(void);
	int Print_board(void);
	bool Check_if_over(void); 
};



Board::Board(void) {
	Reset_board();
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
	count_white=2;
	count_black=2;
return 0;
}

int Board::Print_board(void){
	printf("Move # %i\n\n",move_count);
	int i,j;
	for(i=0; i<8; i++){
		for(j=0;j<8;j++){
			if(board[i][j]==0){
				if(valid_moves[current_color-1][i][j])
					cout<<"?\t";
				else
					cout<<"-\t";
			} 
			if(board[i][j]==1)
				cout<<"X\t"; 
			if(board[i][j]==2)
				cout<<"O\t"; 
		}
		cout<<"\n\n";
	}
	printf("White Score: %i\t\t Black Score: %i\n",count_white, count_black);
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
				for(n=2;n<=i ||n<=j;n++){
					if(board[i-n][j-n]==color){
						valid_moves[cindex][i][j]=true;
						if(color==2){
							comp_move_list_i[valid_move_count[1]]=i;
							comp_move_list_j[valid_move_count[1]]=j;								
						}
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
						if(color==2){
							comp_move_list_i[valid_move_count[1]]=i;
							comp_move_list_j[valid_move_count[1]]=j;								
						}
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
				for(n=2;n<=i||n<(8-j);n++){
					if(board[i-n][j+n]==color){
						valid_moves[cindex][i][j]=true;
						if(color==2){
							comp_move_list_i[valid_move_count[1]]=i;
							comp_move_list_j[valid_move_count[1]]=j;								
						}
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
						if(color==2){
							comp_move_list_i[valid_move_count[1]]=i;
							comp_move_list_j[valid_move_count[1]]=j;								
						}
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
						if(color==2){
							comp_move_list_i[valid_move_count[1]]=i;
							comp_move_list_j[valid_move_count[1]]=j;								
						}
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
				for(n=2;n<(8-i)||n<=j;n++){
					if(board[i+n][j-n]==color){
						valid_moves[cindex][i][j]=true;
						if(color==2){
							comp_move_list_i[valid_move_count[1]]=i;
							comp_move_list_j[valid_move_count[1]]=j;								
						}
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
						if(color==2){
							comp_move_list_i[valid_move_count[1]]=i;
							comp_move_list_j[valid_move_count[1]]=j;								
						}
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
				for(n=2;n<(8-i)||n<(8-j);n++){
					if(board[i+n][j+n]==color){
						valid_moves[cindex][i][j]=true;
						if(color==2){
							comp_move_list_i[valid_move_count[1]]=i;
							comp_move_list_j[valid_move_count[1]]=j;								
						}
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
		int opcolor,n;
		if(color==1)
			opcolor=2;
		else
			opcolor=1;
		//topleft
		if(i>1 && j>1 && board[i-1][j-1]==opcolor){			
			for(n=2;n<=i ||n<=j;n++){
				if(board[i-n][j-n]==color){
					while(n>1){
						--n;
						board[i-n][j-n]=color;
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
			for(n=2;n<=i ||n<(8-j);n++){
				if(board[i-n][j+n]==color){
					while(n>1){
						--n;
						board[i-n][j+n]=color;
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
			for(n=2;n<(8-i)||n<=j;n++){
				if(board[i+1][j-n]==color){
					while(n>1){
						--n;
						board[i+n][j-n]=color;
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
			for(n=2;n<(8-i)||n<(8-j);n++){
				if(board[i+n][j+n]==color){
					while(n>1){
						--n;
						board[i+n][j+n]=color;
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
int Board::Make_move(int j, int i, int color){
	board[i][j]=color;
	Print_board();
	cout<< "Evaluating board\n\n";
	if (!Evaluate_board(i,j, color)){
		cout<<"Valid Move\t Board is now:\n";
		Print_board();
		return 0;
	} else{
		cout<<"Invalid move\t Try Again\n";
		board[i][j]=0;
		Print_board();
		return 1;
	}
}
int Board::Random_move(void){
	if(current_color==2){
		srand (time(NULL));
		int rmove=rand() % valid_move_count[1];
		cout<<"valid_move_count:"<<valid_move_count[1]<<endl;
		cout<<"random move:"<<rmove<<endl;
		int i= comp_move_list_i[rmove];
		int j= comp_move_list_j[rmove];
		board[i][j]=current_color;
		Print_board();
		cout<< "Evaluating board\n\n";
		if (!Evaluate_board(i,j, current_color)){
			cout<<"Valid Move\t Board is now:\n";
			Print_board();
			return 0;
		} else{
			cout<<"Invalid move\t Try Again\n";
			board[i][j]=0;
			Print_board();
			return 1;
		}
	}
	else 
		return 1;
}
bool Board::Check_if_over(void){
	if(valid_move_count[current_color]==0)
		return true;
	else
		return false;
}


int main () {
	int x, y;
	string waiting;
	cout<<"nothing yet"<<endl;
	Board game;
	game.current_color=1;
	game.Find_valid_moves(game.current_color);
	game.Print_board();
	while(!game.Check_if_over()){
		cout<<"Pick one of the valid moves\n";
		cout<<"x value:";
		cin>>x;
		cout<<"\ny value:";
		cin>>y;
		cout<<endl;
		if(game.Make_move(x, y, game.current_color)==0){
			if(game.Check_if_over())
				break;
			cout<<"Computer will now move randomly";
			cin>>waiting;
			cout<<endl;
			game.Random_move();
		}
	}
	return 0;
	}