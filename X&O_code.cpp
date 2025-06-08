#include <graphics.h>
#include <iostream>
#include <utility>
#include <algorithm>
#include <vector>
#include <sstream>
using namespace std;
int pctPlayer=0,pctBot=0;

int isFull(vector<vector<int>>& grid)
{
    for(auto &row :grid)
    {
        for(int cell:row)
        {
            if(cell==-1)
                return 0;
        }
    }
    return 1;
}

int raspuns(vector<vector<int>>& grid)
{
    for(int i = 0; i < 3; i++)
    {
        if(grid[i][0] == grid[i][1] && grid[i][1] == grid[i][2] && grid[i][0] != -1)
        {
            return grid[i][0]; //row win
        }
        if(grid[0][i] == grid[1][i] && grid[1][i] == grid[2][i] && grid[0][i] != -1)
        {
            return grid[0][i]; // Column win
        }
    }
    if(grid[0][0] == grid[1][1] && grid[1][1] == grid[2][2] && grid[0][0] != -1)
    {
        return grid[1][1]; // Main diagonal win
    }
    if(grid[0][2] == grid[1][1] && grid[1][1] == grid[2][0] && grid[0][2] != -1)
    {
        return grid[1][1]; // Anti-diagonal win
    }

    return -1; // No win
}

int minimax(vector<vector<int>> &grid, bool isBotTurn)
{
    int winner = raspuns(grid);
    if(winner == 0) return +1; // a castigat bot-ul
    if(winner == 1) return -1; //a castigat player-ul
    if(isFull(grid)) return 0;

    int best = isBotTurn? -1e9:1e9;

    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            if(grid[i][j]==-1)
            {
                grid[i][j]=isBotTurn?0:1;
                int score = minimax(grid, !isBotTurn);
                grid[i][j]=-1;
                if(isBotTurn)
                {
                    best = max(score,best);
                }else
                {
                    best = min(score,best);
                }
            }
        }
    }
    return best;
}

pair<int,int> bestMove(vector<vector<int>>& grid)
{
    int bestscore = -1e9;
    pair<int,int> move = {-1, -1};

    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            if(grid[i][j]==-1)
            {
                //mereu bot-ul va juca cu 0, trebuie sa ma asigur ca la 
                //printare  verificam cine e cine
                grid[i][j]=0;
                int score = minimax(grid,false);
                grid[i][j]=-1;
                if(score>bestscore)
                {
                    bestscore=score;
                    move = {i,j};
                }
            }
        }
    }
    return move;
}
int valid(int row, int col, vector<vector<int>>& grid)
{
    if(grid[row-1][col-1]!=-1)
    {
        return 0;
    }
    return 1;
}
void drawGrid() 
{
    // Vertical lines
    setbkcolor(BLACK);
    line(200, 0, 200, 600);
    line(400, 0, 400, 600);
    // Horizontal lines
    line(0, 200, 600, 200);
    line(0, 400, 600, 400);
}
void updateScore(int pctBot, int pctPlayer)
{
    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(0,600,600,640);
    setcolor(WHITE);
    ostringstream oss1, oss2;
    oss1 << "Bot: " << pctBot;
    oss2 << "Player: " << pctPlayer;
    setbkcolor(DARKGRAY);
    outtextxy(60, 610, (char*)oss1.str().c_str());
    outtextxy(450, 610, (char*)oss2.str().c_str());
}
void clearGrid(vector<vector<int>> & grid)
{
    setfillstyle(SOLID_FILL, BLACK); // assuming white is your background
    bar(0, 0, 600, 600); // clear only the grid area, not the score bar

    grid = vector<vector<int>>(3,vector<int>(3,-1));
    drawGrid();
    updateScore(pctBot,pctPlayer);
}
int getCellFromClick(int x, int y)
{
    int row = y/200;
    int col = x/200;
    return row*3+col;
}
void drawX(int cell)
{
    int row = cell/3;
    int col = cell%3;
    int x = col*200;
    int y = row*200;
    setcolor(CYAN);
    line(x+50,y+50,x+150,y+150);
    line(x+150, y+50, x+50, y+150);
}
void drawO(int cell)
{
    int row = cell/3;
    int col = cell%3;
    int x = col*200+100;
    int y = row*200+100;
    setcolor(RED);
    circle(x, y, 50);
}
void drawScoreBar()
{
    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(0,600,600,640);
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
}
void gameLoop(vector<vector<int>> &grid, bool playerTurn)
{
    int winner=1;

    if(!playerTurn)
    {
        pair<int,int> miscare = bestMove(grid);
        grid[miscare.first][miscare.second]=0;
        drawX(miscare.first*3+miscare.second);
    }
    while(!isFull(grid))
    {
        int row=-1, col=-1;
        int cell, x, y=-1;
        
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            int clickx = mousex();
            int clicky = mousey();
            clearmouseclick(WM_LBUTTONDOWN);
            int cell = getCellFromClick(clickx, clicky);
            int row = cell/3;
            int col = cell%3;
            if(valid(row+1, col+1, grid))
            {
                grid[row][col]=1;
                if(playerTurn)
                    drawX(cell);
                else drawO(cell);
                if(!isFull(grid) && raspuns(grid)==-1)
                {
                    pair<int,int> miscare = bestMove(grid);
                    grid[miscare.first][miscare.second] = 0;
                    if(playerTurn)
                        drawO(miscare.first*3+miscare.second);
                    else drawX(miscare.first*3+miscare.second);
                }
            }
            winner = raspuns(grid);
            if(winner!=-1)
                break;
        }
    }
    if(winner==1)
        pctPlayer++;
    else if(winner==0)
        pctBot++;

    updateScore(pctBot,pctPlayer);
    outtextxy(170, 610, (char*)"Click anywhere to play again.");
    while(!ismouseclick(WM_LBUTTONDOWN))
    {
        delay(10);
    }
    clearmouseclick(WM_LBUTTONDOWN);
    clearGrid(grid);
}
// cand revii maine la cod
//2 adauga ideea de scor dintre bot si persoana
int main()
{
    vector<vector<int>> grid(3,vector<int>(3,-1));
    initwindow(600,640, "X si 0");
    drawGrid();
    drawScoreBar();
    updateScore(pctBot,pctPlayer);
    bool playerTurn = true;
    while(true)
    {
        gameLoop(grid,playerTurn);

        playerTurn=!playerTurn;
    }
   

    getch();
    closegraph();
    return 0;
}