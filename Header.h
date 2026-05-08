#pragma once
#include <iostream>
#include <string>
using namespace std;

class Piece;

class MoveValidator
{
public:
    static bool isStraightClear(int fr, int fc, int tr, int tc, Piece* b[8][8]);
    static bool isDiagonalClear(int fr, int fc, int tr, int tc, Piece* b[8][8]);
};
class Piece {
protected:
    char color, symbol;
public:
    Piece(char color, char symbol);
    virtual ~Piece() {}
    char getColor()  const;
    char getSymbol() const;
    virtual bool isValidMove(int fr, int fc, int tr, int tc, Piece* board[8][8]) = 0;
};

class Pawn : public Piece { public: Pawn(char c);   bool isValidMove(int, int, int, int, Piece* [8][8]); };
class Rook : public Piece { public: Rook(char c);   bool isValidMove(int, int, int, int, Piece* [8][8]); };
class Knight : public Piece { public: Knight(char c); bool isValidMove(int, int, int, int, Piece* [8][8]); };
class Bishop : public Piece { public: Bishop(char c); bool isValidMove(int, int, int, int, Piece* [8][8]); };
class Queen : public Piece { public: Queen(char c);  bool isValidMove(int, int, int, int, Piece* [8][8]); };
class King : public Piece { public: King(char c);   bool isValidMove(int, int, int, int, Piece* [8][8]); };

class Board {
    Piece* grid[8][8];
    char   turn;
    void   setup();
    bool   hasLegalMove(char color);
public:
    Board();
    ~Board();
    bool   movePiece(int fr, int fc, int tr, int tc);
    bool   isInCheck(char color);
    bool   isCheckmate(char color);
    bool   isStalemate(char color);
    char   getTurn() const;
    Piece* getPiece(int r, int c) const;
};
