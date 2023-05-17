#include <stdio.h>
#include <stdbool.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>


void printBoard(char board[][26], int n) {
      char c;
  int i = 0;
  printf("  ");
    for (c = 'a';i < n; i++) {
        printf("%c", c);
        c++;
    }
  printf("\n");
  c = 'a';
  for (int row = 0; row < n; row++) {
    for (int col = 0; col < n; col ++) {
      if (col == 0) {
        printf("%c ", c);
        c++;
      }
      printf("%c", board[row][col]);
    
    }
    printf("\n");
  }
}

bool positionInBounds(int n, int row, int col) {
     return (row < n && row > -1 && col > -1 && col < n);
}

bool checkLegalInDirection(char board[][26], int n, int row, int col,
                           char colour, int deltaRow, int deltaCol) {
    
    int checkRow = row + deltaRow;
    int checkCol = col + deltaCol;

    if (board[row][col] != 'U' || !(positionInBounds(n, checkRow, checkCol))){
        return false;
    }
    
    if (!positionInBounds(n, row, col)) {
      return false;
    }

    char oppColor = 'W';

    if (colour == 'W') {
        oppColor = 'B';
    }

    

    if (board[checkRow][checkCol] != oppColor) {
        return false;
    }

    while (true) {
        checkRow += deltaRow;
        checkCol += deltaCol;

        if (!(positionInBounds(n, checkRow, checkCol))) {
            return false;
        }

        if (board[checkRow][checkCol] == 'U') {
            return false;
        }

        if (board[checkRow][checkCol] == colour) {
            return true;
        }
    }
    


}

bool isLegal(char board[26][26], int n, char turn, int row, int col) {
if (!positionInBounds(n, row, col) || board[row][col] != 'U') {
    return false;
  }

  for (int i = -1; i < 2; i++) {
    for (int j = -1; j <2; j++) {
      if (checkLegalInDirection(board, n, row, col, turn, i, j)) {
        return true;
      }
    }
  }
  return false;
}

int calcScore(char board[26][26], int n, char turn) {
  int score = 0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (board[i][j] == turn) {
        score++;
      }
    }
  }
  return score;
}


int movesAvailable(char board[26][26], int n,char colour) {
  int count = 0;

  for (int row = 0; row < n; row++) {
    for (int col = 0; col < n; col++) {
      if (board[row][col] == 'U') {
        for (int i = -1; i < 2; i++) {
          for (int j = -1; j < 2; j++) {
            if (i == 0 && j ==0) {
              continue;
            }

             if (checkLegalInDirection(board, n, row, col, colour, i, j)) {
                  count++;
                  break;
              }

        }
        if (count > 0) {
            break;
        }
      }
      }
    }
  }
  return count;

}


void updateBoard(char board[26][26], int n, char turn, int row, int col) {
  for (int i = -1; i < 2; i++) {
    for (int j = -1; j < 2; j++) {

      if (checkLegalInDirection(board, n, row, col, turn, i, j)) {
        int r = row + i;
        int c = col + j;
        while (positionInBounds(n, r, c) && board[r][c] != turn && board[r][c] != 'U') {
          board[r][c] = turn;
          r += i;
          c += j;
        }
      }

    }
  }
  board[row][col] = turn;
}

