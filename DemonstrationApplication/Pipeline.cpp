#include "Pipeline.h"
#include "Common.h"

Pipeline::Pipeline() {
    m_window =  Window();

    m_inputHandler = NULL;
    m_shader = NULL;
    m_camera = NULL;
    m_capture = NULL;

    m_CaptureDisplay = NULL;
    m_Floor = NULL;

}

Pipeline::~Pipeline() {
    delete m_inputHandler;
    delete m_shader;
    delete m_camera;
    delete m_capture;

    delete m_CaptureDisplay;
    delete m_Floor;
}

bool Pipeline::Initialize() {

    // initialize Shader
    m_shader = new Shader();
    m_shader->Initialize(
        "shaders/mainVertex.vert", GL_VERTEX_SHADER,
        "shaders/mainFragment.frag", GL_FRAGMENT_SHADER
    );
    m_shader->LinkShader();

    // initialize Camera
    m_camera = new Camera;
    m_camera->Initialize(m_window.Width(), m_window.Height());
    
    // initialize Capture
    m_capture = new Capture;
    m_capture->Initialize();

    cout << "Initialize!" << endl;

    // initialize InputHandeler
    m_inputHandler = new InputHandler();
    m_inputHandler->Initialize(m_window.getWindow());
    InitializeInputs();

    wchar_t buffer[MAX_PATH];
    GetCurrentDirectoryW(MAX_PATH, buffer);
    char narrowBuffer[MAX_PATH];
    if (WideCharToMultiByte(CP_UTF8, 0, buffer, -1, narrowBuffer, MAX_PATH, NULL, NULL)) {
        std::cout << "Current Working Directory: " << narrowBuffer << std::endl;
    }
    else {
        std::cerr << "Error converting wide string to narrow string." << std::endl;
    }

    m_model.Initialize("assets\\models\\MOCRNN_Model_2.pt");


    // initialize GameObjects

    Quad DisplayQuad = Quad();
    DisplayQuad.Create(m_capture->GetWidth() / 4, m_capture->GetHeight() / 4);

    m_CaptureDisplay = new GameObject();
    m_CaptureDisplay->Create(DisplayQuad, m_capture->getTextureID());
    m_CaptureDisplay->setModelMatrix(
        glm::vec3(130.f, -100.f, 1.f),
        glm::vec3(0.f, 0.f, 180.f),
        glm::vec3(1.f)
    );

    Quad FloorQuad = Quad();
    FloorQuad.Create(10.f, 10.f);

    m_Floor = new GameObject();
    m_Floor->Create(FloorQuad, "assets\\textures\\tile.png", true);
    m_Floor->setModelMatrix(
        glm::vec3(0.f, -2.f, -5.f),
        glm::vec3(-90.f, 0.f, 0.f),
        glm::vec3(1.f)
    );

    Cube LandmarkCubeGreen = Cube();
    LandmarkCubeGreen.Create(0.1f, glm::vec3(0.f, 1.f, 0.f));

    Cube LandmarkCubeRed = Cube();
    LandmarkCubeRed.Create(0.1f, glm::vec3(1.f, 0.f, 0.f));

    Cube LandmarkCubeBlue = Cube();
    LandmarkCubeBlue.Create(0.1f, glm::vec3(0.f, 0.f, 1.f));

    Cube LandmarkCubeYellow = Cube();
    LandmarkCubeYellow.Create(0.1f, glm::vec3(0.9f, 0.9f, 0.f));

    GameObject* landmarkGO = new GameObject();
    landmarkGO->Create(LandmarkCubeYellow, "None", false);
    landmarkGO->setModelMatrix(
        glm::vec3(-1.f, 1.f, 4.f),
        glm::vec3(0.f, 0.f, 0.f),
        glm::vec3(1.f)
    );

    m_landmarks.push_back(landmarkGO);

    for (int i = 0; i < 5; i++) {
        GameObject* landmarkGO1 = new GameObject();
        landmarkGO1->Create(LandmarkCubeGreen, "None", false);
        landmarkGO1->setModelMatrix(
            glm::vec3(1.f, 1.f, 2.f * i),
            glm::vec3(0.f, 0.f, 0.f),
            glm::vec3(1.f)
        );

        m_landmarks.push_back(landmarkGO1);

        GameObject* landmarkGO2 = new GameObject();
        landmarkGO2->Create(LandmarkCubeBlue, "None", false);
        landmarkGO2->setModelMatrix(
            glm::vec3(3.f, 1.f, 2.f * i),
            glm::vec3(0.f, 0.f, 0.f),
            glm::vec3(1.f)
        );

        m_landmarks.push_back(landmarkGO2);

        GameObject* landmarkGO3 = new GameObject();
        landmarkGO3->Create(LandmarkCubeRed, "None", false);
        landmarkGO3->setModelMatrix(
            glm::vec3(5.f, 1.f, 2.f * i),
            glm::vec3(0.f, 0.f, 0.f),
            glm::vec3(1.f)
        );

        m_landmarks.push_back(landmarkGO3);
    }
    



    return true;

}

