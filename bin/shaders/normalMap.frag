//a simple phong colour shader
#version 410

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;
in vec3 vTangent;
in vec3 vBiTangent;

uniform vec3 CameraPosition;

uniform vec3 Ka; // the ambiant material colour
uniform vec3 Kd; // the diffuse material colour
uniform vec3 Ks; // the specular material colour

uniform float specularPower; // the power of the material

uniform vec3 AmbientColour; // the ambient light colour

uniform vec3 LightColour; // the diffuse and specular light colour
uniform vec3 LightDirection;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D normalTexture;

out vec4 FragColour;

void main()
{
    //make sure the normal and light direction are normalised 
    vec3 N = normalize(vNormal);
    vec3 L = normalize(LightDirection);
    vec3 T = normalize(vTangent);
    vec3 B = normalize(vBiTangent);

    mat3 TBN = mat3(T, B, N);

    vec3 texDiffuse = texture(diffuseTexture, vTexCoord).rgb;
    vec3 texsSpecular = texture(specularTexture, vTexCoord).rgb;
    vec3 textNormal = texture(normalTexture, vTexCoord).rgb;

    N = TBN * (textNormal * 2 - 1);

    //then calculate the lambert term (negating light direction)
    float lambertTerm = max(0, min(1, dot(N, -L)));

    vec3 V = normalize(CameraPosition - vPosition.xyz);
    vec3 R = reflect(L, N);

    float specularTerm = pow(max(0, dot(R, V)), specularPower);

    //calculate the diffuse, ambient and specular colour of the model
    vec3 ambient = AmbientColour * Ka * texDiffuse;
    vec3 diffuse = LightColour * Kd * texDiffuse * lambertTerm;
    vec3 specular = LightColour * Ks * texsSpecular * specularTerm;

    FragColour = vec4(ambient + diffuse + specular, 1);
}