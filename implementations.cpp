#include "Header.h"

static int absVal(int x) { return x < 0 ? -x : x; }

bool MoveValidator::isStraightClear(int fr, int fc, int tr, int tc, Piece* b[8][8])
{
    if (fr == tr)
    {
        int step = (tc > fc) ? 1 : -1;
        for (int c = fc + step; c != tc; c += step)
            if (b[fr][c]) return false;
    }
    else
    {
        int step = (tr > fr) ? 1 : -1;
        for (int r = fr + step; r != tr; r += step)
            if (b[r][fc]) return false;
    }
    return true;
}

bool MoveValidator::isDiagonalClear(int fr, int fc, int tr, int tc, Piece* b[8][8])
{
    int rStep = (tr > fr) ? 1 : -1;
    int cStep = (tc > fc) ? 1 : -1;
    int r = fr + rStep, c = fc + cStep;
    while (r != tr && c != tc)
    {
        if (b[r][c]) return false;
        r += rStep; c += cStep;
    }
    return true;
}

// ── Piece base ────────────────────────────────────────────────────────────────

Piece::Piece(char color, char symbol) : color(color), symbol(symbol) {}

char Piece::getColor()  const { return color; }
char Piece::getSymbol() const { return symbol; }

string Piece::toString() const                                            
{
    string side = (color == 'W') ? "White" : "Black";
    return side + " Piece";   // fallback; each subclass overrides this
}

// ── Pawn ─────────────────────────────────────────────────────────────────────

Pawn::Pawn(char c) : Piece(c, c == 'W' ? 'P' : 'p') {}

bool Pawn::isValidMove(int fr, int fc, int tr, int tc, Piece* b[8][8])
{
    int dir = color == 'W' ? -1 : 1;
    int start = color == 'W' ? 6 : 1;
    if (tc == fc && tr == fr + dir && !b[tr][tc]) return true;
    if (tc == fc && fr == start && tr == fr + 2 * dir && !b[fr + dir][fc] && !b[tr][tc]) return true;
    if (absVal(tc - fc) == 1 && tr == fr + dir && b[tr][tc] && b[tr][tc]->getColor() != color) return true;
    return false;
}

string Pawn::toString() const                                             
{
    return (color == 'W' ? "White" : "Black") + string(" Pawn");
}

// ── Rook ─────────────────────────────────────────────────────────────────────

Rook::Rook(char c) : Piece(c, c == 'W' ? 'R' : 'r') {}

bool Rook::isValidMove(int fr, int fc, int tr, int tc, Piece* b[8][8])
{
    if (fr != tr && fc != tc) return false;
    if (b[tr][tc] && b[tr][tc]->getColor() == color) return false;
    if (!MoveValidator::isStraightClear(fr, fc, tr, tc, b)) return false;
    return true;
}

string Rook::toString() const                                               
{
    return (color == 'W' ? "White" : "Black") + string(" Rook");
}

// ── Knight ───────────────────────────────────────────────────────────────────

Knight::Knight(char c) : Piece(c, c == 'W' ? 'N' : 'n') {}

bool Knight::isValidMove(int fr, int fc, int tr, int tc, Piece* b[8][8])
{
    int dr = absVal(tr - fr), dc = absVal(tc - fc);
    if (!((dr == 2 && dc == 1) || (dr == 1 && dc == 2))) return false;
    if (b[tr][tc] && b[tr][tc]->getColor() == color) return false;
    return true;
}

string Knight::toString() const                                              
{
    return (color == 'W' ? "White" : "Black") + string(" Knight");
}

// ── Bishop ───────────────────────────────────────────────────────────────────

Bishop::Bishop(char c) : Piece(c, c == 'W' ? 'B' : 'b') {}

bool Bishop::isValidMove(int fr, int fc, int tr, int tc, Piece* b[8][8])
{
    int dr = absVal(tr - fr), dc = absVal(tc - fc);
    if (dr != dc || dr == 0) return false;
    if (b[tr][tc] && b[tr][tc]->getColor() == color) return false;
    if (!MoveValidator::isDiagonalClear(fr, fc, tr, tc, b)) return false;
    return true;
}

string Bishop::toString() const                                            
{
    return (color == 'W' ? "White" : "Black") + string(" Bishop");
}

// ── Queen ────────────────────────────────────────────────────────────────────

Queen::Queen(char c) : Piece(c, c == 'W' ? 'Q' : 'q') {}

bool Queen::isValidMove(int fr, int fc, int tr, int tc, Piece* b[8][8])
{
    int dr = absVal(tr - fr), dc = absVal(tc - fc);
    if (fr == tr || fc == tc)
    {
        if (b[tr][tc] && b[tr][tc]->getColor() == color) return false;
        if (!MoveValidator::isStraightClear(fr, fc, tr, tc, b)) return false;
        return true;
    }
    if (dr == dc)
    {
        if (b[tr][tc] && b[tr][tc]->getColor() == color) return false;
        if (!MoveValidator::isDiagonalClear(fr, fc, tr, tc, b)) return false;
        return true;
    }
    return false;
}

