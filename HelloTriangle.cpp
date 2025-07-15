#include<iostream>
#include<glad/glad.h> // glad must be included before GLFW
#include<GLFW/glfw3.h>

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

	// RENDERING TRIANGLE
    
    // Vertex shader source code (GLSL)
    const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n" // Input vertex position (location = 0)
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

    // Fragment shader source code (GLSL)
    const char* fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n" // Output color variable
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n" // Set fixed orange color
        "}\n\0";

    // VERTEX SHADER 
    unsigned int vertexShader; // Shader object ID for vertex shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER); // Create a vertex shader

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // Attach source code to the shader
    glCompileShader(vertexShader); // Compile the vertex shader

    // Check compilation status
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    //* VERTEX SHADER

    // FRAGMENT SHADER
    unsigned int fragmentShader; // Shader object ID for fragment shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // Create a fragment shader
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); // Attach source code
    glCompileShader(fragmentShader); // Compile the fragment shader
     
    // Check compilation status
    int  success2;
    char infoLog2[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success2);

    if (!success2)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog2);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog2 << std::endl;
    }
    //* FRAGMENT SHADER

    // SHADER PROGRAM
    unsigned int shaderProgram; // Shader program ID
    shaderProgram = glCreateProgram(); // Create a shader program object
     
    glAttachShader(shaderProgram, vertexShader); // Attach vertex shader
    glAttachShader(shaderProgram, fragmentShader); // Attach fragment shader
    glLinkProgram(shaderProgram); // Link shaders into a program

    // Check link status
    int  success3;
    char infoLog3[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success3);

    if (!success3) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog3);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog3 << std::endl;
    }

    //* SHADER PROGRAM

    // Delete shader objects, no longer needed after linking
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    /*
    float vertices[] = { // FOR RECTANGLE
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };
    */

    float vertices[] = { // FOR TRIANGLE
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
	}; // Vertex data for a triangle (Normalized device coordinates, NDC)
    
    /*
    // FOR RECTANGLE (Indexed Drawing to avoid overhead when drawing rectangle with 2 triangles (2 vertices are repated))
    unsigned int EBO; // Element Buffor Objects, EBO (a buffer that stores indices that OpenGL uses to decide what vertices to draw)
    glGenBuffers(1, &EBO); // Generate an Element Buffer Object (EBO) ID
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // Bind EBO as the current element array buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // Copy index data to GPU memory
    */
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

    // 1. Then set the vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // Define vertex attribute layout (location=0, 3 floats per vertex)
    glEnableVertexAttribArray(0); // Enable vertex attribute at location 0
    
    //* RENDERING TRIANGLE
    
    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        processInput(window); // Process input

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Set the clear color (background color)
        glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer

        // TRIANGLE
        // 2. Use our shader program when we want to render an object
        glUseProgram(shaderProgram); // Activate the shader program for rendering
        glBindVertexArray(VAO); // Bind the VAO containing vertex data and attribute config
        glDrawArrays(GL_TRIANGLES, 0, 3); // Draw 3 vertices as one triangle

        // RECTANGLE (with 2 triangles)
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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