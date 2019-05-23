#include "game.h"

Game::Game() : mWindow(sf::VideoMode(500,600), "Tetris")
{
    // Vaš kod dolazi ovdje (ako bude potrebe).   
}

void Game::run(){
    sf::Clock clock;
    float protekloVrijeme = sf::Time::Zero.asSeconds();

    while(mWindow.isOpen()){

        processEvents();
        update();

        protekloVrijeme += clock.restart().asSeconds();
        mBoard.update(protekloVrijeme, mLeftPressed, mUpPressed, mRightPressed, mSpacePressed);

        render();
    }
}

void Game::processEvents()
{
    sf::Event event;
    while(mWindow.pollEvent(event))
    {
        switch(event.type)
        {
        case sf::Event::Closed:
            mWindow.close();
            break;

        case sf::Event::KeyPressed:
            // Vaš kod dolazi ovdje !!!
            if (event.key.code == sf::Keyboard::Left)
                mLeftPressed = true;
            if (event.key.code == sf::Keyboard::Right)
                mRightPressed = true;
            if (event.key.code == sf::Keyboard::Up)
                mUpPressed = true;
            if (event.key.code == sf::Keyboard::Space)
                mSpacePressed = true;
            break;

        case sf::Event::KeyReleased:
            if(event.key.code == sf::Keyboard::Left)
                            mLeftPressed = false;
            if(event.key.code == sf::Keyboard::Right)
                            mRightPressed = false;
            if(event.key.code == sf::Keyboard::Up)
                            mUpPressed = false;
            if(event.key.code == sf::Keyboard::Space)
                            mSpacePressed = false;
            break;

        case sf::Event::Resized:
            // Spriječi deformaciju objekata prilikom promjene veličine prozora
            sf::FloatRect viewRect(0, 0, event.size.width, event.size.height);
            mWindow.setView(sf::View(viewRect));
            break;
        }
    }
}

void Game::update()
{
    if (mBoard.isGameOver()){
        mScore.setGameOver();
        while(1);
    }
    mScore.addNoLinesCompleted(mBoard.getNoLinesCompleted());
    mBoard.clearNoLinesCompleted();
    mScore.update();
}

void Game::render()
{
     mWindow.clear();


     mWindow.draw(mBoard);
     mWindow.draw(mScore);

     mWindow.display();
}
