
// Playing Tic-Tac-Toe.
//
// Two games are played on each invocation of this effect.
// One "brainless" random moves game, and one using some more "brains".
// The board changes color (green) if "brains" are enabled.
// "Brains" is just adding two checks: (1) find own winning move, (2) block others winning move.
// So not all that clever anyway, but this keeps the game a bit more interesting (and winnable).
//
// Thijs Kaper (13/8/2021).

#define TTT_BACKGROUND_COLOR   RgbColor(0, 0, 0)
#define TTT_MATRIX_COLOR_DUMB  RgbColor(20, 5, 0)
#define TTT_MATRIX_COLOR_SMART RgbColor(0, 15, 0)
#define TTT_CIRCLE_COLOR       RgbColor(20, 20, 0)
#define TTT_CROSS_COLOR        RgbColor(20, 0, 0)
#define TTT_WIN_CIRCLE_COLOR   RgbColor(0, 20, 20)
#define TTT_WIN_CROSS_COLOR    RgbColor(20, 0, 20)

#define membersof(x) (sizeof(x) / sizeof(x[0]))

// The pixels of the tic-tac-toe matrix/grid
const uint8_t ttt_matrix[] = {
  // vertical: top left to bottom left
  178, 130, 90, 58, 34, 33, 32, 52, 80, 116, 160,
  // vertical: top right to bottom right
  136, 96, 64, 40, 24, 25, 26, 46, 74, 110, 154,
  // horizontal: top left to top right
  172, 126, 88, /* 58, */ 36, 21, 22, /* 40, */ 66, 100, 142,
  // horizontal: bottom left to bottom right
  166, 120, 82, /* 52, */ 30, 29, 28, /* 46, */ 72, 106, 148
};

// Let's give the game positions a number:
//
// 0 # 1 # 2
// #########
// 3 # 4 # 5
// #########
// 6 # 7 # 8

// We need graphics data for two types (X and O) for three positions (corner = pos 0, middle = pos 1, center = pos 4).
// For the others we can rotate them (pos 0 and pos 1) around the board.

const uint8_t ttt_corner_x[] = { 234, 175, 128, 174, /* 175, */ 176 };
const uint8_t ttt_middle_x[] = { 180, 93, 62, 92, /* 93, */ 134 };
const uint8_t ttt_center_x[] = { 8, 0, 4, 6, /* 0, */ 2 };

const uint8_t ttt_corner_o[] = { 234, 176, 128, 174, 233 };
const uint8_t ttt_middle_o[] = { 180, 133, 134, 94, 62, 61, 92, 132 };
const uint8_t ttt_center_o[] = { 8, 1, 2, 3, 4, 5, 6, 7 };

// To check for winners, all possible triplets are listed here.
// Ok, we could have computed those, but this was quick and simple.
const uint8_t ttt_winning_lines[][5] = {
  // The dataset consists of 5 values: 3 board positions, a line-type to use, and a rotation factor for the line type.
  { 0, 1, 2, /* line: */ 0, 0 }, // top horizontal, no rotate.
  { 3, 4, 5, /* line: */ 1, 0 }, // middle horizontal, no rotate.
  { 6, 7, 8, /* line: */ 0, 2 }, // top horizontal, rotate 180 degrees to be at bottom.
  { 0, 3, 6, /* line: */ 0, 3 }, // top horizontal, rotate 270 degrees to be at left vertical.
  { 1, 4, 7, /* line: */ 1, 1 }, // middle horizontal, rotate 90 degrees to be middle vertical.
  { 2, 5, 8, /* line: */ 0, 1 }, // top horizontal, rotate 90 degrees to be at right vertical.
  { 0, 4, 8, /* line: */ 2, 1 }, // diagonal bottom-left to top right, rotate 90 degrees to be top-left to bottom-right.
  { 6, 4, 2, /* line: */ 2, 0 }, // diagonal bottom-left to top right, no rotate.
};

// winning lines (value 255 is not to be drawn, just filler to make all 15 elements).
const uint8_t ttt_win_lines[][15] = {
  { 175, 129, 130, 131, 132, 93, 94, 95, 96, 97, 139, 255, 255, 255, 255 }, // horizontal top line
  { 123, 85, 55, 33, 18, 7, 0, 3, 12, 25, 43, 69, 103, 255, 255 }, // horizontal middle line
  { 163, 118, 81, 52, 31, 17, 6, 0, 2, 11, 23, 40, 65, 98, 139 } // diagonal bottom-left to top-right line
};

