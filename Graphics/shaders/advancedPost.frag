// a simple post processing shader

#version 410

in vec2 vTexCoord;

uniform sampler2D colourTarget;
uniform int postProcessTarget;
uniform int messed;

out vec4 FragColor;

vec4 Default(vec2 texCoord)
{
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

void main()
{
    // First calculate the texels size

    vec2 texSize = textureSize(colourTarget, 0);
    vec2 texelSize = 1.0f / texSize;

    // Adjust the scale and coordinates

    vec2 scale = (texSize - texelSize) / texSize;

    vec2 texCoord = vTexCoord / scale + texelSize * 0.5f;

    // This will output, the desired post processing effect
    int temp = 0;

    switch(temp)
    {
        case 0: // default
        {
            FragColor = Default(texCoord);
            break;
        }
        case 1: // BoxBlur
        {
            FragColor = BoxBlur(texCoord);
            break;
        }
        case 2: // Distort
        {
            FragColor = Distort(texCoord);
            break;
        }
        case 3: // Edge detection
        {
            FragColor = Default(texCoord);
            break;
        }
        case 4: // Sepia
        {
            FragColor = Sepia(texCoord);
            break;
        }
        case 5: // Scanless
        {
            FragColor = Default(texCoord);
            break;
        }
        case 6: // gray Scale
        {
            FragColor = Default(texCoord);
            break;
        }
        case 7: // Invert
        {
            FragColor = Default(texCoord);
            break;
        }
        case 8: // pixelLizer
        {
            FragColor = Default(texCoord);
            break;
        }
        case 9: // Posterization
        {
            FragColor = Default(texCoord);
            break;
        }
        case 10: // Distance Fog
        {
            FragColor = Default(texCoord);
            break;
        }
        case 11: // Depth of field
        {
            FragColor = Default(texCoord);
            break;
        }
    }
}