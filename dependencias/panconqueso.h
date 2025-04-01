#ifndef _DEPS_H
#define _DEPS_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <cmath>
#include <iostream>
#include <Particula.hpp>
#include <Individuo.hpp> 
#include <Rastrigin2D.hpp>

#define HEIGHT 600
#define WIDTH 1000
//Variables Globales

float last_time = 0.0f, delta_time = 0.0f, current_time;
GLFWwindow* ventana;
ImGuiWindowFlags flags = 0;
//int width = WIDTH, height = HEIGHT;

Rastrigin2D* Rastrigin;

//funciones
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

inline GLFWwindow* initDepsAndCreateWin(int width, int height)
{
    //
    // GLFW & GLEW
    //
    if (!glfwInit())
    {
        std::cerr << "ERROR::NO_SE_PUDO_INICIAR_GLFW" << std::endl;
        return NULL;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* mi_ventana = glfwCreateWindow(width, height, "HolaMundo!", NULL, NULL);
    if (!mi_ventana)
    {
        std::cerr << "No se pudo crear la ventana" << std::endl;
        glfwTerminate();
        return NULL;
    };
    glfwMakeContextCurrent(mi_ventana);
    glfwSwapInterval(1);
    glfwSetFramebufferSizeCallback(mi_ventana, framebuffer_size_callback);
    
    if (glewInit() != GLEW_OK) 
    {
        std::cerr << "ERROR::NO_SE_PUDO_INICIAR_GLEW" << std::endl;
        glfwTerminate();
        return NULL;
    }
    Rastrigin = new Rastrigin2D();
    int wid, hei;
    glfwGetFramebufferSize(mi_ventana, &wid, &hei);
    glViewport(0, 0, hei,  hei);
    //glfwGetWindowSize(mi_ventana, &wid, &hei);
    Rastrigin->updateResolution(hei, hei);
    #ifdef PSO_RASTRIGIN_
    Particula::setVAO(Rastrigin->getVAO());
    Particula::initShader();
    Particula::updateResolution(hei, hei);
    #endif
    #ifdef AE_RASTRIGIN_
    Individuo::setVAO(Rastrigin->getVAO());
    Individuo::initShader();
    Individuo::updateResolution(hei, hei);
    #endif
    std::cout << "OpenGL iniciado correctamente\nVersion: " << glGetString(GL_VERSION) << std::endl;
    //glEnable(GL_DEPTH_TEST);

    //
    // IMGUI & IMPLOT
    //
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    //ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // IF using Multi-Viewport (?)

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(mi_ventana, true);
    ImGui_ImplOpenGL3_Init();

    return mi_ventana;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, height,  height);
    int win_wid, win_hei;
    glfwGetWindowSize( window ,&win_wid, &win_hei);
    Rastrigin->updateResolution(height,  height);
    #ifdef PSO_RASTRIGIN_
    Particula::updateResolution(height,  height);
    #endif
    #ifdef AE_RASTRIGIN_
    Individuo::updateResolution(height, height);
    #endif
};

inline void newFrame()
{
    current_time = static_cast<float>(glfwGetTime());
    delta_time = current_time - last_time;
    last_time = current_time;
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}
#endif