#include<iostream>
#include<glad/glad.h> // glad must be included before GLFW
#include<GLFW/glfw3.h>

#include "shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

    unsigned int texture;
    glGenTextures(1, &texture); // how many textures we want to generate, array of uint (single uint in this case)
    glBindTexture(GL_TEXTURE_2D, texture);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // texture wrapping option
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // texture wrapping option
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // linearly interpolate btw. closest mipmaps and sample via linear interpolation
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // use linear interpolation when magnifying (mipmap is based on downscaling, not magnifying)

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    // texture target, mipmap level for current texture, format to store, width, height, legacy stuff dont care, format of source image, data type of source image, image data

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data); // :)

	// RENDERING TRIANGLE

    Shader ourShader("shaders/3.4.shader.vs", "shaders/3.4.shader.fs");

    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    float vertices[] = {
        // positions          // colors           // texture coords
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
       -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
       -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // Define vertex attribute layout (location=0, 3 floats per vertex)
    // (location of vertex attrib (location = 0 in vertex shader), size of attrib. (vec3 so 3 values), data type, normalize data or not, stride (space btw. consecutive vertex attribs., 24 bytes in this case), offset (where position data begins)
    glEnableVertexAttribArray(0); // Enable vertex attribute at location 0

    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    // texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        processInput(window); // Process input

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Set the clear color (background color)
        glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer

        // float timeValue = glfwGetTime();
        // float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        // int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor"); // can be before setting the shader program

        // TRIANGLE
        // 2. Use our shader program when we want to render an object
        ourShader.use(); // Activate the shader program for rendering
        // glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f); // setting the uniform value (should be after shader program selection bc it sets the current shader program's)

        // RECTANGLE (with 2 triangles)
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // (what are we drawing, num of indices, type of indices, EBO offset)

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
}
