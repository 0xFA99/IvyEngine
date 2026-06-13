#ifndef IVY_GFX_H
#define IVY_GFX_H

#include "ivy/types.h"
#include "ivy/math.h"

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef IVY_DEFAULT_SHADER_ATTRIB_NAME_POSITION
    #define IVY_DEFAULT_SHADER_ATTRIB_NAME_POSITION     "vertexPosition"
#endif
#ifndef IVY_DEFAULT_SHADER_ATTRIB_NAME_TEXCOORD
    #define IVY_DEFAULT_SHADER_ATTRIB_NAME_TEXCOORD     "vertexTexCoord"
#endif
#ifndef IVY_DEFAULT_SHADER_ATTRIB_NAME_NORMAL
    #define IVY_DEFAULT_SHADER_ATTRIB_NAME_NORMAL       "vertexNormal"
#endif
#ifndef IVY_DEFAULT_SHADER_ATTRIB_NAME_COLOR
    #define IVY_DEFAULT_SHADER_ATTRIB_NAME_COLOR        "vertexColor"
#endif
#ifndef IVY_DEFAULT_SHADER_ATTRIB_NAME_TANGENT
    #define IVY_DEFAULT_SHADER_ATTRIB_NAME_TANGENT      "vertexTangent"
#endif
#ifndef IVY_DEFAULT_SHADER_ATTRIB_NAME_TEXCOORD2
    #define IVY_DEFAULT_SHADER_ATTRIB_NAME_TEXCOORD2    "vertexTexCoord2"
#endif
#ifndef IVY_DEFAULT_SHADER_ATTRIB_NAME_BONEINDICES
    #define IVY_DEFAULT_SHADER_ATTRIB_NAME_BONEINDICES  "vertexBoneIndices"
#endif
#ifndef IVY_DEFAULT_SHADER_ATTRIB_NAME_BONEWEIGHTS
    #define IVY_DEFAULT_SHADER_ATTRIB_NAME_BONEWEIGHTS  "vertexBoneWeights"
#endif
#ifndef IVY_DEFAULT_SHADER_ATTRIB_NAME_INSTANCETRANSFORM
    #define IVY_DEFAULT_SHADER_ATTRIB_NAME_INSTANCETRANSFORM "instanceTransform"
#endif

// shader uniform/sampler
#ifndef IVY_DEFAULT_SHADER_UNIFORM_NAME_MVP
    #define IVY_DEFAULT_SHADER_UNIFORM_NAME_MVP         "mvp"
#endif
#ifndef IVY_DEFAULT_SHADER_UNIFORM_NAME_COLOR
    #define IVY_DEFAULT_SHADER_UNIFORM_NAME_COLOR       "colDiffuse"
#endif
#ifndef IVY_DEFAULT_SHADER_SAMPLER2D_NAME_TEXTURE0
    #define IVY_DEFAULT_SHADER_SAMPLER2D_NAME_TEXTURE0  "texture0"
#endif

// shader attribute locations
#ifndef IVY_DEFAULT_SHADER_ATTRIB_LOCATION_POSITION
    #define IVY_DEFAULT_SHADER_ATTRIB_LOCATION_POSITION             0
#endif
#ifndef IVY_DEFAULT_SHADER_ATTRIB_LOCATION_TEXCOORD
    #define IVY_DEFAULT_SHADER_ATTRIB_LOCATION_TEXCOORD             1
#endif
#ifndef IVY_DEFAULT_SHADER_ATTRIB_LOCATION_NORMAL
    #define IVY_DEFAULT_SHADER_ATTRIB_LOCATION_NORMAL               2
#endif
#ifndef IVY_DEFAULT_SHADER_ATTRIB_LOCATION_COLOR
    #define IVY_DEFAULT_SHADER_ATTRIB_LOCATION_COLOR                3
#endif
#ifndef IVY_DEFAULT_SHADER_ATTRIB_LOCATION_TANGENT
    #define IVY_DEFAULT_SHADER_ATTRIB_LOCATION_TANGENT              4
#endif
#ifndef IVY_DEFAULT_SHADER_ATTRIB_LOCATION_TEXCOORD2
    #define IVY_DEFAULT_SHADER_ATTRIB_LOCATION_TEXCOORD2            5
#endif
#ifndef IVY_DEFAULT_SHADER_ATTRIB_LOCATION_BONEINDICES
    #define IVY_DEFAULT_SHADER_ATTRIB_LOCATION_BONEINDICES          7
#endif
#ifndef IVY_DEFAULT_SHADER_ATTRIB_LOCATION_BONEWEIGHTS
    #define IVY_DEFAULT_SHADER_ATTRIB_LOCATION_BONEWEIGHTS          8
