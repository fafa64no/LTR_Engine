#include "characters.h"
#include "input.h"

// ############################################################################
//                            Character functions
// ############################################################################
Character::Character(RenderInterface::Node* charNode){
    this->charNode=charNode;
    this->nodeId=RenderInterface::storeNode(this->charNode);
}
void Character::moveInDir(glm::vec3 movDir){
    this->charNode->translate(movDir);
}
void Character::Render(){
    this->renderId=RenderInterface::addNodeToRender(RenderInterface::nodeContainer->nodes[this->nodeId]);
}

// ############################################################################
//                            Player functions
// ############################################################################
Player::Player(Character* character){
    this->character=character;
    this->character->Render();
}