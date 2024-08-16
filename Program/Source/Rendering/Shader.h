/**
 * @file Shader.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Utilities for the creation, binding, and polling of shaders.
 * @date 2024-08-11
 *
 * @copyright (c) 2024 - Israfiel
 */

#ifndef _MSENG_SHADER_RENDERING_SYSTEM_
#define _MSENG_SHADER_RENDERING_SYSTEM_

#include <GLAD/opengl.h>
#include <stdbool.h>

#define SHADER_PATH "./Assets/Shaders/"
#define SHADER_MAX_LENGTH 1024

typedef enum
{
    deleted = 0,
    vertex = GL_VERTEX_SHADER,
    fragment = GL_FRAGMENT_SHADER
} shader_type_t;

typedef struct
{
    uint32_t id;
    const char* path;
    shader_type_t type;
} shader_component_t;

typedef struct
{
    bool in_use;
    uint32_t id;
} shader_t;

shader_component_t CreateShaderComponent(const char* file_path,
                                         shader_type_t type);

shader_t CreateShader(shader_component_t* vertex,
                      shader_component_t* fragment);

void DestroyShaderComponent(shader_component_t* component);
void DestroyShader(shader_t* shader);

#endif // _MSENG_SHADER_RENDERING_SYSTEM_
