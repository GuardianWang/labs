#include "glwidget.h"
#include "settings.h"
#include <math.h>

#include "cs123_lib/resourceloader.h"
#include "openglshape.h"

#include "gl/shaders/ShaderAttribLocations.h"

using namespace CS123::GL;

#define PI 3.14159265

GLWidget::GLWidget(QGLFormat format, QWidget *parent)
    : QGLWidget(format, parent),
      m_program(0),
      m_triangle(nullptr)
{

}

GLWidget::~GLWidget()
{
}

void GLWidget::initializeGL() {
    ResourceLoader::initializeGlew();
    m_program = ResourceLoader::createShaderProgram(":/shaders/shader.vert", ":/shaders/shader.frag");
    glViewport(0, 0, width(), height());
    glEnable(GL_CULL_FACE); // Hides the back faces of triangles.
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Defines the color the screen will be cleared to.

    initializeTriangle();
}

void GLWidget::paintGL() {
    glUseProgram(m_program);       // Installs the shader program. You'll learn about this later.
    glClear(GL_COLOR_BUFFER_BIT);  // Clears the color buffer. (i.e. Sets the screen to black.)

    // This draws only the lines of the triangles if "Draw lines only" is checked. Otherwise they
    // are filled in like normal.
    glPolygonMode(GL_FRONT_AND_BACK, settings.linesEnabled ? GL_LINE : GL_FILL);

    // TODO [Task 8]: Draw a triangle
    initializeTriangle();
    m_triangle->draw();


    glUseProgram(0); // Uninstalls the shader program.
}

void GLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void GLWidget::initializeTriangle() {
    m_triangle = std::make_unique<OpenGLShape>();
    std::vector<GLfloat> triData = {-.5f, -.5f, 0.f,
                                    .5f, -.5f, 0.f,
                                    0.f,  .5f, 0.f};

    // TODO [Task 7]
    m_triangle->setVertexData(triData.data(), triData.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLES, 3);
    m_triangle->setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_triangle->buildVAO();
}

