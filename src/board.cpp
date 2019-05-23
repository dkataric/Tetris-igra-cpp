#include "board.h"

// Vaš kod dolazi ovdje

Board::Board()
{
    mPiece.mClock.restart();
    mOriginX = mLeftMargin;
    mOriginY = mTopMargin;
    mDy = mWidth / mCols;
    mDx = mHeight / mRows;
    for (int i = 0; i < mRows; ++i)
        for (int j = 0; j < mCols; ++j)
            mGridContent[i][j] = 0;
}

void Board::update(float &vrijeme, bool &lijevo, bool &gore, bool &desno, bool &space)
{
    if (mPiece.mXposition == 0 && mPiece.mYposition == 0)
        spawnPiece();

    if(lijevo){
        removePiece();
        movePiece(0,-1);
        lijevo = false;
    }
    if(desno){
        removePiece();
        movePiece(0,1);
        desno = false;
    }
    if(gore){
        removePiece();
        rotatePiece();
        gore = false;
    }
    if(space){
        removePiece();
        movePiece(-2,0);
        space = false;
    }


    if (vrijeme > mDt){
        vrijeme = 0.f;
        if(movePiece(-1,0)){
            placePiece();
        }
        else{
            placePiece();
            clearLines();
            spawnPiece();
            if (!checkSpace())
                mGameOver = true;
        }
    }

}

void Board::spawnPiece()
{
    //mPiece.mRotacija = 0;
    //gdje se tocka rotacije tetrimina nalazi na ploci(0-9)
    mPiece.mXposition = 0;
    mPiece.mYposition = 5;

   // mPiece.mType = static_cast<Tetrimino::Types>(0);


    std::uniform_int_distribution<> mUniformDist2(0,6);
    mPiece.mType = static_cast<Tetrimino::Types>(mUniformDist2(mRNE));

    //je li prvi red slobodan
    for (int i = 0; i < 4; i++)
        if (mPiece.mDijelovi[mPiece.mType][mPiece.mRotacija][0][i] > 0)
            mPiece.mXposition = 1;
}

bool Board::isGameOver() const
{
    return mGameOver;
}

int Board::getNoLinesCompleted() const
{
    return mNoLinesCompleted;
}

void Board::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    sf::VertexArray mline(sf::LineStrip, 5);
    mline[0].position = sf::Vector2f(mOriginX, mOriginY);
    mline[1].position = sf::Vector2f(mOriginX + mWidth, mOriginY);
    mline[2].position = sf::Vector2f(mOriginX + mWidth, mOriginY + mHeight);
    mline[3].position = sf::Vector2f(mOriginX, mOriginY + mHeight);
    mline[4].position = sf::Vector2f(mOriginX, mOriginY);
    target.draw(mline, states);

    for (int i = 0; i < mRows; ++i)
        for (int j = 0; j < mCols; j++)
            if (mGridContent[i][j] > 0){
                //konstrukcija pojedinog cetverokuta
                sf::VertexArray pom(sf::Quads, 4);
                pom[0].position = sf::Vector2f(mOriginX + j * mDx, mOriginY + i * mDy);
                pom[1].position = sf::Vector2f(mOriginX + j * mDx, mOriginY + (i+1) * mDy);
                pom[2].position = sf::Vector2f(mOriginX + (j+1) * mDx, mOriginY + (i+1) * mDy);
                pom[3].position = sf::Vector2f(mOriginX + (j+1) * mDx, mOriginY + i * mDy);

                //boja za pojedini četverokut
                for (int k = 0; k < 4; ++k)
                    pom[k].color = mPiece.mColors[mGridContent[i][j] - 1];
                //iscrtavanje
                target.draw(pom,states);
            }
}

