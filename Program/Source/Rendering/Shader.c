#include "Shader.h"
#include <Input/File.h>
#include <Output/Error.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

shader_component_t CreateShaderComponent(const char* file_path,
                                         shader_type_t type)
{
    char full_file_path[256] = SHADER_PATH;
    (void)strncat(full_file_path, file_path, 255);

    char shader_text_contents[SHADER_MAX_LENGTH];
    ReadFileContents(full_file_path, shader_text_contents,
                     SHADER_MAX_LENGTH);
    const char* shader_text = shader_text_contents;

    uint32_t shader = glCreateShader(type);
    if (shader == 0) ReportError(opengl_shader_creation_failure);

    glShaderSource(shader, 1, &shader_text, NULL);
    glCompileShader(shader);

    int compilation_status = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compilation_status);
    if (!compilation_status)
    {
        // send this to the error interface
        char log[1000];
        GLsizei len;
        glGetShaderInfoLog(shader, 1000, &len, log);
        fprintf(stderr, "Error: compiling %s: %*s\n",
                type == GL_VERTEX_SHADER ? "vertex" : "fragment", len,
                log);
        exit(1);
    }

    return (shader_component_t){shader, full_file_path, type};
}

shader_t CreateShader(shader_component_t* vertex,
                      shader_component_t* fragment)
{
    uint32_t program = glCreateProgram();
    glAttachShader(program, vertex->id);
    glAttachShader(program, fragment->id);
    glLinkProgram(program);

    int compilation_status = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &compilation_status);
    if (!compilation_status)
    {
        // send this to the error interface
        char log[1000];
        GLsizei len;
        glGetProgramInfoLog(program, 1000, &len, log);
        fprintf(stderr, "Error: linking:\n%*s\n", len, log);
        exit(1);
    }

    DestroyShaderComponent(vertex);
    DestroyShaderComponent(fragment);

    return (shader_t){true, program};
}

void DestroyShaderComponent(shader_component_t* component)
{
    glDeleteShader(component->id);
    component->id = 0;
    component->path = NULL;
    component->type = deleted;
}

void DestroyShader(shader_t* shader)
{
    glDeleteProgram(shader->id);
    shader->id = 0;
    shader->in_use = false;
}
