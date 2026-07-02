#ifndef IVY_GL_H
#define IVY_GL_H

#include <stddef.h>

#ifdef _WIN32
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #include <windows.h>
    #include <GL/gl.h>
#else
    #define GL_GLEXT_PROTOTYPES
    #include <GL/gl.h>
#endif

#include "ivy/gfx.h"

#ifndef GL_ARRAY_BUFFER
#define GL_ARRAY_BUFFER 0x8892
#endif
#ifndef GL_ELEMENT_ARRAY_BUFFER
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#endif
#ifndef GL_STATIC_DRAW
#define GL_STATIC_DRAW 0x88E4
#endif
#ifndef GL_DYNAMIC_DRAW
#define GL_DYNAMIC_DRAW 0x88E8
#endif
#ifndef GL_FRAGMENT_SHADER
#define GL_FRAGMENT_SHADER 0x8B30
#endif
#ifndef GL_VERTEX_SHADER
#define GL_VERTEX_SHADER 0x8B31
#endif
#ifndef GL_COMPILE_STATUS
#define GL_COMPILE_STATUS 0x8B81
#endif
#ifndef GL_LINK_STATUS
#define GL_LINK_STATUS 0x8B82
#endif
#ifndef GL_TEXTURE0
#define GL_TEXTURE0 0x84C0
#endif
#ifndef GL_FRAMEBUFFER
#define GL_FRAMEBUFFER 0x8D40
#endif
#ifndef GL_READ_FRAMEBUFFER
#define GL_READ_FRAMEBUFFER 0x8CA8
#endif
#ifndef GL_DRAW_FRAMEBUFFER
#define GL_DRAW_FRAMEBUFFER 0x8CA9
#endif
#ifndef GL_RENDERBUFFER
#define GL_RENDERBUFFER 0x8D41
#endif
#ifndef GL_COLOR_ATTACHMENT0
#define GL_COLOR_ATTACHMENT0 0x8CE0
#endif
#ifndef GL_DEPTH_ATTACHMENT
#define GL_DEPTH_ATTACHMENT 0x8D00
#endif
#ifndef GL_DEPTH_COMPONENT
#define GL_DEPTH_COMPONENT 0x1902
#endif
#ifndef GL_DEPTH_COMPONENT24
#define GL_DEPTH_COMPONENT24 0x81A6
#endif
#ifndef GL_FRAMEBUFFER_COMPLETE
#define GL_FRAMEBUFFER_COMPLETE 0x8CD5
#endif
#ifndef GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE 0x8CD0
#endif
#ifndef GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME 0x8CD1
#endif
#ifndef GL_TEXTURE_BASE_LEVEL
#define GL_TEXTURE_BASE_LEVEL 0x813C
#endif
#ifndef GL_TEXTURE_MAX_LEVEL
#define GL_TEXTURE_MAX_LEVEL 0x813D
#endif
#ifndef GL_RG
#define GL_RG 0x8227
#endif
#ifndef GL_RG8
#define GL_RG8 0x822B
#endif
#ifndef GL_RGBA8
#define GL_RGBA8 0x8058
#endif
#ifndef GL_TEXTURE_SWIZZLE_R
#define GL_TEXTURE_SWIZZLE_R 0x8E42
#endif
#ifndef GL_TEXTURE_SWIZZLE_G
#define GL_TEXTURE_SWIZZLE_G 0x8E43
#endif
#ifndef GL_TEXTURE_SWIZZLE_B
#define GL_TEXTURE_SWIZZLE_B 0x8E44
#endif
#ifndef GL_TEXTURE_SWIZZLE_A
#define GL_TEXTURE_SWIZZLE_A 0x8E45
#endif
#ifndef GL_TEXTURE_CUBE_MAP_SEAMLESS
#define GL_TEXTURE_CUBE_MAP_SEAMLESS 0x884F
#endif
#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE 0x809D
#endif
#ifndef GL_NUM_EXTENSIONS
#define GL_NUM_EXTENSIONS 0x821D
#endif

#ifndef APIENTRY
#define APIENTRY
#endif
#ifndef GLAPIENTRY
#define GLAPIENTRY APIENTRY
#endif