#endif
#ifndef IVY_DEFAULT_SHADER_ATTRIB_LOCATION_INSTANCETRANSFORM
    #define IVY_DEFAULT_SHADER_ATTRIB_LOCATION_INSTANCETRANSFORM    9
#endif
#ifndef IVY_MAX_SHADER_LOCATIONS
    #define IVY_MAX_SHADER_LOCATIONS                                 32
#endif

#ifndef IVY_DEFAULT_BATCH_DRAWCALLS
    #define IVY_DEFAULT_BATCH_DRAWCALLS             256
#endif
#ifndef IVY_DEFAULT_BATCH_MAX_TEXTURE_UNITS
    #define IVY_DEFAULT_BATCH_MAX_TEXTURE_UNITS       4
#endif

// Misc default defines
#ifndef IVY_MAX_MATRIX_STACK_SIZE
    #define IVY_MAX_MATRIX_STACK_SIZE                32
#endif
#ifndef IVY_DEFAULT_BATCH_BUFFERS
    #define IVY_DEFAULT_BATCH_BUFFERS                 1
#endif

#define IVY_DEFAULT_BATCH_BUFFER_ELEMENTS  8192
#define IVY_SHADER_LOC_MAP_DIFFUSE         IVY_SHADER_LOC_MAP_ALBEDO

#define IVY_LINES                                0x0001
#define IVY_TRIANGLES                            0x0004
#define IVY_QUADS                                0x0007

#define IVY_MODELVIEW                            0x1700
#define IVY_PROJECTION                           0x1701
#define IVY_TEXTURE                              0x1702

// #define GL_LINES 0x0001
// #define GL_TRIANGLES 0x0004
// #define GL_QUADS 0x0007

typedef void *(*IvyGLLoadProc)(const char *name);

typedef enum {
    IVY_VERTEX_POSITION  = 0,
    IVY_VERTEX_TEXCOORD  = 1,
    IVY_VERTEX_NORMAL    = 2,
    IVY_VERTEX_COLOR     = 3,
    IVY_VERTEX_TANGENT   = 4,
    IVY_VERTEX_MAX_ATTRIBS,
} IvyVertexAttrib;

typedef enum {
    IVY_SHADER_LOC_VERTEX_POSITION = 0,
    IVY_SHADER_LOC_VERTEX_TEXCOORD01,
    IVY_SHADER_LOC_VERTEX_TEXCOORD02,
    IVY_SHADER_LOC_VERTEX_NORMAL,
    IVY_SHADER_LOC_VERTEX_TANGENT,
    IVY_SHADER_LOC_VERTEX_COLOR,
    IVY_SHADER_LOC_MATRIX_MVP,
    IVY_SHADER_LOC_MATRIX_VIEW,
    IVY_SHADER_LOC_MATRIX_PROJECTION,
    IVY_SHADER_LOC_MATRIX_MODEL,
    IVY_SHADER_LOC_MATRIX_NORMAL,
    IVY_SHADER_LOC_VECTOR_VIEW,
    IVY_SHADER_LOC_COLOR_DIFFUSE,
    IVY_SHADER_LOC_COLOR_SPECULAR,
    IVY_SHADER_LOC_COLOR_AMBIENT,
    IVY_SHADER_LOC_MAP_ALBEDO,
    IVY_SHADER_LOC_MAP_METALNESS,
    IVY_SHADER_LOC_MAP_NORMAL,
    IVY_SHADER_LOC_MAP_ROUGHNESS,
    IVY_SHADER_LOC_MAP_OCCLUSION,
    IVY_SHADER_LOC_MAP_EMISSION,
    IVY_SHADER_LOC_MAP_HEIGHT,
    IVY_SHADER_LOC_MAP_CUBEMAP,
    IVY_SHADER_LOC_MAP_IRRADIANCE,
    IVY_SHADER_LOC_MAP_PREFILTER,
    IVY_SHADER_LOC_MAP_BRDF
} IvyShaderLocIndex;

