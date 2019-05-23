#pragma once
#include <SFML/Graphics.hpp>
#include "board.h"
#include "score.h"

class Game{
	public:
            Game();
	    void run();
	private:
            sf::RenderWindow mWindow;
            Board mBoard;
            Score mScore;

            // Dodatne varijable dolaze ovdje
            bool mLeftPressed = false;
            bool mRightPressed = false;
            bool mUpPressed = false;
            bool mSpacePressed = false;

            void processEvents();
            void update();
            void render();
};
