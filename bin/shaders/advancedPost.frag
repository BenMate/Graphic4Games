// a simple post processing shader

#version 410

in vec2 vTexCoord;

uniform sampler2D colourTarget;
uniform int postProcessTarget;
uniform int pixelAmount;
uniform float blurStrength = 1.0f;

out vec4 FragColour;

vec4 Default(vec2 texCoord)
{
    return texture(colourTarget, texCoord);
}

vec4 BoxBlur(vec2 texCoord)
{
    vec2 texel = blurStrength / textureSize(colourTarget, 0);
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

vec4 Distort(vec2 texCoord)
{
    vec2 mid = vec2(0.5f);

    float distanceFromCentre = distance(texCoord, mid);
    vec2 normalizedCoord = normalize(texCoord - mid);

    float bias = distanceFromCentre + sin(distanceFromCentre * 15) * 0.05f;
    vec2 newCoord = mid + bias * normalizedCoord;

    return texture(colourTarget, newCoord);
}

vec4 Sepia(vec2 texCoord)
{
    vec4 baseColour = texture(colourTarget, texCoord);
    vec4 sepia = vec4(
        dot(baseColour.rgb, vec3(0.393f, 0.769,  0.189f)),
        dot(baseColour.rgb, vec3(0.349f, 0.686f, 0.168f)),
        dot(baseColour.rgb, vec3(0.272f, 0.534f, 0.131f)),
        1.0f
    );
    return sepia;
}

vec4 Pixel(vec2 texCoord)
{
    vec2 grid = round(texCoord * float(pixelAmount)) / float(pixelAmount);
    vec4 colour = texture(colourTarget, grid);
    return colour;
}

vec4 Invert(vec2 texCoord)
{  
    return texture(colourTarget, 1-texCoord);
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

    switch(postProcessTarget)
    {
        case 0: // default
        {
            FragColour = Default(texCoord);
            break;
        }
        case 1: // BoxBlur
        {
            FragColour = BoxBlur(texCoord);
            break;
        }
        case 2: // Distort
        {
            FragColour = Distort(texCoord);
            break;
        }
        case 3: // Edge detection
        {
            FragColour = Default(texCoord);
            break;
        }
        case 4: // Sepia
        {
            FragColour = Sepia(texCoord);
            break;
        }
        case 5: // Scanless
        {
            FragColour = Default(texCoord);
            break;
        }
        case 6: // gray Scale
        {
            FragColour = Default(texCoord);
            break;
        }
        case 7: // Invert
        {
            FragColour = Invert(texCoord);
            break;
        }
        case 8: // pixelLizer
        {
            FragColour = Pixel(texCoord);
            break;
        }
        case 9: // Posterization
        {
            FragColour = Default(texCoord);
            break;
        }
        case 10: // Distance Fog
        {
            FragColour = Default(texCoord);
            break;
        }
        case 11: // Depth of field
        {
            FragColour = Default(texCoord);
            break;
        }
    }
}