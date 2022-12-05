//
// Created by hrads on 29. 11. 2022.
//

#ifndef PPGSO_SURFACE_H
#define PPGSO_SURFACE_H



#include <ppgso/ppgso.h>
#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>
#include <shaders/my_texture_frag_glsl.h>
#include "object.h"
#include "scene.h"

#define POINTS 11

/*!
* Simple object representing the player
        * Reads keyboard status and manipulates its own position
* On Update checks collisions with Boat objects in the scene
*/
// Object to represent Bezier patch
class BezierPatch {
private:


    // Texture coordinates
    std::vector<glm::vec2> texCoords;

    // Define our face using indexes to 3 vertices
    struct face {
        GLuint v0, v1, v2;
    };

    // These will hold the data and object buffers
    GLuint vao, vbo, tbo, ibo;
    glm::mat4 modelMatrix{1.0f};

    glm::vec3 bezierPoint(const glm::vec3 controlPoints[POINTS], float t) {
        // TODO: Compute 3D point on bezier curve
        std::vector<glm::vec3> points;
        for (int i = 0; i < POINTS; i++) {
            points.push_back(controlPoints[i]);
        }

        for (int i = 0; i < POINTS-1; i++) {
            for (int j = 0; j < POINTS-1 - i; j++) {
                //points[j] = (1 - t) * points[j] + t * points[j + 1];
                points[j] = glm::lerp(points[j], points[j + 1], t);
            }
        }
        return points[0];
    }

    ppgso::Shader program{texture_vert_glsl, my_texture_frag_glsl};
    ppgso::Texture texture{ppgso::image::loadBMP("Sand2.bmp")};
public:
    // Public attributes that define position, color ..
    glm::vec3 position{0, 0, 0};
    glm::vec3 rotation{0, 0, 0};
    glm::vec3 scale{1, 1, 1};

    // 3D vectors define points/vertices of the shape
    std::vector<glm::vec3> vertices;

    // Define our mesh as collection of faces
    std::vector<face> mesh;

    BezierPatch(const glm::vec3 controlPoints[POINTS][POINTS]) {
        bezierPatch(controlPoints);
    }

    // Initialize object data buffers
    void bezierPatch(const glm::vec3 controlPoints[POINTS][POINTS]) {
        // Generate Bezier patch points and incidences
        vertices = {};
        mesh = {};
        unsigned int PATCH_SIZE = 100;
        for (unsigned int i = 0; i < PATCH_SIZE; i++) {
            glm::vec3 points[POINTS];
            float t = i * (1.f / PATCH_SIZE);
            for (unsigned int k = 0; k < POINTS; k++) {
                points[k] = bezierPoint(controlPoints[k], t);
            }
            for (unsigned int j = 0; j < PATCH_SIZE; j++) {
                // TODO: Compute points on the bezier patch
                // HINT: Compute u, v coordinates
                float t2 = j * (1.f / PATCH_SIZE);
                vertices.push_back(bezierPoint(points, t2));
                texCoords.emplace_back(t, 1 - t2);
            }
        }
//        for(int i = 0; i < POINTS; i++){
//            for(int j = 0; j < POINTS; j++){
//                vertices.push_back(controlPoints[i][j]);
//                texCoords.emplace_back(i / (float)POINTS, j / (float)POINTS);
//            }
//        }
        // Generate indices
        for (unsigned int i = 1; i < PATCH_SIZE; i++) {
            for (unsigned int j = 1; j < PATCH_SIZE; j++) {
                // TODO: Compute indices for triangle 1
                mesh.push_back({(i - 1) * PATCH_SIZE + j, (i - 1) * PATCH_SIZE + j - 1,  i * PATCH_SIZE + j - 1});
                // TODO: Compute indices for triangle 2
                mesh.push_back({i + PATCH_SIZE * (j - 1), (i - 1) + PATCH_SIZE * j, (i) + PATCH_SIZE * (j)});
            }
        }

        // Copy data to OpenGL
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // Copy positions to gpu
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

        // Set vertex program inputs
        auto position_attrib = program.getAttribLocation("Position");
        glEnableVertexAttribArray(position_attrib);
        glVertexAttribPointer(position_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

        // Copy texture positions to gpu
        glGenBuffers(1, &tbo);
        glBindBuffer(GL_ARRAY_BUFFER, tbo);
        glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(glm::vec2), texCoords.data(), GL_STATIC_DRAW);

        // Set vertex program inputs
        auto texCoord_attrib = program.getAttribLocation("TexCoord");
        glEnableVertexAttribArray(texCoord_attrib);
        glVertexAttribPointer(texCoord_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

        // Copy indices to gpu
        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.size() * sizeof(face), mesh.data(), GL_STATIC_DRAW);

    };

    // Clean up
    ~BezierPatch() {
        // Delete data from OpenGL
        glDeleteBuffers(1, &ibo);
        glDeleteBuffers(1, &tbo);
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }

    // Set the object transformation matrix
    void update() {
        // TODO: Compute transformation by scaling, rotating and then translating the shape
        modelMatrix = glm::translate(glm::mat4(1.0f), position);
        modelMatrix = glm::rotate(modelMatrix, rotation.x, glm::vec3(1, 0, 0));
        modelMatrix = glm::rotate(modelMatrix, rotation.y, glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, rotation.z, glm::vec3(0, 0, 1));
        modelMatrix = glm::scale(modelMatrix, scale);


//
//        modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
//         modelMatrix = glm::rotate(modelMatrix, glm::radians(1.0f), glm::vec3(1, 0, 0));
//        modelMatrix = glm::translate(modelMatrix, position);
//        modelMatrix = glm::scale(modelMatrix, scale);

    }

    // Draw polygons
    void render(Scene &scene) {
        // Update transformation and color uniforms in the shader
        program.use();

        // Initialize projection
        // Create projection matrix (field of view (radians), aspect ratio, near plane distance, far plane distance)
        // You can think of this as the camera objective settings
        program.setUniform("ProjectionMatrix", scene.camera->projectionMatrix);

        // Create view matrix (translate camera a bit backwards, so we can see the geometry)
        // This can be seen as the camera position/rotation in space

        program.setUniform("ViewMatrix", scene.camera->viewMatrix);

        // Set model position
        program.setUniform("ModelMatrix", modelMatrix);

        // Bind texture
        program.setUniform("Texture", texture);

        glBindVertexArray(vao);
        // TODO: Use correct rendering mode to draw the result
        glDrawElements(GL_TRIANGLES, mesh.size() * 3, GL_UNSIGNED_INT, nullptr);
    };
};

#endif //PPGSO_SURFACE_H