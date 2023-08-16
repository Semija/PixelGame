#pragma once



#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include "PixelGrid.h"

PixelGrid MainGrid;

GLFWwindow* window;
uint32_t windowWidth = 64; // in Pixels
uint32_t windowHeight = 64;// in Pixels
uint32_t customPixelSize = 20;
uint32_t realWindowWidth = windowWidth * customPixelSize;
uint32_t realWindowHeight = windowHeight * customPixelSize;
double fps = 0;

#pragma region Set
/*In Pixel size*/
void UpdateRealWindowSize(int newPixelWidth, int newPixelHeight, int newCustomPixelSize) {
    windowWidth = newPixelWidth;
    windowHeight = newPixelHeight;
    customPixelSize = newCustomPixelSize;
    realWindowWidth = (newPixelWidth * customPixelSize);
    realWindowHeight = (newPixelHeight * customPixelSize);
}

/*In Real size*/
void UpdateRealWindowSize(int newWidth, int newHeight) {
    realWindowWidth = newWidth;
    realWindowHeight = newHeight;
}

/*In Pixel size*/
void SetWindowSize(int newPixelWidth, int newPixelHeight, int newCustomPixelSize) {
    UpdateRealWindowSize(newPixelWidth, newPixelHeight, newCustomPixelSize);
    glfwSetWindowSize(window, realWindowWidth, realWindowHeight);
}

/*In Real size*/
void SetWindowSize(int newWidth, int newHeight) {
    glfwSetWindowSize(window, newWidth, newHeight);
}

void SetVsync(bool enabled) {
    glfwSwapInterval(enabled);
}

void SetTitle(std::string title) {
    glfwSetWindowTitle(window, title.c_str());
}



#pragma endregion

#pragma region Change

void OnWindowChanged(GLFWwindow* window, int newWidth, int newHeight) {
    glfwSetWindowSize(window, newWidth, newHeight);
    UpdateRealWindowSize(newWidth, newHeight);
    glViewport(0, 0, newWidth, newHeight);
    }

void CloseWindow() {
    glfwSetWindowShouldClose(window, true);
}



#pragma endregion

#pragma region Get

double GetTime() {
    return glfwGetTime();
}

double GetFPS() {
    return fps;
}

std::string GetFPSinString() {
    return std::to_string(GetFPS());
}

#pragma endregion

class Renderer {
private:
    // Time variables
    double safeTime = 0;
    double lastTime = 0.0f;
    double deltaTime = 0.0f;


	void SetWindow(GLFWwindow* newWindow) {
		window = newWindow;
	}

	struct StringToGL {
		const char* p;
		StringToGL(const std::string& s) : p(s.c_str()) {}
		operator const char** () { return &p; }
	};

public:

	Renderer(void(&Start)(), void(&Update)()) {
        // Set up the main window
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Create a main window object
        window = glfwCreateWindow(realWindowWidth, realWindowHeight, "CbEngine", NULL, NULL);
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, OnWindowChanged);

        // Initialise GLAD and load all function pointers
        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

        // Generate shaders
        const char* vertexSource =
            "#version 330 core\n"
            "layout(location = 0) in vec3 aPos;\n"
            "layout(location = 1) in vec3 aColor;\n"
            "layout(location = 2) in vec2 aTexCoord;\n"
            "out vec3 ourColor;\n"
            "out vec2 TexCoord;\n"
            "void main()\n"
            "{\n"
            "    gl_Position = vec4(aPos, 1.0);\n"
            "    TexCoord = aTexCoord;\n"
            "}\n";

        const char* fragSource =
            "#version 330 core \n"
            "out vec4 FragColor; \n"
            "in vec3 ourColor; \n"
            "in vec2 TexCoord; \n"
            "uniform sampler2D ourTexture; \n"
            "void main() \n"
            "{\n"
            "  FragColor = texture(ourTexture, TexCoord);\n"
            "}\n";

        // Define shaders
        unsigned int vertexShader;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        unsigned int fragmentShader;
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        // Compile the shaders
        glShaderSource(vertexShader, 1, StringToGL(vertexSource), NULL);
        glCompileShader(vertexShader);
        glShaderSource(fragmentShader, 1, StringToGL(fragSource), NULL);
        glCompileShader(fragmentShader);

        // Create a shader program to combine the fragment and the vertex shaders
        unsigned int shaderProgram;
        shaderProgram = glCreateProgram();

        // Link shaders
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        // Delete no longer needed shaders
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        // Assign verticies
        float vertices[] = {
            // positions        / colors          / texture coords
             1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top right
             1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // bottom right
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // bottom left
            -1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f  // top left
        };
        unsigned int indices[] = {
            0, 1, 3, // 1 Triangle
            1, 2, 3, // 2 Triangle
        };

        unsigned int VBO, VAO, EBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // Color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // Texture coord attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(0);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        //Set up main texture that is rendered
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        // Texture filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // Call the start function now everything is initialised 
        Start();

        // Main loop
        while (!glfwWindowShouldClose(window)) {

            glfwPollEvents();

            if (&window != NULL) { Update(); }

            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            glUseProgram(shaderProgram);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, MainGrid.width,
                MainGrid.height, 0, GL_RGB, GL_UNSIGNED_BYTE, MainGrid.pixels);

            glBindTexture(GL_TEXTURE_2D, texture);
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            glfwSwapBuffers(window);

            deltaTime = glfwGetTime() - lastTime;
            lastTime = glfwGetTime();

            safeTime += deltaTime;
            if (safeTime > 1) {
                fps = (1 / deltaTime);
                safeTime = 0;
            }
        }

        // De-allocate rescources
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteProgram(shaderProgram);

        glfwTerminate();
	}

};

void CreateWindow(void(&Start)(), void(&Update)()) {
    Renderer renderer(Start, Update);
}

void CreateWindow(void(&Start)(), void(&Update)(), void(&Finish)()) {
    Renderer renderer(Start, Update);
    Finish();
}
