#ifndef IVY_MATH_H
#define IVY_MATH_H

#include "ivy/types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define IVY_MATH_PI 3.14159265358979323846f
#define IVY_MATH_DEG2RAD (IVY_MATH_PI / 180.0f)

typedef struct {
    int x;
    int y;
} IvyPoint;

typedef struct {
    u32 width;
    u32 height;
} IvySize;

typedef struct {
    float x;
    float y;
} IvyVector2;

typedef struct {
    float x, y;
    float width, height;
} IvyRectangle;

typedef struct {
    float m0,  m4,  m8,  m12;
    float m1,  m5,  m9,  m13;
    float m2,  m6,  m10, m14;
    float m3,  m7,  m11, m15;
} IvyMatrix;

typedef struct {
    float v[16];
} IvyFloat16;

IvyMatrix  Ivy_Math_MatrixIdentity(void);
IvyMatrix  Ivy_Math_MatrixScale(float x, float y, float z);
IvyMatrix  Ivy_Math_MatrixMultiply(IvyMatrix left, IvyMatrix right);
IvyFloat16 Ivy_Math_MatrixToFloatV(IvyMatrix mat);
IvyMatrix Ivy_Math_MatrixTranspose(IvyMatrix mat);
IvyMatrix Ivy_Math_MatrixInvert(IvyMatrix mat);

float Ivy_Math_FloorF(float f);
float Ivy_Math_SinF(float x);
float Ivy_Math_CosF(float x);

#define Ivy_Math_MatrixToFloat(mat) (Ivy_Math_MatrixToFloatV(mat).v)

#ifdef __cplusplus
}
#endif

#endif
