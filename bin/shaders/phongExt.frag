//a simple phong ext colour shader
#version 410

in vec4 vPosition;
in vec3 vNormal;
in vec4 objPosition;

uniform vec3 CameraPosition;

uniform sampler2D seamlessTexture;
uniform sampler2D hatchingTexture;
uniform sampler2D rampTexture;

uniform vec3 Ka; // the ambiant material colour
uniform vec3 Kd; // the diffuse material colour
uniform vec3 Ks; // the specular material colour

uniform float specularPower; // the power of the material

uniform vec3 AmbientColour; // the ambient light colour
uniform vec3 LightColour; // the diffuse and specular light colour
uniform vec3 LightDirection;

out vec4 FragColour;

//float Rim(in vec3 N, in vec3, ) {}

void main()
{
    //make sure the normal and light direction are normalised 
    vec3 N = normalize(vNormal);
    vec3 L = normalize(LightDirection);

    //then calculate the lambert term (negating light direction)
    float lambertTerm = max(0, min(1, dot(N, -L)));

    vec3 V = normalize(CameraPosition - vPosition.xyz);
    vec3 R = reflect(L, N);

    float specularTerm = pow(max(0, dot(R, V)), specularPower);

    //vec4 sampleXY = texture(seamlessTexture, objPosition.xy * 0.1);
    //vec4 sampleYZ = texture(seamlessTexture, objPosition.yz * 0.1);
    //vec4 sampleXZ = texture(seamlessTexture, objPosition.xz * 0.1);
    //vec4 sampleTP = N.x * N.x * sampleYZ + N.y * N.y * sampleXZ + N.z * N.z * sampleXY;
    //sampleTP = vec4(ivec4(sampleTP * 4 + 0.5)) / 4;

    //float intensity = AmbientColour.r + lambertTerm + specularTerm;

    //vec4 hatching = texture(hatchingTexture, gl_FragCoord.xy / 5);
    //intensity = step(hatching.x, intensity);
    

    //calculate the diffuse, ambient and specular colour of the model
    //vec3 ambient = AmbientColour * Ka;
    //vec3 diffuse = (AmbientColour + step(vec3(0.5, 0.5, 0.5), LightColour * lambertTerm)) * sampleTP.xyz;
    //LightColour * Kd * lambertTerm;
    //vec3 specular = LightColour * Ks * step(0.1, specularTerm);

    //float rim = 1 - max(0, dot(N, V));
    //rim = rim * rim * rim;
    //float alpha = rim + specular.r;

    //if(alpha < 0.1)
    //   discard;

    //FragColour = vec4(ambient + diffuse + specular + rim, alpha);
    //FragColour = vec4(diffuse + specular, 1);
    //FragColour = vec4(intensity * vec3(1, 1, 1), 1); 
    //FragColour = vec4(intensity * vec3(1,1,1) * sampleTP.xyz, 1);

    FragColour = texture(rampTexture, vec2(lambertTerm, 0.03f));
}