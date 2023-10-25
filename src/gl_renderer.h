#pragma once
#include "glcorearb.h"
#include "platform.h"

// ############################################################################
//                            OpenGL Function Pointers
// ############################################################################
static PFNGLCREATEPROGRAMPROC glCreateProgram_ptr;
static PFNGLDELETETEXTURESPROC glDeleteTextures_ptr;
static PFNGLGENTEXTURESPROC glGenTextures_ptr;
static PFNGLBINDTEXTUREPROC glBindTexture_ptr;
static PFNGLDRAWBUFFERPROC glDrawBuffer_ptr;
static PFNGLDRAWARRAYSPROC glDrawArrays_ptr;
static PFNGLCREATESHADERPROC glCreateShader_ptr;
static PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation_ptr;
static PFNGLUNIFORM1FPROC glUniform1f_ptr;
static PFNGLUNIFORM2FVPROC glUniform2fv_ptr;
static PFNGLUNIFORM3FVPROC glUniform3fv_ptr;
static PFNGLUNIFORM1IPROC glUniform1i_ptr;
static PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv_ptr;
static PFNGLVERTEXATTRIBDIVISORPROC glVertexAttribDivisor_ptr;
static PFNGLACTIVETEXTUREPROC glActiveTexture_ptr;
static PFNGLBUFFERSUBDATAPROC glBufferSubData_ptr;
static PFNGLDRAWARRAYSINSTANCEDPROC glDrawArraysInstanced_ptr;
static PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer_ptr;
static PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus_ptr;
static PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers_ptr;
static PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D_ptr;
static PFNGLDRAWBUFFERSPROC glDrawBuffers_ptr;
static PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers_ptr;
static PFNGLBLENDFUNCIPROC glBlendFunci_ptr;
static PFNGLBLENDEQUATIONPROC glBlendEquation_ptr;
static PFNGLCLEARBUFFERFVPROC glClearBufferfv_ptr;
static PFNGLSHADERSOURCEPROC glShaderSource_ptr;
static PFNGLCOMPILESHADERPROC glCompileShader_ptr;
static PFNGLGETSHADERIVPROC glGetShaderiv_ptr;
static PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog_ptr;
static PFNGLATTACHSHADERPROC glAttachShader_ptr;
static PFNGLLINKPROGRAMPROC glLinkProgram_ptr;
static PFNGLVALIDATEPROGRAMPROC glValidateProgram_ptr;
static PFNGLGETPROGRAMIVPROC glGetProgramiv_ptr;
static PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog_ptr;
static PFNGLGENBUFFERSPROC glGenBuffers_ptr;
static PFNGLGENVERTEXARRAYSPROC glGenVertexArrays_ptr;
static PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation_ptr;
static PFNGLBINDVERTEXARRAYPROC glBindVertexArray_ptr;
static PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray_ptr;
static PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer_ptr;
static PFNGLBINDBUFFERPROC glBindBuffer_ptr;
static PFNGLBINDBUFFERBASEPROC glBindBufferBase_ptr;
static PFNGLBUFFERDATAPROC glBufferData_ptr;
static PFNGLGETVERTEXATTRIBPOINTERVPROC glGetVertexAttribPointerv_ptr;
static PFNGLUSEPROGRAMPROC glUseProgram_ptr;
static PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays_ptr;
static PFNGLDELETEBUFFERSPROC glDeleteBuffers_ptr;
static PFNGLDELETEPROGRAMPROC glDeleteProgram_ptr;
static PFNGLDETACHSHADERPROC glDetachShader_ptr;
static PFNGLDELETESHADERPROC glDeleteShader_ptr;
static PFNGLDRAWELEMENTSINSTANCEDPROC glDrawElementsInstanced_ptr;
static PFNGLGENERATEMIPMAPPROC glGenerateMipmap_ptr;
static PFNGLDEBUGMESSAGECALLBACKPROC glDebugMessageCallback_ptr;
 
 
void load_gl_functions()
{
  // Load OpenGL Functions from the Operating System / Graphics Card
  glCreateProgram_ptr = (PFNGLCREATEPROGRAMPROC)platform_load_gl_function("glCreateProgram");
  glDeleteTextures_ptr = (PFNGLDELETETEXTURESPROC)platform_load_gl_function("glDeleteTextures");
  glGenTextures_ptr = (PFNGLGENTEXTURESPROC)platform_load_gl_function("glGenTextures");
  glBindTexture_ptr = (PFNGLBINDTEXTUREPROC)platform_load_gl_function("glBindTexture");
  glDrawArrays_ptr = (PFNGLDRAWARRAYSPROC)platform_load_gl_function("glDrawArrays");
  glCreateShader_ptr = (PFNGLCREATESHADERPROC) platform_load_gl_function("glCreateShader");
  glGetUniformLocation_ptr = (PFNGLGETUNIFORMLOCATIONPROC) platform_load_gl_function("glGetUniformLocation");
  glUniform1f_ptr = (PFNGLUNIFORM1FPROC) platform_load_gl_function("glUniform1f");
  glUniform2fv_ptr = (PFNGLUNIFORM2FVPROC) platform_load_gl_function("glUniform2fv");
  glUniform3fv_ptr = (PFNGLUNIFORM3FVPROC) platform_load_gl_function("glUniform3fv");
  glUniform1i_ptr = (PFNGLUNIFORM1IPROC) platform_load_gl_function("glUniform1i");
  glUniformMatrix4fv_ptr = (PFNGLUNIFORMMATRIX4FVPROC) platform_load_gl_function("glUniformMatrix4fv");
  glVertexAttribDivisor_ptr = (PFNGLVERTEXATTRIBDIVISORPROC) platform_load_gl_function("glVertexAttribDivisor");
  glActiveTexture_ptr = (PFNGLACTIVETEXTUREPROC) platform_load_gl_function("glActiveTexture");
  glBufferSubData_ptr = (PFNGLBUFFERSUBDATAPROC) platform_load_gl_function("glBufferSubData");
  glDrawArraysInstanced_ptr = (PFNGLDRAWARRAYSINSTANCEDPROC) platform_load_gl_function("glDrawArraysInstanced");
  glBindFramebuffer_ptr = (PFNGLBINDFRAMEBUFFERPROC) platform_load_gl_function("glBindFramebuffer");
  glCheckFramebufferStatus_ptr = (PFNGLCHECKFRAMEBUFFERSTATUSPROC) platform_load_gl_function("glCheckFramebufferStatus");
  glGenFramebuffers_ptr = (PFNGLGENFRAMEBUFFERSPROC) platform_load_gl_function("glGenFramebuffers");
  glFramebufferTexture2D_ptr = (PFNGLFRAMEBUFFERTEXTURE2DPROC) platform_load_gl_function("glFramebufferTexture2D");
  glDrawBuffers_ptr = (PFNGLDRAWBUFFERSPROC) platform_load_gl_function("glDrawBuffers");
  glDeleteFramebuffers_ptr = (PFNGLDELETEFRAMEBUFFERSPROC) platform_load_gl_function("glDeleteFramebuffers");
  glBlendFunci_ptr = (PFNGLBLENDFUNCIPROC) platform_load_gl_function("glBlendFunci");
  glBlendEquation_ptr = (PFNGLBLENDEQUATIONPROC) platform_load_gl_function("glBlendEquation");
  glClearBufferfv_ptr = (PFNGLCLEARBUFFERFVPROC) platform_load_gl_function("glClearBufferfv");
  glShaderSource_ptr = (PFNGLSHADERSOURCEPROC) platform_load_gl_function("glShaderSource");
  glCompileShader_ptr = (PFNGLCOMPILESHADERPROC) platform_load_gl_function("glCompileShader");
  glGetShaderiv_ptr = (PFNGLGETSHADERIVPROC) platform_load_gl_function("glGetShaderiv");
  glGetShaderInfoLog_ptr = (PFNGLGETSHADERINFOLOGPROC) platform_load_gl_function("glGetShaderInfoLog");
  glAttachShader_ptr = (PFNGLATTACHSHADERPROC) platform_load_gl_function("glAttachShader");
  glLinkProgram_ptr = (PFNGLLINKPROGRAMPROC) platform_load_gl_function("glLinkProgram");
  glValidateProgram_ptr = (PFNGLVALIDATEPROGRAMPROC) platform_load_gl_function("glValidateProgram");
  glGetProgramiv_ptr = (PFNGLGETPROGRAMIVPROC) platform_load_gl_function("glGetProgramiv");
  glGetProgramInfoLog_ptr = (PFNGLGETPROGRAMINFOLOGPROC) platform_load_gl_function("glGetProgramInfoLog");
  glGenBuffers_ptr = (PFNGLGENBUFFERSPROC) platform_load_gl_function("glGenBuffers");
  glGenVertexArrays_ptr = (PFNGLGENVERTEXARRAYSPROC) platform_load_gl_function("glGenVertexArrays");
  glGetAttribLocation_ptr = (PFNGLGETATTRIBLOCATIONPROC) platform_load_gl_function("glGetAttribLocation");
  glBindVertexArray_ptr = (PFNGLBINDVERTEXARRAYPROC) platform_load_gl_function("glBindVertexArray");
  glEnableVertexAttribArray_ptr = (PFNGLENABLEVERTEXATTRIBARRAYPROC) platform_load_gl_function("glEnableVertexAttribArray");
  glVertexAttribPointer_ptr = (PFNGLVERTEXATTRIBPOINTERPROC) platform_load_gl_function("glVertexAttribPointer");
  glBindBuffer_ptr = (PFNGLBINDBUFFERPROC) platform_load_gl_function("glBindBuffer");
  glBindBufferBase_ptr = (PFNGLBINDBUFFERBASEPROC) platform_load_gl_function("glBindBufferBase");
  glBufferData_ptr = (PFNGLBUFFERDATAPROC) platform_load_gl_function("glBufferData");
  glGetVertexAttribPointerv_ptr = (PFNGLGETVERTEXATTRIBPOINTERVPROC) platform_load_gl_function("glGetVertexAttribPointerv");
  glUseProgram_ptr = (PFNGLUSEPROGRAMPROC) platform_load_gl_function("glUseProgram");
  glDeleteVertexArrays_ptr = (PFNGLDELETEVERTEXARRAYSPROC) platform_load_gl_function("glDeleteVertexArrays");
  glDeleteBuffers_ptr = (PFNGLDELETEBUFFERSPROC) platform_load_gl_function("glDeleteBuffers");
  glDeleteProgram_ptr = (PFNGLDELETEPROGRAMPROC) platform_load_gl_function("glDeleteProgram");
  glDetachShader_ptr = (PFNGLDETACHSHADERPROC) platform_load_gl_function("glDetachShader");
  glDeleteShader_ptr = (PFNGLDELETESHADERPROC) platform_load_gl_function("glDeleteShader");
  glDrawElementsInstanced_ptr = (PFNGLDRAWELEMENTSINSTANCEDPROC) platform_load_gl_function("glDrawElementsInstanced");
  glGenerateMipmap_ptr = (PFNGLGENERATEMIPMAPPROC) platform_load_gl_function("glGenerateMipmap");
  glDebugMessageCallback_ptr = (PFNGLDEBUGMESSAGECALLBACKPROC)platform_load_gl_function("glDebugMessageCallback");
}

