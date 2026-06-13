#ifndef IVY_GL_LOADER_H
#define IVY_GL_LOADER_H

#include <stdbool.h>


#include "ivy/types.h"

#define GL_COLOR_BUFFER_BIT             0x00004000
#define GL_DEPTH_BUFFER_BIT             0x00000100
#define GL_ARRAY_BUFFER                 0x8892
#define GL_LEQUAL                       0x0203
#define GL_DEPTH_TEST                   0x0B71
#define GL_SRC_ALPHA                    0x0302
#define GL_BLEND                        0x0BE2
#define GL_BACK                         0x0405
#define GL_ONE_MINUS_SRC_ALPHA          0x0303
#define GL_CCW                          0x0901
#define GL_CULL_FACE                    0x0B44
#define GL_TEXTURE_CUBE_MAP_SEAMLESS    0x884F
#define GL_FRAGMENT_SHADER              0x8B30
#define GL_VERTEX_SHADER                0x8B31
#define GL_COMPILE_STATUS               0x8B81
#define GL_LINK_STATUS                  0x8B82
#define GL_DYNAMIC_DRAW                 0x88E8
#define GL_UNSIGNED_BYTE                0x1401
#define GL_STATIC_DRAW                  0x88E4
#define GL_ELEMENT_ARRAY_BUFFER         0x8893
#define GL_FLOAT                        0x1406
#define GL_TEXTURE_2D                   0x0DE1
#define GL_UNPACK_ALIGNMENT             0x0CF5
#define GL_R16F                         0x822D
#define GL_RGBA32F                      0x8814
#define GL_RGBA16F                      0x881A
#define GL_RGB16F                       0x881B
#define GL_RGB32F                       0x8815
#define GL_RGB                          0x1907
#define GL_RED                          0x1903
#define GL_RGBA                         0x1908
#define GL_HALF_FLOAT                   0x140B
#define GL_RG                           0x8227
#define GL_R32F                         0x822E
#define GL_UNSIGNED_SHORT_5_6_5         0x8363
#define GL_UNSIGNED_SHORT_5_5_5_1       0x8034
#define GL_UNSIGNED_SHORT_4_4_4_4       0x8033
#define GL_R8                           0x8229
#define GL_RG8                          0x822B
#define GL_RGB565                       0x8D62
#define GL_RGB8                         0x8051
#define GL_RGB5_A1                      0x8057
#define GL_RGBA4                        0x8056
#define GL_RGBA8                        0x8058
#define GL_TEXTURE_SWIZZLE_RGBA         0x8E46
#define GL_ONE                          1
#define GL_GREEN                        0x1904
#define GL_TEXTURE_WRAP_S               0x2802
#define GL_TEXTURE_WRAP_T               0x2803
#define GL_TEXTURE_MAG_FILTER           0x2800
#define GL_REPEAT                       0x2901
#define GL_LINEAR                       0x2601
#define GL_NEAREST                      0x2600
#define GL_LINEAR_MIPMAP_LINEAR         0x2703
#define GL_TEXTURE_BASE_LEVEL           0x813C
#define GL_TEXTURE_MAX_LEVEL            0x813D
#define GL_TEXTURE_MIN_FILTER           0x2801
#define GL_FRAMEBUFFER 0x8D40
#define GL_DEPTH_COMPONENT 0x1902
#define GL_RENDERBUFFER 0x8D41
#define GL_COLOR_ATTACHMENT0 0x8CE0
#define GL_DEPTH_ATTACHMENT 0x8D00
#define GL_TEXTURE 0x1702
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME 0x8CD1
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE 0x8CD0
#define GL_TRUE 1
#define GL_UNSIGNED_INT 0x1405
#define GL_TRIANGLES 0x0004
#define GL_TEXTURE0 0x84C0
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X 0x8515

