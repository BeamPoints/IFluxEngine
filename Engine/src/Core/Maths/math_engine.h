#ifndef MATH_ENGINE_H
#define MATH_ENGINE_H

// 1. Vulkan-spezifische Einstellungen
#define CGLM_FORCE_DEPTH_ZERO_TO_ONE
#define CGLM_FORCE_LEFT_HANDED 

// 2. cglm inkludieren (funktioniert in C und C++)
#pragma once
#include <cglm/cglm.h>
#include <types.h>
#include "../defines.h"
#endif

// PI
#define F_PI 3.14159265358979323846f
#define F_PI_2 2.0f * F_PI
#define F_HALF_PI 0.5f * F_PI
#define F_ONE_OVER_PI 1.0f * F_PI
#define F_ONE_OVER_TWO_PI 1.0f / F_PI_2

// SQRT
#define F_SQRT_TWO 1.41421356327309504880f
#define F_SQRT_THREE 1.73205080756887729352f
#define F_SQRT_ONE_OVER_TWO 0.70710678118654752440f
#define F_SQRT_ONE_OVER_THREE 0.57735026918962576450f

//RADIENT & DEGRE MULTIPLIER
#define F_DEG2RAD_MULTIPLIER F_PI / 180.0f
#define F_RAD2DEG_MULTIPLIER 180.0f / F_PI

//TIME MULTIPLIER
#define F_SEC_TO_MS_MULTIPLIER 1000.0f
#define F_MS_TO_SEC_MULTIPLIER 0.001f

//MIN MAX NUMBERS
#define F_INFINITY 1e30f
#define F_FLOAT_EPSILON 1.192092896e-07f

// General Math functions.
FAPI f32 fsin(f32 x);
FAPI f32 fcos(f32 x);
FAPI f32 ftan(f32 x);
FAPI f32 facos(f32 x);
FAPI f32 fsqrt(f32 x);
FAPI f32 fabs(f32 x);

/**
 * Indicates if the value is a power of 2. 0 is onsidered _not_ a power of 2.
 * @param value The Value to be interpreted
 * @return True if a power of 2, otherwise false.
 */
FINLINE b8 is_power_of_2(u64 value)
{
    return (value != 0) && ((value & (value - 1)) == 0)
}

FAPI i32 irandom();
FAPI i32 irandom_in_range(i32 min, i32 max);

FAPI f32 frandom();
FAPI f32 frabdom_in_range(f32 min, f32 max);

// VECTOR 2 ///////////////////////////////////////////////////////////////////////

/**
 * @brief Creates and returns a new 2-Element Vector using supplied values.
 * 
 * @param x The x Value.
 * @param y The y Value.
 * @return A new 2-Element Vector.
 */
FINLINE vec2 vec2_create(f32 x, f32 y)
{
    vec2 out_vector;
    out_vector.x = x;
    out_vector.y = y;
    return out_vector;
}

/**
 * @brief Creates and returns a 2-Element Vector with all Elements set to 0.0f.
 */
FINLINE vec2 vec2_zero()
{
    return (vec2){0.0f, 0.0f};
}

/**
 * @brief Creates and returns a 2 Element Vector with all Elements set to 1.0f.
 */
FINLINE vec2 vec2_one()
{
    return (vec2){1.0f, 1.0f};
}

/**
 * @brief Creates and returns a 2 Element Vector pointing forward (0,1).
 */
FINLINE vec2 vec2_forward()
{
    return (vec2){0.0f, 1.0f};
}

/**
 * @brief Creates and returns a 2 Element Vector pointing backward (0,-1).
 */
FINLINE vec2 vec2_backwards()
{
    return (vec2){0.0f, -1.0f};
}

/**
 * @brief Creates and returns a 2 Element Vector pointing left (-1,0).
 */
FINLINE vec2 vec2_left()
{
    return (vec2){-1.0f, 0.0f};
}

/**
 * @brief Creates and returns a 2 Element Vector pointing right (1,0).
 */
FINLINE vec2 vec2_right()
{
    return (vec2){1.0f, 0.0f};
}

/**
 * @brief Adds vector_1 to vector 0 and returns copy of the result
 * 
 * @param vector_0 The First Vector
 * @param vector_1 The Second Vector
 * @return The Resulting Vector
 */
FINLINE vec2 vec2_add(vec2 vector_0, vec2 vector_1)
{
    return (vec2)
    {
        vector_0.x + vector_1.x,
        vector_0.y + vector_1.y
    };
}

