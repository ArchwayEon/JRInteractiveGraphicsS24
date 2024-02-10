#version 430
in vec4 fragColor;
in vec2 fragTexCoord;

out vec4 color;

uniform sampler2D texUnit;

void main()
{
   vec4 texFragColor = texture(texUnit, fragTexCoord) * fragColor;
   color = texFragColor;
}