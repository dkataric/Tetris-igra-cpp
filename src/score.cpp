#include "score.h"
#include <cstdlib>
#include <iostream>

Score::Score()
{
    if(!mFont.loadFromFile("Gasalt-Regular.ttf")){
        std::cerr << "Ne mogu učitati font: Gasalt-Regular.ttf !" << std::endl;
        std::exit(1);
    }
    mScore = 0;
    std::string score = "Score: ";
    std::string score_int = std::to_string((mScore));
    score += score_int;
    mScoreText.setFont(mFont);
    mScoreText.setString(score);
    mScoreText.setCharacterSize(24);
    mScoreText.setPosition(sf::Vector2f(260.f,50.f));

}

void Score::addNoLinesCompleted(int n)
{
    mScore += n*10;
    std::string score = "Score: ";
    std::string score_int = std::to_string(mScore);
    score += score_int;
    mScoreText.setString(score);
}

void Score::setGameOver()
{
    mGameOverText.setFont(mFont);
    mGameOverText.setString("Game Over");
    mGameOverText.setCharacterSize(24);
    mGameOverText.setPosition(sf::Vector2f(260.f,65.f));

}

void Score::update()
{
    return;
}

void Score::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
  states.transform *= getTransform();
  target.draw(mScoreText,states);
  target.draw(mGameOverText,states);
}
