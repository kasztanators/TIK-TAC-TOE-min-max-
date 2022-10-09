#define _CRT_SECURE_NO_WARNINGS
#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <iostream>
#define COMMAND_SIZE 40

using namespace std;

struct Game{
	int N, M, K, ActivePlayer;

    void printSingleBoard(int** board, int newI, int newJ) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                if (i == newI && j == newJ)  printf("%d ", ActivePlayer);
                else  printf("%d ", board[i][j]);
            }
            printf("\n");
        }
    }
    void printCombination(int** board, bool** arrofempty) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                if (arrofempty[i][j] == true)
                {
                    printSingleBoard( board, i, j);
                }
            }
        }
    }
    const bool emptySpace(int** board) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                if (board[i][j] == 0)return true;
            }
        }
        return false;
    }
    static int min(int a, int b) {
        if (a >= b) return b;
        else return a;
    }
    static int max(int a, int b) {
        if (a >= b) return a;
        else return b;
    }
    const bool winningCondition(int** board, int player) {
        int neighbour = 0;
        int firstX, firstY, secondX, secondY;
        for (int i = 0; i < N; i++) {          // rows
            for (int j = 0; j <= (M - K); j++) {
                for (int k = j; k < K + j; k++) {
                    if (board[i][k] != player) {
                        secondX = k;
                        secondY = i;
                        break;
                    }
                    else {
                        if (neighbour == 0) {
                            firstX = k;
                            firstY = i;
                        }
                        neighbour++;
                    }
                }
                if (neighbour == K - 1) {
                    if (firstX > 0 && secondX < N) {
                        if (board[firstY][firstX - 1] == 0 && board[secondY][secondX + 1] == 0)
                            return true;
                    }

                }
                else if (neighbour >= K) return true;
                else neighbour = 0;
            }
        }
        for (int i = 0; i < M; i++) {          // columns
            for (int j = 0; j <= (N - K); j++) {
                for (int k = j; k < K + j; k++) {
                    if (board[k][i] != player) {
                        secondX = i;
                        secondY = k;
                        break;
                    }
                    else {
                        if (neighbour == 0) {
                            firstX = i;
                            firstY = k;
                        }
                        neighbour++;
                    }
                }
                if (neighbour == K - 1) {
                    if (firstY > 0 && secondY < N) {
                        if (board[firstY - 1][firstX] == 0 && board[secondY + 1][secondX] == 0)return true;
                    }
                    else if (neighbour >= K) return true;
                    else neighbour = 0;
                }
            }
            for (int i = 0; i <= M - K; i++) {
                for (int j = 0; j <= (N - K); j++) {
                    for (int k = 0; k < K; k++) {
                        if (board[k + j][k + i] != player) break;
                        else neighbour++; //diagonal
                    }
                    if (neighbour >= K) return true;
                    else neighbour = 0;
                }
            }
            for (int i = 0; i <= M - K; i++) {
                for (int j = 0; j <= (N - K); j++) {
                    for (int k = 0; k < K; k++) {
                        if (board[N - 1 - k - j][k + i] != player) break;
                        else neighbour++;  //inverse diagonal
                    }
                    if (neighbour >= K) return true;
                    else neighbour = 0;
                }
            }
            return false;
        }
    }
	void genAllPosMov(int** board) {
        bool** emptySpace = new bool* [N];
        for (int i = 0; i < N; ++i)
            emptySpace[i] = new bool[M];
        int count = 0;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                if (board[i][j] == 0) { 
                    emptySpace[i][j] = true;
                    count++;
                }
                else emptySpace[i][j] = false;
            }
        }
        if (winningCondition(board, 1))
        {
            count = 0;
            printf("%d\n", count);
        }
        else if (winningCondition(board, 2)) {
            count = 0;
            printf("%d\n", count);
        }
        else {
            printf("%d\n", count);
            printCombination(board, emptySpace);
        }

	}
    void genAllPosMovCut(int** board) {
        bool canWin = false;
        int otherPlayer;
        if (ActivePlayer == 1)otherPlayer = 2;
        else otherPlayer = 1;
        if (winningCondition(board, otherPlayer))  printf("0\n");
        else {
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < M; j++) {
                    if (board[i][j] == 0) {
                        board[i][j] = ActivePlayer;
                        if (winningCondition(board, ActivePlayer) && canWin == false) {
                            canWin = true;
                            printf("1\n");
                            printSingleBoard(board, -1, -1);
                          //  break;
                        }
                        else {
                            board[i][j] = 0;
                        }
                    }
                }
            }
            if (!canWin)genAllPosMov(board);
        }
    }
    int minMax(int** board, int depth, bool isMax,  int alpha, int beta ){
        int score = 0;
        if (isMax) {
            score = winningCondition(board, 1);
            if (score == 1) {
                score = -10;
                return score;
            }
        }
        else {
            score = winningCondition(board, 2);
            if (score == 1) {
                score = 10;
                return score;
            }
        }
        if (emptySpace(board) == false) {
            if (winningCondition(board,1)) {
                return -10;
            }
            else if (winningCondition(board, 2)) {
                return 10;
            }
            return 0; //tie
        }

        if (isMax)
        {
            int best = -1000;
            for (int j = 0; j < M; j++) {
                for (int i = 0; i < N; i++) {
                    if (board[i][j] == 0)
                    {
                        board[i][j] = 2;
                         int value = minMax(board, depth + 1, !isMax,  alpha ,beta);
                        // Undo the move
                        board[i][j] = 0;
                        best = max(best, value);
                        alpha = max(alpha, best);
                        if (beta <= alpha)break;
                    }
                }
            }
            return best;
        }
        else
        {
           int best = 1000;
            for (int j = 0; j < M; j++) {
                for (int i = 0; i < N; i++) {
                    if (board[i][j] == 0)
                    {
                        board[i][j] = 1;
                        int value = minMax(board, depth + 1, !isMax, alpha ,beta);
                        // Undo the move
                        board[i][j] = 0;
                        best = min(best, value);
                        beta = min(beta, best);
                        if (beta <= alpha)break;
                    }
                }
            }
            return best;
        }
    }
    void solveGameState(int** board) {
        int result = 0;
        bool maximizing = true;
        int  alpha =-1000, beta = 1000;
        if (ActivePlayer == 1) {
            maximizing = false;
        }
        result = minMax(board, 1,  maximizing, alpha, beta);
        
        if (result == -10)  printf("FIRST_PLAYER_WINS\n");
        else if (result == 10)  printf("SECOND_PLAYER_WINS\n");
        else if (result == 0) printf("BOTH_PLAYERS_TIE\n");
    }
	void handlingInput() {
		char command[COMMAND_SIZE]="";
		while (true) {
			scanf("%40s", &command);
			if (feof(stdin) != 0){break;}
			scanf("%i %i %i %i", &N, &M, &K, &ActivePlayer);
			int** board = new int*[N];
			for (int i = 0; i < N; i++)board[i] = new int[M];
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < M; j++) {
					scanf("%i", &board[i][j]);
				}
			}
			if (strcmp(command, "GEN_ALL_POS_MOV") == 0) {
				genAllPosMov(board);
			}
			else if (strcmp(command, "GEN_ALL_POS_MOV_CUT_IF_GAME_OVER") == 0) {
                genAllPosMovCut(board);
			}
			else if (strcmp(command, "SOLVE_GAME_STATE") == 0) {
                solveGameState(board);
			}
            for (int i = 0; i < N; i++)delete[] board[i] ;
            delete[] board;
		}
	}
};
int main() {
	Game newGame;
	newGame.handlingInput();
	return 0;
}