bool Board::checkSpace()
{
    //ako provjeravam samo donja tri reda
    if (mPiece.mXposition == 0){
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 4; ++j){
                if (mGridContent[mPiece.mXposition + i][mPiece.mYposition + j - 2] > 0 &&
                   mPiece.mDijelovi[mPiece.mType][mPiece.mRotacija][i + 1][j] > 0)
                    return false;
            }
    }
    //provjera ako su prva dva stupca vani sa lijeve strane
    if (mPiece.mYposition == 0){
        if (colFull(0) || colFull(1))
            return false;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 2; j++)
                if (mGridContent[mPiece.mXposition - 1 + i][j] > 0 &&
                        mPiece.mDijelovi[mPiece.mType][mPiece.mRotacija][i][j+2] > 0)
                    return false;
    }
    //provjera ako je jedan stupac vani sa lijeve strane
    if (mPiece.mYposition == 1){
        if (colFull(0))
            return false;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 3; ++j)
                if (mGridContent[mPiece.mXposition - 1 + i][j] > 0 &&
                        mPiece.mDijelovi[mPiece.mType][mPiece.mRotacija][i][j+1] > 0)
                    return false;

    }
    //provjera ako je jedan stupac vani sa desne strane
    if (mPiece.mYposition == mCols - 1){
        if (colFull(3))
            return false;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 3; ++j)
                if (mGridContent[mPiece.mXposition - 1 + i][mCols - 3 + j] > 0 &&
                        mPiece.mDijelovi[mPiece.mType][mPiece.mRotacija][i][j] > 0)
                    return false;
    }
    //ako su dva reda ispod
    if (mPiece.mXposition == mRows - 1){
        if (rowFull(2) || rowFull(3))
            return false;
        for (int i = 0; i < 2; ++ i)
            for (int j = 0; j < 4; j++)
                if (mGridContent[mRows - 2 + i][mPiece.mYposition - 2 + j] > 0 &&
                        mPiece.mDijelovi[mPiece.mType][mPiece.mRotacija][i][j] > 0)
                    return false;
    }
    //ako je jedan red ispod
    if (mPiece.mXposition == mRows - 2){
        if (rowFull(3))
            return false;
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 4; j++)
                if (mGridContent[mRows - 3 + i][mPiece.mYposition - 2 + j] > 0 &&
                        mPiece.mDijelovi[mPiece.mType][mPiece.mRotacija][i][j] > 0)
                    return false;
    }
    //svi nemoguci slucajevi
    if (mPiece.mXposition > mRows - 1 || mPiece.mXposition < 0 ||
             mPiece.mYposition > mCols - 1 || mPiece.mYposition < 0){
        return false;
    }
    //provjeravam cijeli 4x4 blok
    if (mPiece.mXposition > 0 && mPiece.mXposition < mRows - 2 &&
            mPiece.mYposition > 1 && mPiece.mYposition < mCols - 1) {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                if (mGridContent[mPiece.mXposition -1 + i][mPiece.mYposition -2 + j] > 0 &&
                   mPiece.mDijelovi[mPiece.mType][mPiece.mRotacija][i][j] > 0)
                    return false;
    }
    return true;

}

void Board::placePiece()
{
    //ako stavljam blok sa tri reda
    if (mPiece.mXposition == 0){
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 4; j++)
                if (mPiece.mDijelovi[mPiece.mType][mPiece.mRotacija][i+1][j] > 0){
                    mGridContent[i][mPiece.mYposition - 2 + j] = mPiece.mType + 1;
                }
    }
    else {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j){
                if (mPiece.mDijelovi[mPiece.mType][mPiece.mRotacija][i][j] > 0)
                mGridContent[mPiece.mXposition + i - 1][mPiece.mYposition + j - 2] = mPiece.mType + 1;
            }
    }
}

void Board::removePiece()
{
    //ako je tetrimino od tri reda
    if (mPiece.mXposition == 0){
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 4; ++j){
                if (mPiece.mDijelovi[mPiece.mType][mPiece.mRotacija][i+1][j] > 0)
                    mGridContent[mPiece.mXposition + i][mPiece.mYposition + j - 2] = 0;
            }
    }

    //4x4 tetrimino
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j){
            if (mPiece.mDijelovi[mPiece.mType][mPiece.mRotacija][i][j] > 0)
                mGridContent[mPiece.mXposition -1 + i][mPiece.mYposition -2 + j] = 0;
        }
}

bool Board::movePiece(int x, int y)
{
    //ako su joj dva stupca vani sa lijeve strane - nema dalje
    if (mPiece.mYposition == 0 && y == -1)
        return false;

    //ako joj je jedan stupac vani sa lijeve strane
    if (mPiece.mYposition == 1 && y == -1)
        if (colFull(1))
            return false;

    //ako je na lijevom rubu
    if (mPiece.mYposition == 2 && y == -1){
        if (colFull(0)){
            return false;
        }
}
    //ako je jedan stupac vani sa desne strane - nema dalje
    if (mPiece.mYposition == mCols - 1 && y == 1)
        return false;

    //ako je na desnom rubu
    if (mPiece.mYposition == mCols - 2 && y == 1)
        if (colFull(3))
            return false;

    //prirodni pomak za jedan dolje
    if (x == -1){
        removePiece();
        mPiece.mXposition++;
        if (checkSpace()){
            return true;
        }
        else{
            mPiece.mXposition--;
            return false;
        }
    }

    //stisnut space...
    if (x == -2){
        while (checkSpace()){
            mPiece.mXposition++;
        }
        mPiece.mXposition--;
        placePiece();
    }

    if (y == -1){
        mPiece.mYposition--;
        if (checkSpace())
            return true;
        else {
            mPiece.mYposition++;
            return false;
        }
    }

    if (y == 1){
        mPiece.mYposition++;
        if (checkSpace())
            return true;
        else {
            mPiece.mYposition--;
            return false;
        }
    }
    return true;
}

