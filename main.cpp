#include<iostream>
#include<glad/glad.h> // glad must be included before GLFW
#include<GLFW/glfw3.h>

#include "shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// camera
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

int main()
{
    glfwInit(); // Initialize GLFW

    // Set GLFW version to 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Set GLFW to use the core profile (no deprecated functions)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL); 
    
    // Check if window creation was successful
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window); // Make the window's context current (required for OpenGL calls)

    // Initialize GLAD (OpenGL function loader)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600); // Set the viewport size for OpenGL rendering

    // Set the viewport size when the window is resized (add a callback function when the window is resized)
    void framebuffer_size_callback(GLFWwindow * window, int width, int height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    void processInput(GLFWwindow* window); // Function prototype for processing input


    // TEXTURE LOAD
    int width, height, nrChannels;
    unsigned char *data = stbi_load("textures/container.jpg", &width, &height, &nrChannels, 0);

    unsigned int texture1;
    glGenTextures(1, &texture1); // how many textures we want to generate, array of uint (single uint in this case)
    glBindTexture(GL_TEXTURE_2D, texture1);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // texture wrapping option
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // texture wrapping option
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // linearly interpolate btw. closest mipmaps and sample via linear interpolation
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // use linear interpolation when magnifying (mipmap is based on downscaling, not magnifying)

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    // texture target, mipmap level for current texture, format to store, width, height, legacy stuff dont care, format of source image, data type of source image, image data

    glGenerateMipmap(GL_TEXTURE_2D);

    // TEXTURE LOAD 2
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data2 = stbi_load("textures/awesomeface.png", &width, &height, &nrChannels, 0);

    unsigned int texture2;
    glGenTextures(1, &texture2); // how many textures we want to generate, array of uint (single uint in this case)
    glBindTexture(GL_TEXTURE_2D, texture2);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // texture wrapping option
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // texture wrapping option
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // linearly interpolate btw. closest mipmaps and sample via linear interpolation
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // use linear interpolation when magnifying (mipmap is based on downscaling, not magnifying)

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
    // texture target, mipmap level for current texture, format to store, width, height, legacy stuff dont care, format of source image, data type of source image, image data

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data); // :)

	// RENDERING TRIANGLE

    Shader ourShader("shaders/coordinatesystems.vs", "shaders/coordinatesystems.fs");

    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

   //  float vertices[] = {
   //      // positions          // colors           // texture coords
   //      0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
   //      0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
   //     -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
   //     -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
   // };

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    // 0. Copy our vertices array in a buffer for OpenGL to use
    unsigned int VAO; // Vertex Array Object, VAO 
    glGenVertexArrays(1, &VAO); // Generate a VAO ID
    glBindVertexArray(VAO); // Bind the VAO to record vertex attribute configuration

	unsigned int VBO; // Vertex Buffer Object, VBO (stores vertex data in GPU memory for efficiency)
	glGenBuffers(1, &VBO); // Generate a buffer ID 
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind the buffer to the GL_ARRAY_BUFFER 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Copy vertex data to the buffer
        // Arguments: 
	    // 1. The target buffer type (GL_ARRAY_BUFFER)
	    // 2. The size of the vertex data in bytes (sizeof(vertices))
	    // 3. The actual vertex data (vertices)
	    // 4. How we want the graphics card to manage the given data, it can be:
        //      GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
	    //      GL_STATIC_DRAW: the data is set only once and used by the GPU many times.
	    //      GL_DYNAMIC_DRAW: the data is changed a lot and used by the GPU many times.

    // EBO should be after VAO!
    unsigned int EBO; // Element Buffer Objects, EBO (a buffer that stores indices that OpenGL uses to decide what vertices to draw)
    glGenBuffers(1, &EBO); // Generate an Element Buffer Object (EBO) ID
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // Bind EBO as the current element array buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // Copy index data to GPU memory


    // 1. Then set the vertex POSITION attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); // Define vertex attribute layout (location=0, 3 floats per vertex)
    // (location of vertex attrib (location = 0 in vertex shader), size of attrib. (vec3 so 3 values), data type, normalize data or not, stride (space btw. consecutive vertex attribs., 24 bytes in this case), offset (where position data begins)
    glEnableVertexAttribArray(0); // Enable vertex attribute at location 0

    // texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    ourShader.use(); // Activate the shader program for rendering
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1); // or with shader class

    // Transformation
    // glm::mat4 trans = glm::mat4(1.0f); // creating identity matrix (otherwise it becomes null)
    // // ACTUAL TRANSFORMATION ORDER IS REVERSE! FIRST ROTATION THEN TRANSLATION
    // trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0)); // matrix to load data, angle in radians, rotation axis (must be unit vector!)
    // trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
    //
    // // sending uniform transform data to vs
    // unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
    // glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans)); // location, how many matrices, transpose, data (glm stores a bit different so we need value_ptr)

    glEnable(GL_DEPTH_TEST); // Enable automatic depth testing to discard fragments behind

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window); // Process input

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Set the clear color (background color)
        glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT); // Clear the color buffer and depth buffer

        // Texture unit GL_TEXTURE0 is always by default activated
        glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
        glBindTexture(GL_TEXTURE_2D, texture1); // bind that texture to the currently active texture unit
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // Coordinate Systems
        // glm::mat4 model = glm::mat4(1.0f);
        // model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

        // glm::mat4 view = glm::mat4(1.0f);
        // // note that we're translating the scene in the reverse direction of where we want to move
        // view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

        // glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
        //
        // glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f); // camera is pointing the origin
        // glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget); // named direction but it points the reverse direction of what is targeting
        //
        // glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        // glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection)); // up cross cameraDir gives +x axis direction (switched order of cross product would give -x)
        //
        // glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight); // camera dir cross +x axis of camera gives up axis of camera

        // swinging camera
        // const float radius = 10.0f;
        // float camX = sin(glfwGetTime()) * radius;
        // float camZ = cos(glfwGetTime()) * radius;
        // glm::mat4 view;
        // view = glm::lookAt(glm::vec3(camX, 0.0, camZ), // lookAt automatically creates the matrix (camera pos, camera target pos, up vector)
        //                  glm::vec3(0.0, 0.0, 0.0),
        //                     glm::vec3(0.0, 1.0, 0.0));

        glm::mat4 view;
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        // ourShader.setMat4("model", model);
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // RECTANGLE (with 2 triangles)
        glBindVertexArray(VAO);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // (what are we drawing, num of indices, type of indices, EBO offset)
        glBindVertexArray(VAO);
        for(unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            ourShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window); // Swap the front and back buffers (Look for double buffering on google)
        glfwPollEvents(); // Poll for and process events
    }

    glfwTerminate(); // Terminate GLFW

    return 0;
}

// Callback function to adjust the viewport when the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// Function to process input (e.g., close the window when the escape key is pressed)
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    const float cameraSpeed = 2.5f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}
