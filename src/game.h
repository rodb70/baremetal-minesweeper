#define MINE 9
#define gameSize (gameHeight * gameWidth)
#define gamePos OFFSET(gameY, gameX, gameWidth)
#define gamePosOld OFFSET(gameYOld, gameXOld, gameWidth)
#define START_ROW 0
#define START_COL 0
#define DENSITY 5
#define PPI 8

// Game state
enum {TITLE, NORMAL, EXPLOSION, GAMEOVER};

// Cell status
enum {NONE, FLAG, UNKNOWN, REVEALED};

// Difficulties
enum {EASY, INTERMEDIATE, HARD};

// Colors
enum {BLACK, WHITE, RED};

/* Struture representing a tile */
typedef struct {
	int value; // mines surrounding, or if it is a MINE
	int status; // flagged, etc.
	volatile int redrawBuffer1;
	volatile int redrawBuffer2;
} TILE;

extern int frame;
extern int state;
extern const int BOARD_WIDTH[3];
extern const int BROAD_HEIGHT[3];
extern const int BOARD_MINES[3];

void refresh(void);
void reset(void);
void startGame(void);
void endGame(int won);
void drawBoard(void);
void drawCursor(void);
void moveCursor(int x, int y);
void checkTile(int r, int c);
void reveal(void);
void toggleFlag(void);
void putMine(int r, int c);
void countMines(int r, int c);
uint16_t* getTileImage(TILE tile);
void drawTime(void);
void drawMenu(void);
void moveMenu(int y);