// The main game routine.
void ticTacToe() {
  for (int nrOfPlays=2;nrOfPlays>0;nrOfPlays--) {
    // First game is brainless (random), second game is using some more brain power (spot winning move, and block others winning move).
    boolean useSomeBrains = (nrOfPlays == 1);

    // Show the board, using color depending on "brain" capacity.
    tttDrawBoard(useSomeBrains);
    handlingDelay(1000);
  
    // set up empty board
    uint8_t board[9];
    tttInitBoard(board);
  
    // choose random starting "color"/player
    boolean playerX = (random(2) == 1);

    // do some moves, max 9, bailing out when someone wins.
    for (int i=0;i<9;i++) {
      // "thinking time"
      handlingDelay(600);

      // Determine next move, for given player and brain capactity.
      int movePosition = tttFindMoveToDo(board, playerX, useSomeBrains);
      
      // Check for winning the game.
      const uint8_t * win = tttCheckForWinningMove(board, playerX, movePosition);

      // Draw the move, and memorize it.
      tttDrawMove(movePosition, playerX, false);
      board[movePosition] = playerX ? 'X':'O';

      // If the user did win, show it! (and stop the game).
      if (win!=0) {
        for(int w=0;w<3;w++) {
          int pos = win[w];
          tttDrawMove(pos, playerX, true);
        }
        tttDrawWinningLine(win[3], win[4], playerX);
        break;
      }
  
      // switch turn to other player
      playerX = !playerX;
    }
  
    handlingDelay(3000);
    
    // Wipe out the board - from outer to inner.
    tttWipeBoardGameOver();
  }

  handlingDelay(500);
}

// Empty the board (memory version, not display).
void tttInitBoard(uint8_t board[]) {
    for (int i=0;i<9;i++) {
      board[i]=0;
    }
}

// wipe out the board at end of game.
void tttWipeBoardGameOver() {
  for (int ring=RINGS-1;ring>=0;ring--) {
    for(int i=startLEDs[ring];i<startLEDs[ring+1];i++) {
      strip.SetPixelColor(i, TTT_BACKGROUND_COLOR);
    }
    strip.Show();
    handlingDelay(100);
  }
}

// Check for the given move, if it would be a winning move for the given player.
// If it is a winning move, return array pointer to the 3 winning squares. If not, return 0 pointer.
const uint8_t * tttCheckForWinningMove(uint8_t board[], boolean playerX, int movePosition) {
  // Check if position already taken, if so, not a winning move (or cheating if you would do that move).
  if (board[movePosition]!=0) {
    return 0;
  }

  uint8_t player = playerX ? 'X':'O';

  // Go through the ttt_winning_lines array, to check the necessary game positions.
  for(int i=0;i<membersof(ttt_winning_lines);i++) {
    int matchCount = 0;
    for(int j=0;j<3;j++) {
      int checkPos = ttt_winning_lines[i][j];
      if (checkPos == movePosition || board[checkPos] == player) {
        matchCount++;
      }
    }
    if(matchCount==3) {
      // We have a possible winner!
      return ttt_winning_lines[i];
    }
  }

  // No winner.
  return 0;
}

// Determine next move for given player. With or withour brains.
int tttFindMoveToDo(uint8_t board[], boolean playerX, boolean useSomeBrains) {
  if (!useSomeBrains) {
    // No brains, just do random moves.
    return tttFindRandomFreeSpotOnBoard(board);
  }

  // Brute force, check all positions for possible own winning move.
  // Note: What's less optimal, is that we always look at same spots in the same order.
  // This could lead sometimes to similar moves. Would be nice to randomize this check order maybe.
  for (int i=0;i<9;i++) {
    const uint8_t * myWin = tttCheckForWinningMove(board, playerX, i);
    if (myWin!=0) {
      return i;
    }
  }

  // Brute force, check all positions for other playes move to block.
  // See note in for-loop above on check order.
  for (int i=0;i<9;i++) {
    const uint8_t * otherWin = tttCheckForWinningMove(board, !playerX, i);
    if (otherWin!=0) {
      return i;
    }
  }

  // No win/defence? Then random move.
  // Note: this part could be made even more clever, but let's not do that to keep the game winnable/interesting.
  // More clever meaning to do strategic moves, instead of random ones.
  return tttFindRandomFreeSpotOnBoard(board);
}

// Find a random free spot on the board.
// Note: board spots can have three values: 0 = free, 'X' or 'O' is used by that player.
int tttFindRandomFreeSpotOnBoard(uint8_t board[]) {
  // Find number of free slots.
  int freeCount = 0;
  for (int i=0;i<9;i++) {
    if(board[i]==0) {
      freeCount++;
    }
  }

  // Pick a random one.
  int slot = random(freeCount);

  // Find the slot in on the board.
  int pos;
  for (int i=0;i<9;i++) {
    if(board[i]==0) {
      if(slot--==0) {
        pos = i;
        break;
      }
    }
  }
  return pos;
}