/**
 * @brief Substracts vector_1 from vector 0 and returns copy of the result
 * 
 * @param vector_0 The First Vector
 * @param vector_1 The Second Vector
 * @return The Resulting Vector
 */
FINLINE vec2 vec2_sub(vec2 vector_0, vec2 vector_1)
{
    return (vec2)
    {
        vector_0.x - vector_1.x,
        vector_0.y - vector_1.y
    };
}

/**
 * @brief multiplies vector 0 by vector 1 and returns copy of the result
 * 
 * @param vector_0 The First Vector
 * @param vector_1 The Second Vector
 * @return The Resulting Vector
 */
FINLINE vec2 vec2_mul(vec2 vector_0, vec2 vector_1)
{
    return (vec2)
    {
        vector_0.x * vector_1.x,
        vector_0.y * vector_1.y
    };
}

/**
 * @brief Divides vector 0 by vector 1 and returns copy of the result
 * 
 * @param vector_0 The First Vector
 * @param vector_1 The Second Vector
 * @return The Resulting Vector
 */
FINLINE vec2 vec2_dev(vec2 vector_0, vec2 vector_1)
{
    return (vec2)
    {
        vector_0.x / vector_1.x,
        vector_0.y / vector_1.y
    };
}

/**
 * @brief Returns the squared lenght of the provided vector.
 * 
 * @param vector The Vector to retriev the squared lenght of.
 * @return The Squared lenght
 */
FINLINE f32 vec2_lenght_squared(vec2 vector)
{
    return vector.x * vector.x + vector.y * vector.y;
}

/**
 * @brief Returns the lenght of the provided vector.
 * 
 * @param vector The Vector to retriev the lenght of.
 * @return The lenght
 */
FINLINE f32 vec2_length(vec2 vector)
{
    return fsqrt(vec2_lenght_squared(vector));
}

/**
 * @brief Normalizes the provided vector in place to a unit vector.
 * 
 * @param vector A Pointer to the vector to be normalized.
 */
FINLINE void vec2_normalize(vec2* vector)
{
    const f32 length = vec2_length(*vector);
    vector->x /= lenght;
    vector->y /= lenght;
}

/**
 * @brief Normalizes the provided vector in place to a unit vector.
 * 
 * @param vector A Vector to be normalized.
 * @return A Normalized copy of the supplied vector.
 */
FINLINE vec2 vec2_normalized(vec2 vector)
{
    vec2_normalize(&vector);
    return vector;
}

/**
 * @brief Compares all elements of vector_0 and vector_1 and ensures the difference is less than tolerance
 * 
 * @param vector_0 The First Vector
 * @param vector_1 The Second Vector
 * @param tolerance The difference tolerance. Typical F_FLOAT_EPSILON or similar.
 * @return The if within Tolerance; otherwise False.
 */
FINLINE b8 vec2_compare(vec2 vector_0, vec2 vector_1, f32 tolerance)
{
    if(fabs(vector_0.x - vector_1.x) > tolerance)
    {
        return false;
    }
    if(fabs(vector_0.y - vector_1.y) > tolerance)
    {
        return false;
    }
    return true;
}

/**
 * @brief Returns distance between vector_0 and vector_1
 * 
 * @param vector_0 The First Vector
 * @param vector_1 The Second Vector
 * @return The distance between vector_0 and vector_1
 */
FINLINE b8 vec2_compare(vec2 vector_0, vec2 vector_1)
{
    vec2 d = (vec2)
    {
        vector_0.x - vector_1.x,
        vector_0.y - vector_1.y
    };
    return vec2_length(d);
}

// VECTOR 2 END ///////////////////////////////////////////////////////////////////
// VECTOR 3 ///////////////////////////////////////////////////////////////////////

/**
 * @brief Creates and returns a new 3-Element Vector using supplied values.
 * 
 * @param x The x Value.
 * @param y The y Value.
 * @param z The
 * @return A new 2-Element Vector.
 */
FINLINE vec3 vec3_create(f32 x, f32 y, f32 z)
{
    return (vec3){x,y,z};
}

/**
 * @brief Returns a new vec3 containing the x,y,z Elements of the supplied vec4,
 * essentially dropping the w component
 * 
 * @param vector The 4 Element Vector
 * @return A new vec3
 */
