#pragma once

#include <GameEngine2D/SpriteBatch.h>
#include "Ball.h"

class BallRenderer {
public:
    void renderBall(GameEngine2D::SpriteBatch& spriteBatch, Ball& ball);
};
