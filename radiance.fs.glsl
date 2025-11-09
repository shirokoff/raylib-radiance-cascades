#version 150

in vec2 fragTexCoord;
uniform sampler2D uCells;
out vec4 finalColor;

void main()
{
    finalColor = texture(uCells, fragTexCoord);   
}