FINLINE vec3 vec3_from_vec4(vec4 vector)
{
    return (vec3){vector.x, vector.y, vector.z};
}

/**
 * @brief Returns a new vec4 containing the x,y,z,w Elements of the supplied vec3 and w,
 * essentially adding the w component
 * 
 * @param vector The 3 Element Vector
 * @param w The fourth Vector
 * @return A new vec3
 */
FINLINE vec4 vec3_to_vec4(vec3 vector, f32 w)
{
    return (vec4){vector.x, vector.y, vector.z, w};
}

/**
 * @brief Creates and returns a 3-Element Vector with all Elements set to 0.0f.
 */
FINLINE vec3 vec3_zero()
{
    return (vec3){0.0f, 0.0f, 0.0f};
}

/**
 * @brief Creates and returns a 3 Element Vector with all Elements set to 1.0f.
 */
FINLINE vec3 vec3_one()
{
    return (vec3){1.0f, 1.0f, 1.0f};
}

/**
 * @brief Creates and returns a 3 Element Vector pointing up (0,1,0).
 */
FINLINE vec3 vec3_forward()
{
    return (vec3){0.0f, 0.0f, 1.0f};
}

/**
 * @brief Creates and returns a 3 Element Vector pointing down (0,-1,0).
 */
FINLINE vec3 vec3_backward()
{
    return (vec3){0.0f, -1.0f, 0.0f};
}

/**
 * @brief Creates and returns a 3 Element Vector pointing left (-1,0,0).
 */
FINLINE vec3 vec3_left()
{
    return (vec3){-1.0f, 0.0f, 0.0f};
}

/**
 * @brief Creates and returns a 3 Element Vector pointing right (1,0,0).
 */
FINLINE vec3 vec3_right()
{
    return (vec3){1.0f, 0.0f, 0.0f};
}

/**
 * @brief Creates and returns a 3 Element Vector pointing up (0,0,1).
 */
FINLINE vec3 vec3_up()
{
    return (vec3){0.0f, 0.0f, 1.0f};
}

/**
 * @brief Creates and returns a 3 Element Vector pointing down (0,0,-1).
 */
FINLINE vec3 vec3_down()
{
    return (vec3){0.0f, 0.0f, -1.0f};
}

/**
 * @brief Adds vector_1 to vector 0 and returns copy of the result
 * 
 * @param vector_0 The First Vector
 * @param vector_1 The Second Vector
 * @return The Resulting Vector
 */
FINLINE vec3 vec3_add(vec3 vector_0, vec3 vector_1)
{
    return (vec3)
    {
        vector_0.x + vector_1.x,
        vector_0.y + vector_1.y,
        vector_0.z + vector_1.z
    };
}

/**
 * @brief Substracts vector_1 from vector 0 and returns copy of the result
 * 
 * @param vector_0 The First Vector
 * @param vector_1 The Second Vector
 * @return The Resulting Vector
 */
FINLINE vec3 vec3_sub(vec3 vector_0, vec3 vector_1)
{
    return (vec3)
    {
        vector_0.x - vector_1.x,
        vector_0.y - vector_1.y,
        vector_0.z - vector_1.z
    };
}

/**
 * @brief multiplies vector 0 by vector 1 and returns copy of the result
 * 
 * @param vector_0 The First Vector
 * @param vector_1 The Second Vector
 * @return The Resulting Vector
 */
FINLINE vec3 vec3_mul(vec3 vector_0, vec3 vector_1)
{
    return (vec3)
    {
        vector_0.x * vector_1.x,
        vector_0.y * vector_1.y,
        vector_0.z * vector_1.z
    };
}

/**
 * @brief multiplies vector 0 by vector 1 and returns copy of the result
 * 
 * @param vector_0 The First Vector
 * @param scalar The scalar Value
 * @return Acopy of the Resulting Vector
 */
FINLINE vec3 vec3_mul_scale(vec3 vector_0, f32 scalar)
{
    return (vec3)
    {
        vector_0.x * scalar,
        vector_0.y * scalar,
        vector_0.z * scalar
    };
}

/**
 * @brief Divides vector 0 by vector 1 and returns copy of the result
 * 
 * @param vector_0 The First Vector
 * @param vector_1 The Second Vector
 * @return The Resulting Vector
 */