void Pipeline::Update() {
    m_capture->Update();
    m_inputHandler->Update(m_deltaTime);
    UpdateLandmarks(m_model.GetLandmarksPositions(m_capture->GetMOCRNNInput()));

    m_loop_itterations += 1;
    m_totalTime += m_deltaTime;
}

void Pipeline::Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    

    m_shader->UseProgram();
    
    //std::cout << "3" << "\n";
    ////////////////////////////
    //  Perspective Renders   //
    ////////////////////////////

    glEnable(GL_DEPTH_TEST);
    
    m_shader->SetUniform("projection", m_camera->GetPerspectiveProjMatrix());
    m_shader->SetUniform("view", m_camera->GetPerspectiveViewMatrix());
    m_shader->SetUniform("lighting", 1);
    m_shader->SetUniform("lightColor", glm::vec3(0.6f, 0.9f, 0.8f));
    m_shader->SetUniform("lightDirection", glm::vec3(0.3f, 1.5f, 0.5f));
    



    // render Floor
    m_shader->SetUniform("textured", 1);
    m_shader->SetUniform("repeat", 25.f);
    m_Floor->Render(m_shader);

    // render landmark cube
    m_shader->SetUniform("textured", 0);
    for (int i = 0; i < 16; i++) {
        m_landmarks[i]->Render(m_shader);
    }


    glm::mat4 eyeMat = glm::mat4(1.0f);
    m_shader->SetUniform("model", eyeMat);
    m_shader->SetUniform("lighting", 0);
    for (int i = 0; i < connector_verts.size(); i += 2) {
        RenderLine(connector_verts[i], connector_verts[i+1]);
    }
    
    /////////////////////////////
    //  Othrographic Renders   //
    /////////////////////////////

    glDisable(GL_DEPTH_TEST);

    m_shader->SetUniform("projection", m_camera->GetOrthographicProjMatrix());
    m_shader->SetUniform("view", m_camera->GetOrthographicViewMatrix());
    m_shader->SetUniform("lighting", 0);
    m_shader->SetUniform("textured", 1);

    // render CaptureDisplay
    m_shader->SetUniform("repeat", 1.f);
    m_CaptureDisplay->Render(m_shader);


    // Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);
    std::cout << m_loop_itterations << " " << m_totalTime << "\n";
    
}

void Pipeline::InitializeInputs() {
    //keyboard inputs
    m_inputHandler->AddKeyboardCallback(GLFW_KEY_W, [this](KeyboardEvent e) {
        Camera* camera = GetCamera();
        if (camera) {
            camera->Forwards(e);
        }
     });

    m_inputHandler->AddKeyboardCallback(GLFW_KEY_S, [this](KeyboardEvent e) {
        Camera* camera = GetCamera();
        if (camera) {
            camera->Backwards(e);
        }
    });

    m_inputHandler->AddKeyboardCallback(GLFW_KEY_A, [this](KeyboardEvent e) {
        Camera* camera = GetCamera();
        if (camera) {
            camera->StrafeLeft(e);
        }
    });

    m_inputHandler->AddKeyboardCallback(GLFW_KEY_D, [this](KeyboardEvent e) {
        Camera* camera = GetCamera();
        if (camera) {
            camera->StrafeRight(e);
        }
    });

    // Mouse inputs
    m_inputHandler->AddMouseCallback([this](MouseEvent e) {
        Camera* camera = GetCamera();
        if (camera) {
            camera->SetViewByMouse(e);
        }
    });
}

