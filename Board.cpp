/*
Author: Alysa Liann Vega
Course: CSCI-135
Instructor: Tong Yi
Date: April 19, 2022
Assignment: Memory Game Lab

This program creates a game similar to the online game 1024.
*/

#include "Board.hpp"
#include <iostream>
#include <string>
#include <iomanip> //setw
#include <cstdlib> //srand, rand
#include <vector>
using namespace std;

//Task A:
Board::Board(){     //default constructor
    numRows = 3;
    numCols = 3;
    allocateMemory();
    max = 0;
    target = 32;
    srand(1);
    //srand(time(NULL));
}

Board::Board(int m){    //construct a 3 x 3 panel
    if (m >= 1){
        numRows = m;
        numCols = m;
    }
    else {
        numRows = 3;
        numCols = 3;
    }
    panel = new int*[numRows];        //keep * after new int, else becomes local var
    for (int row = 0; row < numRows; row++){
        panel[row] = new int[numCols];
    }
    max = 0;
    target = 32;
}

Board::Board(int m, int n){     //construct a m x n panel
    if (m >= 1 && n > 1){
        numRows = m;
        numCols = n;
    }
    else {
        numRows = 3;
        numCols = 3;
    }
    panel = new int*[numRows];         //make new int, from lecture
    for (int row = 0; row < numRows; row++){
        panel[row] = new int[numCols];
    }
    max = 0;
    target = 32;
}

Board::~Board(){    //destructor, release dynamic memory of unneeded object
    delete[] panel;
    panel = nullptr;
}

void Board::allocateMemory(){   //apply dynamic memory for panel so that panel has numRows rows and numCols columns
    int *panel = new int[numRows * numCols];
}

void Board::print() const{  //print the panel
    
    for (int row = 0; row < numRows; row++) {
        
        cout << "+";                            //cell borders
        for (int i = 0; i < numCols; i++){
            cout << "----+";
        }
        cout << endl;
        cout << "|";
        for (int col = 0; col < numCols; col++) {       //cells
            if (panel[row][col] == 0)
                cout << setw(5) << "|";
            else
                cout << setw(4) << panel[row][col] << "|";
        }
        cout << endl;
        if (row == numRows-1){                  //bottom cell border
            cout << "+";
            for (int i = 0; i < numCols; i++){
                cout << "----+";
            }
            cout << endl;
        }
    }
    
}

//Task B:
bool Board::noAdjacentSameValue() const{       //if there is no two adjacent cells share same value, return true, otherwise, return false.
    
    for (int row = 0; row < numRows; row++) {
        for (int col = 0; col < numCols; col++) {
            if (panel[row][col] == 0)
                return false;   //at least 1 cell empty, game not over
        }
    }
    
    for (int row = 0; row < numRows; row++){
        for (int col = 0; col < numCols-1; col ++){
            if (panel[row][col] == panel[row][col+1]){  //adjacent cells (rows)
                return false;   //game not over
            }
        }
    }
    
    for (int col = 0; col < numCols; col++){
        for (int row = 0; row < numRows-1; row++){
            if (panel[row][col] == panel[row+1][col]){  //adjacent cells (cols)
                return false;   //game not over
            }
        }
    }
    
    if (max == target)
        return true;        //game over (win)
    
    return true;            //game over (lose)
}

void Board::selectRandomCell(int& row, int& col){  //select a random cell from empty cell
    
    if (noAdjacentSameValue()){
        cout << "Game over. Try again." << endl;
        exit(1);
    }
    
    int empty = 0, num = 0;
    int temp[numRows][numCols];
    
    //numbers cells in temp array (0 to total-1):
    for (int r = 0; r < numRows; r++) {
        for (int c = 0; c < numCols; c++) {
            if (panel[r][c] == 0)
                empty++;
            temp[r][c] = num;
            num++;
        }
    }
    int *emptyCells = new int[empty];       //array of empty cells in panel
    
    //fills emptyCells array with indexes of panel array's zeroes:
    num = 0;
    for (int r = 0; r < numRows; r++) {
        for (int c = 0; c < numCols; c++) {
            if (panel[r][c] == 0){
                emptyCells[num] = temp[r][c];     //holds locations of zeroes
                //cout << emptyCells[num] << endl;
                num++;
            }
        }
    }
    
    //generates random cell from emptyCells:
    int randCell = rand() % empty;
    
    //sets row and col equal to randomly generated cell:
    for (int r = 0; r < numRows; r++) {
        for (int c = 0; c < numCols; c++) {
            if (temp[r][c] == emptyCells[randCell]){
                row = r;
                col = c;
            }
        }
    }
    
    panel[row][col] = 1;
    
    print();
    if (noAdjacentSameValue()){
        cout << "Game over. Try again." << endl;
        exit(1);
    }
    
}