typedef enum {
    IVY_PIXELFORMAT_UNCOMPRESSED_GRAYSCALE = 1,
    IVY_PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA,
    IVY_PIXELFORMAT_UNCOMPRESSED_R5G6B5,
    IVY_PIXELFORMAT_UNCOMPRESSED_R8G8B8,
    IVY_PIXELFORMAT_UNCOMPRESSED_R5G5B5A1,
    IVY_PIXELFORMAT_UNCOMPRESSED_R4G4B4A4,
    IVY_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
    IVY_PIXELFORMAT_UNCOMPRESSED_R32,
    IVY_PIXELFORMAT_UNCOMPRESSED_R32G32B32,
    IVY_PIXELFORMAT_UNCOMPRESSED_R32G32B32A32,
    IVY_PIXELFORMAT_UNCOMPRESSED_R16,
    IVY_PIXELFORMAT_UNCOMPRESSED_R16G16B16,
    IVY_PIXELFORMAT_UNCOMPRESSED_R16G16B16A16,
    IVY_PIXELFORMAT_COMPRESSED_DXT1_RGB,
    IVY_PIXELFORMAT_COMPRESSED_DXT1_RGBA,
    IVY_PIXELFORMAT_COMPRESSED_DXT3_RGBA,
    IVY_PIXELFORMAT_COMPRESSED_DXT5_RGBA,
    IVY_PIXELFORMAT_COMPRESSED_ETC1_RGB,
    IVY_PIXELFORMAT_COMPRESSED_ETC2_RGB,
    IVY_PIXELFORMAT_COMPRESSED_ETC2_EAC_RGBA,
    IVY_PIXELFORMAT_COMPRESSED_PVRT_RGB,
    IVY_PIXELFORMAT_COMPRESSED_PVRT_RGBA,
    IVY_PIXELFORMAT_COMPRESSED_ASTC_4x4_RGBA,
    IVY_PIXELFORMAT_COMPRESSED_ASTC_8x8_RGBA
} IvyPixelFormat;

typedef enum {
    IVY_ATTACHMENT_COLOR_CHANNEL0 = 0,
    IVY_ATTACHMENT_COLOR_CHANNEL1 = 1,
    IVY_ATTACHMENT_COLOR_CHANNEL2 = 2,
    IVY_ATTACHMENT_COLOR_CHANNEL3 = 3,
    IVY_ATTACHMENT_COLOR_CHANNEL4 = 4,
    IVY_ATTACHMENT_COLOR_CHANNEL5 = 5,
    IVY_ATTACHMENT_COLOR_CHANNEL6 = 6,
    IVY_ATTACHMENT_COLOR_CHANNEL7 = 7,
    IVY_ATTACHMENT_DEPTH = 100,
    IVY_ATTACHMENT_STENCIL = 200,
} IvyFramebufferAttachType;

typedef enum {
    IVY_ATTACHMENT_CUBEMAP_POSITIVE_X = 0,
    IVY_ATTACHMENT_CUBEMAP_NEGATIVE_X = 1,
    IVY_ATTACHMENT_CUBEMAP_POSITIVE_Y = 2,
    IVY_ATTACHMENT_CUBEMAP_NEGATIVE_Y = 3,
    IVY_ATTACHMENT_CUBEMAP_POSITIVE_Z = 4,
    IVY_ATTACHMENT_CUBEMAP_NEGATIVE_Z = 5,
    IVY_ATTACHMENT_TEXTURE2D = 100,
    IVY_ATTACHMENT_RENDERBUFFER = 200,
} rlFramebufferAttachTextureType;

typedef struct {
    int mode;
    int vertexCount;
    int vertexAlignment;
    u32 textureId;
} IvyDrawCall;

typedef struct {
    int    elementCount;
    float *vertices;
    float *texCoords;
    float *normals;
    u8    *colors;
    u32   *indices;
    u32    vaoId;
    u32    vboId[IVY_VERTEX_MAX_ATTRIBS];
} IvyVertexBuffer;

typedef struct {
    int             bufferCount;
    int              currentBuffer;
    IvyVertexBuffer *vertexBuffer;
    IvyDrawCall     *draws;
    int drawCounter;
    float currentDepth;
} IvyGLRenderBatch;

typedef struct {
    int vertexCounter;
    float texcoordx, texcoordy;
    float normalx, normaly, normalz;
    unsigned char colorr, colorg, colorb, colora;

    int currentMatrixMode;
    IvyMatrix *currentMatrix;
    IvyMatrix modelview;
    IvyMatrix projection;
    IvyMatrix transform;
    bool transformRequired;
    IvyMatrix stack[IVY_MAX_MATRIX_STACK_SIZE];
    // int stackCounter;

    u32 currentTextureId;
    u32 defaultTextureId;
    u32 activeTextureId[IVY_DEFAULT_BATCH_MAX_TEXTURE_UNITS];
    u32 defaultVShaderId;
    u32 defaultFShaderId;
    u32 defaultShaderId;
    int *defaultShaderLocs;
    int *currentShaderLocs;
    u32 currentShaderId;

    bool stereoRender;
    IvyMatrix projectionStereo[2];
    IvyMatrix viewOffsetStereo[2];

    // int currentBlendMode;
    // int glBlendSrcFactor;
    // int glBlendDstFactor;
    // int glBlendEquation;
    // int glBlendSrcFactorRGB;
    // int glBlendDestFactorRGB;
    // int glBlendSrcFactorAlpha;
    // int glBlendDestFactorAlpha;
    // int glBlendEquationRGB;
    // int glBlendEquationAlpha;
    // bool glCustomBlendModeModified;

    int framebufferWidth;
    int framebufferHeight;
} IvyGLState;

