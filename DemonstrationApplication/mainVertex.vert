#version 330 core

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform mat3 normal;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoords;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec3 inColor;

out vec3 vNormal;
out vec2 vTexCoords;
out vec3 vColor;

void main() {
    // Combine transformations
    mat4 MVP = projection * view * model;
    
    // Apply transformations to the vertex position
    vec4 transformedPosition = MVP * vec4(inPosition, 1.0);
    
    // Pass transformed position to the next stage
    gl_Position = transformedPosition;

    vNormal = inNormal;
    vTexCoords = inTexCoords;
    vColor = inColor;
}