void Board::pressUp(){
    
    vector<int> nonZeros;
    
    for (int col = 0; col < numCols; col++){
        
        bool loop = true;
        for (int row = 0; row < numRows; row++){
            if (panel[row][col] != 0){
                loop = false;       //loop false if any number non-zero
            }
        }
        if (loop) continue;     //move to next col in panel (i think)
        
        for (int row = 0; row < numRows; row++){
            if (panel[row][col] != 0){
                nonZeros.push_back(panel[row][col]);     //add non-zero values to vector
            }
        }
        
        //make nonZeros same size as numRows with remaining elms = 0:
        while (nonZeros.size() != numRows){
            nonZeros.push_back(0);
        }
        
        //change col to equal vector nonZeros (from up down):
        for (int row = 0; row < numRows; row++){
            panel[row][col] = nonZeros[row];
        }
        
        for (int row = 0; row < numRows-1; row++){
            if (panel[row][col] == panel[row+1][col]){
                panel[row][col] *= 2;
                panel[row+1][col] = panel[row+2][col];
                panel[row+2][col] = 0;
                break;
            }
        }
        nonZeros.clear();
    }
    
    //update max:
    for (int row = 0; row < numRows; row++){
        for (int col = 0; col < numCols; col++){
            if (panel[row][col] > max)
                max = panel[row][col];      //update max if necessary
        }
    }
    
    
    //call selectRandomCell
    int row = 0, col = 0;
    selectRandomCell(row,col);
}

void Board::pressDown(){
    
    vector<int> nonZeros;
    
    for (int col = 0; col < numCols; col++){
        
        bool loop = true;
        for (int row = 0; row < numRows; row++){
            if (panel[row][col] != 0){
                loop = false;       //loop false if any number non-zero
            }
        }
        if (loop) continue;     //move to next col in panel (i think)
        
        for (int row = 0; row < numRows; row++){
            if (panel[row][col] != 0){
                nonZeros.push_back(panel[row][col]);     //add non-zero values to vector
            }
        }
        
        //reverses order of nonZeros, makes it same size as numRows with remaining elems = 0
        while (nonZeros.size() != numRows){
            int end = nonZeros.size() - 1;
            nonZeros.push_back(nonZeros[end]);      //expand vector by 1
            for (int i = end; i > 0; i--) {
                nonZeros[i] = nonZeros[i - 1];      //move all elems down
            }
            nonZeros[0] = 0;                        //make first elem 0
        }
        
        //change col to equal vector nonZeros (from down up):
        for (int row = numRows-1; row >= 0; row--){
            panel[row][col] = nonZeros[row];     //works bc order of nonZeros vector!
        }
        
        for (int row = numRows-1; row > 0; row--){
            if (panel[row][col] == panel[row-1][col]){
                panel[row][col] *= 2;
                panel[row-1][col] = panel[row-2][col];
                for (int i = row-1; i > 0; i--){
                    panel[i][col] = panel[i-1][col];
                    panel[0][col] = 0;
                }
                break;
            }
        }
        nonZeros.clear();
    }
    
    //update max:
    for (int row = 0; row < numRows; row++){
        for (int col = 0; col < numCols; col++){
            if (panel[row][col] > max)
                max = panel[row][col];      //update max if necessary
        }
    }
    
    //call selectRandomCell
    int row = 0, col = 0;
    selectRandomCell(row,col);
}