Camera* Pipeline::GetCamera() {
    return m_camera;
}

void Pipeline::UpdateLandmarks(std::vector<glm::vec3> positions) {
    float multip = 5.f;

    for (int i = 0; i < 16; i++) {
        m_landmarks[i]->setModelMatrix(
            positions[i] * multip,
            glm::vec3(0.f, 0.f, 0.f),
            glm::vec3(1.f)
        );
    }

    //std::cout << "1" << "\n";

    // 0,   1, 2, 3,    4, 5, 6,    7, 8, 9,    10, 11, 12,     13, 14, 15
    connector_verts.clear();
    connector_verts.push_back(positions[0] * multip);
    connector_verts.push_back(positions[1] * multip);

    connector_verts.push_back(positions[0] * multip);
    connector_verts.push_back(positions[4] * multip);

    connector_verts.push_back(positions[0] * multip);
    connector_verts.push_back(positions[7] * multip);

    connector_verts.push_back(positions[0] * multip);
    connector_verts.push_back(positions[10] * multip);

    connector_verts.push_back(positions[0] * multip);
    connector_verts.push_back(positions[13] * multip);
    //
    connector_verts.push_back(positions[1] * multip);
    connector_verts.push_back(positions[2] * multip);

    connector_verts.push_back(positions[2] * multip);
    connector_verts.push_back(positions[3] * multip);
    //
    connector_verts.push_back(positions[4] * multip);
    connector_verts.push_back(positions[5] * multip);

    connector_verts.push_back(positions[5] * multip);
    connector_verts.push_back(positions[6] * multip);
    //
    connector_verts.push_back(positions[7] * multip);
    connector_verts.push_back(positions[8] * multip);

    connector_verts.push_back(positions[8] * multip);
    connector_verts.push_back(positions[9] * multip);
    //
    connector_verts.push_back(positions[10] * multip);
    connector_verts.push_back(positions[11] * multip);

    connector_verts.push_back(positions[11] * multip);
    connector_verts.push_back(positions[12] * multip);
    //
    connector_verts.push_back(positions[13] * multip);
    connector_verts.push_back(positions[14] * multip);

    connector_verts.push_back(positions[14] * multip);
    connector_verts.push_back(positions[15] * multip);

    //std::cout << "2" << "\n";
}

void Pipeline::RenderLine(const glm::vec3& start, const glm::vec3& end) {
    // Define the vertices of the line
    GLfloat data[] = {
        //  position                        color
            start.x, start.y, start.z,      1.f, 1.f, 1.f,
            end.x, end.y, end.z,            1.f, 1.f, 1.f
    };

    // Vertex buffer object (VBO) setup
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

    // Specify the vertex attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(0 * sizeof(GLfloat)));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    

    // Unbind VAO and VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Use a simple shader program for rendering lines

    // Draw the line
    glLineWidth(5.0f);
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(0);

    // Clean up (delete VAO and VBO)
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void APIENTRY OpenGLErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    // Print or handle the error message here
    //std::cerr << "OpenGL Error (" << id << "): " << message << std::endl;
}

void Pipeline::Execute() {
    // initialize Window
    bool winInit = m_window.Init();
    if (!winInit) {
        // error handleing for failed initialization

        return;
    }

    // After initializing OpenGL context and GLEW
    if (GLEW_ARB_debug_output) {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(OpenGLErrorCallback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }

    bool pipInit = Initialize();
    if (!pipInit) {
        // Error handling for initialization failure
        return;
    }
    double startTime, endTime;
    m_deltaTime = 0.0;

    while (!m_window.Deinit()) {
        startTime = glfwGetTime();

        // Call the Pipeline functions bellow:
        Update();
        Render();
        m_window.SwapBuffers();

        // Poll for events and handle input here if needed
        glfwPollEvents();

        endTime = glfwGetTime();
        m_deltaTime = endTime - startTime;
    }
}