// Show us the board. Draw it line by line to have some movement, like someone is drawing it by hand.
// The "smart" flag chooses the board (line) color to use.
void tttDrawBoard(boolean smart) {
  strip.ClearTo(TTT_BACKGROUND_COLOR);
  RgbColor color = smart ? TTT_MATRIX_COLOR_SMART : TTT_MATRIX_COLOR_DUMB;
  
  // Draw matrix, show pixel by pixel.
  for (int i=0; i<sizeof(ttt_matrix);i++) {
    strip.SetPixelColor(ttt_matrix[i], color);
    strip.Show();
    handlingDelay(50);
  }
}

// Draw an X or O on the board.
// movePosition = 0..8, playerX: TRUE = X, FALSE = O
// The "winner" flag indicates to use alternate colors.
void tttDrawMove(int movePosition, boolean playerX, boolean winner) {

  RgbColor color = playerX ? TTT_CROSS_COLOR : TTT_CIRCLE_COLOR;
  if (winner) {
    color = playerX ? TTT_WIN_CROSS_COLOR : TTT_WIN_CIRCLE_COLOR;
  }
  
  if (movePosition == 4) {
    tttDrawMoveCenter(playerX, color);
    return;  
  }

  int rotate;
  boolean corner;
  switch(movePosition) {
    case 0: tttDrawMoveOther(playerX, color, 0, true); break;
    case 1: tttDrawMoveOther(playerX, color, 0, false); break;
    case 2: tttDrawMoveOther(playerX, color, 1, true); break;

    case 3: tttDrawMoveOther(playerX, color, 3, false); break;
    // case 4 = center
    case 5: tttDrawMoveOther(playerX, color, 1, false); break;
    
    case 6: tttDrawMoveOther(playerX, color, 3, true); break;
    case 7: tttDrawMoveOther(playerX, color, 2, false); break;
    case 8: tttDrawMoveOther(playerX, color, 2, true); break;
    default: break;
  }
}

// Draw X or O in center position.
void tttDrawMoveCenter(boolean playerX, RgbColor color) {
  int len = playerX ? sizeof(ttt_center_x) : sizeof(ttt_center_o);
  for (int i=0; i<len;i++) {
    strip.SetPixelColor(playerX ? ttt_center_x[i] : ttt_center_o[i], color);
    strip.Show();
    handlingDelay(50);
  }
}

// Draw X or O in non-center position. For given rotation, and corner or middle pattern.
void tttDrawMoveOther(boolean playerX, RgbColor color, int rotate, boolean corner) {
  int len;
  if (corner) {
    len = playerX ? sizeof(ttt_corner_x) : sizeof(ttt_corner_o);
  } else {
    len = playerX ? sizeof(ttt_middle_x) : sizeof(ttt_middle_o);
  }
  for (int i=0; i<len;i++) {
    int pos;
    if (corner) {
      pos = playerX ? ttt_corner_x[i] : ttt_corner_o[i];
    } else {
      pos = playerX ? ttt_middle_x[i] : ttt_middle_o[i];
    }

    drawRotatedPixel(pos, rotate, color);
    strip.Show();
    handlingDelay(50);
  }
}

// Draw rotated pixel.
// pos = original led pixel position in strip.
// rotateQuadrant = 0..3 to rotate in steps of 90 degrees clockwise.
// color = color pixel to draw.
void drawRotatedPixel(int pos, int rotateQuadrant, RgbColor color) {

  // findRingForPos is currently in effects-maze file, but could be moved to a general utility file (as could drawRotatedPixel).
  int ring = findRingForPos(pos);
  
  int lastPos = startLEDs[ring+1]-1;
  int ringSize = ringSizes[ring];
  int shift = ringSize / 4 * rotateQuadrant;

  // Handle rotation
  pos = pos + shift;
  if (pos > lastPos) {
    pos = pos - ringSize;
  }

  strip.SetPixelColor(pos, color);
}

// Draw a winning line. Use one of 3 types, and rotate into position.
void tttDrawWinningLine(int type, int rotation, boolean playerX) {
  RgbColor color = playerX ? TTT_WIN_CROSS_COLOR : TTT_WIN_CIRCLE_COLOR;
  for(int i=0;i<sizeof(ttt_win_lines[0]);i++) {
    int pos = ttt_win_lines[type][i];
    drawRotatedPixel(pos, rotation, color);
    strip.Show();
    handlingDelay(50);
  }
}
