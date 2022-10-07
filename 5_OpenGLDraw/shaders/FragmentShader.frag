#version 330 core

in vec4 vColor;
in vec3 positionWorldSpace;
in vec3 normalCameraSpace;
in vec3 eyeDirectionCameraSpace;
in vec3 lightDirectionCameraSpace;

out vec3 color;

uniform mat4 mv;
uniform vec3 lightPositionWorldSpace;

void main()
{
    
//    color = vColor;
//    color.a = 1.0;
        
	//color = vec4(1.0f,0.2f,0.3f,1.0f);
    
    vec3 materialDiffuseColor = vec3(1.0f, 0.0f, 0.0f);
    vec3 materialAmbientColor = vec3(0.5f, 0.5f, 0.5f) * materialDiffuseColor;
    vec3 materialSpecularColor = vec3(0.3f, 0.3f, 0.3f);
    
    vec3 lightColor = vec3(1, 1, 1);
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
    
    //color = vec3(1.0, 0.0, 0.0);
    
}
