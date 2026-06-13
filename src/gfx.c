#include "ivy/core.h"
#include "ivy/gfx.h"
#include "ivy/math.h"

#include "ivy/arena/linear.h"
#include "ivy/glad_loader.h"
#include "ivy/glfw_loader.h"

extern IvyCoreData     IVY_CORE;
extern IvyPlatformData IVY_PLATFORM;

IvyGLData IVY_GL = {0};

static void SetupViewport(int width, int height)
{
    IVY_CORE.window.render.width = width;
    IVY_CORE.window.render.height = height;

    glViewport(IVY_CORE.window.renderOffset.x/2, IVY_CORE.window.renderOffset.y/2, IVY_CORE.window.render.width, IVY_CORE.window.render.height);

    Ivy_Gfx_MatrixMode(IVY_PROJECTION);
    Ivy_Gfx_LoadIdentity();

    Ivy_Gfx_Ortho(0, IVY_CORE.window.render.width, IVY_CORE.window.render.height, 0, 0.0f, 1.0f);

    Ivy_Gfx_MatrixMode(IVY_MODELVIEW);
    Ivy_Gfx_LoadIdentity();
}

void Ivy_Gfx_Init(const int width, const int height)
{
    const u8 pixels[4] = { 255, 255, 255, 255 };

    IVY_GL.state.defaultTextureId = Ivy_Gfx_LoadTextureId(pixels, 1, 1, IVY_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
    IVY_CHECK(IVY_GL.state.defaultTextureId != 0, "[ERROR] Failed to load default texture!");

    IVY_GL.state.currentTextureId = IVY_GL.state.defaultTextureId;

    Ivy_Gfx_LoadShaderDefault();
    IVY_GL.state.currentShaderId = IVY_GL.state.defaultShaderId;
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
    Ivy_Gfx_DrawRenderBatchActive();
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

void Ivy_Gfx_SwapBuffers(void)
{
    glfwSwapBuffers(IVY_PLATFORM.handle);
    glfwPollEvents();
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

void Ivy_Gfx_DrawRenderBatchActive(void)
{
    Ivy_Gfx_DrawRenderBatch(IVY_GL.currentBatch);
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

void Ivy_Gfx_UnloadShaderDefault(void)
{
    glUseProgram(0);

    glDetachShader(IVY_GL.state.defaultShaderId, IVY_GL.state.defaultVShaderId);
    glDetachShader(IVY_GL.state.defaultShaderId, IVY_GL.state.defaultFShaderId);
    glDeleteShader(IVY_GL.state.defaultVShaderId);
    glDeleteShader(IVY_GL.state.defaultFShaderId);

    glDeleteProgram(IVY_GL.state.defaultShaderId);
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

    return id;
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

IvyRenderTexture Ivy_Gfx_LoadRenderTexture(const int width, const int height)
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

void Ivy_Gfx_EnableFramebuffer(const u32 id)
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

void Ivy_Gfx_FramebufferAttach(const u32 id, const u32 texId, const int attachType, const int texType, const int mipLevel)
{
    glBindFramebuffer(GL_FRAMEBUFFER, id);

    switch (attachType)
    {
        case IVY_ATTACHMENT_COLOR_CHANNEL0:
        case IVY_ATTACHMENT_COLOR_CHANNEL1:
        case IVY_ATTACHMENT_COLOR_CHANNEL2:
        case IVY_ATTACHMENT_COLOR_CHANNEL3:
        case IVY_ATTACHMENT_COLOR_CHANNEL4:
        case IVY_ATTACHMENT_COLOR_CHANNEL5:
        case IVY_ATTACHMENT_COLOR_CHANNEL6:
        case IVY_ATTACHMENT_COLOR_CHANNEL7:
        {
            if (texType == IVY_ATTACHMENT_TEXTURE2D) glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachType, GL_TEXTURE_2D, texId, mipLevel);
            else if (texType == IVY_ATTACHMENT_RENDERBUFFER) glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachType, GL_RENDERBUFFER, texId);
            else if (texType >= IVY_ATTACHMENT_CUBEMAP_POSITIVE_X) glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachType, GL_TEXTURE_CUBE_MAP_POSITIVE_X + texType, texId, mipLevel);
        } break;
        case IVY_ATTACHMENT_DEPTH:
        {
            if (texType == IVY_ATTACHMENT_TEXTURE2D) glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texId, mipLevel);
            else if (texType == IVY_ATTACHMENT_RENDERBUFFER)  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, texId);
        } break;
        default: break;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Ivy_Gfx_UnloadRenderTexture(const IvyRenderTexture target)
{
    glDeleteTextures(1, &target.texture.id);
    Ivy_Gfx_UnloadFramebuffer(target.id);
}

void Ivy_Gfx_SetTexture(const u32 id)
{
    if (id == 0) {
        if (IVY_GL.state.vertexCounter >= IVY_GL.currentBatch->vertexBuffer[IVY_GL.currentBatch->currentBuffer].elementCount * 4) {
            Ivy_Gfx_DrawRenderBatch(IVY_GL.currentBatch);
        }
        IVY_GL.state.currentTextureId = IVY_GL.state.defaultTextureId;
    }
    else {
        IVY_GL.state.currentTextureId = id;

        if (IVY_GL.currentBatch->draws[IVY_GL.currentBatch->drawCounter - 1].textureId != id) {
            if (IVY_GL.currentBatch->draws[IVY_GL.currentBatch->drawCounter - 1].vertexCount > 0){
                if (IVY_GL.currentBatch->draws[IVY_GL.currentBatch->drawCounter - 1].mode == IVY_LINES) IVY_GL.currentBatch->draws[IVY_GL.currentBatch->drawCounter - 1].vertexAlignment = ((IVY_GL.currentBatch->draws[IVY_GL.currentBatch->drawCounter - 1].vertexCount < 4) ? IVY_GL.currentBatch->draws[IVY_GL.currentBatch->drawCounter - 1].vertexCount : IVY_GL.currentBatch->draws[IVY_GL.currentBatch->drawCounter - 1].vertexCount % 4);
                else if (IVY_GL.currentBatch->draws[IVY_GL.currentBatch->drawCounter - 1].mode == IVY_TRIANGLES) IVY_GL.currentBatch->draws[IVY_GL.currentBatch->drawCounter - 1].vertexAlignment = ((IVY_GL.currentBatch->draws[IVY_GL.currentBatch->drawCounter - 1].vertexCount < 4) ? 1 : (4 - (IVY_GL.currentBatch->draws[IVY_GL.currentBatch->drawCounter - 1].vertexCount % 4)));
                else IVY_GL.currentBatch->draws[IVY_GL.currentBatch->drawCounter - 1].vertexAlignment = 0;

                if (!Ivy_Gfx_CheckRenderBatchLimit(IVY_GL.currentBatch->draws[IVY_GL.currentBatch->drawCounter - 1].vertexAlignment)) {
                    IVY_GL.state.vertexCounter += IVY_GL.currentBatch->draws[IVY_GL.currentBatch->drawCounter - 1].vertexAlignment;
                    IVY_GL.currentBatch->drawCounter++;
                    IVY_GL.currentBatch->draws[IVY_GL.currentBatch->drawCounter - 1].mode = IVY_GL.currentBatch->draws[IVY_GL.currentBatch->drawCounter - 2].mode;
                }
            }

            if (IVY_GL.currentBatch->drawCounter >= IVY_DEFAULT_BATCH_DRAWCALLS) Ivy_Gfx_DrawRenderBatch(IVY_GL.currentBatch);

            IVY_GL.currentBatch->draws[IVY_GL.currentBatch->drawCounter - 1].textureId = id;
            IVY_GL.currentBatch->draws[IVY_GL.currentBatch->drawCounter - 1].vertexCount = 0;
        }
    }
}

void Ivy_Gfx_DrawRenderBatch(IvyGLRenderBatch *batch)
{
    if (IVY_LIKELY(IVY_GL.state.vertexCounter > 0)) {
        if (IVY_LIKELY(IVY_GL.extSupported.vao)) glBindVertexArray(batch->vertexBuffer[batch->currentBuffer].vaoId);

        // Vertex positions buffer
        glBindBuffer(GL_ARRAY_BUFFER, batch->vertexBuffer[batch->currentBuffer].vboId[0]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, (ptrdiff_t)(IVY_GL.state.vertexCounter * 3 * sizeof(float)), batch->vertexBuffer[batch->currentBuffer].vertices);

        // Texture coordinates buffer
        glBindBuffer(GL_ARRAY_BUFFER, batch->vertexBuffer[batch->currentBuffer].vboId[1]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, (ptrdiff_t)(IVY_GL.state.vertexCounter * 2 * sizeof(float)), batch->vertexBuffer[batch->currentBuffer].texCoords);

        // Normals buffer
        glBindBuffer(GL_ARRAY_BUFFER, batch->vertexBuffer[batch->currentBuffer].vboId[2]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, (ptrdiff_t)(IVY_GL.state.vertexCounter * 3 * sizeof(float)), batch->vertexBuffer[batch->currentBuffer].normals);

        // Colors buffer
        glBindBuffer(GL_ARRAY_BUFFER, batch->vertexBuffer[batch->currentBuffer].vboId[3]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, (ptrdiff_t)(IVY_GL.state.vertexCounter * 4 * sizeof(unsigned char)), batch->vertexBuffer[batch->currentBuffer].colors);

        if (IVY_LIKELY(IVY_GL.extSupported.vao)) glBindVertexArray(0);
    }

    IvyMatrix matrixProjection = IVY_GL.state.projection;
    IvyMatrix matrixModelView = IVY_GL.state.modelview;

    int eyeCount = 1;
    if (IVY_GL.state.stereoRender) eyeCount = 2;

    for (int eye = 0; eye < eyeCount; eye++) {
        if (eyeCount == 2) {
            glViewport((int)(eye * IVY_GL.state.framebufferWidth * 0.5), 0, (int)(IVY_GL.state.framebufferWidth * 0.5), IVY_GL.state.framebufferHeight);
            IVY_GL.state.modelview = Ivy_Math_MatrixMultiply(matrixModelView, IVY_GL.state.viewOffsetStereo[eye]);
            IVY_GL.state.projection = IVY_GL.state.projectionStereo[eye];
        }

        if (IVY_GL.state.vertexCounter > 0) {
            glUseProgram(IVY_GL.state.currentShaderId);

            IvyMatrix matrixMVP = Ivy_Math_MatrixMultiply(IVY_GL.state.modelview, IVY_GL.state.projection);
            glUniformMatrix4fv(IVY_GL.state.currentShaderLocs[IVY_SHADER_LOC_MATRIX_MVP], 1, false, Ivy_Math_MatrixToFloat(matrixMVP));

            if (IVY_GL.state.currentShaderLocs[IVY_SHADER_LOC_MATRIX_PROJECTION] != -1) {
                glUniformMatrix4fv(IVY_GL.state.currentShaderLocs[IVY_SHADER_LOC_MATRIX_PROJECTION], 1, false, Ivy_Math_MatrixToFloat(IVY_GL.state.projection));
            }

            if (IVY_GL.state.currentShaderLocs[IVY_SHADER_LOC_MATRIX_VIEW] != -1) {
                glUniformMatrix4fv(IVY_GL.state.currentShaderLocs[IVY_SHADER_LOC_MATRIX_VIEW], 1, false, Ivy_Math_MatrixToFloat(IVY_GL.state.modelview));
            }

            if (IVY_GL.state.currentShaderLocs[IVY_SHADER_LOC_MATRIX_MODEL] != -1) {
                glUniformMatrix4fv(IVY_GL.state.currentShaderLocs[IVY_SHADER_LOC_MATRIX_MODEL], 1, false, Ivy_Math_MatrixToFloat(IVY_GL.state.transform));
            }

            if (IVY_GL.state.currentShaderLocs[IVY_SHADER_LOC_MATRIX_NORMAL] != -1) {
                glUniformMatrix4fv(IVY_GL.state.currentShaderLocs[IVY_SHADER_LOC_MATRIX_NORMAL], 1, false, Ivy_Math_MatrixToFloat(Ivy_Math_MatrixTranspose(Ivy_Math_MatrixInvert(IVY_GL.state.transform))));
            }

           if (IVY_GL.extSupported.vao) {
               glBindVertexArray(batch->vertexBuffer[batch->currentBuffer].vaoId);
           }
           else {
               // Bind vertex attrib: position (shader-location = 0)
               glBindBuffer(GL_ARRAY_BUFFER, batch->vertexBuffer[batch->currentBuffer].vboId[0]);
               glVertexAttribPointer(IVY_GL.state.currentShaderLocs[IVY_SHADER_LOC_VERTEX_POSITION], 3, GL_FLOAT, 0, 0, 0);
               glEnableVertexAttribArray(IVY_GL.state.currentShaderLocs[IVY_SHADER_LOC_VERTEX_POSITION]);

               // Bind vertex attrib: texcoord (shader-location = 1)
               glBindBuffer(GL_ARRAY_BUFFER, batch->vertexBuffer[batch->currentBuffer].vboId[1]);
               glVertexAttribPointer(IVY_GL.state.currentShaderLocs[IVY_SHADER_LOC_VERTEX_TEXCOORD01], 2, GL_FLOAT, 0, 0, 0);
               glEnableVertexAttribArray(IVY_GL.state.currentShaderLocs[IVY_SHADER_LOC_VERTEX_TEXCOORD01]);

               // Bind vertex attrib: normal (shader-location = 2)
               glBindBuffer(GL_ARRAY_BUFFER, batch->vertexBuffer[batch->currentBuffer].vboId[2]);
               glVertexAttribPointer(IVY_GL.state.currentShaderLocs[IVY_SHADER_LOC_VERTEX_NORMAL], 3, GL_FLOAT, 0, 0, 0);
               glEnableVertexAttribArray(IVY_GL.state.currentShaderLocs[IVY_SHADER_LOC_VERTEX_NORMAL]);

               // Bind vertex attrib: color (shader-location = 3)
               glBindBuffer(GL_ARRAY_BUFFER, batch->vertexBuffer[batch->currentBuffer].vboId[3]);
               glVertexAttribPointer(IVY_GL.state.currentShaderLocs[IVY_SHADER_LOC_VERTEX_COLOR], 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);
               glEnableVertexAttribArray(IVY_GL.state.currentShaderLocs[IVY_SHADER_LOC_VERTEX_COLOR]);

               glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batch->vertexBuffer[batch->currentBuffer].vboId[4]);
           }

            glUniform4f(IVY_GL.state.currentShaderLocs[IVY_SHADER_LOC_COLOR_DIFFUSE], 1.0f, 1.0f, 1.0f, 1.0f);
            glUniform1i(IVY_GL.state.currentShaderLocs[IVY_SHADER_LOC_MAP_DIFFUSE], 0);  // Active default sampler2D: texture0

            for (int i = 0; i < IVY_DEFAULT_BATCH_MAX_TEXTURE_UNITS; i++)
            {
                if (IVY_GL.state.activeTextureId[i] > 0)
                {
                    glActiveTexture(GL_TEXTURE0 + 1 + i);
                    glBindTexture(GL_TEXTURE_2D, IVY_GL.state.activeTextureId[i]);
                }
            }

            glActiveTexture(GL_TEXTURE0);

            for (int i = 0, vertexOffset = 0; i < batch->drawCounter; i++) {
                glBindTexture(GL_TEXTURE_2D, batch->draws[i].textureId);

                if ((batch->draws[i].mode == IVY_LINES) || (batch->draws[i].mode == IVY_TRIANGLES)) glDrawArrays(batch->draws[i].mode, vertexOffset, batch->draws[i].vertexCount);
                else {
                    glDrawElements(GL_TRIANGLES, batch->draws[i].vertexCount/4*6, GL_UNSIGNED_INT, (void *)(vertexOffset / 4 * 6 * sizeof(u32)));
                }

                vertexOffset += (batch->draws[i].vertexCount + batch->draws[i].vertexAlignment);
            }

            if (!IVY_GL.extSupported.vao) {
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            }

            glBindTexture(GL_TEXTURE_2D, 0);
        }

        if (IVY_GL.extSupported.vao) glBindVertexArray(0);

        glUseProgram(0);
    }

    if (eyeCount == 2) {
        glViewport(0, 0, IVY_GL.state.framebufferWidth, IVY_GL.state.framebufferHeight);
    }

    IVY_GL.state.vertexCounter = 0;

    batch->currentDepth = -1.0f;

    // Restore projection/modelview matrices
    IVY_GL.state.projection = matrixProjection;
    IVY_GL.state.modelview = matrixModelView;

    for (int i = 0; i < IVY_DEFAULT_BATCH_DRAWCALLS; i++)
    {
        batch->draws[i].mode = IVY_QUADS;
        batch->draws[i].vertexCount = 0;
        batch->draws[i].textureId = IVY_GL.state.defaultTextureId;
    }

    // Reset active texture
    for (int i = 0; i < IVY_DEFAULT_BATCH_MAX_TEXTURE_UNITS; i++) IVY_GL.state.activeTextureId[i] = 0;

    // Reset draws counter
    batch->drawCounter = 1;

    batch->currentBuffer++;
    if (batch->currentBuffer >= batch->bufferCount) batch->currentBuffer = 0;
}

bool Ivy_Gfx_CheckRenderBatchLimit(const int vCount)
{
    bool overflow = false;

    if ((IVY_GL.state.vertexCounter + vCount) >= (IVY_GL.currentBatch->vertexBuffer[IVY_GL.currentBatch->currentBuffer].elementCount * 4)) {
        overflow = true;

        const int currentMode = IVY_GL.currentBatch->draws[IVY_GL.currentBatch->drawCounter - 1].mode;
        const int currentTexture = IVY_GL.currentBatch->draws[IVY_GL.currentBatch->drawCounter - 1].textureId;

        Ivy_Gfx_DrawRenderBatch(IVY_GL.currentBatch);

        IVY_GL.currentBatch->draws[IVY_GL.currentBatch->drawCounter - 1].mode = currentMode;
        IVY_GL.currentBatch->draws[IVY_GL.currentBatch->drawCounter - 1].textureId = currentTexture;
    }

    return overflow;
}

void Ivy_Gfx_DrawTexturePro(IvyTexture texture, IvyRectangle src, IvyRectangle dest, IvyVector2 origin, float rotation, IvyColor color)
{
    if (IVY_LIKELY(texture.id > 0)) {
        float width = (float)texture.width;
        float height = (float)texture.height;

        bool flipX = false;

        if (IVY_UNLIKELY(src.width < 0)) { flipX = true; src.width *= -1; }
        if (IVY_UNLIKELY(src.height < 0)) src.y -= src.height;

        if (IVY_UNLIKELY(dest.width < 0)) dest.width *= -1;
        if (IVY_UNLIKELY(dest.height < 0)) dest.height *= -1;

        IvyVector2 topLeft = {0};
        IvyVector2 topRight = {0};
        IvyVector2 bottomLeft = {0};
        IvyVector2 bottomRight = {0};

        if (IVY_LIKELY(rotation == 0.0f)) {
            const float x = dest.x - origin.x;
            const float y = dest.y - origin.y;
            topLeft = (IvyVector2){ x, y };
            topRight = (IvyVector2){ x + dest.width, y };
            bottomLeft = (IvyVector2){ x, y + dest.height };
            bottomRight = (IvyVector2){ x + dest.width, y + dest.height };
        }
        else {
            const float sinRotation = Ivy_Math_SinF(rotation * IVY_MATH_DEG2RAD);
            const float cosRotation = Ivy_Math_CosF(rotation * IVY_MATH_DEG2RAD);
            const float x = dest.x;
            const float y = dest.y;
            const float dx = -origin.x;
            const float dy = -origin.y;

            topLeft.x = x + dx * cosRotation - dy * sinRotation;
            topLeft.y = y + dx * sinRotation + dy * cosRotation;

            topRight.x = x + (dx + dest.width) * cosRotation - dy * sinRotation;
            topRight.y = y + (dx + dest.width) * sinRotation + dy * cosRotation;

            bottomLeft.x = x + dx * cosRotation - (dy + dest.height) * sinRotation;
            bottomLeft.y = y + dx * sinRotation + (dy + dest.height) * cosRotation;

            bottomRight.x = x + (dx + dest.width) * cosRotation - (dy + dest.height) * sinRotation;
            bottomRight.y = y + (dx + dest.width) * sinRotation + (dy + dest.height) * cosRotation;
        }

        Ivy_Gfx_SetTexture(texture.id);
        Ivy_Gfx_Begin(IVY_QUADS);

        Ivy_Gfx_Color4UB(color.r, color.g, color.b, color.a);
        Ivy_Gfx_Normal3F(0.0f, 0.0f, 1.0f);

        // Top-left corner for texture and quad
        if (flipX) Ivy_Gfx_TextureCoord2F((src.x + src.width)/width, src.y/height);
        else Ivy_Gfx_TextureCoord2F(src.x/width, src.y/height);
        Ivy_Gfx_Vertex2F(topLeft.x, topLeft.y);

        // Bottom-left corner for texture and quad
        if (flipX) Ivy_Gfx_TextureCoord2F((src.x + src.width)/width, (src.y + src.height)/height);
        else Ivy_Gfx_TextureCoord2F(src.x/width, (src.y + src.height)/height);
        Ivy_Gfx_Vertex2F(bottomLeft.x, bottomLeft.y);

        // Bottom-right corner for texture and quad
        if (flipX) Ivy_Gfx_TextureCoord2F(src.x/width, (src.y + src.height)/height);
        else Ivy_Gfx_TextureCoord2F((src.x + src.width)/width, (src.y + src.height)/height);
        Ivy_Gfx_Vertex2F(bottomRight.x, bottomRight.y);

        // Top-right corner for texture and quad
        if (flipX) Ivy_Gfx_TextureCoord2F(src.x/width, src.y/height);
        else Ivy_Gfx_TextureCoord2F((src.x + src.width)/width, src.y/height);
        Ivy_Gfx_Vertex2F(topRight.x, topRight.y);

        Ivy_Gfx_End();
        Ivy_Gfx_SetTexture(0);
    }
}

void Ivy_Gfx_Begin(const int mode)
{
    if (IVY_GL.currentBatch->draws[IVY_GL.currentBatch->drawCounter - 1].mode != mode) {
        if (IVY_GL.currentBatch->draws[IVY_GL.currentBatch->drawCounter - 1].vertexCount > 0) {
            if (IVY_GL.currentBatch->draws[IVY_GL.currentBatch->drawCounter - 1].mode == IVY_LINES) IVY_GL.currentBatch->draws[IVY_GL.currentBatch->drawCounter - 1].vertexAlignment = ((IVY_GL.currentBatch->draws[IVY_GL.currentBatch->drawCounter - 1].vertexCount < 4) ? IVY_GL.currentBatch->draws[IVY_GL.currentBatch->drawCounter - 1].vertexCount : IVY_GL.currentBatch->draws[IVY_GL.currentBatch->drawCounter - 1].vertexCount % 4);
            else if (IVY_GL.currentBatch->draws[IVY_GL.currentBatch->drawCounter - 1].mode == IVY_TRIANGLES) IVY_GL.currentBatch->draws[IVY_GL.currentBatch->drawCounter - 1].vertexAlignment = ((IVY_GL.currentBatch->draws[IVY_GL.currentBatch->drawCounter - 1].vertexCount < 4) ? 1 : (4 - (IVY_GL.currentBatch->draws[IVY_GL.currentBatch->drawCounter - 1].vertexCount % 4)));
            else IVY_GL.currentBatch->draws[IVY_GL.currentBatch->drawCounter - 1].vertexAlignment = 0;

            if (!Ivy_Gfx_CheckRenderBatchLimit(IVY_GL.currentBatch->draws[IVY_GL.currentBatch->drawCounter - 1].vertexAlignment))
            {
                IVY_GL.state.vertexCounter += IVY_GL.currentBatch->draws[IVY_GL.currentBatch->drawCounter - 1].vertexAlignment;
                IVY_GL.currentBatch->drawCounter++;
            }
        }

        if (IVY_GL.currentBatch->drawCounter >= IVY_DEFAULT_BATCH_DRAWCALLS) Ivy_Gfx_DrawRenderBatch(IVY_GL.currentBatch);

        IVY_GL.currentBatch->draws[IVY_GL.currentBatch->drawCounter - 1].mode = mode;
        IVY_GL.currentBatch->draws[IVY_GL.currentBatch->drawCounter - 1].textureId = IVY_GL.state.currentTextureId;
        IVY_GL.state.currentTextureId = IVY_GL.state.defaultTextureId;
    }
}

void Ivy_Gfx_End(void)
{
    IVY_GL.currentBatch->currentDepth += (1.0f/20000.0f);
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

void Ivy_Gfx_BeginTextureMode(IvyRenderTexture target)
{
    Ivy_Gfx_DrawRenderBatchActive();

    Ivy_Gfx_EnableFramebuffer(target.id);

    glViewport(0, 0, target.texture.width, target.texture.height);

    IVY_GL.state.framebufferWidth = target.texture.width;
    IVY_GL.state.framebufferHeight = target.texture.height;

    Ivy_Gfx_MatrixMode(IVY_PROJECTION);
    Ivy_Gfx_LoadIdentity();

    Ivy_Gfx_Ortho(0, target.texture.width, target.texture.height, 0, 0.0f, 1.0f);

    Ivy_Gfx_MatrixMode(IVY_MODELVIEW);
    Ivy_Gfx_LoadIdentity();

    IVY_CORE.window.currentFBO.width = target.texture.width;
    IVY_CORE.window.currentFBO.height = target.texture.height;
    IVY_CORE.window.usingFBO = true;
}

void Ivy_Gfx_EndTextureMode(void)
{
    Ivy_Gfx_DrawRenderBatchActive();

    Ivy_Gfx_DisableFramebuffer();

    SetupViewport(IVY_CORE.window.render.width, IVY_CORE.window.render.height);

    Ivy_Gfx_MatrixMode(IVY_MODELVIEW);
    Ivy_Gfx_LoadIdentity();
    Ivy_Gfx_MatrixMultiply(Ivy_Math_MatrixToFloat(IVY_CORE.window.screenScale));

    IVY_CORE.window.currentFBO.width = IVY_CORE.window.render.width;
    IVY_CORE.window.currentFBO.height = IVY_CORE.window.render.height;
    IVY_CORE.window.usingFBO = false;
}

void Ivy_Gfx_UnloadFramebuffer(u32 id)
{
    int depthType = 0;
    glBindFramebuffer(GL_FRAMEBUFFER, id);
    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &depthType);

    int depthId = 0;
    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &depthId);

    u32 depthIdU = (u32)depthId;
    if (depthType == GL_RENDERBUFFER) glDeleteRenderbuffers(1, &depthIdU);
    else if (depthType == GL_TEXTURE) glDeleteTextures(1, &depthIdU);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &id);
}

