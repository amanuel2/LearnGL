#version 330 core
out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos; // to retrieve dot-product between this and up for diffused lighting

in vec3 Normal;
in vec3 FragPos; // from vert shader

void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // simple ambient
    //FragColor = vec4(ambient * objectColor, 1.0);

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 dirVector = normalize(lightPos - FragPos); // to retrieve vector between two points sub inital-final point

    float diff = max(dot(norm, dirVector), 0.0);
    vec3 diffuse = diff * lightColor; // kind of like what were doing with ambient
    FragColor = vec4( (ambient+diffuse) * objectColor, 1.0); // add to concat both light calcs
}