#include "LTR_Engine_lib.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "input.h"
#include "gl_renderer.h"
#include "text_renderer.h"

namespace TextRender{
    // ############################################################################
    //                            Text struct and classes
    // ############################################################################
    FontStorage::FontStorage(const char* fontPath,unsigned short fontSize){
        //Init freetype
        FT_Library ftLib;
        SM_ASSERT(!FT_Init_FreeType(&ftLib),"Failed to load freetype");
        //Load face
        FT_Face ftFace;
        SM_ASSERT(!FT_New_Face(ftLib,fontPath,0,&ftFace),"Failed to create font face");
        FT_Set_Pixel_Sizes(ftFace,0,fontSize);
        //Set byte alignement
        glPixelStorei(GL_UNPACK_ALIGNMENT,1);
        unsigned char c=0;
        for(int i=0;i<256;i++){
            //Load glyph
            SM_ASSERT(!FT_Load_Char(ftFace,c,FT_LOAD_RENDER),"Failed to load character glyph");
            //Generate texture
            unsigned int texture;
            glGenTextures(1,&texture);
            glBindTexture(GL_TEXTURE_2D,texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                ftFace->glyph->bitmap.width,
                ftFace->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                ftFace->glyph->bitmap.buffer
            );
            //Setup texture
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
            //Store character
            Character character{
                texture,
                glm::ivec2(ftFace->glyph->bitmap.width,ftFace->glyph->bitmap.rows),
                glm::ivec2(ftFace->glyph->bitmap_left,ftFace->glyph->bitmap_top),
                (unsigned int)ftFace->glyph->advance.x
            };
            this->characters.insert(std::pair<char,Character>(c,character));
            c++;
        }
        FT_Done_Face(ftFace);
        FT_Done_FreeType(ftLib);
    }
    void FontStorage::RenderText(std::string text,float x,float y,float scale,glm::vec3 color){
        textShader->use();
        textShader->setVec3("textColor",color);
        glm::mat4 modelMat=glm::mat4(1.0f);
        //modelMat=glm::scale(modelMat,glm::vec3(input->screenRatio,1.0f/1920.0f,1));
        textShader->setMat4("model",modelMat);
        textShader->setFloat("containerDepth",-0.5f-0.05f);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(textVAO);
        std::string::const_iterator c;
        for(c=text.begin();c!=text.end();c++){
            float scalx=scale*0.001f*input->screenRatio;
            float scaly=scale*0.001f;
            Character ch=this->characters[*c];
            float xpos=x+(float)ch.bearing.x*scalx;
            float ypos=y+(float)ch.bearing.y*scaly;
            float w=(float)ch.size.x*scalx;
            float h=(float)ch.size.y*scaly;
            float vertices[24]{
                xpos    ,ypos   ,0.0f,0.0f,
                xpos    ,ypos-h ,0.0f,1.0f,
                xpos+w  ,ypos-h ,1.0f,1.0f,
                xpos    ,ypos   ,0.0f,0.0f,
                xpos+w  ,ypos-h ,1.0f,1.0f,
                xpos+w  ,ypos   ,1.0f,0.0f
            };
            glBindTexture(GL_TEXTURE_2D,ch.textureId);
            glBindBuffer(GL_ARRAY_BUFFER,textVBO);
            glBufferSubData(GL_ARRAY_BUFFER,0,24*sizeof(float),&vertices);
            glDrawArrays(GL_TRIANGLES,0,6);
            //glDrawArrays(GL_LINES,0,6);
            glBindBuffer(GL_ARRAY_BUFFER,0);
            x+=(float)(ch.advance>>6)*scalx;
        }
        //glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D,0);
    }
}