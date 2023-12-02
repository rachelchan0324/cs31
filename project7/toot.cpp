// toot.cpp

// Portions you are to complete are marked with a TODO: comment.
// We've provided some incorrect return statements (so indicated) just
// to allow this skeleton program to compile and run, albeit incorrectly.
// The first thing you probably want to do is implement the trivial
// functions (marked TRIVIAL). Then get City::display going. That gives
// you more flexibility in the order you tackle the rest of the functionality.
// As you finish implementing each TODO: item, remove its TODO: comment.

#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
using namespace std;

///////////////////////////////////////////////////////////////////////////
// Manifest constants
///////////////////////////////////////////////////////////////////////////

const int MAXROWS = 20;   // max number of rows in a city
const int MAXCOLS = 30;   // max number of columns in a city
const int MAXTOOTERS = 125;   // max number of Tooters allowed

const int UP = 0;
const int DOWN = 1;
const int LEFT = 2;
const int RIGHT = 3;
const int NUMDIRS = 4;

///////////////////////////////////////////////////////////////////////////
// Type definitions
///////////////////////////////////////////////////////////////////////////

class City; // This is needed to let the compiler know that City is a
   // type name, since it's mentioned in the Tooter declaration.

class Tooter {
    public:
    // Constructor
    Tooter(City* cp, int r, int c);

    // Accessors
    int row() const;
    int col() const;

    // Mutators
    void move();

    private:
    City* m_city;
    int m_row;
    int m_col;
};

class Player {
    public:
    // Constructor
    Player(City *cp, int r, int c);

    // Accessors
    int row() const;
    int col() const;
    int age() const;
    int health() const;
    bool isPassedOut() const;

    // Mutators
    void preach();
    void move(int dir);
    void getGassed();

    private:
    City* m_city;
    int m_row;
    int m_col;
    int m_age;
    int m_health;
};

class City {
    public:
    // Constructor/destructor
    City(int nRows, int nCols);
    ~City();

    // Accessors
    int rows() const;
    int cols() const;
    Player* player() const;
    bool isPlayerAt(int r, int c) const;
    int tooterCount() const;
    int nTootersAt(int r, int c) const;
    bool determineNewPosition(int& r, int& c, int dir) const;
    void display() const;

    // Mutators
    bool addTooter(int r, int c);
    bool addPlayer(int r, int c);
    void preachToTootersAroundPlayer();
    void moveTooters();

    private:
    int m_rows;
    int m_cols;
    Player* m_player;
    Tooter* m_tooters[MAXTOOTERS];
    int m_nTooters;

    // Helper functions
    bool isInBounds(int r, int c) const;
};

class Game {
    public:
    // Constructor/destructor
    Game(int rows, int cols, int nTooters);
    ~Game();

    // Mutators
    void play();

    private:
    City* m_city;
};

///////////////////////////////////////////////////////////////////////////
// Auxiliary function declarations
///////////////////////////////////////////////////////////////////////////

int decodeDirection(char dir);
int randInt(int min, int max);
void clearScreen();

///////////////////////////////////////////////////////////////////////////
// Tooter implementation
///////////////////////////////////////////////////////////////////////////

Tooter::Tooter(City* cp, int r, int c) {
    if (cp == nullptr) {
        cout << "***** A Tooter must be created in some City!" << endl;
        exit(1);
    }
    if (r < 1 || r > cp->rows() || c < 1 || c > cp->cols()) {
        cout << "***** Tooter created with invalid coordinates (" << r << ","
        << c << ")!" << endl;
        exit(1);
    }
    m_city = cp;
    m_row = r;
    m_col = c;
}

int Tooter::row() const {
    return m_row;
}

int Tooter::col() const {
    return m_col;
}

void Tooter::move() {
    int dir = randInt(0, NUMDIRS-1); // dir is now UP, DOWN, LEFT, or RIGHT
    
    // Attempt to move in a random direction; if it can't move, don't move.
    // If the player is there, don't move.
    int ogRow = m_row;
    int ogCol = m_col;
    m_city->determineNewPosition(m_row, m_col, dir);
    if(m_city->isPlayerAt(m_row,m_col)){
        m_row = ogRow;
        m_col = ogCol;
    }
}

