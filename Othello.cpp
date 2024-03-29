//Shivam Mevawala
//AI Project 1
//Othello


#include <iostream>
#include <stdio.h>
#include <climits>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <fstream>
using namespace std;

struct Value_struct {
		    int score;
		    int i;
		    int j;
};
int skip=-1;
class Board{
	int piece_count[2];
	Value_struct minimax(Board state,int i, int j, int depth,int a, int b, bool maximizingPlayer, clock_t t, int oldi, int oldj);
	int onFrontier(int i, int j);
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
	double DFS_timeout;
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
		char c;
        int player, i = 0, j = 0;
        fstream boardFile;
        boardFile.open(filename.c_str(), fstream::in);

        //make sure user enters a valid file path
        while(boardFile.fail()){
            cout << "Invalid file!" << endl;
            cout << "Enter a file name: ";
            cin >> filename;
            boardFile.open(filename.c_str(), fstream::in);
        }

        while((c = boardFile.get()) != EOF){
            if((c == '0') || (c == '1') || (c == '2')){
                board[i][j++] = c-'0';
                if(c-'0'==1 || c-'0'==2)
                	move_count++;
                if(j == 7){
                    j = 0;
                    if(++i == 7)
                        break;
                }
            }
        }

        boardFile >> player;

        boardFile >> DFS_timeout;
	return player;
}
int Board::Print_board(void){
	string color_name;
	if(current_color==1)
		color_name="Black (Red)";
	else
		color_name="White (Blue)";

	printf("Move # %i \t %s turn\n\n",move_count, color_name.c_str());
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
	// cout<<"Valid move count:"<<valid_move_count[current_color-1]<<endl;
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

int Board::onFrontier(int i, int j){
	if(board[i][j]!=0){
		if(board[i-1][j-1]==0)
				return 1;
		if(board[i-1][j]==0)
				return 1;
		if(board[i-1][j+1]==0)
				return 1;
		if(board[i][j-1]==0)
				return 1;
		if(board[i][j+1]==0)
				return 1;
		if(board[i+1][j-1]==0)
				return 1;
		if(board[i+1][j]==0)
				return 1;
		if(board[i+1][j+1]==0)
				return 1;
		return 0;
	}
	else
		return 0;

}




int Board::Make_move(){
	 // board[i][j]=color;
	int cindex=current_color-1, a=128, i,j;
	// Print_board();

	while(a>=valid_move_count[cindex]){
		cout<< "Enter cooresponding valid move number ";
		if(!(cin>>a)){
			a=128;
		}
		a=a-1;
		if(a<valid_move_count[cindex] && a>=0){
			// cout<<a<<endl;
			i=movelist_i[cindex][a];
			j=movelist_j[cindex][a];
		}
		else
			a=128;
	}

	// cout<< "Evaluating board\n\n";
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

}
int Board::AI_move(Board game){
	// cout<<"got here in ai\n";
	int cindex=current_color-1, d;
	Board state(game);
	clock_t s, t, e, addend;
	Value_struct val;
	addend=(DFS_timeout-.03) * 1000000;
	// Print_board();
	// if(valid_move_count[cindex]!=0){
		if(valid_move_count[cindex]==1){
			if(!Evaluate_board(movelist_i[cindex][0],movelist_j[cindex][0],current_color)){
				cout<<"Valid Move\t Board is now:\n";
				Print_board();
				return 0;
			}
			else{
				cout<<"you messed up with one valid move\n";
				printf("ai i=%i \t ai j= %i\n",movelist_i[cindex][0],movelist_j[cindex][0]);
				exit(1);
			}
		}
		// time(&s);
		s=clock();
		// printf("%lu\n",s);
		t=s+addend;
		// printf("%lu\n",t);
		for(d=1;clock()<t && d<(63-move_count);d++){
			Board state(game);
			val=minimax(state,9,10, d,INT_MIN,INT_MAX, true, t, val.i,val.j);
			// printf("in loop, depth %i, i:%i , j:%i\n",d,val.i, val.j);
			// cout<<d<<endl;
		}
		e=clock();
		printf("ai i=%i \t ai j= %i with depth %i in %lf seconds\n",val.i,val.j,d, double(e-s)/1000000);
		if (!Evaluate_board(val.i,val.j, current_color)){
			cout<<"Valid Move\t Board is now:\n";
			Print_board();
			return 0;
		}
		else{
			cout<<"you messed up\n";
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

Value_struct Board::minimax(Board state,int i, int j, int depth, int a, int b, bool maximizingPlayer, clock_t t, int oldi, int oldj){
	if(clock()>t){
		Value_struct bad;
		bad.score=0;

		bad.i=oldi;
		bad.j=oldj;
		return bad;
	}
	else{
		if(i!=9){
			state.Evaluate_board(i, j, state.current_color);
			state.move_count++;

		}
		int cindex=state.current_color-1;
	    if(depth==0 || state.valid_move_count[cindex]==0){
			int opcolor, ccolor=current_color;
			cindex=ccolor-1;
			int opindex=!cindex;
			opcolor=opindex+1;
			state.Find_valid_moves(opcolor);
	    	int corners=0, edges=0, win=0, xpiece=0, cpiece=0, frontier=0;
	    	if(state.board[0][0]==ccolor)
	    		corners++;
	    	if(state.board[0][7]==ccolor)
	    		corners++;
	    	if(state.board[7][0]==ccolor)
	    		corners++;
	    	if(state.board[7][7]==ccolor)
	    		corners++;
	    	if(state.board[0][0]==opcolor)
	    		corners--;
	    	if(state.board[0][7]==opcolor)
	    		corners--;
	    	if(state.board[7][0]==opcolor)
	    		corners--;
	    	if(state.board[7][7]==opcolor)
	    		corners--;
	    	for(int n=2;n<6;n++){
	    		if(state.board[0][n]==ccolor)
	    			edges++;
	    		if(state.board[7][n]==ccolor)
	    			edges++;
	    		if(state.board[n][0]==ccolor)
	    			edges++;
	    		if(state.board[n][7]==ccolor)
	    			edges++;
	    		if(state.board[0][n]==opcolor)
	    			edges--;
	    		if(state.board[7][n]==opcolor)
	    			edges--;
	    		if(state.board[n][0]==opcolor)
	    			edges--;
	    		if(state.board[n][7]==opcolor)
	    			edges--;
	    	}
	    	if(state.board[0][1]==ccolor) cpiece++;
	    	if(state.board[0][1]==opcolor) cpiece--;
	    	if(state.board[0][6]==ccolor) cpiece++;
	    	if(state.board[0][6]==opcolor) cpiece--;
	    	if(state.board[1][0]==ccolor) cpiece++;
	    	if(state.board[1][0]==opcolor) cpiece--;
	    	if(state.board[1][7]==ccolor) cpiece++;
	    	if(state.board[1][7]==opcolor) cpiece--;
	    	if(state.board[6][0]==ccolor) cpiece++;
	    	if(state.board[6][0]==opcolor) cpiece--;
	    	if(state.board[6][7]==ccolor) cpiece++;
	    	if(state.board[6][7]==opcolor) cpiece--;
	    	if(state.board[7][1]==ccolor) cpiece++;
	    	if(state.board[7][1]==opcolor) cpiece--;
	    	if(state.board[7][6]==ccolor) cpiece++;
	    	if(state.board[7][6]==opcolor) cpiece--;

	    	if(state.board[1][1]==ccolor) xpiece++;
	    	if(state.board[1][1]==opcolor) xpiece--;
	    	if(state.board[1][6]==ccolor) xpiece++;
	    	if(state.board[1][6]==opcolor) xpiece--;
	    	if(state.board[6][1]==ccolor) xpiece++;
	    	if(state.board[6][1]==opcolor) xpiece--;
	    	if(state.board[6][6]==ccolor) xpiece++;
	    	if(state.board[6][6]==opcolor) xpiece--;

	    	for(int n=1;n<7;n++){
	    		for(int m=1;m<7;m++){
	    			if(state.onFrontier(n,m)){
	    				if(board[n][m]==ccolor)
	    					frontier++;
	    				if(board[n][m]==opcolor)
	    					frontier--;	    				
	    			}
	    		}
	    	}

			// if(state.valid_move_count[cindex]==0 && state.valid_move_count[opindex]==0){
			// 		if(state.piece_count[cindex]>state.piece_count[opindex]){
			// 			win=INT_MAX-19999;
			// 			// cout<<"someone can win\n";
			// 		}
			// 		else{
			// 			win=INT_MIN+19999;
			// 			// cout<<"someone can lose\n";
			// 		}
			// }
			Value_struct val;
			val.i=i;
			val.j=j;
			// val.score=corners;
			int wc,we,wp,wxp,wcp,wm, wf;
			wc=state.move_count*(300)/60+3100;
			we=state.move_count*(-10)/60+50;
			wp=state.move_count*(18)/60-3;
			wxp=state.move_count*(285)/60-300;
			wcp=state.move_count*(105)/60-100;
			wm=state.move_count*(-8)/60+48;
			wf=state.move_count*(-84)/60-6;	

			val.score=win+corners*wc+edges*we+wf*frontier+wxp*xpiece+wcp*cpiece+wp*(state.piece_count[cindex]-state.piece_count[opindex])+wm*(state.valid_move_count[cindex]-state.valid_move_count[opindex]);
	    	return val;
	        // return the heuristic value of node;  also remeber to account for move skipping
		}
		int n=0;
	    if(maximizingPlayer){
	    	// cout<<"maximizingPlayer\n";
	    	Value_struct bestValue;
	       	bestValue.score=INT_MIN;
	    	// a=INT_MIN;
	        for(n=0; n<state.valid_move_count[state.current_color-1];n++){
	        	Value_struct val;
	        	// cout<<"before recursion\n";
				// printf("i=%i\tj=%i\n",state.movelist_i[cindex][n], state.movelist_j[cindex][n]);
	            val = minimax(state, state.movelist_i[cindex][n], state.movelist_j[cindex][n], depth - 1,a,b, false,t, oldi, oldj);

	            if(val.score>bestValue.score){
	            	bestValue.score=val.score;
	            	bestValue.i=state.movelist_i[cindex][n];
	            	bestValue.j=state.movelist_j[cindex][n];
	            }
	            if(val.score==a){
	            	if(rand()%2){
	            	bestValue.score=val.score;
	            	bestValue.i=state.movelist_i[cindex][n];
	            	bestValue.j=state.movelist_j[cindex][n];
	            	}
	            }
	            if(b<=a){
	            	return bestValue;
	            }
	            if(a<bestValue.score)
	            	a=bestValue.score;
	            // bestValue = max(bestValue, val);
	        }
	        return bestValue;
	    }
	    else{
	    	// cout<<"minimizingPlayer\n";
	    	Value_struct bestValue;
	        bestValue.score=INT_MAX;	
	        for(n=0; n<state.valid_move_count[state.current_color-1];n++){
	        	Value_struct val;
	            val = minimax(state, state.movelist_i[cindex][n], state.movelist_j[cindex][n], depth - 1,a,b, true,t, oldi, oldj);
	            if(val.score<b){
	            	bestValue.score=val.score;
	            	bestValue.i=state.movelist_i[cindex][n];
	            	bestValue.j=state.movelist_j[cindex][n];
	            }
	    	    if(val.score==b){
	            	if(rand()%2){
	            	bestValue.score=val.score;	
	            	bestValue.i=state.movelist_i[cindex][n];
	            	bestValue.j=state.movelist_j[cindex][n];
	            	}
	            }
	            if(b<=a){
	            	return bestValue;
	            }
	            if(b>bestValue.score)
	            	b=bestValue.score;
	            // bestValue =min(bestValue, val);
	        }
	        return bestValue;
	    }
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
			if(piece_count[0]<piece_count[1]){
				printf("White (Blue) won, %i to %i\n", piece_count[1],piece_count[0]);
			}
			if(piece_count[1]<piece_count[0]){
				printf("Black (Red) won, %i to %i\n",piece_count[0],piece_count[1]);
			}

			return true;
		}
		else{
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
	int a,b, importskip;
	bool importfile=false;
	double timeout;
	string infilename;
	// bool SecondStartFirst=false;
	cout<<"nothing yet"<<endl;
	Board game;
	game.Reset_board();
	game.move_count=0;
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
				game.Import_board(infilename);
				cout<<"\nPick mode of play"<<endl;
				cout<<"1:\tHuman plays first"<<endl;
				cout<<"2:\tComputer plays first"<<endl;
				cout<<"3:\tComputer vs Computer"<<endl;
				cout<<":";
				cin>>b;
				importfile=true;
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
	if(!importfile){
		cout<<"Enter AI timeout:\t";
		cin>>timeout;
		game.DFS_timeout=timeout;
	}
	game.Find_valid_moves(game.current_color);
	game.Print_board();
	while(1){

		if(importskip==1){
			if(game.valid_move_count[game.current_color-1]!=0){
				game.move_count++;
				if(game.PlayerFirst){
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
		}
		else{
			importskip=1;
		}
		if(game.valid_move_count[game.current_color-1]!=0){
			game.move_count++;
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
		if(game.move_count>62)
			break;
	}


}




