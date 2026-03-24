#include "../defines.h"

typedef float vec3_t[3];

typedef struct FEntity
{
    vec3_t position;
    vec3_t velocity;
    float padding; // Wichtig für SIMD-Alignment (16-Byte-Grenzen)

} FEntity;

typedef union vec2_u
{   
    // An Array Of X,Y
    f32 elements[2];
    struct 
    {
        union
        {
            //The First element.
            f32 x, r, s, u;  
        };
        union 
        {
            //The Second element.
            f32 y, g, t, v;  
        };
    };
}vec2;

typedef struct vec3_u
{   
    union 
    {
        // An Array Of X,Y
        f32 elements[3];
        struct 
        {
            union
            {
                //The First element.
                f32 x, r, s, u;  
            };
            union 
            {
                //The Second element.
                f32 y, g, t, v;  
            };
            union 
            {
                //The Third element.
                f32 z, b, p, w;  
            };
        };
    };
}vec3;

typedef union vec4_u
{   
    #if defined(FUSE_SIMD)
    //Used for SIMD operations
    alignas(16) __m128 data;
    #endif
    // An Array Of X,Y
    alignas(16) f32 elements[4];
    union
    {
        struct 
        {
            union
            {
                //The First element.
                f32 x, r, s, u;  
            };
            union 
            {
                //The Second element.
                f32 y, g, t, v;  
            };
            union 
            {
                //The Third element.
                f32 z, b, p, w;  
            };
            union 
            {
                //The Fourth element.
                f32 w, a, q;  
            };
        };
    }
}vec4;

typedef vec4 quat;