string Queen::toString() const                                               
{
    return (color == 'W' ? "White" : "Black") + string(" Queen");
}

// ── King ─────────────────────────────────────────────────────────────────────

King::King(char c) : Piece(c, c == 'W' ? 'K' : 'k') {}

bool King::isValidMove(int fr, int fc, int tr, int tc, Piece* b[8][8])
{
    int dr = absVal(tr - fr), dc = absVal(tc - fc);
    if (dr > 1 || dc > 1 || (dr == 0 && dc == 0)) return false;
    if (b[tr][tc] && b[tr][tc]->getColor() == color) return false;
    return true;
}

string King::toString() const                                                
{
    return (color == 'W' ? "White" : "Black") + string(" King");
}

// ── Board ────────────────────────────────────────────────────────────────────

Board::Board()
{
    for (int r = 0; r < 8; r++) for (int c = 0; c < 8; c++) grid[r][c] = nullptr;
    turn = 'W';
    setup();
}

Board::~Board()
{
    for (int r = 0; r < 8; r++) for (int c = 0; c < 8; c++) delete grid[r][c];
}

void Board::setup()
{
    grid[0][0] = new Rook('B');   grid[0][7] = new Rook('B');
    grid[0][1] = new Knight('B'); grid[0][6] = new Knight('B');
    grid[0][2] = new Bishop('B'); grid[0][5] = new Bishop('B');
    grid[0][3] = new Queen('B');  grid[0][4] = new King('B');
    for (int c = 0; c < 8; c++) grid[1][c] = new Pawn('B');

    grid[7][0] = new Rook('W');   grid[7][7] = new Rook('W');
    grid[7][1] = new Knight('W'); grid[7][6] = new Knight('W');
    grid[7][2] = new Bishop('W'); grid[7][5] = new Bishop('W');
    grid[7][3] = new Queen('W');  grid[7][4] = new King('W');
    for (int c = 0; c < 8; c++) grid[6][c] = new Pawn('W');
}

Piece* Board::getPiece(int r, int c) const
{
    if (r < 0 || r > 7 || c < 0 || c > 7) return nullptr;
    return grid[r][c];
}

bool Board::isInCheck(char color)
{
    char ksym = color == 'W' ? 'K' : 'k';
    char enemy = color == 'W' ? 'B' : 'W';
    int  kr = -1, kc = -1;
    for (int r = 0; r < 8; r++) for (int c = 0; c < 8; c++)
        if (grid[r][c] && grid[r][c]->getSymbol() == ksym) { kr = r; kc = c; }
    if (kr == -1) return true;
    for (int r = 0; r < 8; r++) for (int c = 0; c < 8; c++)
        if (grid[r][c] && grid[r][c]->getColor() == enemy)
            if (grid[r][c]->isValidMove(r, c, kr, kc, grid)) return true;
    return false;
}

bool Board::hasLegalMove(char color)
{
    for (int fr = 0; fr < 8; fr++) for (int fc = 0; fc < 8; fc++)
    {
        if (!grid[fr][fc] || grid[fr][fc]->getColor() != color) continue;
        for (int tr = 0; tr < 8; tr++) for (int tc = 0; tc < 8; tc++)
        {
            if (!grid[fr][fc]->isValidMove(fr, fc, tr, tc, grid)) continue;
            Piece* cap = grid[tr][tc]; grid[tr][tc] = grid[fr][fc]; grid[fr][fc] = nullptr;
            bool safe = !isInCheck(color);
            grid[fr][fc] = grid[tr][tc]; grid[tr][tc] = cap;
            if (safe) return true;
        }
    }
    return false;
}

bool Board::isCheckmate(char color) { return  isInCheck(color) && !hasLegalMove(color); }
bool Board::isStalemate(char color) { return !isInCheck(color) && !hasLegalMove(color); }
char Board::getTurn()               const { return turn; }

bool Board::movePiece(int fr, int fc, int tr, int tc)
{
    if (fr < 0 || fr > 7 || fc < 0 || fc > 7 || tr < 0 || tr > 7 || tc < 0 || tc > 7) return false;
    Piece* p = grid[fr][fc];
    if (!p || p->getColor() != turn) return false;
    if (!p->isValidMove(fr, fc, tr, tc, grid)) return false;
    Piece* cap = grid[tr][tc];
    grid[tr][tc] = p; grid[fr][fc] = nullptr;
    if (isInCheck(turn)) { grid[fr][fc] = p; grid[tr][tc] = cap; return false; }
    if (cap) delete cap;
    if ((p->getSymbol() == 'P' && tr == 0) || (p->getSymbol() == 'p' && tr == 7))
    {
        grid[tr][tc] = new Queen(turn); delete p;
    }
    turn = turn == 'W' ? 'B' : 'W';
    return true;
}
