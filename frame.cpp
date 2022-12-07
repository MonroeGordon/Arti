/**********************************************************************************************************************/
//Artilab
//File: frame.cpp
/**********************************************************************************************************************/

//Include files
#include "artilab.h"

//Static variable definitions
ImVec4 Frame::DEF_CLEAR_COLOR = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

//Glfw error callback function
static void glfw_error_callback(int error, const char* description) {
    throw ArtilabException(std::format("Artilab Exception: glfw error {} - {}\n", error, description));
}

//Frame constructor function
Frame::Frame(const char* title, float width, float height) {
    //Initialize variables
    wndSize = { width, height };
    pos = { 0.0f, 0.0f };
    clrClear = DEF_CLEAR_COLOR;
    close = false;
    mainViewport = MainViewport();

    //Setup glfw
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        throw ArtilabException("Artilab Exception: failed to initialize glfw\n");

    //GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);    //3.0+ only

    //Create window with graphics context
    window = glfwCreateWindow((int)width, (int)height, "Artilab", NULL, NULL);
    if (window == NULL)
        throw ArtilabException("Artilab Exception: failed to create window\n");
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);    //Enable vsync
    glfwMaximizeWindow(window);

    //Setup imgui and implot context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();

    //Setup I/O
    io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   //Enable keyboard controls
    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arial.ttf", 16.0f);
    io.FontDefault = io.Fonts->Fonts[0];
    io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;

    //Setup imgui style
    ImGui::StyleColorsDark();

    //Setup platform/renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

//Frame destructor function
Frame::~Frame() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}

//Frame launch function
void Frame::launch() {
    //Main window loop
    while (!glfwWindowShouldClose(window) && !close)
    {
        //Poll and handle events
        glfwPollEvents();

        //Start the imgui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //Set the main viewport
        mainViewport.setViewport();

        //Open the window
        open();

        //Render
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clrClear.x * clrClear.w, clrClear.y * clrClear.w, clrClear.z * clrClear.w, clrClear.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        //Double buffer
        glfwSwapBuffers(window);
    }
}