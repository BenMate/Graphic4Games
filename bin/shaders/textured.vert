//a textured shader
#version 410

layout(location = 0) in vec4 Position;
layout(location = 2) in vec2 TexCoord;

uniform mat4 ProjectionViewModel;
out vec2 vTexCoord;

void main ()
{
    vTexCoord = TexCoord;
    gl_Position = ProjectionViewModel * Position;
}