void Ivy_Gfx_DrawTextureEx(IvyTexture texture, IvyVector2 position, float rotation, float scale, IvyColor tint)
{
    IvyRectangle src = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
    IvyRectangle dst = { position.x, position.y, (float)texture.width * scale, (float)texture.height * scale };
    IvyVector2 origin = { 0.0f, 0.0f };

    Ivy_Gfx_DrawTexturePro(texture, src, dst, origin, rotation, tint);
}

void Ivy_Gfx_DrawTexture(IvyTexture texture, int posX, int posY, IvyColor tint)
{
    Ivy_Gfx_DrawTextureEx(texture, (IvyVector2){ (float)posX, (float)posY }, 0.0f, 1.0f, tint);
}

void Ivy_Gfx_TextureCoord2F(const float x, const float y)
{
    IVY_GL.state.texcoordx = x;
    IVY_GL.state.texcoordy = y;
}

void Ivy_Gfx_Vertex3F(const float x, const float y, const float z)
{
    float tx = x;
    float ty = y;
    float tz = z;

    if (IVY_GL.state.transformRequired) {
        tx = IVY_GL.state.transform.m0*x + IVY_GL.state.transform.m4*y + IVY_GL.state.transform.m8*z + IVY_GL.state.transform.m12;
        ty = IVY_GL.state.transform.m1*x + IVY_GL.state.transform.m5*y + IVY_GL.state.transform.m9*z + IVY_GL.state.transform.m13;
        tz = IVY_GL.state.transform.m2*x + IVY_GL.state.transform.m6*y + IVY_GL.state.transform.m10*z + IVY_GL.state.transform.m14;
    }

    if (IVY_GL.state.vertexCounter > (IVY_GL.currentBatch->vertexBuffer[IVY_GL.currentBatch->currentBuffer].elementCount * 4 - 4))
    {
        if ((IVY_GL.currentBatch->draws[IVY_GL.currentBatch->drawCounter - 1].mode == IVY_LINES) && (IVY_GL.currentBatch->draws[IVY_GL.currentBatch->drawCounter - 1].vertexCount%2 == 0))
            Ivy_Gfx_CheckRenderBatchLimit(2 + 1);

        else if ((IVY_GL.currentBatch->draws[IVY_GL.currentBatch->drawCounter - 1].mode == IVY_TRIANGLES) && (IVY_GL.currentBatch->draws[IVY_GL.currentBatch->drawCounter - 1].vertexCount % 3 == 0))
            Ivy_Gfx_CheckRenderBatchLimit(3 + 1);

        else if ((IVY_GL.currentBatch->draws[IVY_GL.currentBatch->drawCounter - 1].mode == IVY_QUADS) && (IVY_GL.currentBatch->draws[IVY_GL.currentBatch->drawCounter - 1].vertexCount % 4 == 0))
            Ivy_Gfx_CheckRenderBatchLimit(4 + 1);
    }

    // Add vertices
    IVY_GL.currentBatch->vertexBuffer[IVY_GL.currentBatch->currentBuffer].vertices[3 * IVY_GL.state.vertexCounter] = tx;
    IVY_GL.currentBatch->vertexBuffer[IVY_GL.currentBatch->currentBuffer].vertices[3 * IVY_GL.state.vertexCounter + 1] = ty;
    IVY_GL.currentBatch->vertexBuffer[IVY_GL.currentBatch->currentBuffer].vertices[3 * IVY_GL.state.vertexCounter + 2] = tz;

    // Add current texcoord
    IVY_GL.currentBatch->vertexBuffer[IVY_GL.currentBatch->currentBuffer].texCoords[2 * IVY_GL.state.vertexCounter] = IVY_GL.state.texcoordx;
    IVY_GL.currentBatch->vertexBuffer[IVY_GL.currentBatch->currentBuffer].texCoords[2 * IVY_GL.state.vertexCounter + 1] = IVY_GL.state.texcoordy;

    // Add current normal
    IVY_GL.currentBatch->vertexBuffer[IVY_GL.currentBatch->currentBuffer].normals[3 * IVY_GL.state.vertexCounter] = IVY_GL.state.normalx;
    IVY_GL.currentBatch->vertexBuffer[IVY_GL.currentBatch->currentBuffer].normals[3 * IVY_GL.state.vertexCounter + 1] = IVY_GL.state.normaly;
    IVY_GL.currentBatch->vertexBuffer[IVY_GL.currentBatch->currentBuffer].normals[3 * IVY_GL.state.vertexCounter + 2] = IVY_GL.state.normalz;

    // Add current color
    IVY_GL.currentBatch->vertexBuffer[IVY_GL.currentBatch->currentBuffer].colors[4 * IVY_GL.state.vertexCounter] = IVY_GL.state.colorr;
    IVY_GL.currentBatch->vertexBuffer[IVY_GL.currentBatch->currentBuffer].colors[4 * IVY_GL.state.vertexCounter + 1] = IVY_GL.state.colorg;
    IVY_GL.currentBatch->vertexBuffer[IVY_GL.currentBatch->currentBuffer].colors[4 * IVY_GL.state.vertexCounter + 2] = IVY_GL.state.colorb;
    IVY_GL.currentBatch->vertexBuffer[IVY_GL.currentBatch->currentBuffer].colors[4 * IVY_GL.state.vertexCounter + 3] = IVY_GL.state.colora;

    IVY_GL.state.vertexCounter++;
    IVY_GL.currentBatch->draws[IVY_GL.currentBatch->drawCounter - 1].vertexCount++;
}

