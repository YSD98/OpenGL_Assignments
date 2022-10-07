#version 330 core

out vec4 color;
in vec4 vColor;

in vec3 positionWorldSpace;
in vec3 normalCameraSpace;
in vec3 eyeDirectionCameraSpace;
in vec3 lightDirectionCameraSpace;



uniform mat4 mv;
uniform vec3 lightPositionWorldSpace;

void main()
{
    vec4 materialDiffuseColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    vec4 materialAmbientColor = vec4(0.5f, 0.5f, 0.5f, 1.0f) * materialDiffuseColor;
    vec4 materialSpecularColor = vec4(0.3f, 0.3f, 0.3f, 1.0f);
    
    vec4 lightColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    float lightIntensity = 100.0f;
    
    //Distance of the light
    float cameraDistance = length(lightPositionWorldSpace - positionWorldSpace);
    
    //Normal of the computed fragment in camera space.
    vec3 n = normalize(normalCameraSpace);
    
    //Direction of the light
    vec3 l = normalize(lightDirectionCameraSpace);
    
    float cosDiffuse = clamp(dot(n, l), 0, 1);
    
    //Eye vector
    vec3 e = normalize(eyeDirectionCameraSpace);
    
    //Direction at which the triangle reflects the light.
    vec3 r = reflect(-l, n);
    
    float cosSpec = clamp(dot(e, r), 0, 1);
    
    //color = Ambient + Diffuse + Specular;
   color = //materialAmbientColor +
    //Diffuse
    materialDiffuseColor * lightColor * lightIntensity * cosDiffuse / (cameraDistance * cameraDistance) +
    //Specular
   materialSpecularColor * lightColor * lightIntensity * pow(cosSpec, 5) / (cameraDistance * cameraDistance);

   //color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
