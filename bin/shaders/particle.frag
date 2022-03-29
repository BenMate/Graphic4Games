//a simple flat colour shader
#version 410

in vec4 vColour;
out vec4 FragColour;

void main ()
{
    FragColour = vColour;
}