typedef struct {
    bool  vao;
    bool  instancing;
    bool  texNPOT;
    bool  texFloat32;
    bool  texFloat16;
    bool  texAnisoFilter;
    bool  texMirrorClamp;
    bool  texCompASTC;
    bool  texCompDXT;
    bool  texCompETC2;
    float maxAnisotropyLevel;
} IvyGLExtSupported;

typedef struct {
    IvyGLRenderBatch *currentBatch;
    IvyGLRenderBatch  defaultBatch;
    IvyGLLoadProc     loader;
    IvyGLState        state;
    IvyGLExtSupported extSupported;
} IvyGLData;

typedef struct {
    u8 r, g, b, a;
} IvyColor;

typedef struct {
    u32 id;
    int width;
    int height;
    int mipmaps;
    int format;
} IvyTexture;

typedef struct {
    u32 id;
    IvyTexture texture;
    IvyTexture depth;
} IvyRenderTexture;

void Ivy_Gfx_Init(int width, int height);
void Ivy_Gfx_BeginDrawing(void);
void Ivy_Gfx_EndDrawing(void);
void Ivy_Gfx_ClearBackground(IvyColor color);
void Ivy_Gfx_SwapBuffers(void);

void Ivy_Gfx_LoadIdentity(void);
void Ivy_Gfx_MatrixMultiply(float *matrix);
void Ivy_Gfx_DrawRenderBatchActive(void);
IvyGLRenderBatch Ivy_Gfx_LoadRenderBatch(int numBuffers, int bufferElements);

void Ivy_Gfx_UnloadShaderDefault(void);
void Ivy_Gfx_LoadShaderDefault(void);
u32 Ivy_Gfx_LoadShaderProgramEx(unsigned vsId, u32 fsId);
u32 Ivy_Gfx_LoadTextureId(const void *data, int width, int height, int format, int mipmapCount);
u32 Ivy_Gfx_LoadShader(const char *code, int type);

IvyRenderTexture Ivy_Gfx_LoadRenderTexture(int width, int height);
u32 Ivy_Gfx_LoadFramebuffer(void);
void Ivy_Gfx_EnableFramebuffer(u32 id);
void Ivy_Gfx_DisableFramebuffer(void);

u32 Ivy_Gfx_LoadTextureDepth(int width, int height);
void Ivy_Gfx_FramebufferAttach(u32 id, u32 texId, int attachType, int texType, int mipLevel);
void Ivy_Gfx_UnloadRenderTexture(IvyRenderTexture target);
void Ivy_Gfx_SetTexture(u32 id);
void Ivy_Gfx_DrawRenderBatch(IvyGLRenderBatch *batch);
bool Ivy_Gfx_CheckRenderBatchLimit(int vCount);

void Ivy_Gfx_DrawTexturePro(IvyTexture texture, IvyRectangle src, IvyRectangle dst, IvyVector2 origin, float rotation, IvyColor tint);

void Ivy_Gfx_Begin(int mode);
void Ivy_Gfx_End(void);

int Ivy_Gfx_GetPixelDataSize(int width, int height, int format);
void Ivy_Gfx_GetGLTextureFormats(int format, u32 *glInternalFormat, u32 *glFormat, u32 *glType);
void Ivy_Gfx_BeginTextureMode(IvyRenderTexture target);
void Ivy_Gfx_UnloadFramebuffer(u32 id);
void Ivy_Gfx_DrawTextureEx(IvyTexture texture, IvyVector2 position, float rotation, float scale, IvyColor tint);
void Ivy_Gfx_DrawTexture(IvyTexture texture, int posX, int posY, IvyColor tint);
void Ivy_Gfx_TextureCoord2F(float x, float y);
void Ivy_Gfx_Vertex3F(float x, float y, float z);
void Ivy_Gfx_Vertex2F(float x, float y);
void Ivy_Gfx_Color4UB(u8 x, u8 y, u8 z, u8 w);
void Ivy_Gfx_Normal3F(float x, float y, float z);

void Ivy_Gfx_EndTextureMode(void);
void Ivy_Gfx_MatrixMode(int mode);
void Ivy_Gfx_Ortho(double left, double right, double bottom, double top, double znear, double zfar);

#ifdef __cplusplus
}
#endif

#endif