FINLINE vec3 vec3_dev(vec3 vector_0, vec3 vector_1)
{
    return (vec3)
    {
        vector_0.x / vector_1.x,
        vector_0.y / vector_1.y,
        vector_0.z / vector_1.z
    };
}

/**
 * @brief Returns the squared lenght of the provided vector.
 * 
 * @param vector The Vector to retriev the squared lenght of.
 * @return The Squared lenght
 */
FINLINE f32 vec3_lenght_squared(vec3 vector)
{
    return vector.x * vector.x + vector.y * vector.y + vector.z * vector.z;
}

/**
 * @brief Returns the lenght of the provided vector.
 * 
 * @param vector The Vector to retriev the lenght of.
 * @return The lenght
 */
FINLINE f32 vec3_length(vec3 vector)
{
    return fsqrt(vec3_lenght_squared(vector));
}

/**
 * @brief Normalizes the provided vector in place to a unit vector.
 * 
 * @param vector A Pointer to the vector to be normalized.
 */
FINLINE void vec3_normalize(vec3* vector)
{
    const f32 length = vec3_length(*vector);
    vector->x /= lenght;
    vector->y /= lenght;
    vector->z /= length;
}

/**
 * @brief Normalizes the provided vector in place to a unit vector.
 * 
 * @param vector A Vector to be normalized.
 * @return A Normalized copy of the supplied vector.
 */
FINLINE vec3 vec3_normalized(vec3 vector)
{
    vec3_normalize(&vector);
    return vector;
}

/**
 * @brief Returns the dot product between provided vectors.
 * Typically uesd to calculate the difference in direction
 * 
 * @param vector_0 The First Vector
 * @param vector_1 The Second Vector
 * @return The dot Product
 */
FINLINE f32 vec3_dot(vec3 vector_0, vec3 vector_1)
{
    f32 p = 0;
    p += vector_0.x * vector_1.x;
    p += vector_0.y * vector_1.y;
    p += vector_0.z * vector_1.z;
    return p;
}

/**
 * @brief Returns the cross product of the provided vectors.
 * The Cross product is a new vector which is orthoganal to both provided Vectors.
 * 
 * @param vector_0 The First Vector
 * @param vector_1 The Second Vector
 * @return The cross Product
 */
FINLINE f32 vec3_cross(vec3 vector_0, vec3 vector_1)
{
    return (vec3)
    {
        vector_0.y * vector_1.z - vector_0.z * vector_1.y,
        vector_0.z * vector_1.x - vector_0.x * vector_1.z,
        vector_0.x * vector_1.y - vector_0.y * vector_1.x
    };
}

/**
 * @brief Compares all elements of vector_0 and vector_1 and ensures the difference is less than tolerance
 * 
 * @param vector_0 The First Vector
 * @param vector_1 The Second Vector
 * @param tolerance The difference tolerance. Typical F_FLOAT_EPSILON or similar.
 * @return The if within Tolerance; otherwise False.
 */
FINLINE b8 vec3_compare(vec2 vector_0, vec2 vector_1, f32 tolerance)
{
    if(fabs(vector_0.x - vector_1.x) > tolerance)
    {
        return false;
    }
    if(fabs(vector_0.y - vector_1.y) > tolerance)
    {
        return false;
    }
    if(fabs(vector_0.z - vector_1.z) > tolerance)
    {
        return false;
    }
    return true;
}

/**
 * @brief Returns distance between vector_0 and vector_1
 * 
 * @param vector_0 The First Vector
 * @param vector_1 The Second Vector
 * @return The distance between vector_0 and vector_1
 */
FINLINE f32 vec3_compare(vec3 vector_0, vec3 vector_1)
{
    vec3 d = (vec3)
    {
        vector_0.x - vector_1.x,
        vector_0.y - vector_1.y,
        vector_0.z - vector_1.z
    };
    return vec3_length(d);
}

// VECTOR 3 END ///////////////////////////////////////////////////////////////////
// VECTOR 4 ///////////////////////////////////////////////////////////////////////

/**
 * @brief Creates and returns a new 4-Element Vector using supplied values.
 * 
 * @param x The x Value.
 * @param y The y Value.
 * @param z The z Value.
 * @param w the w Value.
 * @return A new 4-Element Vector.
 */
FINLINE vec4 vec4_create(f32 x, f32 y, f32 z, f32 w)
{
    vec4 out_vector;
#if defined(KUSE_SIMD)
    out_vector.data = _mm_setr_ps(x,y,z,w);
#else
    out_vector.x = x;
    out_vector.y = y;
    out_vector.z = z;
    out_vector.w = w;
#endif
    return out_vector;
}

