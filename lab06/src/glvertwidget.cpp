#include "glvertwidget.h"
#include "settings.h"

#include "cs123_lib/resourceloader.h"

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"            // glm::vec*, mat*, and basic glm functions
#include "glm/gtx/transform.hpp"  // glm::translate, scale, rotate
#include "glm/gtc/type_ptr.hpp"   // glm::value_ptr

#include "openglshape.h"
#include "gl/shaders/ShaderAttribLocations.h"
#include <iostream>
#include "cs123_lib/resourceloader.h"

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"            // glm::vec*, mat*, and basic glm functions
#include "glm/gtx/transform.hpp"  // glm::translate, scale, rotate
#include "glm/gtc/type_ptr.hpp"   // glm::value_ptr

#include "openglshape.h"
#include "gl/shaders/ShaderAttribLocations.h"
#include <math.h>
#include <iostream>
#include "sphere.h"

GLVertWidget::GLVertWidget(QGLFormat format, QWidget *parent, bool tab0)
    : QGLWidget(format, parent),
      m_square(nullptr),
      m_solidProgramID(0),
      m_gradientProgramID(0),
      m_textureProgramID(0),
      m_textureID(0),
      m_tab0(tab0),
      m_program(0),
      m_sphere(nullptr),
      m_timer(this),
      m_fps(60.0f),
      m_increment(0)
{
    // Set up 60 FPS draw loop.
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(tick()));
    m_timer.start(1000.0f / m_fps);
    m_tick = 1.0;
    m_angle = 0.f;
}

GLVertWidget::~GLVertWidget()
{

}


void GLVertWidget::initializeGL() {
    initializeGLTransformationsVertexShaders();
}

void GLVertWidget::initializeGLTransformationsVertexShaders() {
    ResourceLoader::initializeGlew();

    // Set the color to set the screen when the color buffer is cleared.scale
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Creates the shader program that will be used for drawing.
    m_program = ResourceLoader::createShaderProgram(":/shaders/shader.vert", ":/shaders/shader.frag");

    // Initialize sphere with radius 0.5 centered at origin.
    std::vector<GLfloat> sphereData = SPHERE_VERTEX_POSITIONS;

    m_sphere = std::make_unique<OpenGLShape>();
    m_sphere->setVertexData(&sphereData[0], sphereData.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLES, NUM_SPHERE_VERTICES);
    m_sphere->setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_sphere->buildVAO();

    // Initialize square on XY plane that extends from -1 to 1.
    std::vector<GLfloat> squareData = {-1.f, -1.f, 0.f,
                                       -1.f, +1.f, 0.f,
                                       +1.f, -1.f, 0.f,
                                       +1.f, +1.f, 0.f};

    m_square = std::make_unique<OpenGLShape>();
    m_square->setVertexData(&squareData[0], squareData.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLE_STRIP, 4);
    m_square->setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_square->buildVAO();

    // TODO: Enable depth testing. (Task 6)
    glEnable(GL_DEPTH_TEST);

}



void GLVertWidget::paintGLTransformationsVertexShaders() {
    // Clear the color and depth buffers.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float time = m_increment++ / (float) m_fps;      // Time in seconds.

        float fieldOfViewY = 0.8f;                       // Vertical field of view angle, in radians.
        float aspectRatio = (float)width() / height();   // Aspect ratio of the window.
        float nearClipPlane = 0.1f;                      // Near clipping plane.
        float farClipPlane = 100.f;                      // Far clipping plane.

        // TODO: Adjust the eye coordinates so the camera goes in a circle of radius 6 where
        // y is always equal to 1. (Task 7)

        glm::vec3 eye = glm::vec3(6.f * glm::cos(m_angle), 1.f, 6.f * glm::sin(m_angle));        // Camera position.
        glm::vec3 center = glm::vec3(0.f, 1.f, 0.f);     // Where camera is looking.
        glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);         // Up direction.

        glUseProgram(m_program);

        glUniform3f(glGetUniformLocation(m_program, "color"), 0.5, 0.5, 0.5);

        // TODO: Use the equation to translate the ball. (Task 7)
        float ballY = 0.5f + std::fabs(std::sin(3 * time));


        // TODO: Generate model matrix and pass it to vertex shader. (Task 3)
        glm::mat4 model = glm::translate(glm::mat4(1.f), glm::vec3({0.f, ballY, 0.f}));
        glUniformMatrix4fv(glGetUniformLocation(m_program, "model"), 1, GL_FALSE, glm::value_ptr(model));


        // TODO: Generate view matrix and pass it to vertex shader. (Task 4)
        glm::mat4 view = glm::lookAt(eye, center, up);
        glUniformMatrix4fv(glGetUniformLocation(m_program, "view"), 1, GL_FALSE, glm::value_ptr(view));


        // TODO: Generate projection matrix and pass it to vertex shader. (Task 4)
        glm::mat4 perspective = glm::perspective(fieldOfViewY, aspectRatio, nearClipPlane, farClipPlane);
        glUniformMatrix4fv(glGetUniformLocation(m_program, "perspective"), 1, GL_FALSE, glm::value_ptr(perspective));


        // TODO: Draw sphere here! (Task 1)
        m_sphere->draw();

        // TODO: Change color. (Task 5)
        glUniform3f(glGetUniformLocation(m_program, "color"), 0.1, 0.7, 0.9);

        // TODO: Scale the square x2. (Task 7)
        model = glm::scale(glm::mat4(1.f), glm::vec3(2.f));


        // TODO: Rotate the square to lie flat on the XZ plane. (Task 7)
        model = glm::rotate(model, glm::pi<float>() / 2.f, glm::vec3(1.f, 0.f, 0.f));
        glUniformMatrix4fv(glGetUniformLocation(m_program, "model"), 1, GL_FALSE, glm::value_ptr(model));


        // TODO: Draw the square. (Task 5)
        m_square->draw();


        glUseProgram(0);
}


/** Repaints the canvas. Called 60 times per second. */
void GLVertWidget::tick()
{
    m_tick += 1;

    m_angle += M_PI / 60.f;

    if(m_angle >= 2 * M_PI) {
        m_angle = 0.f;
    }

    update();
}

void GLVertWidget::paintGL() {
    paintGLTransformationsVertexShaders();
}

void GLVertWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}