typedef char GLchar;
typedef ptrdiff_t GLintptr;
typedef ptrdiff_t GLsizeiptr;

typedef void (GLAPIENTRY * PFNGLACTIVETEXTUREPROC) (GLenum texture);
typedef void (GLAPIENTRY * PFNGLATTACHSHADERPROC) (GLuint program, GLuint shader);
typedef void (GLAPIENTRY * PFNGLBINDATTRIBLOCATIONPROC) (GLuint program, GLuint index, const GLchar *name);
typedef void (GLAPIENTRY * PFNGLBINDBUFFERPROC) (GLenum target, GLuint buffer);
typedef void (GLAPIENTRY * PFNGLBINDFRAMEBUFFERPROC) (GLenum target, GLuint framebuffer);
typedef void (GLAPIENTRY * PFNGLBINDRENDERBUFFERPROC) (GLenum target, GLuint renderbuffer);
typedef void (GLAPIENTRY * PFNGLBINDVERTEXARRAYPROC) (GLuint array);
typedef void (GLAPIENTRY * PFNGLBUFFERDATAPROC) (GLenum target, GLsizeiptr size, const void *data, GLenum usage);
typedef void (GLAPIENTRY * PFNGLBUFFERSUBDATAPROC) (GLenum target, GLintptr offset, GLsizeiptr size, const void *data);
typedef void (GLAPIENTRY * PFNGLCOMPILESHADERPROC) (GLuint shader);
typedef GLuint (GLAPIENTRY * PFNGLCREATEPROGRAMPROC) (void);
typedef GLuint (GLAPIENTRY * PFNGLCREATESHADERPROC) (GLenum type);
typedef void (GLAPIENTRY * PFNGLDELETEFRAMEBUFFERSPROC) (GLsizei n, const GLuint *framebuffers);
typedef void (GLAPIENTRY * PFNGLDELETEPROGRAMPROC) (GLuint program);
typedef void (GLAPIENTRY * PFNGLDELETERENDERBUFFERSPROC) (GLsizei n, const GLuint *renderbuffers);
typedef void (GLAPIENTRY * PFNGLDELETESHADERPROC) (GLuint shader);
typedef void (GLAPIENTRY * PFNGLDETACHSHADERPROC) (GLuint program, GLuint shader);
typedef void (GLAPIENTRY * PFNGLENABLEVERTEXATTRIBARRAYPROC) (GLuint index);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERRENDERBUFFERPROC) (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTURE2DPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (GLAPIENTRY * PFNGLGENBUFFERSPROC) (GLsizei n, GLuint *buffers);
typedef void (GLAPIENTRY * PFNGLGENFRAMEBUFFERSPROC) (GLsizei n, GLuint *framebuffers);
typedef void (GLAPIENTRY * PFNGLGENRENDERBUFFERSPROC) (GLsizei n, GLuint *renderbuffers);
typedef void (GLAPIENTRY * PFNGLGENVERTEXARRAYSPROC) (GLsizei n, GLuint *arrays);
typedef GLint (GLAPIENTRY * PFNGLGETATTRIBLOCATIONPROC) (GLuint program, const GLchar *name);
typedef GLenum (GLAPIENTRY * PFNGLCHECKFRAMEBUFFERSTATUSPROC) (GLenum target);
typedef void (GLAPIENTRY * PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC) (GLenum target, GLenum attachment, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMINFOLOGPROC) (GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMIVPROC) (GLuint program, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * PFNGLGETSHADERINFOLOGPROC) (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void (GLAPIENTRY * PFNGLGETSHADERIVPROC) (GLuint shader, GLenum pname, GLint *params);
typedef GLint (GLAPIENTRY * PFNGLGETUNIFORMLOCATIONPROC) (GLuint program, const GLchar *name);
typedef void (GLAPIENTRY * PFNGLLINKPROGRAMPROC) (GLuint program);
typedef void (GLAPIENTRY * PFNGLRENDERBUFFERSTORAGEPROC) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC) (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLBLITFRAMEBUFFERPROC) (GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
typedef void (GLAPIENTRY * PFNGLGETINTEGERVPROC) (GLenum pname, GLint *data);
typedef void (GLAPIENTRY * PFNGLSHADERSOURCEPROC) (GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length);
typedef void (GLAPIENTRY * PFNGLUNIFORM1IPROC) (GLint location, GLint v0);
typedef void (GLAPIENTRY * PFNGLUNIFORM4FPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX4FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GLAPIENTRY * PFNGLUSEPROGRAMPROC) (GLuint program);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBPOINTERPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);

typedef void (GLAPIENTRY * PFNGLVIEWPORTPROC) (GLint x, GLint y, GLsizei width, GLsizei height);
typedef GLenum (GLAPIENTRY * PFNGLGETERRORPROC) (void);
typedef void (GLAPIENTRY * PFNGLDEPTHFUNCPROC) (GLenum func);
typedef void (GLAPIENTRY * PFNGLDISABLEPROC) (GLenum cap);
typedef void (GLAPIENTRY * PFNGLBLENDFUNCPROC) (GLenum sfactor, GLenum dfactor);
typedef void (GLAPIENTRY * PFNGLENABLEPROC) (GLenum cap);
typedef void (GLAPIENTRY * PFNGLCULLFACEPROC) (GLenum mode);
typedef void (GLAPIENTRY * PFNGLFRONTFACEPROC) (GLenum mode);
typedef void (GLAPIENTRY * PFNGLCLEARCOLORPROC) (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void (GLAPIENTRY * PFNGLCLEARDEPTHPROC) (GLdouble depth);
typedef void (GLAPIENTRY * PFNGLCLEARPROC) (GLbitfield mask);
typedef void (GLAPIENTRY * PFNGLBINDTEXTUREPROC) (GLenum target, GLuint texture);
typedef void (GLAPIENTRY * PFNGLPIXELSTOREIPROC) (GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLGENTEXTURESPROC) (GLsizei n, GLuint *textures);
typedef void (GLAPIENTRY * PFNGLTEXIMAGE2DPROC) (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
typedef void (GLAPIENTRY * PFNGLTEXPARAMETERIPROC) (GLenum target, GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLDELETETEXTURESPROC) (GLsizei n, const GLuint *textures);
typedef void (GLAPIENTRY * PFNGLDRAWARRAYSPROC) (GLenum mode, GLint first, GLsizei count);
typedef void (GLAPIENTRY * PFNGLDRAWELEMENTSPROC) (GLenum mode, GLsizei count, GLenum type, const void *indices);
typedef void (GLAPIENTRY * PFNGLREADPIXELSPROC) (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels);

#ifdef IVY_GL_IMPLEMENTATION
    #define IVY_GL_API
#else
    #define IVY_GL_API extern
#endif

IVY_GL_API PFNGLACTIVETEXTUREPROC ivy_glActiveTexture;
IVY_GL_API PFNGLATTACHSHADERPROC ivy_glAttachShader;
IVY_GL_API PFNGLBINDATTRIBLOCATIONPROC ivy_glBindAttribLocation;
IVY_GL_API PFNGLBINDBUFFERPROC ivy_glBindBuffer;
IVY_GL_API PFNGLBINDFRAMEBUFFERPROC ivy_glBindFramebuffer;
IVY_GL_API PFNGLBINDRENDERBUFFERPROC ivy_glBindRenderbuffer;
IVY_GL_API PFNGLBINDVERTEXARRAYPROC ivy_glBindVertexArray;
IVY_GL_API PFNGLBUFFERDATAPROC ivy_glBufferData;
IVY_GL_API PFNGLBUFFERSUBDATAPROC ivy_glBufferSubData;
IVY_GL_API PFNGLCOMPILESHADERPROC ivy_glCompileShader;
IVY_GL_API PFNGLCREATEPROGRAMPROC ivy_glCreateProgram;
IVY_GL_API PFNGLCREATESHADERPROC ivy_glCreateShader;
IVY_GL_API PFNGLDELETEFRAMEBUFFERSPROC ivy_glDeleteFramebuffers;
IVY_GL_API PFNGLDELETEPROGRAMPROC ivy_glDeleteProgram;
IVY_GL_API PFNGLDELETERENDERBUFFERSPROC ivy_glDeleteRenderbuffers;
IVY_GL_API PFNGLDELETESHADERPROC ivy_glDeleteShader;
IVY_GL_API PFNGLDETACHSHADERPROC ivy_glDetachShader;
IVY_GL_API PFNGLENABLEVERTEXATTRIBARRAYPROC ivy_glEnableVertexAttribArray;
IVY_GL_API PFNGLFRAMEBUFFERRENDERBUFFERPROC ivy_glFramebufferRenderbuffer;
IVY_GL_API PFNGLFRAMEBUFFERTEXTURE2DPROC ivy_glFramebufferTexture2D;
IVY_GL_API PFNGLGENBUFFERSPROC ivy_glGenBuffers;
IVY_GL_API PFNGLGENFRAMEBUFFERSPROC ivy_glGenFramebuffers;
IVY_GL_API PFNGLGENRENDERBUFFERSPROC ivy_glGenRenderbuffers;
IVY_GL_API PFNGLGENVERTEXARRAYSPROC ivy_glGenVertexArrays;
IVY_GL_API PFNGLGETATTRIBLOCATIONPROC ivy_glGetAttribLocation;
IVY_GL_API PFNGLCHECKFRAMEBUFFERSTATUSPROC ivy_glCheckFramebufferStatus;
IVY_GL_API PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC ivy_glGetFramebufferAttachmentParameteriv;
IVY_GL_API PFNGLGETPROGRAMINFOLOGPROC ivy_glGetProgramInfoLog;
IVY_GL_API PFNGLGETPROGRAMIVPROC ivy_glGetProgramiv;
IVY_GL_API PFNGLGETSHADERINFOLOGPROC ivy_glGetShaderInfoLog;
IVY_GL_API PFNGLGETSHADERIVPROC ivy_glGetShaderiv;
IVY_GL_API PFNGLGETUNIFORMLOCATIONPROC ivy_glGetUniformLocation;
IVY_GL_API PFNGLLINKPROGRAMPROC ivy_glLinkProgram;
IVY_GL_API PFNGLRENDERBUFFERSTORAGEPROC ivy_glRenderbufferStorage;
IVY_GL_API PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC ivy_glRenderbufferStorageMultisample;
IVY_GL_API PFNGLBLITFRAMEBUFFERPROC ivy_glBlitFramebuffer;
IVY_GL_API PFNGLGETINTEGERVPROC ivy_glGetIntegerv;
IVY_GL_API PFNGLSHADERSOURCEPROC ivy_glShaderSource;
IVY_GL_API PFNGLUNIFORM1IPROC ivy_glUniform1i;
IVY_GL_API PFNGLUNIFORM4FPROC ivy_glUniform4f;
IVY_GL_API PFNGLUNIFORMMATRIX4FVPROC ivy_glUniformMatrix4fv;
IVY_GL_API PFNGLUSEPROGRAMPROC ivy_glUseProgram;
IVY_GL_API PFNGLVERTEXATTRIBPOINTERPROC ivy_glVertexAttribPointer;

IVY_GL_API PFNGLVIEWPORTPROC ivy_glViewport;
IVY_GL_API PFNGLGETERRORPROC ivy_glGetError;
IVY_GL_API PFNGLDEPTHFUNCPROC ivy_glDepthFunc;
IVY_GL_API PFNGLDISABLEPROC ivy_glDisable;
IVY_GL_API PFNGLBLENDFUNCPROC ivy_glBlendFunc;
IVY_GL_API PFNGLENABLEPROC ivy_glEnable;
IVY_GL_API PFNGLCULLFACEPROC ivy_glCullFace;
IVY_GL_API PFNGLFRONTFACEPROC ivy_glFrontFace;
IVY_GL_API PFNGLCLEARCOLORPROC ivy_glClearColor;
IVY_GL_API PFNGLCLEARDEPTHPROC ivy_glClearDepth;
IVY_GL_API PFNGLCLEARPROC ivy_glClear;
IVY_GL_API PFNGLBINDTEXTUREPROC ivy_glBindTexture;
IVY_GL_API PFNGLPIXELSTOREIPROC ivy_glPixelStorei;
IVY_GL_API PFNGLGENTEXTURESPROC ivy_glGenTextures;
IVY_GL_API PFNGLTEXIMAGE2DPROC ivy_glTexImage2D;
IVY_GL_API PFNGLTEXPARAMETERIPROC ivy_glTexParameteri;
IVY_GL_API PFNGLDELETETEXTURESPROC ivy_glDeleteTextures;
IVY_GL_API PFNGLDRAWARRAYSPROC ivy_glDrawArrays;
IVY_GL_API PFNGLDRAWELEMENTSPROC ivy_glDrawElements;
IVY_GL_API PFNGLREADPIXELSPROC ivy_glReadPixels;

#define glActiveTexture ivy_glActiveTexture
#define glAttachShader ivy_glAttachShader
#define glBindAttribLocation ivy_glBindAttribLocation
#define glBindBuffer ivy_glBindBuffer
#define glBindFramebuffer ivy_glBindFramebuffer
#define glBindRenderbuffer ivy_glBindRenderbuffer
#define glBindVertexArray ivy_glBindVertexArray
#define glBufferData ivy_glBufferData
#define glBufferSubData ivy_glBufferSubData
#define glCompileShader ivy_glCompileShader
#define glCreateProgram ivy_glCreateProgram
#define glCreateShader ivy_glCreateShader
#define glDeleteFramebuffers ivy_glDeleteFramebuffers
#define glDeleteProgram ivy_glDeleteProgram
#define glDeleteRenderbuffers ivy_glDeleteRenderbuffers
#define glDeleteShader ivy_glDeleteShader
#define glDetachShader ivy_glDetachShader
#define glEnableVertexAttribArray ivy_glEnableVertexAttribArray
#define glFramebufferRenderbuffer ivy_glFramebufferRenderbuffer
#define glFramebufferTexture2D ivy_glFramebufferTexture2D
#define glGenBuffers ivy_glGenBuffers
#define glGenFramebuffers ivy_glGenFramebuffers
#define glGenRenderbuffers ivy_glGenRenderbuffers
#define glGenVertexArrays ivy_glGenVertexArrays
#define glGetAttribLocation ivy_glGetAttribLocation
#define glCheckFramebufferStatus ivy_glCheckFramebufferStatus
#define glGetFramebufferAttachmentParameteriv ivy_glGetFramebufferAttachmentParameteriv
#define glGetProgramInfoLog ivy_glGetProgramInfoLog
#define glGetProgramiv ivy_glGetProgramiv
#define glGetShaderInfoLog ivy_glGetShaderInfoLog
#define glGetShaderiv ivy_glGetShaderiv
#define glGetUniformLocation ivy_glGetUniformLocation
#define glLinkProgram ivy_glLinkProgram
#define glRenderbufferStorage ivy_glRenderbufferStorage
#define glRenderbufferStorageMultisample ivy_glRenderbufferStorageMultisample
#define glBlitFramebuffer ivy_glBlitFramebuffer
#define glGetIntegerv ivy_glGetIntegerv
#define glShaderSource ivy_glShaderSource
#define glUniform1i ivy_glUniform1i
#define glUniform4f ivy_glUniform4f
#define glUniformMatrix4fv ivy_glUniformMatrix4fv
#define glUseProgram ivy_glUseProgram
#define glVertexAttribPointer ivy_glVertexAttribPointer

#define glViewport ivy_glViewport
#define glGetError ivy_glGetError
#define glDepthFunc ivy_glDepthFunc
#define glDisable ivy_glDisable
#define glBlendFunc ivy_glBlendFunc
#define glEnable ivy_glEnable
#define glCullFace ivy_glCullFace
#define glFrontFace ivy_glFrontFace
#define glClearColor ivy_glClearColor
#define glClearDepth ivy_glClearDepth
#define glClear ivy_glClear
#define glBindTexture ivy_glBindTexture
#define glPixelStorei ivy_glPixelStorei
#define glGenTextures ivy_glGenTextures
#define glTexImage2D ivy_glTexImage2D
#define glTexParameteri ivy_glTexParameteri
#define glDeleteTextures ivy_glDeleteTextures
#define glDrawArrays ivy_glDrawArrays
#define glDrawElements ivy_glDrawElements
#define glReadPixels ivy_glReadPixels

bool Ivy_GL_LoadProcs(IvyGLLoadProc loader);

#endif
