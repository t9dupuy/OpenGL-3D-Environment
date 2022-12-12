#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;
out vec4 vertexColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float const_div;

void main ()
{
    vertexColor = vec4 (aColor.rgb, 1.0);
    gl_Position = projection * view * model * vec4(aPosition / vec3(const_div), 1.0f);
}