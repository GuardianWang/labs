#version 330 core

in vec3 vertex;                 // The position of the vertex, in camera space
in vec3 vertexToLight;          // Vector from the vertex to the light
in vec3 vertexToCamera;            // Vector from the vertex to the eye
in vec3 eyeNormal;		// Normal of the vertex, in camera space

uniform samplerCube envMap;	// The cube map containing the environment to reflect
uniform vec4 ambient;		// The ambient channel of the color to reflect
uniform vec4 diffuse;		// The diffuse channel of the color to reflect
uniform vec4 specular;		// The specular channel of the color to reflect

uniform mat4 model;             // model matrix
uniform mat4 view;              // view matrix
uniform mat4 mvp;               // model view projection matrix

uniform float r0;		// The Fresnel reflectivity when the incident angle is 0
uniform float m;		// The roughness of the material

const float PI2Inv = 0.101321;

out vec4 fragColor;

void main()
{
    vec3 n = normalize(eyeNormal);
    vec3 l = normalize(vertexToLight);
    vec3 cameraToVertex = normalize(vertex); //remember we are in camera space!

    // F
    float cosNormalLight = dot(n, l);
    float kd = max(0.0, cosNormalLight);
    vec3 wr = reflect(-l, n);
    float cosNormalEye = dot(n, vertexToCamera);
    float rf = r0 * (1 - r0) * pow(1.0 - cosNormalEye, 5.0);
    // D
    float dAlpha = 1.0 / (m * m * pow(cosNormalEye, 4.0)) *
            exp(-pow(tan(acos(cosNormalEye)) / m, 2.0));
    // G
    vec3 h = (vertexToCamera + wr) / 2.0;
    float dotnh = dot(n, h);
    float g = min(1.0,
              min(2 * dotnh * cosNormalLight / dot(wr, h),
                  2 * dotnh * cosNormalEye / dot(vertexToCamera, h)));
    // ks
    float ks = max(0.0, rf * dAlpha * g * PI2Inv / (cosNormalLight * cosNormalEye));

    // skybox
    mat4 cam2obj = inverse(view * model);
    vec3 reflectEye = reflect(cameraToVertex, n);
    vec3 reflectEye_Object = (cam2obj * vec4(reflectEye, 0.0)).xyz;
    vec4 reflectColor = texture(envMap, reflectEye_Object);

    vec4 objColor = ambient + kd * diffuse + ks * specular;
    fragColor = mix(objColor, reflectColor, rf);
}