///////////////////////////////////////////////////////////////////////////
// Player implementations
///////////////////////////////////////////////////////////////////////////

Player::Player(City* cp, int r, int c) {
    if (cp == nullptr) {
        cout << "***** The player must be created in some City!" << endl;
        exit(1);
    }
    if (r < 1 || r > cp->rows() || c < 1 || c > cp->cols()) {
        cout << "**** Player created with invalid coordinates (" << r
        << "," << c << ")!" << endl;
        exit(1);
    }
    m_city = cp;
    m_row = r;
    m_col = c;
    m_age = 0;
    m_health = 12;
}

int Player::row() const {
    return m_row;
}

int Player::col() const
{
    return m_col;
}

int Player::age() const {
    return m_age;
}

int Player::health() const {
    return m_health;
}

bool Player::isPassedOut() const {
    if(m_health == 0)
        return true;
    return false;
}

void Player::preach() {
    m_age++;
    m_city->preachToTootersAroundPlayer();
}

void Player::move(int dir) {
    m_age++;
    
    int ogRow = m_row;
    int ogCol = m_col;
    m_city->determineNewPosition(m_row, m_col, dir);
    if(m_city->nTootersAt(m_row, m_col) > 0){
        m_row = ogRow;
        m_col = ogCol;
    }
}

void Player::getGassed() {
    m_health--;
}

///////////////////////////////////////////////////////////////////////////
// City implementations
///////////////////////////////////////////////////////////////////////////

City::City(int nRows, int nCols) {
    if (nRows <= 0 || nCols <= 0 || nRows > MAXROWS || nCols > MAXCOLS) {
        cout << "***** City created with invalid size " << nRows << " by "
        << nCols << "!" << endl;
        exit(1);
    }
    m_rows = nRows;
    m_cols = nCols;
    m_player = nullptr;
    m_nTooters = 0;
}

City::~City() {
    cerr << "Entering City destructor" << endl;
    delete m_player ;
    for(int k = 0; k < m_nTooters; k++){
        delete m_tooters[k];
    }
    cerr << "Leaving City destructor" << endl;
}

int City::rows() const {
    return m_rows;
}

int City::cols() const {
    return m_cols;
}

Player* City::player() const {
    return m_player;
}

bool City::isPlayerAt(int r, int c) const {
    if(m_player->row() == r && m_player->col() == c)
        return true;
    return false;
}

int City::tooterCount() const {
    return m_nTooters;
}

int City::nTootersAt(int r, int c) const {
    int toots = 0;
    for(int k = 0; k < m_nTooters; k++){
        if(m_tooters[k]->row() == r && m_tooters[k] ->col() == c)
            toots++;
    }
    return toots;
}

bool City::determineNewPosition(int& r, int& c, int dir) const {
    switch (dir) {
        case UP:
            if(isInBounds(r - 1, c))
               r--;
            break;
        case DOWN:
            if(isInBounds(r + 1, c))
                r++;
            break;
        case LEFT:
            if(isInBounds(c - 1, r))
                c--;
            break;
        case RIGHT:
            if(isInBounds(c + 1, r))
                c++;
            break;
        default:
            return false;
    }
    return true;
}

void City::display() const {
    // Position (row,col) in the city coordinate system is represented in
    // the array element grid[row-1][col-1]
    char grid[MAXROWS][MAXCOLS];
    int r, c;

    // Fill the grid with dots
    for (r = 0; r < rows(); r++)
        for (c = 0; c < cols(); c++)
            grid[r][c] = '.';

    // Indicate each Tooter's position
    // If it's 2 though 8, set it to '2' through '8'.
    // For 9 or more, set it to '9'.
    
    for(r = 0; r < rows(); r++){
        for(c = 0; c < cols(); c++){
            int nToots = nTootersAt(r + 1, c + 1);
            if(nToots == 1)
                grid[r][c] = 'T';
            else if(nToots >= 2 && nToots <= 8 )
                grid[r][c] = nToots + '0';
            else if(nToots > 9)
                grid[r][c] = '9';
        }
    }
    
    // Indicate player's position
    if (m_player != nullptr) {
    // Set the char to '@', unless there's also a Tooter there
    // (which should never happen), in which case set it to '*'.
        char& gridChar = grid[m_player->row()-1][m_player->col()-1];
        if (gridChar == '.')
            gridChar = '@';
        else
            gridChar = '*';
    }

    // Draw the grid
    clearScreen();
    for (r = 0; r < rows(); r++) {
        for (c = 0; c < cols(); c++)
            cout << grid[r][c];
        cout << endl;
    }
    cout << endl;

    // Write message, Tooter, and player info
    cout << "There are " << m_nTooters << " unconverted Tooters remaining." << endl;
    if (m_player == nullptr)
        cout << "There is no player." << endl;
    else {
        if (m_player->age() > 0)
            cout << "The player has lasted " << m_player->age() << " steps." << endl;
        if (m_player->isPassedOut())
            cout << "The player has passed out." << endl;
        else
            cout << "The player's health level is " << m_player->health() << endl;
    }
}

