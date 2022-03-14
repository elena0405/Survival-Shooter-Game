#include "object2D.h"

#include <vector>
#include <cmath>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2D::CreateSquare(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2D::CreateCircle(const std::string &name, float radix, glm::vec3 color, glm::vec3 center, bool fill) {
    float angle = 0;
    int x, y, size;
    unsigned int i;
    std::vector<VertexFormat> vertices;
    vertices.push_back(VertexFormat(center, color));
    std::vector<unsigned int> indices;

    // pe baza unui unghi se pot determina cu ajutorul functiilor 
    // sinus si cosinus toate punctele de pe cerc de coordonate x si y
    while (angle < 360) {
        x = center.x + radix * cos(angle * AI_MATH_PI / 180);
        y = center.y + radix * sin(angle * AI_MATH_PI / 180);

        // adaug noua pereche de coordonate in vectorul vertices
        vertices.push_back(VertexFormat(glm::vec3(x, y, 0), color));

        angle += 0.01f;
    }

    size = vertices.size();
    i = 1;

    // adaug indicii triunghiurilor din care va fi format cercul
    while (i < size - 1) {
        indices.push_back(0);
        indices.push_back(i);
        indices.push_back(i + 1);

        i++;
    }

    // la final, adaug indicii si pentru ultimul triunghi
    indices.push_back(0);
    indices.push_back(size - 1);
    indices.push_back(1);

    Mesh* circle = new Mesh(name);
    // desenez cercul din triunghiuri
    circle->SetDrawMode(GL_TRIANGLES);
  
    circle->InitFromData(vertices, indices);

    return circle;
}

Mesh* object2D::CreateRectangle(const std::string& name, glm::vec3 leftBottomCorner, float length, float width, glm::vec3 color, bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, width, 0), color),
        VertexFormat(corner + glm::vec3(0, width, 0), color)
    };

    Mesh* rectangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        rectangle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    rectangle->InitFromData(vertices, indices);

    return rectangle;
}

int object2D::colisionPlayerMap(float centerCircleX, float centerCircleY, float cornerX, float cornerY, float radix, float width) {
    // daca am o coliziune cu dreptunghiul de sus al hartii
    if ((cornerY > centerCircleY) && (abs(abs(cornerY) - abs(centerCircleY)) <= radix)) {
        // atunci am o coliziune de tipul 1: jucator si peretele de sus
        return 1;
    }

    // daca am o coliziune cu dreptunghiul de jos al hartii
    if ((cornerY < centerCircleY) && (abs(abs(centerCircleY) - abs(cornerY)) <= radix + width)) {
        // atunci am o coliziune de tipul 2: jucator si peretele de jos
        return 2;
    }

    // daca am o coliziune cu dreptunghiul din staga al hartii
    if ((cornerX < centerCircleX) && (abs(abs(centerCircleX) - abs(cornerX)) <= radix + width)) {
        // atunci am o coliziune de tipul 3: jucator si peretele din stanga
        return 3;
    }

    // daca am o coliziune cu dreptunghiul din dreapta al hartii
    if ((cornerX > centerCircleX) && (abs(cornerX) - abs(centerCircleX) <= radix + width)) {
        // atunci am o coliziune de tipul 4: jucator si peretele din dreapta
        return 4;
    }

    // daca nu am niciun fel de coliziune, intorc 0
    return 0;
}
