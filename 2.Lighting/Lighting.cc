
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "Draw.h"
#include "Camera.h"


using namespace std;


void input(Camera& camera, GLFWwindow* window, glm::vec3 pos, glm::vec3 target, glm::vec3 up);
void resize_cb(GLFWwindow* win, int width, int height);
void mouse_cb(GLFWwindow* window, double xpos, double ypos);
void scroll_cb(GLFWwindow* window, double xoffset, double yoffset);

static float translateZ = 0.0f;

/* cursor positions */
static float mouseX = 400, mouseY = 300;

/* camera positions */
static float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
static float pitch = 0.0f;

static glm::vec3 lightPos;


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
    glfwSetScrollCallback(window, scroll_cb);


    /* set resize callback */
    glfwSetFramebufferSizeCallback(window, resize_cb);
    glfwSetCursorPosCallback(window, mouse_cb);





    /* compile shaders and all initailzation work */
    auto [shaderColor, shaderLight, VAO_color, VAO_light] = draw_init();


    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));


    while (!glfwWindowShouldClose(window)) {

        // rendering
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        input(camera, window, camera.Position, camera.Front, camera.Up);

        float time = glfwGetTime();
        lightPos = glm::vec3(0.5f, 0.5f, translateZ);

        // First Color
        glUseProgram(shaderColor);
        glUniform3fv(glGetUniformLocation(shaderColor, "objectColor"), 1, glm::value_ptr(glm::vec3(1.0f, 0.5f, 0.31f)));
        glUniform3fv(glGetUniformLocation(shaderColor, "lightColor"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
        glUniform3fv(glGetUniformLocation(shaderColor, "lightPos"), 1, glm::value_ptr(lightPos));

     
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        glUniformMatrix4fv(glGetUniformLocation(shaderColor, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        // from woirld (camera top) into camera view-2d-space
        auto view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shaderColor, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.2f, -0.5f, 1.5f));
        model = glm::scale(model, glm::vec3(0.2f));

        glUniformMatrix4fv(glGetUniformLocation(shaderColor, "model"), 1, GL_FALSE, glm::value_ptr(model));

        glBindVertexArray(VAO_color);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        // Second Lamp (light)
        glUseProgram(shaderLight);
        glUniform3fv(glGetUniformLocation(shaderLight, "lightColor"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
        glUniformMatrix4fv(glGetUniformLocation(shaderLight, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shaderLight, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // In your render loop, for the light:
        model = glm::mat4(1.0f); // Reset model matrix first!
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(glGetUniformLocation(shaderLight, "model"), 1, GL_FALSE, &model[0][0]);

        glBindVertexArray(VAO_light);

        GLint projLoc = glGetUniformLocation(shaderLight, "projection");
        GLint viewLoc = glGetUniformLocation(shaderLight, "view");
        GLint modelLoc = glGetUniformLocation(shaderLight, "model");

        glDrawArrays(GL_TRIANGLES, 0, 36);
   

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO_color);
    glDeleteVertexArrays(1, &VAO_light);
    glfwTerminate();

    return EXIT_SUCCESS;
}

void resize_cb(GLFWwindow* win, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_cb(GLFWwindow* window, double xpos, double ypos) {

}

void scroll_cb(GLFWwindow* window, double xoffset, double yoffset) {
    if (yoffset > 0) {
        translateZ -= 0.1;

    }
    else if (yoffset < 0) {
        translateZ += 0.1;

    }
  
    std::cout << "translate: " << translateZ << std::endl;
}

void input(Camera &camera, GLFWwindow* window, glm::vec3 pos, glm::vec3 target, glm::vec3 up) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, 0.1);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, 0.1f);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, 0.1f);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, 0.1f);
}
