#version 330 core
layout(location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout(location = 1) in vec3 aNormal; // normal / up vector

// mvp
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal; // give normal vector to fragment shader
out vec3 FragPos; // position of fragment


void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0f);

    // want to do lighting calculation in world space
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = aNormal;
}