#version 330 core
layout(location = 0) in vec2 aPos;

out vec2 texCoord;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

void main()
{
    texCoord = aPos;
    gl_Position = uProj * uView * uModel * vec4(aPos, 0.0, 1.0);
}
