#version 330
 
in vec2 coord2d;

void main()
{

   gl_Position = vec4(coord2d, 0.0, 1.0);
}

