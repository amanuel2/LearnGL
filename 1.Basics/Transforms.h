#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>
#include "Camera.h"

glm::mat4 ret_rotation();
void example();
void letsGo3D(unsigned shader);

void rotate(unsigned shader) {
	//unsigned transform = glGetUniformLocation(shader, "transform");
	//glm::mat4 animate = ret_rotation();
 	//glUniformMatrix4fv(transform, 1, GL_FALSE, glm::value_ptr(animate));

    letsGo3D(shader);
}

glm::mat4 ret_rotation() {
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::rotate(trans, /* glm::radians(90.0f) */ (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));
	trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
	return trans;
}

void letsGo3D(unsigned shader) {
    // first get model matruix and apply transforms
    //glm::mat4 model = glm::mat4(1.0f);
    //model = glm::rotate(model, glm::radians(45.0f) /* (float)glfwGetTime()*/, glm::vec3(1.0f, 1.0f, 0.0f)); // lets first rotate in the x axis
    //glUniformMatrix4fv(glGetUniformLocation(shader, "model") , 1, GL_FALSE, glm::value_ptr(model));


    //glm::mat4 view = glm::mat4(1.0f);
    //view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f)); // moving everytghng forward (right hand system)
    //glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, glm::value_ptr(view));

    // lets have a prespective projection
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f/600.0f, 0.1f, 100.0f);
    glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
} 

void example() {
    // Vertex in object space
    glm::vec4 vertex(1.0f, 2.0f, 3.0f, 1.0f);

    // Model, view, and projection matrices
    glm::mat4 model = glm::mat4(1.0f); // Identity matrix for simplicity
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    // Model-View-Projection matrix
    glm::mat4 mvp = projection * view * model;

    // Transform vertex to clip space
    glm::vec4 clipSpaceVertex = mvp * vertex;

    // Perspective division to get normalized device coordinates (NDC)
    glm::vec3 ndc;
    ndc.x = clipSpaceVertex.x / clipSpaceVertex.w;
    ndc.y = clipSpaceVertex.y / clipSpaceVertex.w;
    ndc.z = clipSpaceVertex.z / clipSpaceVertex.w;

    // Print the results
    std::cout << "Clip Space Vertex: ("
        << clipSpaceVertex.x << ", "
        << clipSpaceVertex.y << ", "
        << clipSpaceVertex.z << ", "
        << clipSpaceVertex.w << ")" << std::endl;

    std::cout << "Normalized Device Coordinates: ("
        << ndc.x << ", "
        << ndc.y << ", "
        << ndc.z << ")" << std::endl;
}