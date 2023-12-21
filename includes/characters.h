#pragma once

#include "render_interface.h"

// ############################################################################
//                            Character classes
// ############################################################################
class Character{
public:
    Character(RenderInterface::Node* charNode);
    void moveInDir(glm::vec3 movDir);
    void Render();
    void unRender();
private:
    RenderInterface::Node* charNode;
    int nodeId,renderId;
};

class Player{
public:
    Player(Character* character);
private:
    glm::vec2 camOrientation;
    Character* character;
};

// ############################################################################
//                            Character globals
// ############################################################################
static Character* kobara;
static Character* alder;