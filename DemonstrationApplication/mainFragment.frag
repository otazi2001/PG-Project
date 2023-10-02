#version 330 core
uniform sampler2D sampler0;
uniform float repeat;
uniform int textured;

uniform mat4 model;
uniform mat4 view;

uniform int lighting;
uniform vec3 lightDirection;
uniform vec3 lightColor;

in vec3 vNormal;
in vec2 vTexCoords;
in vec3 vColor;

out vec4 FragColor;

vec3 PhongLighting(vec3 rgb) {
    vec3 ambientColor = vec3(0.2, 0.2, 0.2);
    vec3 normal = normalize((model * vec4(vNormal, 0.0)).xyz);
    vec3 lightDir = normalize(lightDirection);
    vec3 lightDirView = normalize(vec3(view * vec4(lightDir, 0.0)));
    float diff = max(dot(normal, -lightDir), 0.0);
    
    // Define different colors for ambient, diffuse, and specular
    vec3 ambientComponent = rgb;
    vec3 diffuseComponent = vec3(0.6, 0.9, 0.8);
    vec3 specularComponent = vec3(0.9, 0.7, 0.7);

    vec3 diffuseColor = rgb * lightColor * diff * diffuseComponent;

    // Calculate specular reflection (Phong)
    vec3 reflectedLightDirection = reflect(lightDirView, normal);
    float specularIntensity = pow(max(0.0, dot(-reflectedLightDirection, lightDirView)), 110.0); // Adjust shininess
    vec3 specularColor = lightColor * specularIntensity * specularComponent;

    return ambientColor * ambientComponent + diffuseColor + specularColor;
}

void main() {

    vec3 rgb = vec3(1.0, 1.0, 1.0);

    if (textured == 0) {
        rgb = vColor;
    }

    
    vec3 lightingColor = rgb;

    if (lighting != 0) {
        lightingColor = PhongLighting(vec3(1.0, 1.0, 1.0));
    }

    if (textured != 0) {
        vec2 texCoords = vTexCoords * repeat;
        FragColor = texture(sampler0, texCoords) * vec4(lightingColor, 1.0);
    } else {
        FragColor = vec4(vColor * lightingColor, 1.0);
    }
}