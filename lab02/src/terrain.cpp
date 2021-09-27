#include "terrain.h"

#include <math.h>
#include "gl/shaders/ShaderAttribLocations.h"

Terrain::Terrain() : m_numRows(100), m_numCols(m_numRows), m_isFilledIn(true)
{
}


/**
 * Returns a pseudo-random value between -1.0 and 1.0 for the given row and
 * column.
 */
float Terrain::randValue(int row, int col)
{
    return -1.0 + 2.0 * glm::fract(sin(row * 127.1f + col * 311.7f) * 43758.5453123f);
}


/**
 * Returns the object-space position for the terrain vertex at the given row
 * and column.
 */
glm::vec3 Terrain::getPosition(int row, int col)
{
    glm::vec3 position;
    position.x = 10 * row/m_numRows - 5;
    position.z = 10 * col/m_numCols - 5;

    // TODO: Adjust position.y using value noise.
    float downsampleRate = 20.f;
    float increaseFreq = 2.f;
    float amplitude = 1.f;
    int numOctaves = 3;
    float y = 0.f;

    for (int i = 0; i < numOctaves; i++) {
        float newRow = row / downsampleRate, newCol = col / downsampleRate;
        float rowFrac = glm::fract(newRow), colFrac = glm::fract(newCol);
        // A---B
        // -----
        // C---D
        int rowA = static_cast<int>(newRow), colA = static_cast<int>(newCol);
        int rowB = rowA, colB = colA + 1;
        int rowC = rowA + 1, colC = colA;
        int rowD = rowC, colD = colB;
        float A = randValue(rowA, colA), B = randValue(rowB, colB),
                C = randValue(rowC, colC), D = randValue(rowD, colD);

        float interAB = glm::mix(A, B, colFrac);
        float interCD = glm::mix(C, D, colFrac);
        float interFactor = (3 - 2 * rowFrac) * rowFrac * rowFrac;
        y += glm::mix(interAB, interCD, interFactor) * amplitude;

        downsampleRate /= increaseFreq;
        amplitude /= 2.f;
    }
    position.y = y;

    return position;
}


/**
 * Returns the normal vector for the terrain vertex at the given row and
 * column.
 */
glm::vec3 Terrain::getNormal(int row, int col)
{
    // TODO: Compute the normal at the given row and column using the positions
    //       of the neighboring vertices.

    glm::vec3 p = getPosition(row, col);
    glm::vec3 normal(0.f, 0.f, 0.f);
    int numNeighbors = 8;

    // offsets to get 8 neighbors counterclockwise
    // +z is from screen to me
    std::vector<int> dCols = {1, 1,  0, -1, -1, -1, 0, 1};
    std::vector<int> dRows = {0, 1, 1, 1,  0,  -1, -1, -1};
    // vectors pointing from p to neighbors ni in counterclock direction
    std::vector<glm::vec3> pns;
    for (int i = 0; i < numNeighbors; i++) {
        glm::vec3 neighbor = getPosition(row + dRows[i], col + dCols[i]);
        glm::vec3 pn = neighbor - p;
        pns.push_back(pn);
    }
    // average vector normals
    std::vector<glm::vec3> neighborNormals;
    for (int i = 0; i < numNeighbors; i++) {
        glm::vec3 neighborNormal = glm::cross(pns[i], pns[(i + 1) % numNeighbors]);
        neighborNormal = glm::normalize(neighborNormal);
        neighborNormals.push_back(neighborNormal);
    }
    for (int i = 0; i < numNeighbors; i++) {
        normal += neighborNormals[i];
    }
    normal = glm::normalize(normal);

    return normal;
}


bool Terrain::isFilledIn() {
    return m_isFilledIn;
}

/**
 * Initializes the terrain by storing positions and normals in a vertex buffer.
 */
std::vector<glm::vec3> Terrain::init() {
    // Initializes a grid of vertices using triangle strips.
    int numVertices = (m_numRows - 1) * (2 * m_numCols + 2);
    std::vector<glm::vec3> data(2 * numVertices);
    int index = 0;
    for (int row = 0; row < m_numRows - 1; row++) {
        for (int col = m_numCols - 1; col >= 0; col--) {
            data[index++] = getPosition(row, col);
            data[index++] = getNormal  (row, col);
            data[index++] = getPosition(row + 1, col);
            data[index++] = getNormal  (row + 1, col);
        }
        data[index++] = getPosition(row + 1, 0);
        data[index++] = getNormal  (row + 1, 0);
        data[index++] = getPosition(row + 1, m_numCols - 1);
        data[index++] = getNormal  (row + 1, m_numCols - 1);
    }

    return data;
}


/**
 * Draws the terrain.
 */
void Terrain::draw()
{
    openGLShape->draw();
}
