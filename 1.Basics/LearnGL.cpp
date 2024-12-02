
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "Triangle.h"
#include "Transforms.h"
#include "Texture.h"


using namespace std;


glm::vec3 input(GLFWwindow* win, glm::vec3 pos, glm::vec3 target, glm::vec3 up);
void resize_cb(GLFWwindow* win, int width, int height);
void mouse_cb(GLFWwindow* window, double xpos, double ypos);

/* cursor positions */
static float mouseX = 400, mouseY = 300;

/* camera positions */
static glm::vec3 cameraPos;
static glm::vec3 cameraFront;
static glm::vec3 cameraUp;
static glm::vec3 cameraRight;
static float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
static float pitch = 0.0f;


int main()
{
	glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    auto* window = glfwCreateWindow(800, 800, "Hi Mom", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "failed initalization" << endl;
        return -1;
    }

    /*glad_glViewport*/
    glViewport(0, 0, 800, 600);
    
    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    /* set resize callback */
    glfwSetFramebufferSizeCallback(window, resize_cb);
    glfwSetCursorPosCallback(window, mouse_cb);
   

    /* compile shaders and all initailzation work */
    auto [ shaderID , VAO ] = draw_init(CUBE);
    auto [ textureID1, textureID2 ] = texture_init(shaderID);
    /* perform glm ops */
    rotate(shaderID);

    /* camera_init */
    cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    while (!glfwWindowShouldClose(window)) {

        // rendering
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        /* set the active texture unit */
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureID2);

        glUseProgram(shaderID);

        cameraPos = input(window, cameraPos, cameraFront, cameraUp);
        changeCameraView(shaderID, cameraPos, cameraFront, cameraUp);

        
        // Update model matrix (in world-space)
        glBindVertexArray(VAO);
        for (unsigned i = 0; i < 10; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float timeValue = glfwGetTime();
            model = glm::rotate(model, timeValue, glm::vec3(0.5f, 1.0f, 0.0f));
            glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glfwTerminate();

	return EXIT_SUCCESS;
}

void resize_cb(GLFWwindow* win, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_cb(GLFWwindow* window, double xpos, double ypos) {
    float xOff = xpos - mouseX, yOff = ypos - mouseY;
    mouseX = xpos, mouseY = ypos;

    xOff *= 0.1, yOff *= 0.1;

    yaw += xOff, pitch += yOff;
    /* clamp pitch */
    //pitch = max(pitch, 89.0f);
    //pitch = min(pitch, -89.0f);

    /* lookup pitch/yaw/scroll on opengl to understand this */
    /* yaw = rotate around y, pitch = depends on angle (either x or z), scroll = z */
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);


    // recalculate right and up vector
    cameraRight = glm::normalize(glm::cross(cameraFront, glm::vec3(0.0f, 1.0f, 0.0f)));
    cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront)); // new up is the cross between right and target_cam
}

glm::vec3 input(GLFWwindow *win, glm::vec3 pos, glm::vec3 target, glm::vec3 up) {
    const float cameraSpeed = 0.005f; // adjust accordingly
    if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS)
        pos += cameraSpeed * target;
    else if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS)
        pos -= cameraSpeed * target;
    else if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS)
        pos -= glm::normalize(glm::cross(target, up)) * cameraSpeed;
    else if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)
        pos += glm::normalize(glm::cross(target, up)) * cameraSpeed;
    return pos;
}