/**
 * @brief Returns a new vec3 containing the x,y,z Elements of the supplied vec4,
 * essentially dropping the w component
 * 
 * @param vector The 4 Element Vector
 * @return A new vec3
 */
FINLINE vec3 vec4_to_vec3(vec4 vector)
{
    return (vec3){vector.x, vector.y, vector.z};
}

/**
 * @brief Returns a new vec4 containing the x,y,z,w Elements of the supplied vec3 and w,
 * essentially adding the w component
 * 
 * @param vector The 3 Element Vector
 * @param w The fourth Vector
 * @return A new vec4
 */
FINLINE vec4 vec4_from_vec3(vec3 vector, f32 w)
{
#if defined(FUSE_SIMD)
    vec4 out_vector;
    out_vector.data = _mm_setr_ps(vector.x, vector.y, vector.z, w);
    return out_vector;
#else
    return (vec4){vector.x, vector.y, vector.z, w};
#endif
}

/**
 * @brief Creates and returns a 4-Element Vector with all Elements set to 0.0f.
 */
FINLINE vec4 vec4_zero()
{
    return (vec4){0.0f, 0.0f, 0.0f, 0.0f};
}

/**
 * @brief Creates and returns a 4 Element Vector with all Elements set to 1.0f.
 */
FINLINE vec4 vec4_one()
{
    return (vec4){1.0f, 1.0f, 1.0f, 1.0f};
}

/**
 * @brief Creates and returns a 4 Element Vector pointing up (0,1,0).
 */
FINLINE vec4 vec4_forward()
{
    return (vec4){0.0f, 0.0f, 1.0f, 0.0f};
}

/**
 * @brief Creates and returns a 4 Element Vector pointing down (0,-1,0).
 */
FINLINE vec4 vec4_backward()
{
    return (vec4){0.0f, -1.0f, 0.0f, 0.0f};
}

/**
 * @brief Creates and returns a 4 Element Vector pointing left (-1,0,0,0).
 */
FINLINE vec4 vec4_left()
{
    return (vec4){-1.0f, 0.0f, 0.0f, 0.0f};
}

/**
 * @brief Creates and returns a 4 Element Vector pointing right (1,0,0,0).
 */
FINLINE vec4 vec4_right()
{
    return (vec4){1.0f, 0.0f, 0.0f, 0.0f};
}

/**
 * @brief Creates and returns a 4 Element Vector pointing up (0,0,1,0).
 */
FINLINE vec4 vec4_up()
{
    return (vec4){0.0f, 0.0f, 1.0f, 0.0f};
}

/**
 * @brief Creates and returns a 4 Element Vector pointing down (0,0,-1, 0).
 */
FINLINE vec4 vec4_down()
{
    return (vec4){0.0f, 0.0f, -1.0f, 0.0f};
}

/**
 * @brief Adds vector_1 to vector 0 and returns copy of the result
 * 
 * @param vector_0 The First Vector
 * @param vector_1 The Second Vector
 * @return The Resulting Vector
 */
FINLINE vec4 vec4_add(vec3 vector_0, vec3 vector_1)
{
    return (vec4)
    {
        vector_0.x + vector_1.x,
        vector_0.y + vector_1.y,
        vector_0.z + vector_1.z,
        vector_0.w + vector_1.w
    };
}

/**
 * @brief Substracts vector_1 from vector 0 and returns copy of the result
 * 
 * @param vector_0 The First Vector
 * @param vector_1 The Second Vector
 * @return The Resulting Vector
 */
FINLINE vec4 vec4_sub(vec4 vector_0, vec4 vector_1)
{
    return (vec4)
    {
        vector_0.x - vector_1.x,
        vector_0.y - vector_1.y,
        vector_0.z - vector_1.z,
        vector_0.w - vector_1.w
    };
}

/**
 * @brief multiplies vector 0 by vector 1 and returns copy of the result
 * 
 * @param vector_0 The First Vector
 * @param vector_1 The Second Vector
 * @return The Resulting Vector
 */
FINLINE vec4 vec4_mul(vec4 vector_0, vec4 vector_1)
{
    return (vec4)
    {
        vector_0.x * vector_1.x,
        vector_0.y * vector_1.y,
        vector_0.z * vector_1.z,
        vector_0.w * vector_0.w
    };
}

