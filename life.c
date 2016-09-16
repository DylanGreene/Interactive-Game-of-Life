#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "gfx_custom.h"

#define WIDTH 400
#define HEIGHT 400
#define TRUE 1
#define FALSE 0

void setSceneFileState(FILE *fp, int liveCell[][40]);
void actOnInput(char c, int liveCell[][40]);
void playContinuousy(int liveCell[][40]);
void iterateSimulation(int liveCell[][40]);
void updateCellStates(int liveCell[][40]);
void drawLiveCell(int i, int j);
void getNewCellPos(int liveCell[][40]);
void killCellPos(int liveCell[][40]);
void reDrawSameState(int liveCell[][40]);

int main(int argc, char *argv[]){
    char c = 0, infile[20];
    int hasSceneFile = FALSE, liveCell[40][40] = {0};

    if(argc == 2){ //The executable and the scene file are specified
        strcpy(infile, argv[1]);
        hasSceneFile = TRUE;
    }else if(argc > 2){ //Three or more entered which is incorrect ussage
        printf("Incorrect usage\n");
        printf("You must type: \"%s\" , or \"%s sceneX.txt\"\n", argv[0], argv[0]);
        return 1;
    }

    gfx_open(WIDTH, HEIGHT, "Game of Life"); //open the window

    FILE *fp;
    if(hasSceneFile && !(fp = fopen(infile, "r"))){ //attempt to open the scpecified file
        printf("Error in opening file %s.\n", infile); //if the file could not be opened
        return 1;
    }else if(hasSceneFile){ //if the file opened succesfully
        setSceneFileState(fp, liveCell); //read in the file state
        fclose(fp);
    }

    actOnInput(c, liveCell); //get into the meat of the program where the user does stuff if a scene was not entered

    return 0;
}

void setSceneFileState(FILE *fp, int liveCell[][40]){
    char c, indices[10];
    char *pEnd;
    int i, j;
    while(!feof(fp)){ //read the file until the end
        c = fgetc(fp); fgetc(fp); //get the first char of each line and the following space
        if(c == 'a'){ //if the first char on that line is an 'a'
            fgets(indices, sizeof indices, fp); //get the rest of the line
            i = strtol(indices, &pEnd, 10); //parse the first int in the string
            j = strtol(pEnd, NULL, 10); //parse the second int in the string
            liveCell[i][j] = TRUE; //make that cell alive
        }else if(c == 'p'){ //after all of the inputs, p means play continuosly
            actOnInput(c, liveCell); //pass 'p' to the main loop of the program and the board with initial values
        }
    }
    return;
}

void actOnInput(char c, int liveCell[][40]){
    printf("Press:\n");
    printf("\t'q' to quit\n");
    printf("\t'a' to add a live cell\n");
    printf("\t'r' to kill a cell\n");
    printf("\t'n' to iterate by one tick\n");
    printf("\t'p' to play the animation\n");
    printf("\t's' to stop the animation\n");
    while(c != 'q'){
        if(c == 'a') getNewCellPos(liveCell); //get a new live cell from the user
        else if(c == 'r') killCellPos(liveCell); //let the user kill a cell
        else if(c == 'n'){ iterateSimulation(liveCell); gfx_wait(); } //iterate the cells by one tick
        else if(c == 'p') playContinuousy(liveCell); //continusly update the ticks

        c = gfx_wait(); //wait for the user to choose a new option
    }
    return;
}

void playContinuousy(int liveCell[][40]){ //play continusly until 's' is pressed for stop
    while(TRUE){
        iterateSimulation(liveCell);
        if(gfx_event_waiting()){ //see if the user has pressed a key
            char c = gfx_wait(); //get the key the user pressed
            if(c == 's'){ //s to stop the continueal animation
                return; //return back to actOnInput to wait for another action
            }
        }
    }
}

void iterateSimulation(int liveCell[][40]){
    updateCellStates(liveCell); //update the cells by one tick
    gfx_clear(); //clear the display of the old cells
    int i, j;
    for(i = 0; i < 40; i++){ //loop through all of the cells
        for(j = 0; j < 40; j++){
            if(liveCell[i][j] == TRUE){ //if the cell at that position is supposed to be alive
                gfx_color(255, 255, 255); //mke sure the color is white for the new cell
                drawLiveCell(i, j); //draw the cell at the appropriate position
            }
        }
    }
    gfx_flush(); //flush the cells to the display
    usleep(100000); //usleep to slow it down so the animation is visible
    return; //return back to actOnInput to wait for another action
}