int hypotheticalScore(char board[26][26], int n, char turn, int row, int col) {
  char opp = 'W';

  if (turn == 'W') {
      opp = 'B';
  }
  
  int opMovesDiff;

  if (board[row][col] != 'U') {
    return 0;
  }

  if (!isLegal(board, n, turn, row, col)) {
    return 0;
  }

  int score = 1; 
  int already = calcScore(board, n, turn);
  char hypBoard[26][26];
  for (int a =0; a < 26; a++) {
    for (int b = 0; b < 26; b++) {
        hypBoard[a][b] = board[a][b];
    }
  } 

  updateBoard(hypBoard, n, turn, row, col);
  score = calcScore(hypBoard, n, turn)- already-1;
  opMovesDiff = movesAvailable(board, n, opp) - movesAvailable(hypBoard, n, opp);
  if ((row == 0 && col == 0) || (row == 0 && col == n-1) || (row == n-1 && col == 0) || (row == n-1 && col == n-1) 
  && isLegal(board, n, turn, row, col)) {
          score += 2;
      }
  
  
  if (opMovesDiff > 2) {
    score += 1;
  }

    if (opMovesDiff > 4) {
    score += 2;
  }



  if (movesAvailable(board,n, turn) > movesAvailable(hypBoard, n, turn) + 2 && turn == 'B') {
    score -= 1;
  }

    if (movesAvailable(board,n, turn) > movesAvailable(hypBoard, n, turn) + 3 && turn == 'W') {
    score -= 1;
  }

  if (movesAvailable(board,n, turn)+2 < movesAvailable(hypBoard, n, turn) && turn == 'B') {
    score += 1;
  }

    if (movesAvailable(board,n, turn)+4 < movesAvailable(hypBoard, n, turn) && turn == 'W') {
    score += 1;
  }


  return score;
}



int makeMove(char board[26][26], int n, char turn, int *row, int *col) {
    int score = 0, maxScore = 0;
    for (int i = 0; i < n; i ++) {
        for (int j = 0; j < n; j++) {
            score = hypotheticalScore(board,n, turn, i, j);

            if (score > maxScore) {
                maxScore = score;
                *row = i;
                *col = j;
            }
        }
    }

    return maxScore;
}



int main(void) {
  int n;
  char comp = 'W', person = 'B';
  bool compTurn = false;
  printf("Enter the board dimension: ");
  scanf("%d", &n);
  printf("Computer plays (B/W): ");
  scanf(" %c", &comp);

  if (comp == 'B') {
    person = 'W';
    compTurn = true;
  }

  char board[26][26];
  char c;
  int i = 0;
  int middle = n/2;
  for (int row = 0; row < n; row ++) {
      for (int col = 0; col < n; col++) {
        if ((row == (middle -1) && col == (middle -1)) || (row == middle && col == middle)  ) {
            board[row][col] = 'W';
        }

        else if ((row == (middle -1) && col == middle) || (row == middle && col == (middle-1))) {
            board[row][col] = 'B';
        }

        else {
          board[row][col] = 'U';
        }
      }
  }
  printBoard(board, n);

  
  int compMoves, personMoves;
  compMoves = movesAvailable(board, n, comp);
  personMoves = movesAvailable(board, n, person);

  int moveRow, moveCol, max;
  char playerRow, playerCol;

  bool playerlegal = true;

  do {
    if (compMoves <= 0 && compTurn) {
      compTurn = false;
      printf("%c player has no valid move.\n", comp);
    }

    if (personMoves <= 0 && !compTurn) {
      compTurn = true;
      printf("%c player has no valid move.\n", person);
    }

    if (compTurn) {
      max = makeMove(board, n, comp, &moveRow, &moveCol);
      printf("Computer places %c at %c%c.\n", comp, moveRow+'a', moveCol+'a');
      updateBoard(board, n, comp, moveRow, moveCol);
      printBoard(board, n);
      compTurn = false;
    }

    else {
      printf("Enter move for colour %c (RowCol): ", person);
        char input[3];
        scanf("%s", input);
        moveRow = input[0] - 'a';
        moveCol = input[1] - 'a';
      playerlegal = isLegal(board, n, person, moveRow, moveCol);
      compTurn = true;
      if (!playerlegal) {
        printf("Invalid move.\n");
        printf("%c player wins.", comp);
        break;
      }

      else {
        updateBoard(board, n, person, moveRow, moveCol);
        printBoard(board, n);
      }

  
      
    }

    compMoves = movesAvailable(board, n, comp);
    personMoves = movesAvailable(board, n, person);



  }while ((playerlegal) && (compMoves > 0 || personMoves > 0));

  if (playerlegal) {
    int scoreW = calcScore(board, n, 'W');
    int scoreB = calcScore(board, n, 'B');

    if (scoreW == scoreB) {
      printf('Draw!');
    }

    else if (scoreB > scoreW) {
      printf("B player wins.");
    }

    else {
      printf("W player wins.");
    }
  }

  return 0;
}
