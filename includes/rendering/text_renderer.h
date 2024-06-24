#pragma once

#include "LTR_Engine_lib.h"
#include "gl_renderer.h"
#include <map>

namespace TextRender{
    // ############################################################################
    //                            Text struct and classes
    // ############################################################################
    struct Character{
        unsigned int textureId;
        glm::ivec2 size;
        glm::ivec2 bearing;
        unsigned int advance;
    };
    class FontStorage{
    public:
        FontStorage(const char* fontPath,unsigned short fontSize);
        void RenderText(std::string text,float x,float y,float scale,glm::vec3 color);
    private:
        std::map<char,Character> characters;
    };
}