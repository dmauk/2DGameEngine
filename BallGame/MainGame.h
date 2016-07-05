#pragma once

#include <GameEngine2D/Camera2D.h>
#include <GameEngine2D/SpriteBatch.h>
#include <GameEngine2D/InputManager.h>
#include <GameEngine2D/Window.h>
#include <GameEngine2D/GLSLProgram.h>
#include <GameEngine2D/Timing.h>
#include <GameEngine2D/SpriteFont.h>
#include <memory>

#include "BallController.h"
#include "BallRenderer.h"
#include "SpatialGrid.h"

enum class GameState { RUNNING, EXIT };

const int CELL_SIZE = 12;

class MainGame {
public:
	~MainGame();
    void run();

private:
    void init();
    void initBalls();
    void update(float deltaTime);
    void draw();
    void drawHud();
    void processInput();

    int m_screenWidth = 0;
    int m_screenHeight = 0;

    std::vector<Ball> m_balls; ///< All the balls

    BallController m_ballController; ///< Controls balls
    BallRenderer m_ballRenderer; ///< Renders balls
	//Old C way -> SpatialGrid* m_spatialGrid and then later allocate with new
	std::unique_ptr<SpatialGrid> m_spatialGrid; ///< Spatial partioning

    GameEngine2D::Window m_window; ///< The main window
    GameEngine2D::SpriteBatch m_spriteBatch; ///< Renders all the balls
    std::unique_ptr<GameEngine2D::SpriteFont> m_spriteFont; ///< For font rendering
    GameEngine2D::Camera2D m_camera; ///< Renders the scene
    GameEngine2D::InputManager m_inputManager; ///< Handles input
    GameEngine2D::GLSLProgram m_textureProgram; ///< Shader for textures]

    GameEngine2D::FpsLimiter m_fpsLimiter; ///< Limits and calculates fps
    float m_fps = 0.0f;

    GameState m_gameState = GameState::RUNNING; ///< The state of the game
};