bool City::addTooter(int r, int c) {
    if (!isInBounds(r, c))
        return false;

    // Don't add a Tooter on a spot with a player
    if (m_player != nullptr && m_player->row() == r && m_player->col() == c)
        return false;

    // If there are MAXTOOTERS unconverted Tooters, return false.
    // Otherwise, dynamically allocate a new Tooter at coordinates (r,c).
    // Save the pointer to the newly allocated Tooter and return true.

    // Your function must work as specified in the preceding paragraph even
    // in this scenario (which won't occur in this game): MAXTOOTERS
    // Tooters are added, then some are converted and removed from the city,
    // then more are added.

    if(m_nTooters >= MAXTOOTERS)
        return false;
    m_tooters[m_nTooters] = new Tooter(this, r, c);
    m_nTooters++;
    return true;
}

bool City::addPlayer(int r, int c) {
    if (! isInBounds(r, c))
        return false;
    
    // Don't add a player if one already exists
    if (m_player != nullptr)
        return false;
    
    // Don't add a player on a spot with a Tooter
    if (nTootersAt(r, c) > 0)
        return false;
    
    // Dynamically allocate a new Player and add it to the city
    m_player = new Player(this, r, c);
    return true;
}

void City::preachToTootersAroundPlayer() {
    // Preach to Tooters orthogonally or diagonally adjacent to player.
    // If a Tooter is converted, destroy it and remove it from the city,
    // since we have no further need to display it or have it interact with
    // the player.

    // iterate through tooters
    for(int k = 0; k < m_nTooters; k++){
        // if a tooter position is orthogonally or diagonally adjacent
        if(abs(m_tooters[k]->row() - m_player->row()) == 1 || abs(m_tooters[k]->col() - m_player->col()) == 1){
            // 2/3 probably of deleting the tooter
            if(randInt(1,3) != 3){
                // delete the tooter and shift the
                delete m_tooters[k];
                m_nTooters--;
                for(int j = k; j < m_nTooters; j++){
                    m_tooters[k] = m_tooters[k+1];
                }
            }
        }
    }
    
    // TODO: Implement this.
}

void City::moveTooters() {
    for (int k = 0; k < m_nTooters; k++) {
        m_tooters[k]->move();
        
        int tootRow = m_tooters[k]->row();
        int tootCol = m_tooters[k]->col();
        
        int playerRow = m_player->row();
        int playerCol = m_player->col();
        
        bool orthoAdjacent = false;
        if(tootRow + 1 == playerRow && tootCol == playerCol)
            orthoAdjacent = true;
        if(tootRow - 1 == playerRow && tootCol == playerCol)
            orthoAdjacent = true;
        if(tootCol + 1 == playerCol && tootRow == playerRow)
            orthoAdjacent = true;
        if(tootCol - 1 == playerCol && tootRow == playerRow)
            orthoAdjacent = true;
            
        if(orthoAdjacent)
            m_player->getGassed();
    }
}

bool City::isInBounds(int r, int c) const {
    return (r >= 1 && r <= m_rows && c >= 1 && c <= m_cols);
}

///////////////////////////////////////////////////////////////////////////
// Game implementations
///////////////////////////////////////////////////////////////////////////

