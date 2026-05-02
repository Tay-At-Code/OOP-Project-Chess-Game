#include "Header.h"
#include <iostream>
#include <string>
using namespace std;

// making the board in center
const string PAD = "                                             ";

#define BG_LIGHT  "\033[48;2;240;217;181m"       // cream
#define BG_DARK   "\033[48;2;181;136;99m"    // brown
#define TXT_WHITE "\033[1;97m"                // bold bright white,White pieces
#define TXT_BLACK "\033[1;30m"                // bold dark,Black pieces
#define RESET     "\033[0m"

void displayBoard(Board& board, const string& message) {
    system("cls");   // clear screen before every redraw

    cout << "\n\n";
    cout << PAD << "  White: K Q R B N P     Black: k q r b n p\n\n";
    cout << PAD << "    a   b   c   d   e   f   g   h\n";
    cout << PAD << "  +---+---+---+---+---+---+---+---+\n";

    for (int r = 0; r < 8; r++) {
        cout << PAD << (8 - r) << " |";

        for (int c = 0; c < 8; c++) {
            // Pick square background colour
            bool light = (r + c) % 2 == 0;
            cout << (light ? BG_LIGHT : BG_DARK);

            Piece* p = board.getPiece(r, c);
            if (p) {
                // Colour the piece letter based on its side
                cout << (p->getColor() == 'W' ? TXT_WHITE : TXT_BLACK);
                cout << " " << p->getSymbol() << " ";
            }
            else {
                cout << "   ";   // empty square
            }

            cout << RESET << "|";
        }

        cout << " " << (8 - r) << "\n";
        cout << PAD << "  +---+---+---+---+---+---+---+---+\n";
    }

    cout << PAD << "    a   b   c   d   e   f   g   h\n\n";
    cout << PAD << "  " << message << "\n\n";
}

int main() {
    Board  board;
    string message = "White's turn.  Enter move (e.g. e2 e4):";

    while (true) {
        displayBoard(board, message);

        // Get player input
        string from, to;
        cout << PAD << "  >> ";
        cin >> from;

        if (from == "quit") { cout << "\n  Game ended.\n"; break; }

        cin >> to;

        // Basic format check
        if (from.size() != 2 || to.size() != 2) {
            message = "Bad format! Use:  e2 e4";
            continue;
        }

        char fc = from[0];  int fr = from[1] - '0';
        char tc = to[0];    int tr = to[1] - '0';

        // Range check
        if (fc < 'a' || fc>'h' || tc < 'a' || tc>'h' || fr < 1 || fr>8 || tr < 1 || tr>8) {
            message = "Out of range! Columns: a-h   Rows: 1-8";
            continue;
        }

        // convert input into index then make the move
        int boardFR = 8 - fr, boardFC = fc - 'a';
        int boardTR = 8 - tr, boardTC = tc - 'a';

        if (!board.movePiece(boardFR, boardFC, boardTR, boardTC)) {
            message = "Invalid move! Try again.";
            continue;
        }

        // Check game state after the move
        char   next = board.getTurn();
        string player = (next == 'W') ? "White" : "Black";

        if (board.isCheckmate(next)) { displayBoard(board, "CHECKMATE!  " + player + " loses!"); break; }
        else if (board.isStalemate(next)) { displayBoard(board, "STALEMATE!  It's a draw.");            break; }
        else if (board.isInCheck(next))     message = "CHECK!  " + player + "'s turn.  Enter move:";
        else                                message = player + "'s turn.  Enter move (e.g. e2 e4):";
    }

    return 0;
}