typedef void (*PFNGLBINDBUFFERPROC)(u32 target, u32 buffer);
typedef void (*PFNGLBINDVERTEXARRAYPROC)(u32 array);
typedef void (*PFNGLBUFFERSUBDATAPROC)(u32 target, ptrdiff_t offset, ptrdiff_t size, const void *data);
typedef void (*PFNGLCLEARCOLORPROC)(float red, float green, float blue, float alpha);
typedef void (*PFNGLCLEARPROC)(u32 mask);
typedef void (*PFNGLDEPTHFUNCPROC)(u32 func);
typedef void (*PFNGLDISABLEPROC)(u32 cap);
typedef void (*PFNGLBLENDFUNCPROC)(u32 sfactor, u32 dfactor);
typedef void (*PFNGLENABLEPROC)(u32 cap);
typedef void (*PFNGLCULLFACEPROC)(u32 mode);
typedef void (*PFNGLFRONTFACEPROC)(u32 mode);
typedef void (*PFNGLCLEARDEPTHPROC)(double depth);
typedef void (*PFNGLUSEPROGRAMPROC)(u32 program);
typedef void (*PFNGLDETACHSHADERPROC)(u32 program, u32 shader);
typedef void (*PFNGLDELETESHADERPROC)(u32 shader);
typedef void (*PFNGLDELETEPROGRAMPROC)(u32 program);
typedef u32  (*PFNGLCREATESHADERPROC)(u32 type);
typedef void (*PFNGLSHADERSOURCEPROC)(unsigned shader, int count, const char **string, const int *length);
typedef void (*PFNGLCOMPILESHADERPROC)(u32 shader);
typedef void (*PFNGLGETSHADERIVPROC)(u32 shader, u32 pname, int *params);
typedef u32  (*PFNGLCREATEPROGRAMPROC)(void);
typedef void (*PFNGLATTACHSHADERPROC)(u32 program, u32 shader);
typedef void (*PFNGLBINDATTRIBLOCATIONPROC)(u32 program, u32 index, const char *name);
typedef void (*PFNGLLINKPROGRAMPROC)(u32 program);
typedef void (*PFNGLGETPROGRAMIVPROC)(u32 program, u32 pname, int *params);
typedef int  (*PFNGLGETATTRIBLOCATIONPROC)(u32 program, const char *name);
typedef int  (*PFNGLGETUNIFORMLOCATIONPROC)(u32 program, const char *name);
typedef void (*PFNGLGENBUFFERSPROC)(int n, u32 *buffers);
typedef void (*PFNGLBUFFERDATAPROC)(u32 target, ptrdiff_t size, const void *data, u32 usage);
typedef void (*PFNGLENABLEVERTEXATTRIBARRAYPROC)(u32 index);
typedef void (*PFNGLVERTEXATTRIBPOINTERPROC)(u32 index, int size, u32 type, u8 normalized, int stride, const void *pointer);
typedef void (*PFNGLGENVERTEXARRAYSPROC)(int n, u32 *arrays);
typedef void (*PFNGLBINDTEXTUREPROC)(u32 target, u32 texture);
typedef void (*PFNGLPIXELSTOREIPROC)(u32 pname, int param);
typedef void (*PFNGLGENTEXTURESPROC)(int n, u32 *textures);
typedef void (*PFNGLTEXIMAGE2DPROC)(u32 target, int level, int internalformat, int width, int height, int border, u32 format, u32 type, const void *pixels);
typedef void (*PFNGLTEXPARAMETERIVPROC)(u32 target, u32 pname, const int *params);
typedef void (*PFNGLTEXPARAMETERIPROC)(u32 target, u32 pname, int param);

typedef void (*PFNGLGENFRAMEBUFFERSPROC)(int n, u32 *framebuffers);
typedef void (*PFNGLBINDFRAMEBUFFERPROC)(u32 target, u32 framebuffer);
typedef void (*PFNGLGENRENDERBUFFERSPROC)(int n, u32 *renderbuffers);
typedef void (*PFNGLBINDRENDERBUFFERPROC)(u32 target, u32 renderbuffer);
typedef void (*PFNGLFRAMEBUFFERTEXTUREPROC)(u32 target, u32 attachment, u32 texture, int level);
typedef void (*PFNGLFRAMEBUFFERRENDERBUFFERPROC)(u32 target, u32 attachment, u32 renderbuffertarget, u32 renderbuffer);
typedef void (*PFNGLRENDERBUFFERSTORAGEPROC)(u32 target, u32 internalformat, int width, int height);
typedef void (*PFNGLFRAMEBUFFERTEXTURE2DPROC)(u32 target, u32 attachment, u32 textarget, u32 texture, int level);
typedef void (*PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)(u32 target, u32 attachment, u32 pname, int *params);
typedef void (*PFNGLDELETERENDERBUFFERSPROC)(int n, const u32 *renderbuffers);
typedef void (*PFNGLDELETETEXTURESPROC)(int n, const u32 *textures);
typedef void (*PFNGLDELETEFRAMEBUFFERSPROC)(int n, const u32 *framebuffers);
typedef void (*PFNGLVIEWPORTPROC)(int x, int y, int width, int height);
typedef void (*PFNGLUNIFORMMATRIX4FVPROC)(int location, int count, bool transpose, const float *value);
typedef void (*PFNGLUNIFORM4FPROC)(int location, float v0, float v1, float v2, float v3);
typedef void (*PFNGLUNIFORM1IPROC)(int location, int v0);
typedef void (*PFNGLACTIVETEXTUREPROC)(u32 texture);
typedef void (*PFNGLDRAWELEMENTSPROC)(u32 mode, int count, u32 type, const void *indices);
typedef void (*PFNGLDRAWARRAYSPROC)(u32 mode, int first, int count);

