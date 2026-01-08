typedef float vec3_t[3];

typedef struct 
{
    vec3_t position;
    vec3_t velocity;
    float padding; // Wichtig für SIMD-Alignment (16-Byte-Grenzen)

} FEntity;