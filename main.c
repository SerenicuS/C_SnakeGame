#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>



struct SnakeSegment {
    int row;
    int col;
    struct SnakeSegment* next;
};

int GetLegalArraySpawnRow(){
    return rand() % 3 + 1; // 1, 2, or 3 (safe spots)
}

int GetLegalArraySpawnColumn(){
    return rand() % 3 + 1; // 1, 2, or 3 (safe spots)
}




bool CheckSnakeCollisionToWalls(int gameSize[5][5], int row, int col) {
    return gameSize[row][col] == 1;
}

bool CheckSnakeCollisionToApples(int gameSize[5][5], int row, int col) {
    return gameSize[row][col] == 2;
}



bool CheckAppleCollision(int gameSize[5][5], int row, int col) {
    return gameSize[row][col] == 1 || gameSize[row][col] == 2;
}

void RenderGame(int gameSize[5][5]) {
    for ( int i = 0; i < 5; i++ ) {
        for (int j = 0; j < 5; j++) {
            printf("%d", gameSize[i][j]);


        }
        printf("\n");
    }
}

void SpawnApple(int gameSize[5][5]) {
    while (1) {
        int applePositionRow = GetLegalArraySpawnRow();
        int applePositionColumn = GetLegalArraySpawnColumn();

        if (!CheckAppleCollision(gameSize, applePositionRow, applePositionColumn)) {
            gameSize[applePositionRow][applePositionColumn] = 2;
            break;
        }
    }
}




void StartGame(int gameSize[5][5], int snakeHeadPositionRow, int snakeHeadPositionColumn) {
    bool gameOver = false;
    int keyPressed = 'w';  //placeholder that the snake will always move upward for testing without user input
    bool appleExist = false;
    while (!gameOver) {
        printf("\n");

        if (!appleExist) { //only spawn an apple if there is no apple left
            SpawnApple(gameSize); // Spawn the apple
            appleExist = true;
        }


        RenderGame(gameSize);

        //usleep(1000000);

        keyPressed = getch(); // update the direction
        getch(); // idk but it prevents game from rendering double array
        switch (keyPressed) {
            case 'w':
                if (CheckSnakeCollisionToWalls(gameSize, snakeHeadPositionRow - 1, snakeHeadPositionColumn) == 1) {
                    gameOver = true;
                    break;
                }
                if (CheckSnakeCollisionToApples(gameSize, snakeHeadPositionRow - 1, snakeHeadPositionColumn)) {
                    appleExist = false;
                }
                gameSize[snakeHeadPositionRow][snakeHeadPositionColumn] = 0;
                snakeHeadPositionRow--;
                gameSize[snakeHeadPositionRow][snakeHeadPositionColumn] = 3;

                break;
            case 'a':
                if (CheckSnakeCollisionToWalls(gameSize, snakeHeadPositionRow, snakeHeadPositionColumn - 1)) {
                    gameOver = true;
                    break;
                }
                if (CheckSnakeCollisionToApples(gameSize, snakeHeadPositionRow, snakeHeadPositionColumn - 1)) {
                    appleExist = false;
                }
                gameSize[snakeHeadPositionRow][snakeHeadPositionColumn] = 0;
                snakeHeadPositionColumn--;
                gameSize[snakeHeadPositionRow][snakeHeadPositionColumn] = 3;

                break;
            case 'd':
                if (CheckSnakeCollisionToWalls(gameSize, snakeHeadPositionRow, snakeHeadPositionColumn + 1)) {
                    gameOver = true;
                    break;
                }
                if (CheckSnakeCollisionToApples(gameSize, snakeHeadPositionRow, snakeHeadPositionColumn + 1)) {
                    appleExist = false;
                }
                gameSize[snakeHeadPositionRow][snakeHeadPositionColumn] = 0;
                snakeHeadPositionColumn++;
                gameSize[snakeHeadPositionRow][snakeHeadPositionColumn] = 3;

                break;
            case 's':
                if (CheckSnakeCollisionToWalls(gameSize, snakeHeadPositionRow + 1, snakeHeadPositionColumn)) {
                    gameOver = true;
                    break;
                }
                if (CheckSnakeCollisionToApples(gameSize, snakeHeadPositionRow + 1, snakeHeadPositionColumn)) {
                    appleExist = false;
                }
                gameSize[snakeHeadPositionRow][snakeHeadPositionColumn] = 0;
                snakeHeadPositionRow++;
                gameSize[snakeHeadPositionRow][snakeHeadPositionColumn] = 3;

                break;


        }

        if (gameOver) break;
    }
    printf("Game OVER");

}




int main(void) {
    srand(time(NULL)); // for some reason even with rand, the snake will spawn at the same position(3,3) so i add this to ensure it is pure random lol
    int userChoice = 0;
    bool gameStart = false;
    printf("SSSNAKEEE Game\n");
    printf("S. Start Game\n");
    printf("Q. Quit\n");


    while (!gameStart) {

        userChoice = getch();

        switch (userChoice) {
            case 'S':
                printf("Starting....");
                gameStart = true;
                break;
            case 'Q':
                printf("Quitting....");
                return 0;
            default:
                return 0;
        }

    }

    getch();

    int gameSize[5][5] = {
        {1, 1, 1, 1, 1},
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {1, 1, 1, 1, 1}

    };

    // Spawn the snake randomly lol to a legal position
    int snakeHeadPositionRow = GetLegalArraySpawnRow();
    int snakeHeadPositionColumn = GetLegalArraySpawnColumn();
    gameSize[snakeHeadPositionRow][snakeHeadPositionColumn] = 3;


    StartGame(gameSize, snakeHeadPositionRow, snakeHeadPositionColumn); // start the game

    return 0;
}