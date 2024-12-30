#include "Character.h"

Character::Character(int winWidth, int winHeight)
{
    texture = LoadTexture("textures/Planes/planeRedSpriteSheet.png");
    spriteRect = {0, 0, texture.width/4.0f, texture.height/1.0f};
    pos = {winWidth/4.0f - (texture.width/4.0f)/2.0f, winHeight/3.0f};
    collisionCircle = {Vector2{pos.x + spriteRect.width/2.0f, pos.y + spriteRect.height/2.0f}, 35};

    smokePuffTexture = LoadTexture("textures/puffSmall.png");

    for (auto& puff : smokePuffPool)
    {
        puff.Init(smokePuffTexture, pos);
    }
}

Character::~Character()
{
    UnloadTexture(texture);
    UnloadTexture(smokePuffTexture);
}

bool Character::OutOfBounds(int winHeight)
{
    if(pos.y >= winHeight - spriteRect.height) return true;
    if(pos.y <= -10) return true;
    
    return false;
}

void Character::Reset(int winWidth, int winHeight)
{
    for (auto& puff : smokePuffPool)
    {
        puff.Reset();
    }
    
    pos = {winWidth/4.0f - (texture.width/4.0f)/2.0f, winHeight/3.0f};
    collisionCircle.pos = {pos.x + spriteRect.width/2.0f, pos.y + spriteRect.height/2.0f};
    yVelocity = 0;
}

void Character::tick(float deltaTime, int winHeight)
{
    //In the air - Apply Gravity
    yVelocity += gravity * deltaTime;

    if(IsKeyPressed(KEY_SPACE))
    {
        yVelocity += jumpVel;

        for (auto& puff : smokePuffPool)
        {
            if(!puff.GetActive())
            {
                puff.Reset();
                puff.SetActive(true);
                puff.SetPosition(pos);
                break;  
            }
        }
    }

    pos.y += yVelocity * deltaTime;
    collisionCircle.pos.y += yVelocity * deltaTime;

    //Character Animation
    runningTime += deltaTime;
    if(runningTime >= updateTime)
    {
        runningTime = 0.0;
        //Update animation frame
        spriteRect.x = frame * spriteRect.width;
        frame++;
        if(frame > maxFrame)
        {
            frame = 0;
        }
    }

    //Tick Particles
    for (auto& puff : smokePuffPool)
    {
        if(puff.GetActive()) puff.tick(deltaTime);
    }

    DrawTextureRec(texture, spriteRect, pos, WHITE);

    //DEBUG Circle
    //DrawCircleLines(collisionCircle.pos.x, collisionCircle.pos.y, collisionCircle.radius, BLUE);
}