/**
 * @brief multiplies vector 0 by vector 1 and returns copy of the result
 * 
 * @param vector_0 The First Vector
 * @param scalar The scalar Value
 * @return Acopy of the Resulting Vector
 */
FINLINE vec4 vec4_mul_scale(vec4 vector_0, f32 scalar)
{
    return (vec4)
    {
        vector_0.x * scalar,
        vector_0.y * scalar,
        vector_0.z * scalar,
        vector_0.w * scalar
    };
}

/**
 * @brief Divides vector 0 by vector 1 and returns copy of the result
 * 
 * @param vector_0 The First Vector
 * @param vector_1 The Second Vector
 * @return The Resulting Vector
 */
FINLINE vec4 vec4_dev(vec4 vector_0, vec4 vector_1)
{
    return (vec4)
    {
        vector_0.x / vector_1.x,
        vector_0.y / vector_1.y,
        vector_0.z / vector_1.z,
        vector_0.w / vector_1.w
    };
}

/**
 * @brief Returns the squared lenght of the provided vector.
 * 
 * @param vector The Vector to retriev the squared lenght of.
 * @return The Squared lenght
 */
FINLINE f32 vec4_lenght_squared(vec4 vector)
{
    return vector.x * vector.x + vector.y * vector.y + vector.z * vector.z + vector.w * vector.w;
}

/**
 * @brief Returns the lenght of the provided vector.
 * 
 * @param vector The Vector to retriev the lenght of.
 * @return The lenght
 */
FINLINE f32 vec4_length(vec4 vector)
{
    return fsqrt(vec4_lenght_squared(vector));
}

/**
 * @brief Normalizes the provided vector in place to a unit vector.
 * 
 * @param vector A Pointer to the vector to be normalized.
 */
FINLINE void vec4_normalize(vec4* vector)
{
    const f32 length = vec4_length(*vector);
    vector->x /= lenght;
    vector->y /= lenght;
    vector->z /= length;
    vector->w /= length;
}

/**
 * @brief Normalizes the provided vector in place to a unit vector.
 * 
 * @param vector A Vector to be normalized.
 * @return A Normalized copy of the supplied vector.
 */
FINLINE vec4 vec4_normalized(vec4 vector)
{
    vec4_normalize(&vector);
    return vector;
}

/**
 * @brief Returns the dot product between provided vectors.
 * Typically uesd to calculate the difference in direction
 * 
 * @param vector_0 The First Vector
 * @param vector_1 The Second Vector
 * @return The dot Product
 */
FINLINE f32 vec4_dot_f32
(
    f32 a0, f32 a1, f32 a2, f32 a3,
    f32 b0, f32 b1, f32 b2, f32 b3
)
{
    f32 p;
    p = a0 * b0 +
        a1 * b1 +
        a2 * b2 +
        a3 * b3;

    return p;
}

/**
 * @brief Compares all elements of vector_0 and vector_1 and ensures the difference is less than tolerance
 * 
 * @param vector_0 The First Vector
 * @param vector_1 The Second Vector
 * @param tolerance The difference tolerance. Typical F_FLOAT_EPSILON or similar.
 * @return The if within Tolerance; otherwise False.
 */
FINLINE b8 vec3_compare(vec2 vector_0, vec2 vector_1, f32 tolerance)
{
    if(fabs(vector_0.x - vector_1.x) > tolerance)
    {
        return false;
    }
    if(fabs(vector_0.y - vector_1.y) > tolerance)
    {
        return false;
    }
    if(fabs(vector_0.z - vector_1.z) > tolerance)
    {
        return false;
    }
    if(fabs(vector_0.w - vector_1.w) > tolerance)
    {
        return false;
    }
    return true;
}

/**
 * @brief Returns distance between vector_0 and vector_1
 * 
 * @param vector_0 The First Vector
 * @param vector_1 The Second Vector
 * @return The distance between vector_0 and vector_1
 */
FINLINE f32 vec4_compare(vec4 vector_0, vec4 vector_1)
{
    vec4 d = (vec4)
    {
        vector_0.x - vector_1.x,
        vector_0.y - vector_1.y,
        vector_0.z - vector_1.z,
        vector_0.w - vector_1.w
    };
    return vec4_length(d);
}

// VECTOR 4 END ///////////////////////////////////////////////////////////////////