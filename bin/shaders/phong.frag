//a simple phong colour shader
#version 410

in vec4 vPosition;
in vec3 vNormal;

uniform vec3 CameraPosition;

uniform vec3 ka; // the ambiant material colour
uniform vec3 kd; // the diffuse material colour
uniform vec3 ks; // the specular material colour

uniform float specularPower; // the power of the material

uniform vec3 AmbientColour; // the ambient light colour

uniform vec3 LightColour; // the diffuse and specular light colour
uniform vec3 LightDirection;

out vec4 FragColour;

void main ()
{
    //make sure the normal and light direction are normalised 
    vec3 N = normalize(vNormal);
    vec3 L = normalize(LightDirection);

    //then calculate the lambert term (negating light direction)
    float lambertTerm = max(0, min(1, dot(N, -L)));

    vec3 V = normalize(CameraPosition - vPosition.xyz);
    vec3 R = reflect(L, N);

    float specularTerm = pow(max(0, dot(R, V)), specularPower);

    //calculate the diffuse, ambient and specular colour of the model
    vec3 ambient = AmbientColour * ka;
    vec3 diffuse = LightColour * kd * lambertTerm;
    vec3 specular = LightColour * ks * specularTerm;


    FragColour = vec4(ambient + diffuse + specular, 1);
}