extern PFNGLBINDBUFFERPROC              glBindBuffer;
extern PFNGLBINDVERTEXARRAYPROC         glBindVertexArray;
extern PFNGLBUFFERSUBDATAPROC           glBufferSubData;
extern PFNGLCLEARCOLORPROC              glClearColor;
extern PFNGLCLEARPROC                   glClear;
extern PFNGLDEPTHFUNCPROC               glDepthFunc;
extern PFNGLDISABLEPROC                 glDisable;
extern PFNGLBLENDFUNCPROC               glBlendFunc;
extern PFNGLENABLEPROC                  glEnable;
extern PFNGLCULLFACEPROC                glCullFace;
extern PFNGLFRONTFACEPROC               glFrontFace;
extern PFNGLCLEARDEPTHPROC              glClearDepth;
extern PFNGLUSEPROGRAMPROC              glUseProgram;
extern PFNGLDETACHSHADERPROC            glDetachShader;
extern PFNGLDELETESHADERPROC            glDeleteShader;
extern PFNGLDELETEPROGRAMPROC           glDeleteProgram;
extern PFNGLCREATESHADERPROC            glCreateShader;
extern PFNGLSHADERSOURCEPROC            glShaderSource;
extern PFNGLCOMPILESHADERPROC           glCompileShader;
extern PFNGLGETSHADERIVPROC             glGetShaderiv;
extern PFNGLCREATEPROGRAMPROC           glCreateProgram;
extern PFNGLATTACHSHADERPROC            glAttachShader;
extern PFNGLBINDATTRIBLOCATIONPROC      glBindAttribLocation;
extern PFNGLLINKPROGRAMPROC             glLinkProgram;
extern PFNGLGETPROGRAMIVPROC            glGetProgramiv;
extern PFNGLGETATTRIBLOCATIONPROC       glGetAttribLocation;
extern PFNGLGETUNIFORMLOCATIONPROC      glGetUniformLocation;
extern PFNGLGENBUFFERSPROC              glGenBuffers;
extern PFNGLBUFFERDATAPROC              glBufferData;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
extern PFNGLVERTEXATTRIBPOINTERPROC     glVertexAttribPointer;
extern PFNGLGENVERTEXARRAYSPROC         glGenVertexArrays;
extern PFNGLBINDTEXTUREPROC             glBindTexture;
extern PFNGLPIXELSTOREIPROC             glPixelStorei;
extern PFNGLGENTEXTURESPROC             glGenTextures;
extern PFNGLTEXIMAGE2DPROC              glTexImage2D;
extern PFNGLTEXPARAMETERIVPROC          glTexParameteriv;
extern PFNGLTEXPARAMETERIPROC           glTexParameteri;

// BARU ----
extern PFNGLGENFRAMEBUFFERSPROC         glGenFramebuffers;
extern PFNGLBINDFRAMEBUFFERPROC         glBindFramebuffer;
extern PFNGLGENRENDERBUFFERSPROC        glGenRenderbuffers;
extern PFNGLBINDRENDERBUFFERPROC        glBindRenderbuffer;
extern PFNGLFRAMEBUFFERTEXTUREPROC      glFramebufferTexture;
extern PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer;
extern PFNGLRENDERBUFFERSTORAGEPROC     glRenderbufferStorage;
extern PFNGLFRAMEBUFFERTEXTURE2DPROC    glFramebufferTexture2D;
extern PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC glGetFramebufferAttachmentParameteriv;
extern PFNGLDELETERENDERBUFFERSPROC     glDeleteRenderbuffers;
extern PFNGLDELETETEXTURESPROC          glDeleteTextures;
extern PFNGLDELETEFRAMEBUFFERSPROC      glDeleteFramebuffers;
extern PFNGLVIEWPORTPROC                glViewport;
extern PFNGLUNIFORMMATRIX4FVPROC        glUniformMatrix4fv;
extern PFNGLUNIFORM4FPROC               glUniform4f;
extern PFNGLUNIFORM1IPROC               glUniform1i;
extern PFNGLACTIVETEXTUREPROC           glActiveTexture;
extern PFNGLDRAWELEMENTSPROC            glDrawElements;
extern PFNGLDRAWARRAYSPROC              glDrawArrays;

#endif
