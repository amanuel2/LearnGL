#version 330 core
out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos; // to retrieve dot-product between this and up for diffused lighting
uniform vec3 viewPos; // used for specular highlights

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
    //FragColor = vec4( (ambient+diffuse) * objectColor, 1.0); // add to concat both light calcs

    vec3 viewDir = normalize(viewPos - FragPos);
    /* The reflect function expects the first vector to point from the light source towards the fragment's position */
    vec3 reflectDir = reflect(-dirVector, norm); /*  calculate the reflection direction for an incident vector and norm vector */

    /* calc the dot to get the theta from view and reflect vectors, which in turn lets us know the strength of the highlights */
    /* 32 lets us know how much direct lighting we want. if the angle is large that means there is a quite a difference so diffuse the lighting a lot, otherwise make it much more direct */
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = 0.5 * spec * lightColor;
    FragColor = vec4((ambient + diffuse + specular) * objectColor, 1.0);
}