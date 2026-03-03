#pragma once

#include "Core/Rendering/RenderingTypes.inl"

struct platform_state;

b8 rendering_backend_create(rendering_backend_type type, struct platform_state* plat_state, rendering_backend* out_rendering_backend);

void rendering_backend_destroy(rendering_backend* rendering_backend);