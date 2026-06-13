#include "ivy/math.h"

IvyMatrix Ivy_Math_MatrixIdentity(void)
{
    const IvyMatrix result = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };
    return result;
}

IvyMatrix Ivy_Math_MatrixScale(const float x, const float y, const float z)
{
    const IvyMatrix result = {
           x, 0.0f, 0.0f, 0.0f,
        0.0f,    y, 0.0f, 0.0f,
        0.0f, 0.0f,    z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };
    return result;
}

IvyMatrix Ivy_Math_MatrixMultiply(const IvyMatrix left, const IvyMatrix right)
{
    IvyMatrix r = { 0 };

    r.m0  = left.m0*right.m0  + left.m1*right.m4  + left.m2*right.m8   + left.m3*right.m12;
    r.m1  = left.m0*right.m1  + left.m1*right.m5  + left.m2*right.m9   + left.m3*right.m13;
    r.m2  = left.m0*right.m2  + left.m1*right.m6  + left.m2*right.m10  + left.m3*right.m14;
    r.m3  = left.m0*right.m3  + left.m1*right.m7  + left.m2*right.m11  + left.m3*right.m15;
    r.m4  = left.m4*right.m0  + left.m5*right.m4  + left.m6*right.m8   + left.m7*right.m12;
    r.m5  = left.m4*right.m1  + left.m5*right.m5  + left.m6*right.m9   + left.m7*right.m13;
    r.m6  = left.m4*right.m2  + left.m5*right.m6  + left.m6*right.m10  + left.m7*right.m14;
    r.m7  = left.m4*right.m3  + left.m5*right.m7  + left.m6*right.m11  + left.m7*right.m15;
    r.m8  = left.m8*right.m0  + left.m9*right.m4  + left.m10*right.m8  + left.m11*right.m12;
    r.m9  = left.m8*right.m1  + left.m9*right.m5  + left.m10*right.m9  + left.m11*right.m13;
    r.m10 = left.m8*right.m2  + left.m9*right.m6  + left.m10*right.m10 + left.m11*right.m14;
    r.m11 = left.m8*right.m3  + left.m9*right.m7  + left.m10*right.m11 + left.m11*right.m15;
    r.m12 = left.m12*right.m0 + left.m13*right.m4 + left.m14*right.m8  + left.m15*right.m12;
    r.m13 = left.m12*right.m1 + left.m13*right.m5 + left.m14*right.m9  + left.m15*right.m13;
    r.m14 = left.m12*right.m2 + left.m13*right.m6 + left.m14*right.m10 + left.m15*right.m14;
    r.m15 = left.m12*right.m3 + left.m13*right.m7 + left.m14*right.m11 + left.m15*right.m15;

    return r;
}

IvyFloat16 Ivy_Math_MatrixToFloatV(const IvyMatrix m)
{
    return (IvyFloat16){ .v = {
        m.m0, m.m1, m.m2,  m.m3,
        m.m4, m.m5, m.m6,  m.m7,
        m.m8, m.m9, m.m10, m.m11,
        m.m12, m.m13, m.m14, m.m15,
    }};
}

float Ivy_Math_FloorF(const float f)
{
    const int i = (int)f;
    return (f < (float)i) ? (float)(i - 1) : (float)i;
}

float Ivy_Math_SinF(float x)
{
    if (x < -IVY_MATH_PI) x += 2.0f * IVY_MATH_PI;
    else if (x > IVY_MATH_PI) x -= 2.0f * IVY_MATH_PI;

    const float y = 1.27323954f * x;        // 4 / PI
    const float z = 0.405284735f * x * x;   // 4 / (PI * PI)
    const float q = (x < 0.0f) ? (y + z) : (y - z);

    return 0.225f * (q * ((q < 0.0f) ? -q : q) - q) + q;
}

float Ivy_Math_CosF(float x)
{
    x += 1.57079632f; // IVY_MATH_PI / 2

    if (x > IVY_MATH_PI) x -= 2.0f * IVY_MATH_PI;

    return Ivy_Math_SinF(x);
}

IvyMatrix Ivy_Math_MatrixTranspose(IvyMatrix mat)
{
    IvyMatrix result = { 0 };

    result.m0 = mat.m0;
    result.m1 = mat.m4;
    result.m2 = mat.m8;
    result.m3 = mat.m12;
    result.m4 = mat.m1;
    result.m5 = mat.m5;
    result.m6 = mat.m9;
    result.m7 = mat.m13;
    result.m8 = mat.m2;
    result.m9 = mat.m6;
    result.m10 = mat.m10;
    result.m11 = mat.m14;
    result.m12 = mat.m3;
    result.m13 = mat.m7;
    result.m14 = mat.m11;
    result.m15 = mat.m15;

    return result;
}

IvyMatrix Ivy_Math_MatrixInvert(IvyMatrix mat)
{
    IvyMatrix result = { 0 };

    // Cache the matrix values (speed optimization)
    float a00 = mat.m0, a01 = mat.m1, a02 = mat.m2, a03 = mat.m3;
    float a10 = mat.m4, a11 = mat.m5, a12 = mat.m6, a13 = mat.m7;
    float a20 = mat.m8, a21 = mat.m9, a22 = mat.m10, a23 = mat.m11;
    float a30 = mat.m12, a31 = mat.m13, a32 = mat.m14, a33 = mat.m15;

    float b00 = a00*a11 - a01*a10;
    float b01 = a00*a12 - a02*a10;
    float b02 = a00*a13 - a03*a10;
    float b03 = a01*a12 - a02*a11;
    float b04 = a01*a13 - a03*a11;
    float b05 = a02*a13 - a03*a12;
    float b06 = a20*a31 - a21*a30;
    float b07 = a20*a32 - a22*a30;
    float b08 = a20*a33 - a23*a30;
    float b09 = a21*a32 - a22*a31;
    float b10 = a21*a33 - a23*a31;
    float b11 = a22*a33 - a23*a32;

    // Calculate the invert determinant (inlined to avoid double-caching)
    float invDet = 1.0f/(b00*b11 - b01*b10 + b02*b09 + b03*b08 - b04*b07 + b05*b06);

    result.m0 = (a11*b11 - a12*b10 + a13*b09)*invDet;
    result.m1 = (-a01*b11 + a02*b10 - a03*b09)*invDet;
    result.m2 = (a31*b05 - a32*b04 + a33*b03)*invDet;
    result.m3 = (-a21*b05 + a22*b04 - a23*b03)*invDet;
    result.m4 = (-a10*b11 + a12*b08 - a13*b07)*invDet;
    result.m5 = (a00*b11 - a02*b08 + a03*b07)*invDet;
    result.m6 = (-a30*b05 + a32*b02 - a33*b01)*invDet;
    result.m7 = (a20*b05 - a22*b02 + a23*b01)*invDet;
    result.m8 = (a10*b10 - a11*b08 + a13*b06)*invDet;
    result.m9 = (-a00*b10 + a01*b08 - a03*b06)*invDet;
    result.m10 = (a30*b04 - a31*b02 + a33*b00)*invDet;
    result.m11 = (-a20*b04 + a21*b02 - a23*b00)*invDet;
    result.m12 = (-a10*b09 + a11*b07 - a12*b06)*invDet;
    result.m13 = (a00*b09 - a01*b07 + a02*b06)*invDet;
    result.m14 = (-a30*b03 + a31*b01 - a32*b00)*invDet;
    result.m15 = (a20*b03 - a21*b01 + a22*b00)*invDet;

    return result;
}
