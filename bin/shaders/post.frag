// a simple post processing shader

#version 410

in vec2 vTexCoord;

uniform sampler2D colourTarget;

out vec4 FragColor;

vec4 Default(vec2 texCoord){

    return texture(colourTarget, texCoord);

}

vec4 BoxBlur(vec2 texCoord)
{
    vec2 texel = 1.0f / textureSize(colourTarget, 0);

    vec4 colour = texture(colourTarget, texCoord);

    colour += texture(colourTarget, texCoord + texel * vec2 (-1 ,  1));
    colour += texture(colourTarget, texCoord + texel * vec2 (-1 ,  0));
    colour += texture(colourTarget, texCoord + texel * vec2 (-1 , -1));

    colour += texture(colourTarget, texCoord + texel * vec2 ( 0,  1));
    colour += texture(colourTarget, texCoord + texel * vec2 ( 0, -1));

    colour += texture(colourTarget, texCoord + texel * vec2 ( 1,  1));
    colour += texture(colourTarget, texCoord + texel * vec2 ( 1,  0));
    colour += texture(colourTarget, texCoord + texel * vec2 ( 1, -1));

    return colour / 9;
}

void main()
{
    // First calculate the texels size

    vec2 texSize = textureSize(colourTarget, 0);
    vec2 texelSize = 1.0f / texSize;

    // Adjust the scale and coordinates

    vec2 scale = (texSize - texelSize) / texSize;

    vec2 texCoord = vTexCoord / scale + texelSize * 0.5f;

    // This will output, the desired post processing effect

    FragColor = Distort(texCoord) * BoxBlur(texCoord);

}