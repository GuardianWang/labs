#version 330 core

in vec3 vertex;                 // The position of the vertex, in camera space!
in vec3 vertexToCamera;         // Vector from the vertex to the eye, which is the camera
in vec3 eyeNormal;	        // Normal of the vertex, in camera space!

uniform float r0;		// The R0 value to use in Schlick's approximation
uniform float eta1D;		// The eta value to use initially
uniform vec3  eta;              // Contains one eta for each channel (use eta.r, eta.g, eta.b in your code)

uniform mat4 view;
uniform mat4 model;

uniform samplerCube envMap;

out vec4 fragColor;

void main()
{
    vec3 n = normalize(eyeNormal);
    vec3 cameraToVertex = normalize(vertex); //remember we are in camera space!
    vec3 vertexToCamera = normalize(vertexToCamera);

    // F
    float cosNormalEye = dot(n, vertexToCamera);
    float rf = r0 * (1 - r0) * pow(1.0 - cosNormalEye, 5.0);

    mat4 cam2obj = inverse(view * model);

    // reflection
    vec3 reflectEye = reflect(cameraToVertex, n);
    vec3 reflectEye_Object = (cam2obj * vec4(reflectEye, 0.0)).xyz;
    vec4 reflectColor = texture(envMap, reflectEye_Object);

    // refraction
    vec3 refractR = refract(cameraToVertex, n, eta.r);
    vec3 refractR_Object = (cam2obj * vec4(refractR, 0.0)).xyz;
    vec4 refractColorR = texture(envMap, refractR_Object);
    vec3 refractG = refract(cameraToVertex, n, eta.g);
    vec3 refractG_Object = (cam2obj * vec4(refractG, 0.0)).xyz;
    vec4 refractColorG = texture(envMap, refractG_Object);
    vec3 refractB = refract(cameraToVertex, n, eta.b);
    vec3 refractB_Object = (cam2obj * vec4(refractB, 0.0)).xyz;
    vec4 refractColorB = texture(envMap, refractB_Object);


    vec4 refractionColor = vec4(refractColorR.r, refractColorG.g, refractColorB.b, 1.0);

    fragColor = mix(refractionColor, reflectColor, rf);
}
