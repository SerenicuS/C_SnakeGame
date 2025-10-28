#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <stdbool.h>
#include <time.h>


struct SnakeSegment {
    int row;
    int col;
    struct SnakeSegment* next;
};

int GetLegalArraySpawnRow(){
    return rand() % 8 + 1; // 1 to 9 (safe spots)
}

int GetLegalArraySpawnColumn(){
    return rand() % 8 + 1; // 1, to 9 (safe spots)
}

bool CheckSelfCollision(struct SnakeSegment* head) {
    struct SnakeSegment* current = head->next; // Start checking from the body
    while (current != NULL) {
        if (head->row == current->row && head->col == current->col) {
            return true; // The head ran into its own body
        }
        current = current->next;
    }
    return false;
}


bool CheckSnakeCollisionToWalls(int gameSize[10][10], const int row, const int col) {
    return gameSize[row][col] == 1;
}
bool CheckSnakeCollisionToApples(int gameSize[10][10], const int row, const int col) {
    return gameSize[row][col] == 2;
}



bool CheckLegitimateAppleSpawn(int gameSize[10][10], const int row, const int col) {
    return gameSize[row][col] == 1 || gameSize[row][col] == 3 || gameSize[row][col] == 4;
}

void RenderGame(int gameSize[10][10]) {
    for ( int i = 0; i < 10; i++ ) {
        for (int j = 0; j < 10; j++) {
            printf("%d", gameSize[i][j]);


        }
        printf("\n");
    }
}

void UpdateGameBoardWithSnake(int gameSize[10][10], struct SnakeSegment* head) {
    // 1. Clear ALL old snake parts (body '3' and head '4') from the WHOLE 10x10 grid
    for (int i = 0; i < 10; i++) {       // <-- FIX: Was 5
        for (int j = 0; j < 10; j++) {   // <-- FIX: Was 5
            if (gameSize[i][j] == 3 || gameSize[i][j] == 4) // <-- FIX: Check for 3 AND 4
                gameSize[i][j] = 0;
        }
    }

    // 2. Draw the new head as '4'
    if (head != NULL) {
        gameSize[head->row][head->col] = 4;
    }

    // 3. Draw the new body (starting from the *next* segment) as '3'
    struct SnakeSegment* current = head->next; // <-- FIX: Start from head->next
    while (current != NULL) {
        gameSize[current->row][current->col] = 3;
        current = current->next;
    }
}

void SpawnApple(int gameSize[10][10]) {
    while (1) {
        int applePositionRow = GetLegalArraySpawnRow();
        int applePositionColumn = GetLegalArraySpawnColumn();

        if (!CheckLegitimateAppleSpawn(gameSize, applePositionRow, applePositionColumn)) {
            gameSize[applePositionRow][applePositionColumn] = 2;
            break;
        }
    }
}

struct SnakeSegment* CreateNode(const int row, const int col) {
    struct SnakeSegment* newPart = malloc (sizeof(struct SnakeSegment));


    if (newPart == NULL) {
        printf("Memory Allocation Error");
        exit(1);
    }

    newPart->row = row;
    newPart->col = col;
    newPart->next = NULL;
    return newPart;

}

void MoveSnake(struct SnakeSegment* head, char direction, bool *shouldGrow) {
    int prevRow = head->row;
    int prevCol = head->col;

    // Move the head first
    switch (direction) {
        case 'w':
            head->row--;
            break;
        case 's': head->row++; break;
        case 'a': head->col--; break;
        case 'd': head->col++; break;
        default: return; // invalid movement
    }

    // Now move the body
    struct SnakeSegment* current = head->next;
    struct SnakeSegment* tail = head; // Keep track of the last segment

    while (current != NULL) {
        int tempRow = current->row;
        int tempCol = current->col;

        current->row = prevRow;
        current->col = prevCol;

        prevRow = tempRow;
        prevCol = tempCol;

        tail = current;
        current = current->next;
    }


    if (*shouldGrow) {
        tail->next = CreateNode(prevRow, prevCol);
        *shouldGrow = false;
    }
}

struct SnakeSegment* GetTail(struct SnakeSegment* head) {
    struct SnakeSegment* current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    return current;
}

void GrowSnake(struct SnakeSegment* head) {
    struct SnakeSegment* tail = GetTail(head);


    struct SnakeSegment* newPart = CreateNode(tail->row, tail->col);

    tail->next = newPart; // Attach it to the end
}


void StartGameWithLinkedList(int gameSize[10][10], struct SnakeSegment* head) {

    int keyPressed = 'w';  //placeholder that the snake will always move upward for testing without user input
    bool appleExist = true;
    bool shouldGrow = false;
    int appleEaten = 0;

    while (1) {
        printf("\n");
        printf("Eaten Apples: %d\n", appleEaten);

        if (!appleExist) {
            SpawnApple(gameSize);
            appleExist = true;
        }

        keyPressed = getchar();
        getchar();

        MoveSnake(head, keyPressed, &shouldGrow);


        if (CheckSnakeCollisionToApples(gameSize, head->row, head->col)) {
            appleExist = false;
            appleEaten++;
            shouldGrow = true;
        }

        if (CheckSnakeCollisionToWalls(gameSize, head->row, head->col) || CheckSelfCollision(head)) {
            break; // Game Over
        }

        UpdateGameBoardWithSnake(gameSize, head);
        RenderGame(gameSize);
    }

    printf("Game Over :<");
}







int main(void) {
    srand(time(NULL)); // for some reason even with rand, the snake will spawn at the same position(3,3) so i add this to ensure it is pure random lol


    int userChoice = 0;
    printf("Snake Game\n");
    printf("S. Start Game\n");
    printf("Q. Quit\n");


    while (1) { // breakpoint

        userChoice = getchar();

        if (userChoice == 's' || userChoice == 'S') {
            printf("Starting....\n");
            break;
        }
        printf("Invalid Input");
        return 0;
    }

    getchar();

    int gameSize[10][10] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

    // Spawn the snake randomly lol to a legal position
    int const snakeHeadPositionRow = GetLegalArraySpawnRow();
    int const snakeHeadPositionColumn = GetLegalArraySpawnColumn();
    gameSize[snakeHeadPositionRow][snakeHeadPositionColumn] = 4;
    struct SnakeSegment* snakeHead = CreateNode(snakeHeadPositionRow, snakeHeadPositionColumn);

    SpawnApple(gameSize);
    RenderGame(gameSize);

    StartGameWithLinkedList(gameSize, snakeHead);



    return 0;
}