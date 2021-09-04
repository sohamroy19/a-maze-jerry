/**
 * Basic Micromouse Solver for 16 x 16 square grids, where the mouse starts from the
 * lower left corner while facing northwards.
 *
 * Competition  :   Shaastra 2021 @ IIT-M
 * Team Name    :   a-MAZE!
 * Date         :   23-02-2021
 */

#include "API.h"
#include <cmath>
#include <iostream>
#include <string>

#define loopCost 20
#define travelCost 5
#define branchReward 5

int score[16][16];      // scores every square, initialised to 0
int history[16][16][3]; // stores last 3 steps of each point

int x, y, facing;  // coordinates & direction faced
int moveCount = 1; // total number of moves made till now
bool inDeadEnd;    // checks if we are inside a deadend

void setWall(int dir, char relativeDir); // sets wall
void think();                            // movement logic (greed)
void move(char relativeDir);             // to move f, l, r

/* this is the main method */
int main(int argc, char *argv[]) {
    std::cerr << "Running..." << std::endl;
    API::setColor(0, 0, 'G');
    API::setText(0, 0, "abc");

    for (int i = 0; i < 16; i++) { // initialise score array and display it
        for (int j = 0; j < 16; j++) {
            score[i][j] = 128 - (abs(2 * i - 15) + abs(2 * j - 15)) / 2;
            API::setText(i, j, std::to_string(score[i][j]));
        }
    }

    while (x < 7 || x > 8 || y < 7 || y > 8) think(); // think() while not yet in central square

    return 0;
}

/* sets the wall for the cell and adjacent cell */
void setWall(int dir, char relativeDir) {
    if (relativeDir == 'f')
        ; // do nothing
    else if (relativeDir == 'l')
        dir--; // rotate anticlockwise
    else if (relativeDir == 'r')
        dir++; // rotate clockwise

    dir = (dir + 4) % 4; // -1 --> 3 and 4 --> 0

    char absoluteDir = (dir == 0) ? 'n' : (dir == 1) ? 'e'
                                      : (dir == 2)   ? 's'
                                                     : 'w';
    API::setWall(x, y, absoluteDir);
}

/* the main logic used for making decisions at squares */
void think() {
    bool wallFront = API::wallFront(),
         wallLeft = API::wallLeft(),
         wallRight = API::wallRight();
    if (wallFront) setWall(facing, 'f');
    if (wallLeft) setWall(facing, 'l');
    if (wallRight) setWall(facing, 'r');

    int wallCount = wallFront + wallRight + wallLeft;

    history[x][y][0] = history[x][y][1]; // discard first value
    history[x][y][1] = history[x][y][2]; // and add current move
    history[x][y][2] = moveCount;        // to end of history

    // if the last 3 times we reached a square were at regular intervals, we are looping
    if (history[x][y][1] - history[x][y][0] == history[x][y][2] - history[x][y][1]) {
        std::cerr << "looping" << std::endl;
        score[x][y] -= loopCost;
        API::setText(x, y, std::to_string(score[x][y]));
    }

    if (wallCount == 3) { // dead end
        std::cerr << "Help :(" << std::endl;
        inDeadEnd = true; // initiate protocol
        move('b');
    } else if (wallCount == 2) { // only one way to go
        if (!wallFront) {
            move('f');
        } else if (!wallLeft) {
            move('l');
        } else {
            move('r');
        }
    } else if (wallCount < 2) {
        inDeadEnd = false; // no longer in dead end if we are thinking

        int dir, u, v;
        int scoreF = -16383, scoreR = -16383, scoreL = -16383;
        if (!wallFront) {
            dir = facing; // dir is straight ahead
            u = (dir == 1) ? x + 1 : (dir == 3) ? x - 1
                                                : x; // take a step
            v = (dir == 0) ? y + 1 : (dir == 2) ? y - 1
                                                : y; // forwards
            scoreF = score[u][v];
        }
        if (!wallRight) {
            dir = (facing + 1) % 4; // dir is right
            u = (dir == 1) ? x + 1 : (dir == 3) ? x - 1
                                                : x; // take a step
            v = (dir == 0) ? y + 1 : (dir == 2) ? y - 1
                                                : y; // rightwards
            scoreR = score[u][v];
        }
        if (!wallLeft) {
            dir = (facing + 3) % 4; // dir is left
            u = (dir == 1) ? x + 1 : (dir == 3) ? x - 1
                                                : x; // take a step
            v = (dir == 0) ? y + 1 : (dir == 2) ? y - 1
                                                : y; // leftwards
            scoreL = score[u][v];
        }

        if (history[x][y][0] == 0 && history[x][y][1] == 0) {
            score[x][y] += branchReward * (2 - wallCount);
            API::setText(x, y, std::to_string(score[x][y]));
        }

        if (scoreF > -8191 && scoreF >= scoreR && scoreF >= scoreL) {
            std::cerr << "go ahead" << std::endl;
            move('f');
        } else if (scoreR > -8191 && scoreR >= scoreL) {
            std::cerr << "take a right" << std::endl;
            move('r');
        } else if (scoreL > -8191) {
            std::cerr << "take a left" << std::endl;
            move('l');
        }

        else if (scoreF > -8191) { // just move somewhere if good moves not allowed
            std::cerr << "go ahead" << std::endl;
            move('f');
        } else if (scoreL > -8191) {
            std::cerr << "take a left" << std::endl;
            move('l');
        } else if (scoreR > -8191) {
            std::cerr << "take a right" << std::endl;
            move('r');
        } else {
            inDeadEnd = true; // no other ways left to go
            move('b');        // only way left to go
        }
    }
}

/* moves jerry in the direction of relativeDir */
/* relativeDir is f/l/r/b for front/left/right/back */
void move(char relativeDir) {
    score[x][y] -= travelCost;
    API::setText(x, y, std::to_string(score[x][y]));
    moveCount++;

    if (inDeadEnd) {
        score[x][y] = -16383; // mark as dead end
        API::setText(x, y, "X");
    }

    if (relativeDir == 'b') { // turn around
        API::turnRight();
        API::turnRight();
        facing = (facing + 2) % 4;
    } else if (relativeDir == 'r') { // turn right
        API::turnRight();
        facing = (facing + 1) % 4;
    } else if (relativeDir == 'l') { // turn left
        API::turnLeft();
        facing = (facing + 3) % 4;
    }

    API::moveForward(); // go ahead,
    x = (facing == 1) ? x + 1 : (facing == 3) ? x - 1
                                              : x; // adjust the
    y = (facing == 0) ? y + 1 : (facing == 2) ? y - 1
                                              : y; // coordinates
}
