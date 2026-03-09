#pragma once

#include "Core/Rendering/RenderingTypes.inl"

struct static_mesh_data;
struct platform_state;

b8 initialize_rendering(const char* application_name, struct platform_state* plat_state);
b8 rendering_draw_frame(rendering_packet* packet);

void rendering_on_resized(u16 width, u16 height);

void shutdown_rendering();
