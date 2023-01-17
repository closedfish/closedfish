#include<iostream>
#include "ConnectUISAI.h"
/*
*@brief Translate the initial board to just pawns (1 are pawns of the bottom player and 2 are pawns of top player)
*@param BoardInput Board from UI
*@return board with just pawns 
*/
int** BoardTranslate::BoardToPawns(int** BoardInput){
    int** PawnsBoard[8][8];
    for(int i=0;i<=7;i++)
    for(int j=0;j<=7;j++)
    {
        if(BoardInput[i][j]%10==1){
            if(BoardInput[i][j]/10==1){
                **PawnsBoard[i][j]= 1;
            }
            else{
                **PawnsBoard[i][j]= 2;
            }
        }
        else{
            **PawnsBoard[i][j]=0;
        }
    }
    return **PawnsBoard;
}
/*
*@brief From the board with just pawns it returns a list with the position of the most advanced pawn of the bottom player.
*@param  PawnsBoard The board with just pawns
*@return List of positions for each column
*/
int* BoardTranslate::ListDownPlayer(int** PawnsBoard){
    int* PawnsDownPlayer[8];
    for(int i=0;i<=7;i++)
    *PawnsDownPlayer[i]=-1;
    for(int i=0;i<=7;i++)
    for(int j=0;j<=7;j++)
    {
        if(*PawnsDownPlayer[j]==-1)
            if(PawnsBoard[i][j]==1)
                 *PawnsDownPlayer[j]=8-i;
    }
    return *PawnsDownPlayer;
}
/*
*@brief From the board with just pawns it returns a list with the position of the most advanced pawn of the top player.
*@param  PawnsBoard The board with just pawns
*@return List of positions for each column
*/
int* BoardTranslate::ListUpPlayer(int** PawnsBoard){
    int* PawnsUpPlayer[8];
    for(int i=0;i<=7;i++)
    *PawnsUpPlayer[i]=8;
    for(int i=0;i<=7;i++)
    for(int j=0;j<=7;j++)
    {
        if(PawnsBoard[i][j]==2)
                 *PawnsUpPlayer[j]=8-i;
    }
    return *PawnsUpPlayer;
}