void Ivy_Gfx_Vertex2F(const float x, const float y)
{
    Ivy_Gfx_Vertex3F(x, y, IVY_GL.currentBatch->currentDepth);
}

void Ivy_Gfx_Color4UB(const u8 x, const u8 y, const u8 z, const u8 w)
{
    IVY_GL.state.colorr = x;
    IVY_GL.state.colorg = y;
    IVY_GL.state.colorb = z;
    IVY_GL.state.colora = w;
}

void Ivy_Gfx_Normal3F(float x, float y, float z)
{
    float normalX = x;
    float normalY = y;
    float normalZ = z;

    if (IVY_GL.state.transformRequired)
    {
        normalX = IVY_GL.state.transform.m0 * x + IVY_GL.state.transform.m4 * y + IVY_GL.state.transform.m8 * z;
        normalY = IVY_GL.state.transform.m1 * x + IVY_GL.state.transform.m5 * y + IVY_GL.state.transform.m9 * z;
        normalZ = IVY_GL.state.transform.m2 * x + IVY_GL.state.transform.m6 * y + IVY_GL.state.transform.m10 * z;
    }

    IVY_GL.state.normalx = normalX;
    IVY_GL.state.normaly = normalY;
    IVY_GL.state.normalz = normalZ;
}

void Ivy_Gfx_MatrixMode(int mode)
{
    if (mode == IVY_PROJECTION) IVY_GL.state.currentMatrix = &IVY_GL.state.projection;
    else if (mode == IVY_MODELVIEW) IVY_GL.state.currentMatrix = &IVY_GL.state.modelview;

    IVY_GL.state.currentMatrixMode = mode;
}

void Ivy_Gfx_Ortho(double left, double right, double bottom, double top, double znear, double zfar)
{
    IvyMatrix matOrtho = { 0 };

    const float rl = (float)(right - left);
    const float tb = (float)(top - bottom);
    const float fn = (float)(zfar - znear);

    matOrtho.m0 = 2.0f/rl;
    matOrtho.m1 = 0.0f;
    matOrtho.m2 = 0.0f;
    matOrtho.m3 = 0.0f;
    matOrtho.m4 = 0.0f;
    matOrtho.m5 = 2.0f/tb;
    matOrtho.m6 = 0.0f;
    matOrtho.m7 = 0.0f;
    matOrtho.m8 = 0.0f;
    matOrtho.m9 = 0.0f;
    matOrtho.m10 = -2.0f/fn;
    matOrtho.m11 = 0.0f;
    matOrtho.m12 = -((float)left + (float)right)/rl;
    matOrtho.m13 = -((float)top + (float)bottom)/tb;
    matOrtho.m14 = -((float)zfar + (float)znear)/fn;
    matOrtho.m15 = 1.0f;

    *IVY_GL.state.currentMatrix = Ivy_Math_MatrixMultiply(*IVY_GL.state.currentMatrix, matOrtho);
}
