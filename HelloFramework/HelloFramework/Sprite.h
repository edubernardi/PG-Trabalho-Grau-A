#pragma once
#include "Object.h"
class Sprite :
    public Object
{
public:
    Sprite(){}
    ~Sprite() {}

    void setSpritesheet(GLuint texID, int nAnimations, int nFrames);
    inline void setAnimation(int iAnimation) { this->iAnimation = iAnimation; }
    inline void setFrame(int iFrame) { this->iFrame = iFrame; }
    void updateSpriteSheet(GLuint texID, int nFrames);
    void update();
    glm::vec4 getRectangle();
protected:
    int nAnimations, nFrames, iAnimation, iFrame;
    float dx, dy;
};

