#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>

void camera() {
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);

	/* target is origin, and the direction we want is the diff */
	/* subntraction is in that direction since we want it to point toward camera positive z axis */
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);

	/* right vector that represents the positive x-axis of the camera space */
	/* to get this do a cross product with up vector(world space) with direction we retrieved above */
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection)); // cross == perpendicular

	/* now to get the y axis relative to camera just do cross of right(from above) and camera direction vector */
	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

	/* now we can transform any vec to the coord space by mul'ing with this mat */
}

void changeCameraView(unsigned shader, glm::vec3 pos, glm::vec3 target, glm::vec3 up) {
	float radius = 10.0f;
	float camX = static_cast<float>(sin(glfwGetTime()) * radius);
	float camZ = static_cast<float>(cos(glfwGetTime()) * radius);
	glm::mat4 view = glm::mat4(1.0f);;

	/* effectively what we did in camera() */
	/* The glm::LookAt function requires a position, target and up vector respectively */
	view = glm::lookAt(pos, target, up);
	
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, glm::value_ptr(view));
}