Game::Game(int rows, int cols, int nTooters) {
    if (nTooters < 0) {
        cout << "***** Cannot create Game with negative number of Tooters!" << endl;
        exit(1);
    }
    if (nTooters > MAXTOOTERS) {
        cout << "***** Trying to create Game with " << nTooters
        << " Tooters; only " << MAXTOOTERS << " are allowed!" << endl;
        exit(1);
    }
    if (rows == 1 && cols == 1 && nTooters > 0) {
        cout << "***** Cannot create Game with nowhere to place the Tooters!" << endl;
        exit(1);
    }

    // Create city
    m_city = new City(rows, cols);

    // Add a player
    int rPlayer = randInt(1, rows);
    int cPlayer = randInt(1, cols);
    m_city->addPlayer(rPlayer, cPlayer);

    // Populate with Tooters
    while (nTooters > 0) {
        int r = randInt(1, rows);
        int c = randInt(1, cols);
        // Don't put a Tooter where the player is
        if (r == rPlayer && c == cPlayer)
            continue;
        m_city->addTooter(r, c);
        nTooters--;
    }
}

Game::~Game() {
    delete m_city;
}

void Game::play() {
    m_city->display();
    Player* p = m_city->player();
    if (p == nullptr)
        return;

    while ( ! p->isPassedOut() && m_city->tooterCount() > 0) {
        cout << "Move (u/d/l/r//q): ";
        string action;
        getline(cin,action);
        if (action.size() == 0) // player preaches
            p->preach();
        else {
            switch (action[0]) {
                default: // if bad move, nobody moves
                    cout << '\a' << endl; // beep
                    continue;
                case 'q':
                    return;
                case 'u':
                case 'd':
                case 'l':
                case 'r':
                    p->move(decodeDirection(action[0]));
                    break;
            }
        }
        m_city->moveTooters();
        m_city->display();
    }
    if (p->isPassedOut())
        cout << "You lose." << endl;
    else
        cout << "You win." << endl;
}

///////////////////////////////////////////////////////////////////////////
// Auxiliary function implementations
///////////////////////////////////////////////////////////////////////////

int decodeDirection(char dir) {
    switch (dir) {
    case 'u': return UP;
    case 'd': return DOWN;
    case 'l': return LEFT;
    case 'r': return RIGHT;
    }
    return -1; // bad argument passed in!
}

// Return a random int from min to max, inclusive
int randInt(int min, int max) {
    if (max < min)
        swap(max, min);
    static random_device rdv;
    static default_random_engine generator(rdv());
    uniform_int_distribution<> distro(min, max);
    return distro(generator);
}

///////////////////////////////////////////////////////////////////////////
// main()
///////////////////////////////////////////////////////////////////////////

int main() {
    // Create a game
    // Use this instead to create a mini-game:   Game g(3, 4, 2);
    Game g(7, 8, 25);

    // Play the game
    g.play();
}

///////////////////////////////////////////////////////////////////////////
// clearScreen implementation
///////////////////////////////////////////////////////////////////////////

// DO NOT MODIFY OR REMOVE ANYTHING BETWEEN HERE AND THE END OF THE FILE!!!
// THE CODE IS SUITABLE FOR VISUAL C++, XCODE, AND g++/g31 UNDER LINUX.

// Note to Xcode users: clearScreen() will just write a newline instead
// of clearing the window if you launch your program from within Xcode.
// That's acceptable. (The Xcode output window doesn't have the capability
// of being cleared.)

#ifdef _WIN32 // Windows

#pragma warning(disable : 4005)
#include <windows.h>

void clearScreen()
{
 HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
 CONSOLE_SCREEN_BUFFER_INFO csbi;
 GetConsoleScreenBufferInfo(hConsole, &csbi);
 DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
 COORD upperLeft = { 0, 0 };
 DWORD dwCharsWritten;
 FillConsoleOutputCharacter(hConsole, TCHAR(' '), dwConSize, upperLeft,
          &dwCharsWritten);
 SetConsoleCursorPosition(hConsole, upperLeft);
}

#else // not Windows

#include <iostream>
#include <cstring>
#include <cstdlib>

void clearScreen() { // will just write a newline in an Xcode output window
    static const char* term = getenv("TERM");
    if (term == nullptr || strcmp(term, "dumb") == 0)
        cout << endl;
    else {
        static const char* ESC_SEQ = "\x1B["; // ANSI Terminal esc seq: ESC [
        cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << flush;
    }
}

#endif
