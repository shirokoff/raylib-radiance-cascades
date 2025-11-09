#version 150

in vec2 fragTexCoord;
uniform sampler2D uCells;
out vec4 finalColor;

void main()
{
    // simple debug fragment shader: output the input texture directly
    vec3 col = texture(uCells, fragTexCoord).rgb;
    finalColor = vec4(col, 1.0);
}