void updateCellStates(int liveCell[][40]){ //does all of the game rule checks for a single tick
    int temp[40][40] = {0}; //temp array to update the cells
    int i, j, numNeighborLiveCells;
    for(i = 0; i < 40; i++){
        for(j = 0; j < 40; j++){
            numNeighborLiveCells = 0;
            //the following ifs check each of the eight surrounding cells to count how many neighbors are alive
            if(i-1 >= 0 && j-1 >= 0 && liveCell[i-1][j-1] == TRUE) numNeighborLiveCells++;
            if(i-1 >= 0 && liveCell[i-1][j] == TRUE) numNeighborLiveCells++;
            if(i-1 >= 0 && j+1 < 40 && liveCell[i-1][j+1] == TRUE) numNeighborLiveCells++;
            if(j-1 >= 0 && liveCell[i][j-1] == TRUE) numNeighborLiveCells++;
            if(j+1 < 40 && liveCell[i][j+1] == TRUE) numNeighborLiveCells++;
            if(i+1 < 40 && j-1 >= 0 && liveCell[i+1][j-1] == TRUE) numNeighborLiveCells++;
            if(i+1 < 40 && liveCell[i+1][j] == TRUE) numNeighborLiveCells++;
            if(i+1 < 40 && j+1 < 40 && liveCell[i+1][j+1] == TRUE) numNeighborLiveCells++;

            //applies the rules for the game of life
            if(numNeighborLiveCells < 2) temp[i][j] = FALSE;
            else if((numNeighborLiveCells == 2 || numNeighborLiveCells == 3) && liveCell[i][j] == TRUE) temp[i][j] = TRUE;
            else if(numNeighborLiveCells > 3) temp[i][j] = FALSE;
            else if(liveCell[i][j] == FALSE && numNeighborLiveCells == 3) temp[i][j] = TRUE;
        }
    }
    //coppies the new cell states
    for(i = 0; i < 40; i++){
        for(j = 0; j < 40; j++){
            liveCell[i][j] = temp[i][j];
        }
    }
    return;
}

void drawLiveCell(int i, int j){ //drawing a live cell consists of
    gfx_fill_rectangle(j*(WIDTH/40), i*(HEIGHT/40), WIDTH/40, HEIGHT/40); //just draws a filled square that represents the cell
    return;
}

void getNewCellPos(int liveCell[][40]){ //allows the user to select a new cell to become alive
    while(TRUE){
        int curx = current_xpos(), cury = current_ypos(); //custom gfx functions: get position of cursor without an event
        gfx_clear(); //clear the display
        gfx_color(0,255,0); //set the color to green
        drawLiveCell((int)floor(cury/(HEIGHT/40)), (int)floor(curx/(WIDTH/40))); //snap a green cell to the location of the mouse on the grid
        reDrawSameState(liveCell); //re draw all of the other cells since we are clearing them for the moving green cell
        if(gfx_event_waiting()){
            if(gfx_wait() != 1) continue; //if the user has left clicked, we will place a new cell there
            int xpos = gfx_xpos(); //get the position of where the mouse was when it was clicked
            int ypos = gfx_ypos();
            gfx_color(255,255,255); //set the color back to white since this is the final cell position
            drawLiveCell((int)floor(ypos/(HEIGHT/40)), (int)floor(xpos/(WIDTH/40))); //draw the new cell snapped to the grid
            liveCell[(int)floor(ypos/(HEIGHT/40))][(int)floor(xpos/(WIDTH/40))] = TRUE; //update the array so the cell will be alive for the next tick
            break; //break out of the loop now that a cell has been placed
        }
        usleep(5000); //usleep to help the animation of the moving green cell
    }
    return; //go back to waiting for more user input
}

void killCellPos(int liveCell[][40]){ //allows the user to kill an already alive cell
    while(TRUE){
        int curx = current_xpos(), cury = current_ypos(); //custom gfx functions: get position of cursor without an event
        gfx_clear(); //clear the display
        reDrawSameState(liveCell); //re draw all of the other cells since we are clearing them for the moving red cell
        gfx_color(255,0,0); //set the color of the temp cell to red
        drawLiveCell((int)floor(cury/(HEIGHT/40)), (int)floor(curx/(WIDTH/40))); //snap a red cell to the location of the mouse on the grid
        if(gfx_event_waiting()){
            if(gfx_wait() != 1) continue; //if the user has left clicked, continue
            int xpos = gfx_xpos(); //get the position of where the mouse was when it was clicked
            int ypos = gfx_ypos();
            if(liveCell[(int)floor(ypos/(HEIGHT/40))][(int)floor(xpos/(WIDTH/40))] == FALSE) continue; //if the spot clicked on is an alive cell, continue
            liveCell[(int)floor(ypos/(HEIGHT/40))][(int)floor(xpos/(WIDTH/40))] = FALSE; //update the array so that the cell is now dead
            gfx_clear(); //clear the display
            reDrawSameState(liveCell); //draw the cells now that the cell is dead
            break;
        }
        usleep(5000); //usleep to help the animation of the moving red cell
    }
    return;
}

void reDrawSameState(int liveCell[][40]){ //draws all of the cells again without applying a tick
    int i, j;
    for(i = 0; i < 40; i++){
        for(j = 0; j < 40; j++){
            if(liveCell[i][j] == TRUE){ //if the cell is alive draw it
                gfx_color(255, 255, 255); //make it white
                drawLiveCell(i, j); //draw
            }
        }
    }
    gfx_flush(); //flush all the cells to the display
}
