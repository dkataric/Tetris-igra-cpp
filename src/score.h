#pragma once
#include <SFML/Graphics.hpp>

class Score : public sf::Drawable, public sf::Transformable{
public:
    Score();
    // Dodaj broj obrisanih linija i aktualiziraj rezultat.
    void addNoLinesCompleted(int n);
    // Postavi Game over string 
    void setGameOver();
    // Aktualizacija ispisa
    void update();
private:
    sf::Font mFont;
    sf::Text mScoreText;
    sf::Text mGameOverText;
    int mScore = 0;
    // Eventualne dodatne varijable dolaze ovdje

    // Iscrtavanje.
    void draw(sf::RenderTarget & target, sf::RenderStates states) const override;
};