void Board::pressLeft(){
    
    vector<int> nonZeros;
    
    for (int row = 0; row < numRows; row++){
        
        bool loop = true;
        for (int col = 0; col < numCols; col++){
            if (panel[row][col] != 0){
                loop = false;       //loop false if any number non-zero
            }
        }
        if (loop) continue;     //move to next col in panel (i think)
        
        for (int col = 0; col < numCols; col++){
            if (panel[row][col] != 0){
                nonZeros.push_back(panel[row][col]);     //add non-zero values to vector
            }
        }
        
        //make nonZeros same size as numRows with remaining elms = 0:
        while (nonZeros.size() != numCols){
            nonZeros.push_back(0);
        }
        
        //change col to equal vector nonZeros (from left to right):
        for (int col = 0; col < numCols; col++){
            panel[row][col] = nonZeros[col];     //works bc order of nonZeros vector!
        }
        
        for (int col = 0; col < numCols-1; col++){
            if (panel[row][col] == panel[row][col+1]){
                panel[row][col] *= 2;
                panel[row][col+1] = panel[row][col+2];
                panel[row][col+2] = 0;
                break;
            }
        }
        
        nonZeros.clear();
    }
    
    //update max:
    for (int row = 0; row < numRows; row++){
        for (int col = 0; col < numCols; col++){
            if (panel[row][col] > max)
                max = panel[row][col];      //update max if necessary
        }
    }
    
    //call selectRandomCell
    int row = 0, col = 0;
    selectRandomCell(row,col);
}

void Board::pressRight(){
    
    vector<int> nonZeros;
    
    for (int row = 0; row < numRows; row++){
        
        bool loop = true;
        for (int col = 0; col < numCols; col++){
            if (panel[row][col] != 0){
                loop = false;       //loop false if any number non-zero
            }
        }
        if (loop) continue;     //move to next col in panel (i think)
        
        for (int col = 0; col < numCols; col++){
            if (panel[row][col] != 0){
                nonZeros.push_back(panel[row][col]);     //add non-zero values to vector
            }
        }
        
        //reverses order of nonZeros, makes it same size as numRows with remaining elems = 0
        while (nonZeros.size() != numCols){
            int end = nonZeros.size() - 1;
            nonZeros.push_back(nonZeros[end]);      //expand vector by 1
            for (int i = end; i > 0; i--) {
                nonZeros[i] = nonZeros[i - 1];      //move all elems down
            }
            nonZeros[0] = 0;                        //make first elem 0
        }
        
        //change col to equal vector nonZeros (from right to left):
        for (int col = numCols-1; col >= 0; col--){
            panel[row][col] = nonZeros[col];     //works bc order of nonZeros vector!
        }
        
        for (int col = numCols-1; col > 0; col--){
            if (panel[row][col] == panel[row][col-1]){
                panel[row][col] *= 2;
                panel[row][col-1] = panel[row][col-2];
                for (int c = col-1; c > 0; c--){
                    panel[row][c] = panel[row][c-1];
                    panel[row][0] = 0;
                }
                break;
            }
        }
        nonZeros.clear();
    }
    
    //update max:
    for (int row = 0; row < numRows; row++){
        for (int col = 0; col < numCols; col++){
            if (panel[row][col] > max)
                max = panel[row][col];      //update max if necessary
        }
    }
    
    //call selectRandomCell
    int row = 0, col = 0;
    selectRandomCell(row,col);
}


void Board::clear(){    //set each cell of the panel to be zero
    for (int row = 0; row < numRows; row++){
        for (int col = 0; col < numCols; col++){
            panel[row][col] = 0;
        }
    }
}

void Board::setGoal(int goal){      //set goal of the game
    target = goal;
}

void Board::start(){
    
    int round = 1;
    clear();
    
    //call selectRandomCell twice to generate two values
    int cell1row = 0, cell1col = 0, cell2row = 0, cell2col = 0;
    selectRandomCell(cell1row, cell1col);
    selectRandomCell(cell2row, cell2col);
    
    while (true) {
        if (max == target){
            cout << "Congratulation!";
            break;
        }
        //reference: h1ps://stackoverflow.com/quesEons/10463201/getch-and-arrow-codes
        if (getchar() == '\033'){ // if the first value is esc
            getchar(); // skip the [
            
            switch(getchar()){  // the real value
                case 'A':   // code for arrow up
                    cout << "Round " << setw(4) << round << ": ";
                    cout << "Press UP. " << endl;
                    pressUp();
                    round++;
                    break;
                case 'B': //arrow down
                    cout << "Round " << setw(4) << round << ": ";
                    cout << "Press DOWN. " << endl;
                    pressDown();
                    round++;
                    break;
                case 'C': //arrow right
                    cout << "Round " << setw(4) << round << ": ";
                    cout << "Press RIGHT. " << endl;
                    pressRight();
                    round++;
                    break;
                case 'D': //arrow left
                    cout << "Round " << setw(4) << round << ": ";
                    cout << "Press LEFT. " << endl;
                    pressLeft();
                    round++;
                    break;
            } //end of switch
        } //end of if
    } //end of while
    
}
