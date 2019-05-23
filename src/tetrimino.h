#ifndef TETRIMINO_H
#define TETRIMINO_H

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Clock.hpp>

/**
 * @brief Klasa  Tetrimino
 *
 * Klasa predstavlja komad na ploči.
 */
class Tetrimino{
public:
    // Tip komada. Imamo 7 različiti komada (SIZE=7).
    enum Types {O=0,I,S,Z,L,J,T,SIZE};
    // Broj rotacija svakog komada.
    static const int N_ROTACIJA = 4;
    // Matrica koja sadrži svaki mogući komad je reda MAT_SIZE x MAT_SIZE.
    static const int MAT_SIZE = 4;
    // Svi komadi i njihove rotacije su dane u ovoj tabeli.
    static const int mDijelovi[SIZE][N_ROTACIJA][MAT_SIZE][MAT_SIZE];
    // Boje pojedinih komada.
    static const sf::Color mColors[SIZE];

    // Tip komada.
    Types mType = O;
    // Trenutna rotacija komada.
    int mRotacija=0;
    // Trenutna pozicija komada.
    int mXposition=0, mYposition=0;
    // Sat za mjerenje proteklog vremena
    sf::Clock mClock;

    void move(int x, int y);
    void rotate();
private:


};

#endif // TETRIMINO_H
