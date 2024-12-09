#include <iostream>
#include <vector>
#include <memory>
#include "raylib.h"
#include "Background.h"
#include "Character.h"
#include "Obstacle.h"
#include "UpperObstacle.h"
#include "LowerObstacle.h"
#include "Structs.h"

int main()
{
    //Window Dimensions
    Vector2 windowDimensions{800, 480};
    InitWindow(windowDimensions.x, windowDimensions.y, "Flappy Bird!");

    Texture2D lowerRock = LoadTexture("textures/rock.png");
    Texture2D upperRock = LoadTexture("textures/rockDown.png");
    const int numEachObstacleType{3};

    std::vector<std::unique_ptr<Obstacle>> obstaclePool;

    for(int i=0; i < numEachObstacleType; i++)
    {
        obstaclePool.push_back(std::make_unique<LowerObstacle>());
        obstaclePool.back()->Init(lowerRock, windowDimensions.x, windowDimensions.y);
        obstaclePool.push_back(std::make_unique<UpperObstacle>());
        obstaclePool.back()->Init(upperRock, windowDimensions.x, windowDimensions.y);
    }

    //obstacle timer
    float runningTime{2.0};
    float obstacleInterval{2.0};

    Character character{static_cast<int>(windowDimensions.x), static_cast<int>(windowDimensions.y)};

    bool gameOver{false};

    Background background{"textures/background.png", 25};

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // Delta time
        float dt = GetFrameTime();

        BeginDrawing();
        ClearBackground(BLACK);

        background.tick(dt);

        if(gameOver)
        {
            //Lose the game
            DrawText("Game Over!", windowDimensions.x/4, windowDimensions.y/2, 50, BLACK);

            if(IsKeyPressed(KEY_R))
            {
                character.Reset(windowDimensions.x, windowDimensions.y);
                gameOver = false;
            }
        }
        else
        {
            runningTime += dt;
            if(runningTime >= obstacleInterval)
            {
                runningTime = 0;

                //find inactive obstacles in pool
                for (auto& obstacle : obstaclePool)
                {
                    if(!obstacle->GetActive())
                    {
                        //Spawn inactive obstacle and stop searching
                        obstacle->SetActive(true);
                        obstacle->SetStartPosition(windowDimensions.x, windowDimensions.y);
                        break;
                    }
                }
            }

            //tick all active obstacles - update position and collisions
            for (auto& obstacle : obstaclePool)
            {
                if(obstacle->GetActive()) obstacle->tick(dt);            
            } 

            character.tick(dt, windowDimensions.y);

            //Check if character has hit the floor
            if(character.OutOfBounds(windowDimensions.y))
            {
                gameOver = true;
            }

            //Check obstacle collisions
            for (auto& obstacle : obstaclePool)
            {                        
                if(CheckCollisionCircleLine(character.GetCollisionCircle().pos, character.GetCollisionCircle().radius, 
                    obstacle->GetCollisionLine().point1, obstacle->GetCollisionLine().point2))
                {
                    gameOver = true;
                }
           }               
        }

        EndDrawing();
    }

    //Unload all textures
    UnloadTexture(lowerRock);
    UnloadTexture(upperRock);

    CloseWindow();
}