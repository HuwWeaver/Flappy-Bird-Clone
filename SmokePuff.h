#ifndef SMOKE_PUFF_H
#define SMOKE_PUFF_H

#include "raylib.h"

class SmokePuff
{
public:
    SmokePuff();
    ~SmokePuff();
    void Init(const char *textureFilePath, Vector2 startPos);
    void tick(float deltaTime);
    bool GetActive() {return active;};
    void SetActive(bool newActive) {active = newActive;};
    void SetPosition(Vector2 newPos) {pos = newPos;};
    void Reset() {SetActive(false);};

protected:
    Texture2D texture{};
    Vector2 pos{};
    int speed{-200};
    bool active{false};
};

#endif