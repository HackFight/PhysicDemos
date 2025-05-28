#version 330 core
out vec4 FragColor;

in vec2 texCoord;

void main()
{
    if(dot(texCoord, texCoord) > 1.0)
    {
        discard;
    }

    FragColor = vec4(1.0);
}
