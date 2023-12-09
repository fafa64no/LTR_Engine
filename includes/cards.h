#pragma once
#include "LTR_Engine_lib.h"


namespace Cards{
    struct Modifier{
        float attackSpeedDuration,attackDamageDuration,movementSpeedDuration,cooldownsDuration,stunDuration,rootDuration,
            burnDuration,poisonDuration,slowDuration,weaknessDuration,
            attackSpeedStrenght,attackDamageStrenght,movementSpeedStrenght,cooldownsStrenght,stunStrenght,rootStrenght,
            burnStrenght,poisonStrenght,slowStrenght,weaknessStrenght;

    };
    struct Effect{

    };
    struct CardRenderer{

    };
    class Card{ 
    public:
        Card();
        void updateSlot(unsigned short slot);
        void RenderCard();
        void RenderEffect();
    	~Card();

    private:
        unsigned short consoMana;
        float cooldown,damage;

        glm::mat4 modelMat;

        CardRenderer cardRenderer;
        Effect effect;
    };

    class Deck{

    };
};

