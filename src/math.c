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
