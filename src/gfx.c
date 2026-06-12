#include "ivy/core.h"
#include "ivy/gfx.h"

#include "ivy/glfw_loader.h"
#include "ivy/glad_loader.h"
#include "ivy/arena/linear.h"

extern IvyCoreData     IVY_CORE;
extern IvyPlatformData IVY_PLATFORM;

IvyGLData IVY_GL = {0};

void Ivy_Gfx_ClearBackground(const IvyColor color)
{
    const float inv255 = 1.0f / 255.0f;

    glClearColor(
        (float)color.r * inv255,
        (float)color.g * inv255,
        (float)color.b * inv255,
        (float)color.a * inv255);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Ivy_Gfx_LoadIdentity(void)
{
    *IVY_GL.state.currentMatrix = Ivy_Math_MatrixIdentity();
}

void Ivy_Gfx_MatrixMultiply(float *matrix)
{
    const IvyMatrix mat = {
        matrix[0],  matrix[4],  matrix[8],  matrix[12],
        matrix[1],  matrix[5],  matrix[9],  matrix[13],
        matrix[2],  matrix[6],  matrix[10], matrix[14],
        matrix[3],  matrix[7],  matrix[11], matrix[15],
    };
    *IVY_GL.state.currentMatrix = Ivy_Math_MatrixMultiply(mat, *IVY_GL.state.currentMatrix);
}

void Ivy_Gfx_DrawRenderBatchActive(const IvyGLRenderBatch *batch)
{
    if (IVY_GL.state.vertexCounter <= 0) return;

    const IvyVertexBuffer *vb = &batch->vertexBuffer[batch->currentBuffer];
    if (IVY_GL.extSupported.vao) glBindVertexArray(vb->vaoId);

    glBindBuffer(GL_ARRAY_BUFFER, vb->vboId[IVY_VERTEX_POSITION]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, (ptrdiff_t)(IVY_GL.state.vertexCounter * 3 * sizeof(float)), vb->vertices);

    glBindBuffer(GL_ARRAY_BUFFER, vb->vboId[IVY_VERTEX_TEXCOORD]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, (ptrdiff_t)(IVY_GL.state.vertexCounter * 2 * sizeof(float)), vb->texCoords);

    glBindBuffer(GL_ARRAY_BUFFER, vb->vboId[IVY_VERTEX_NORMAL]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, (ptrdiff_t)(IVY_GL.state.vertexCounter * 3 * sizeof(float)), vb->normals);

    glBindBuffer(GL_ARRAY_BUFFER, vb->vboId[IVY_VERTEX_COLOR]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, (ptrdiff_t)(IVY_GL.state.vertexCounter * 4 * sizeof(u8)), vb->colors);

    if (IVY_GL.extSupported.vao) glBindVertexArray(0);
}

void Ivy_Gfx_SwapBuffers(void)
{
    glfwSwapBuffers(IVY_PLATFORM.handle);
    glfwPollEvents();
}

void Ivy_Gfx_BeginDrawing(void)
{
    IVY_CORE.time.current = Ivy_Core_GetTime();
    IVY_CORE.time.update  = IVY_CORE.time.current - IVY_CORE.time.prev;
    IVY_CORE.time.prev    = IVY_CORE.time.current;

    Ivy_Gfx_LoadIdentity();
    Ivy_Gfx_MatrixMultiply(Ivy_Math_MatrixToFloatV(IVY_CORE.window.screenScale).v);
}

void Ivy_Gfx_EndDrawing(void)
{
    Ivy_Gfx_DrawRenderBatchActive(IVY_GL.currentBatch);
    Ivy_Gfx_SwapBuffers();

    IVY_CORE.time.current = Ivy_Core_GetTime();
    IVY_CORE.time.draw    = IVY_CORE.time.current - IVY_CORE.time.prev;
    IVY_CORE.time.prev    = IVY_CORE.time.current;
    IVY_CORE.time.frame   = IVY_CORE.time.update + IVY_CORE.time.draw;

    if (IVY_CORE.time.target > 0.0 && IVY_CORE.time.frame < IVY_CORE.time.target) {
        // TODO: platform sleep for the remaining budget
        IVY_CORE.time.current = Ivy_Core_GetTime();
        IVY_CORE.time.frame  += IVY_CORE.time.current - IVY_CORE.time.prev;
        IVY_CORE.time.prev    = IVY_CORE.time.current;
    }

    IVY_CORE.time.frameCounter++;

    Ivy_Input_PoolEvents();
}

int Ivy_Gfx_GetPixelDataSize(const int width, const int height, const int format)
{
    int bpp = 0;

    if (format == IVY_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8) bpp = 32;
    else if (format == IVY_PIXELFORMAT_UNCOMPRESSED_R4G4B4A4) bpp = 16;

    const double bytesPerPixel = (double)bpp / 8.0;
    const int dataSize = (int)(bytesPerPixel * width * height);

    return dataSize;
}

void Ivy_Gfx_GetGLTextureFormats(int format, u32 *glInternalFormat, u32 *glFormat, u32 *glType)
{
    *glInternalFormat = 0;
    *glFormat = 0;
    *glType = 0;

    if (format == IVY_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8) {
        *glInternalFormat = GL_RGBA8;
        *glFormat = GL_RGBA;
        *glType = GL_UNSIGNED_BYTE;

        return;
    }

    if (format == IVY_PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA) {
        *glInternalFormat = GL_RG8;
        *glFormat = GL_RG;
        *glType = GL_UNSIGNED_BYTE;
    }
}

u32 Ivy_Gfx_LoadTextureId(const void *data, const int width, const int height, const int format, const int mipmapCount)
{
    u32 id = 0;

    glBindTexture(GL_TEXTURE_2D, 0);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    int mipWidth = width;
    int mipHeight = height;
    int mipOffset = 0;
    (void)mipOffset;

    unsigned char *dataPtr = NULL;
    if (data != NULL) dataPtr = (u8 *)data;

    for (int i = 0; i < mipmapCount; i++) {
        const u32 mipSize = Ivy_Gfx_GetPixelDataSize(mipWidth, mipHeight, format);

        u32 glInternalFormat, glFormat, glType;
        Ivy_Gfx_GetGLTextureFormats(format, &glInternalFormat, &glFormat, &glType);

        if (glInternalFormat != 0) {
            if (format < IVY_PIXELFORMAT_COMPRESSED_DXT1_RGB) glTexImage2D(GL_TEXTURE_2D, i, (int)glInternalFormat, mipWidth, mipHeight, 0, glFormat, glType, dataPtr);

            if (format == IVY_PIXELFORMAT_UNCOMPRESSED_GRAYSCALE) {
                int swizzleMask[] = { GL_RED, GL_RED, GL_RED, GL_ONE };
                glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
            }
            else if (format == IVY_PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA) {
                int swizzleMask[] = { GL_RED, GL_RED, GL_RED, GL_GREEN };
                glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
            }
        }

        mipWidth /= 2;
        mipHeight /= 2;
        mipOffset += (int)mipSize;
        if (data != NULL) dataPtr += mipSize;

        if (mipWidth < 1) mipWidth = 1;
        if (mipHeight < 1) mipHeight = 1;
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    if (mipmapCount > 1) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mipmapCount - 1);
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    IVY_CHECK(id > 0, "[WARNING] Failed to load texture!");

    return 1;
}

u32 Ivy_Gfx_LoadShader(const char *code, const int type)
{
    u32 shaderId = 0;
    shaderId = glCreateShader(type);
    glShaderSource(shaderId, 1, &code, NULL);

    int success = 0;
    glCompileShader(shaderId);
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

    return shaderId;
}

u32 Ivy_Gfx_LoadShaderProgramEx(const unsigned vsId, const u32 fsId)
{
    u32 programId = 0;
    int success = 0;
    programId = glCreateProgram();

    glAttachShader(programId, vsId);
    glAttachShader(programId, fsId);

    glBindAttribLocation(programId, IVY_DEFAULT_SHADER_ATTRIB_LOCATION_POSITION, IVY_DEFAULT_SHADER_ATTRIB_NAME_POSITION);
    glBindAttribLocation(programId, IVY_DEFAULT_SHADER_ATTRIB_LOCATION_TEXCOORD, IVY_DEFAULT_SHADER_ATTRIB_NAME_TEXCOORD);
    glBindAttribLocation(programId, IVY_DEFAULT_SHADER_ATTRIB_LOCATION_NORMAL, IVY_DEFAULT_SHADER_ATTRIB_NAME_NORMAL);
    glBindAttribLocation(programId, IVY_DEFAULT_SHADER_ATTRIB_LOCATION_COLOR, IVY_DEFAULT_SHADER_ATTRIB_NAME_COLOR);
    glBindAttribLocation(programId, IVY_DEFAULT_SHADER_ATTRIB_LOCATION_TANGENT, IVY_DEFAULT_SHADER_ATTRIB_NAME_TANGENT);
    glBindAttribLocation(programId, IVY_DEFAULT_SHADER_ATTRIB_LOCATION_TEXCOORD2, IVY_DEFAULT_SHADER_ATTRIB_NAME_TEXCOORD2);
    glBindAttribLocation(programId, IVY_DEFAULT_SHADER_ATTRIB_LOCATION_INSTANCETRANSFORM, IVY_DEFAULT_SHADER_ATTRIB_NAME_INSTANCETRANSFORM);
    glBindAttribLocation(programId, IVY_DEFAULT_SHADER_ATTRIB_LOCATION_BONEINDICES, IVY_DEFAULT_SHADER_ATTRIB_NAME_BONEINDICES);
    glBindAttribLocation(programId, IVY_DEFAULT_SHADER_ATTRIB_LOCATION_BONEWEIGHTS, IVY_DEFAULT_SHADER_ATTRIB_NAME_BONEWEIGHTS);

    glLinkProgram(programId);

    glGetProgramiv(programId, GL_LINK_STATUS, &success);

    return programId;
}

void Ivy_Gfx_LoadShaderDefault(void)
{
    IVY_GL.state.defaultShaderLocs = (int *)Ivy_Arena_LinearAlloc(IVY_CORE.arena, IVY_MAX_SHADER_LOCATIONS * sizeof(int));
    memset(IVY_GL.state.defaultShaderLocs, 0, IVY_MAX_SHADER_LOCATIONS * sizeof(int));

    const char *defaultVShaderCode =
        "#version 330                       \n"
        "in vec3 vertexPosition;            \n"
        "in vec2 vertexTexCoord;            \n"
        "in vec4 vertexColor;               \n"
        "out vec2 fragTexCoord;             \n"
        "out vec4 fragColor;                \n"
        "uniform mat4 mvp;                  \n"
        "void main()                        \n"
        "{                                  \n"
        "    fragTexCoord = vertexTexCoord; \n"
        "    fragColor = vertexColor;       \n"
        "    gl_Position = mvp*vec4(vertexPosition, 1.0); \n"
        "}                                  \n";

    const char *defaultFShaderCode =
        "#version 330       \n"
        "in vec2 fragTexCoord;              \n"
        "in vec4 fragColor;                 \n"
        "out vec4 finalColor;               \n"
        "uniform sampler2D texture0;        \n"
        "uniform vec4 colDiffuse;           \n"
        "void main()                        \n"
        "{                                  \n"
        "    vec4 texelColor = texture(texture0, fragTexCoord);   \n"
        "    finalColor = texelColor*colDiffuse*fragColor;        \n"
        "}                                  \n";

    IVY_GL.state.defaultVShaderId = Ivy_Gfx_LoadShader(defaultVShaderCode, GL_VERTEX_SHADER);
    IVY_GL.state.defaultFShaderId = Ivy_Gfx_LoadShader(defaultFShaderCode, GL_FRAGMENT_SHADER);
    IVY_GL.state.defaultShaderId = Ivy_Gfx_LoadShaderProgramEx(IVY_GL.state.defaultVShaderId, IVY_GL.state.defaultFShaderId);

    if (IVY_LIKELY(IVY_GL.state.defaultShaderId > 0)) {
        IVY_GL.state.defaultShaderLocs[IVY_SHADER_LOC_VERTEX_POSITION] = glGetAttribLocation(IVY_GL.state.defaultShaderId, IVY_DEFAULT_SHADER_ATTRIB_NAME_POSITION);
        IVY_GL.state.defaultShaderLocs[IVY_SHADER_LOC_VERTEX_TEXCOORD01] = glGetAttribLocation(IVY_GL.state.defaultShaderId, IVY_DEFAULT_SHADER_ATTRIB_NAME_TEXCOORD);
        IVY_GL.state.defaultShaderLocs[IVY_SHADER_LOC_VERTEX_COLOR] = glGetAttribLocation(IVY_GL.state.defaultShaderId, IVY_DEFAULT_SHADER_ATTRIB_NAME_COLOR);

        IVY_GL.state.defaultShaderLocs[IVY_SHADER_LOC_MATRIX_MVP] = glGetUniformLocation(IVY_GL.state.defaultShaderId, IVY_DEFAULT_SHADER_UNIFORM_NAME_MVP);
        IVY_GL.state.defaultShaderLocs[IVY_SHADER_LOC_COLOR_DIFFUSE] = glGetUniformLocation(IVY_GL.state.defaultShaderId, IVY_DEFAULT_SHADER_UNIFORM_NAME_COLOR);
        IVY_GL.state.defaultShaderLocs[IVY_SHADER_LOC_MAP_DIFFUSE] = glGetUniformLocation(IVY_GL.state.defaultShaderId, IVY_DEFAULT_SHADER_SAMPLER2D_NAME_TEXTURE0);
    }
}

void Ivy_Gfx_UnloadShaderDefault(void)
{
    glUseProgram(0);

    glDetachShader(IVY_GL.state.defaultShaderId, IVY_GL.state.defaultVShaderId);
    glDetachShader(IVY_GL.state.defaultShaderId, IVY_GL.state.defaultFShaderId);
    glDeleteShader(IVY_GL.state.defaultVShaderId);
    glDeleteShader(IVY_GL.state.defaultFShaderId);

    glDeleteProgram(IVY_GL.state.defaultShaderId);
}

IvyGLRenderBatch Ivy_Gfx_LoadRenderBatch(const int numBuffers, const int bufferElements)
{
    IvyGLRenderBatch batch = {0};

    batch.vertexBuffer = (IvyVertexBuffer *)Ivy_Arena_LinearAlloc(IVY_CORE.arena, numBuffers * sizeof(IvyVertexBuffer));
    memset(batch.vertexBuffer, 0, numBuffers * sizeof(IvyVertexBuffer));

    for (int i = 0; i < numBuffers; i++)
    {
        batch.vertexBuffer[i].elementCount = bufferElements;

        batch.vertexBuffer[i].vertices = (float *)Ivy_Arena_LinearAlloc(IVY_CORE.arena, bufferElements * 3 * 4 * sizeof(float));
        memset(batch.vertexBuffer[i].vertices, 0, bufferElements * 3 * 4 * sizeof(float));

        batch.vertexBuffer[i].texCoords = (float *)Ivy_Arena_LinearAlloc(IVY_CORE.arena, bufferElements * 2 * 4 * sizeof(float));
        memset(batch.vertexBuffer[i].texCoords, 0, bufferElements * 2 * 4 * sizeof(float));

        batch.vertexBuffer[i].normals = (float *)Ivy_Arena_LinearAlloc(IVY_CORE.arena, bufferElements * 3 * 4 * sizeof(float));
        memset(batch.vertexBuffer[i].normals, 0, bufferElements * 3 * 4 * sizeof(float));

        batch.vertexBuffer[i].colors = (unsigned char *)Ivy_Arena_LinearAlloc(IVY_CORE.arena, bufferElements * 4 * 4 * sizeof(unsigned char));
        memset(batch.vertexBuffer[i].colors, 0, bufferElements * 4 * 4 * sizeof(unsigned char));

        batch.vertexBuffer[i].indices = (u32 *)Ivy_Arena_LinearAlloc(IVY_CORE.arena, bufferElements * 6 * sizeof(int));

        for (int j = 0; j < (3 * 4 * bufferElements); j++) batch.vertexBuffer[i].vertices[j]  = 0.0f;
        for (int j = 0; j < (2 * 4 * bufferElements); j++) batch.vertexBuffer[i].texCoords[j] = 0.0f;
        for (int j = 0; j < (3 * 4 * bufferElements); j++) batch.vertexBuffer[i].normals[j]   = 0.0f;
        for (int j = 0; j < (4 * 4 * bufferElements); j++) batch.vertexBuffer[i].colors[j]    = 0;

        int k = 0;
        for (int j = 0; j < (6 * bufferElements); j += 6)
        {
            batch.vertexBuffer[i].indices[j] = 4 * k;
            batch.vertexBuffer[i].indices[j + 1] = 4 * k + 1;
            batch.vertexBuffer[i].indices[j + 2] = 4 * k + 2;
            batch.vertexBuffer[i].indices[j + 3] = 4 * k;
            batch.vertexBuffer[i].indices[j + 4] = 4 * k + 2;
            batch.vertexBuffer[i].indices[j + 5] = 4 * k + 3;
            k++;
        }

        IVY_GL.state.vertexCounter = 0;
    }

    // Setup VAO / VBO
    for (int i = 0; i < numBuffers; i++)
    {
        if (IVY_GL.extSupported.vao) {
            glGenVertexArrays(1, &batch.vertexBuffer[i].vaoId);
            glBindVertexArray(batch.vertexBuffer[i].vaoId);
        }

        glGenBuffers(1, &batch.vertexBuffer[i].vboId[0]);
        glBindBuffer(GL_ARRAY_BUFFER, batch.vertexBuffer[i].vboId[0]);
        glBufferData(GL_ARRAY_BUFFER, (ptrdiff_t)(bufferElements * 3 * 4 * sizeof(float)), batch.vertexBuffer[i].vertices, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(IVY_GL.state.currentShaderLocs[IVY_SHADER_LOC_VERTEX_POSITION]);
        glVertexAttribPointer(IVY_GL.state.currentShaderLocs[IVY_SHADER_LOC_VERTEX_POSITION], 3, GL_FLOAT, 0, 0, 0);

        // Vertex texCoord buffer
        glGenBuffers(1, &batch.vertexBuffer[i].vboId[1]);
        glBindBuffer(GL_ARRAY_BUFFER, batch.vertexBuffer[i].vboId[1]);
        glBufferData(GL_ARRAY_BUFFER, (ptrdiff_t)(bufferElements * 2 * 4 * sizeof(float)), batch.vertexBuffer[i].texCoords, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(IVY_GL.state.currentShaderLocs[IVY_SHADER_LOC_VERTEX_TEXCOORD01]);
        glVertexAttribPointer(IVY_GL.state.currentShaderLocs[IVY_SHADER_LOC_VERTEX_TEXCOORD01], 2, GL_FLOAT, 0, 0, 0);

        // Vertex normal buffer
        glGenBuffers(1, &batch.vertexBuffer[i].vboId[2]);
        glBindBuffer(GL_ARRAY_BUFFER, batch.vertexBuffer[i].vboId[2]);
        glBufferData(GL_ARRAY_BUFFER, (ptrdiff_t)(bufferElements * 3 * 4 * sizeof(float)), batch.vertexBuffer[i].normals, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(IVY_GL.state.currentShaderLocs[IVY_SHADER_LOC_VERTEX_NORMAL]);
        glVertexAttribPointer(IVY_GL.state.currentShaderLocs[IVY_SHADER_LOC_VERTEX_NORMAL], 3, GL_FLOAT, 0, 0, 0);

        // Vertex color buffer
        glGenBuffers(1, &batch.vertexBuffer[i].vboId[3]);
        glBindBuffer(GL_ARRAY_BUFFER, batch.vertexBuffer[i].vboId[3]);
        glBufferData(GL_ARRAY_BUFFER, (ptrdiff_t)(bufferElements * 4 * 4 * sizeof(unsigned char)), batch.vertexBuffer[i].colors, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(IVY_GL.state.currentShaderLocs[IVY_SHADER_LOC_VERTEX_COLOR]);
        glVertexAttribPointer(IVY_GL.state.currentShaderLocs[IVY_SHADER_LOC_VERTEX_COLOR], 4, GL_UNSIGNED_BYTE, true, 0, 0);

        // Index buffer
        glGenBuffers(1, &batch.vertexBuffer[i].vboId[4]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batch.vertexBuffer[i].vboId[4]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (ptrdiff_t)(bufferElements * 6 * sizeof(int)), batch.vertexBuffer[i].indices, GL_STATIC_DRAW);
    }

    if (IVY_GL.extSupported.vao) glBindVertexArray(0);

    batch.draws = (IvyDrawCall *)Ivy_Arena_LinearAlloc(IVY_CORE.arena, IVY_DEFAULT_BATCH_DRAWCALLS * sizeof(IvyDrawCall));

    for (int i = 0; i < IVY_DEFAULT_BATCH_DRAWCALLS; i++)
    {
        batch.draws[i].mode = IVY_QUADS;
        batch.draws[i].vertexCount = 0;
        batch.draws[i].vertexAlignment = 0;
        batch.draws[i].textureId = IVY_GL.state.defaultShaderId;
    }

    batch.bufferCount = numBuffers;
    batch.drawCounter = 1;
    batch.currentDepth = -1.0f;

    return batch;
}

void Ivy_Gfx_Init(const int width, const int height)
{
    const u8 pixels[4] = { 255, 255, 255, 255 };

    IVY_GL.state.defaultTextureId = Ivy_Gfx_LoadTextureId(pixels, 1, 1, IVY_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
    IVY_CHECK(IVY_GL.state.defaultTextureId != 0, "[ERROR] Failed to load default texture!");

    IVY_GL.state.currentTextureId = IVY_GL.state.defaultTextureId;

    Ivy_Gfx_LoadShaderDefault();
    IVY_GL.state.currentShaderId = IVY_GL.state.defaultTextureId;
    IVY_GL.state.currentShaderLocs = IVY_GL.state.defaultShaderLocs;

    IVY_GL.state.currentShaderLocs[IVY_SHADER_LOC_VERTEX_NORMAL] = IVY_DEFAULT_SHADER_ATTRIB_LOCATION_NORMAL;
    IVY_GL.defaultBatch = Ivy_Gfx_LoadRenderBatch(IVY_DEFAULT_BATCH_BUFFERS, IVY_DEFAULT_BATCH_BUFFER_ELEMENTS);
    IVY_GL.state.currentShaderLocs[IVY_SHADER_LOC_VERTEX_NORMAL] = -1;
    IVY_GL.currentBatch = &IVY_GL.defaultBatch;

    for (int i = 0; i < IVY_MAX_MATRIX_STACK_SIZE; i++) {
        IVY_GL.state.stack[i] = Ivy_Math_MatrixIdentity();
    }

    IVY_GL.state.transform  = Ivy_Math_MatrixIdentity();
    IVY_GL.state.projection = Ivy_Math_MatrixIdentity();
    IVY_GL.state.modelview  = Ivy_Math_MatrixIdentity();
    IVY_GL.state.currentMatrix = &IVY_GL.state.modelview;

    glDepthFunc(GL_LEQUAL);
    glDisable(GL_DEPTH_TEST);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);

    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    IVY_GL.state.framebufferWidth = width;
    IVY_GL.state.framebufferHeight = height;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

IvyRenderTexture Ivy_Gfx_LoadRenderTexture(int width, int height)
{
    IvyRenderTexture target = {0};

    target.id = Ivy_Gfx_LoadFramebuffer();

    if (IVY_LIKELY(target.id > 0)) {
        Ivy_Gfx_EnableFramebuffer(target.id);

        target.texture.id = Ivy_Gfx_LoadTextureId(NULL, width, height, IVY_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
        target.texture.width = width;
        target.texture.height = height;
        target.texture.format = IVY_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
        target.texture.mipmaps = 1;

        target.depth.id = Ivy_Gfx_LoadTextureDepth(width, height);
        target.depth.width = width;
        target.depth.height = height;
        target.depth.format = 19;
        target.depth.mipmaps = 1;

        Ivy_Gfx_FramebufferAttach(target.id, target.texture.id, IVY_ATTACHMENT_COLOR_CHANNEL0, IVY_ATTACHMENT_TEXTURE2D, 0);
        Ivy_Gfx_FramebufferAttach(target.id, target.depth.id, IVY_ATTACHMENT_DEPTH, IVY_ATTACHMENT_RENDERBUFFER, 0);

        Ivy_Gfx_DisableFramebuffer();
    }

    return target;
}

u32 Ivy_Gfx_LoadFramebuffer(void)
{
    u32 fboId = 0;
    glGenFramebuffers(1, &fboId);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return fboId;
}

void Ivy_Gfx_EnableFramebuffer(u32 id)
{
    glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void Ivy_Gfx_DisableFramebuffer(void)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

u32 Ivy_Gfx_LoadTextureDepth(const int width, const int height)
{
    u32 id = 0;
    const u32 glInternalFormat = GL_DEPTH_COMPONENT;

    glGenRenderbuffers(1, &id);
    glBindRenderbuffer(GL_RENDERBUFFER, id);
    glRenderbufferStorage(GL_RENDERBUFFER, glInternalFormat, width, height);

    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    return id;
}

void Ivy_Gfx_FramebufferAttach(u32 id, u32 texId, int attachType, int texType, int mipLevel) {
    glBindFramebuffer(GL_FRAMEBUFFER, id);

    if (texType == IVY_ATTACHMENT_TEXTURE2D)
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachType, GL_TEXTURE_2D, texId, mipLevel);

    else if (texType == IVY_ATTACHMENT_DEPTH) {
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, texId);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}