bool Board::rotatePiece()
{
    int rotacija = 0;
    if (mPiece.mRotacija < 3)
        ++rotacija;
    //ako cijeli tetrimino nije jos na ploci
    if (mPiece.mXposition == 0)
        return false;

    //ako su mu dva stupca izvan sa lijeve strane
    if (mPiece.mYposition == 0){

        //ne može jer bi napustio plocu
        if (rotColFull(0) || rotColFull((1)))
            return false;

        //ne moze jer je ploca zauzeta
        //provjera za zadnja dva stupca
        for (int i = -1; i < 3; ++i)
            for (int j = 0; j < 2; ++j)
                if (mGridContent[mPiece.mXposition + i][mPiece.mYposition + j] > 0 &&
                   mPiece.mDijelovi[mPiece.mType][rotacija][i+1][j+2] > 0)
                    return false;
    }
    //ako mu je jedan stupac izvan sa lijeve strane
    if (mPiece.mYposition == 1){

        //ne može jer bi napustio plocu
        if (rotColFull(0))
            return false;

        //ne moze jer je ploca zauzeta
        //provjera za zadnja tri stupca
        for (int i = -1; i < 3; ++i)
            for (int j = -1; j < 2; ++j)
                if (mGridContent[mPiece.mXposition + i][mPiece.mYposition + j] > 0 &&
                   mPiece.mDijelovi[mPiece.mType][rotacija][i+1][j+2] > 0)
                    return false;
    }

    //ako mu je jedan stupac izvan sa desne strane
    if (mPiece.mYposition == mCols - 1){

        //ne može jer bi napustio plocu
        if (rotColFull(3))
            return false;

        //ne moze jer je ploca zauzeta
        //provjera za prva tri stupca
        for (int i = -1; i < 3; ++i)
            for (int j = -2; j < 1; ++j)
                if (mGridContent[mPiece.mXposition + i][mPiece.mYposition + j] > 0 &&
                   mPiece.mDijelovi[mPiece.mType][rotacija][i+1][j+2] > 0)
                    return false;
    }

    //ako je u negdje u sredini
    for (int i = -1; i < 3; ++i)
        for (int j = -2; j < 2; ++j){
            if (mGridContent[mPiece.mXposition + i][mPiece.mYposition + j] > 0 &&
               mPiece.mDijelovi[mPiece.mType][rotacija][i+1][j+2] > 0)
                return false;
        }

    mPiece.rotate();
    return true;
}

bool Board::isLineFull(int y) const
{
    for (int i = 0; i < mCols; ++i)
        if (mGridContent[y][i] == 0)
            return false;

    return true;
}

int Board::clearLines()
{
    int obrisane = 0;
    for (int i = mRows - 1; i >= 0; --i)
        if (isLineFull(i)){
            clearAndCopyLine(i);
            obrisane++;
            ++i;
        }
    mNoLinesCompleted += obrisane;
    return obrisane;
}

void Board::clearAndCopyLine(int x)
{
    for (int i = x; i > 0; --i)
        for (int j = 0; j < mCols; ++j){
            mGridContent[i][j] = mGridContent[i-1][j];
    }
    //zadnji red staviti na nulu
    for (int i = 0; i < mCols; ++i)
        mGridContent[0][i] = 0;
}

bool Board::rotColFull(int y)
{
    int rotacija = mPiece.mRotacija;

    if (rotacija == 3)
        rotacija = 0;
    else
        ++rotacija;

    for (int i = 0; i < 4; ++i)
        if(mPiece.mDijelovi[mPiece.mType][rotacija][i][y] > 0)
            return true;
    return false;
}

bool Board::colFull(int y)
{
    for (int i = 0; i < 4; ++i)
        if (mPiece.mDijelovi[mPiece.mType][mPiece.mRotacija][i][y] > 0)
            return true;
    return false;
}

bool Board::rowFull(int x)
{
    for (int i = 0; i < 4; ++i)
        if (mPiece.mDijelovi[mPiece.mType][mPiece.mRotacija][x][i] > 0)
            return true;
    return false;

}
