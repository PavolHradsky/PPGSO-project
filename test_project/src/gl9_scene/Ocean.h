#pragma once

#include <ppgso/ppgso.h>
#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>
#include "object.h"
#include "scene.h"

/*!
 * Simple object representing the player
 * Reads keyboard status and manipulates its own position
 * On Update checks collisions with Boat objects in the scene
 */
// Object to represent Bezier patch
class BezierPatch {
private:
    // 3D vectors define points/vertices of the shape
    std::vector<glm::vec3> vertices;

    // Texture coordinates
    std::vector<glm::vec2> texCoords;

    // Define our face using indexes to 3 vertices
    struct face {
        GLuint v0, v1, v2;
    };

    // Define our mesh as collection of faces
    std::vector<face> mesh;

    // These will hold the data and object buffers
    GLuint vao, vbo, tbo, ibo;
    glm::mat4 modelMatrix{1.0f};

    glm::vec3 bezierPoint(const glm::vec3 controlPoints[25], float t) {
        // TODO: Compute 3D point on bezier curve
        std::vector<glm::vec3> points;
        for (int i = 0; i < 25; i++) {
            points.push_back(controlPoints[i]);
        }

        for (int i = 0; i < 24; i++) {
            for (int j = 0; j < 24 - i; j++) {
                //points[j] = (1 - t) * points[j] + t * points[j + 1];
                points[j] = glm::lerp(points[j], points[j + 1], t);
            }
        }
        return points[0];
    }

    ppgso::Shader program{texture_vert_glsl, texture_frag_glsl};
    ppgso::Texture texture{ppgso::image::loadBMP("Ocean.bmp")};
public:
    // Public attributes that define position, color ..
    glm::vec3 position{0, 0, 0};
    glm::vec3 rotation{0, 0, 0};
    glm::vec3 scale{1, 1, 1};

    BezierPatch(const glm::vec3 controlPoints[25][25]) {
        bezierPatch(controlPoints);
    }

    // Initialize object data buffers
    void bezierPatch(const glm::vec3 controlPoints[25][25]) {
        // Generate Bezier patch points and incidences
        vertices = {};
        mesh = {};
        unsigned int PATCH_SIZE = 100;
        for (unsigned int i = 0; i < PATCH_SIZE; i++) {
            glm::vec3 points[25];
            float t = i * (1.f / PATCH_SIZE);
            for (unsigned int k = 0; k < 25; k++) {
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

class Ocean final : public Object {
private:
    // Static resources (Shared between instances)
    //static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;
    float speed = 0;
    // generate controlPoints .. x up to 25, y = 0, z up to 25
//    glm::vec3 controlPoints[25][25]{
//            {{5*0,  0, 5*0}, {5*0,  0, 5*1}, {5*0,  0, 5*2}, {5*0,  0, 5*3}, {5*0,  0, 5*4}, {5*0,  0, 5*5}, {5*0,  0, 5*6}, {5*0,  0, 5*7}, {5*0,  0, 5*8}, {5*0,  0, 5*9}, {5*0,  0, 5*10}, {5*0,  0, 5*11}, {5*0,  0, 5*12}, {5*0,  0, 5*13}, {5*0,  0, 5*14}, {5*0,  0, 5*15}, {5*0,  0, 5*16}, {5*0,  0, 5*17}, {5*0,  0, 5*18}, {5*0,  0, 5*19}, {5*0,  0, 5*20}, {5*0,  0, 5*21}, {5*0,  0, 5*22}, {5*0,  0, 5*23}, {5*0,  0, 5*24}},
//            {{5*1,  0, 5*0}, {5*1,  0, 5*1}, {5*1,  0, 5*2}, {5*1,  0, 5*3}, {5*1,  0, 5*4}, {5*1,  0, 5*5}, {5*1,  0, 5*6}, {5*1,  0, 5*7}, {5*1,  0, 5*8}, {5*1,  0, 5*9}, {5*1,  0, 5*10}, {5*1,  0, 5*11}, {5*1,  0, 5*12}, {5*1,  0, 5*13}, {5*1,  0, 5*14}, {5*1,  0, 5*15}, {5*1,  0, 5*16}, {5*1,  0, 5*17}, {5*1,  0, 5*18}, {5*1,  0, 5*19}, {5*1,  0, 5*20}, {5*1,  0, 5*21}, {5*1,  0, 5*22}, {5*1,  0, 5*23}, {5*1,  0, 5*24}},
//            {{5*2,  0, 5*0}, {5*2,  0, 5*1}, {5*2,  0, 5*2}, {5*2,  0, 5*3}, {5*2,  0, 5*4}, {5*2,  0, 5*5}, {5*2,  0, 5*6}, {5*2,  0, 5*7}, {5*2,  0, 5*8}, {5*2,  0, 5*9}, {5*2,  0, 5*10}, {5*2,  0, 5*11}, {5*2,  0, 5*12}, {5*2,  0, 5*13}, {5*2,  0, 5*14}, {5*2,  0, 5*15}, {5*2,  0, 5*16}, {5*2,  0, 5*17}, {5*2,  0, 5*18}, {5*2,  0, 5*19}, {5*2,  0, 5*20}, {5*2,  0, 5*21}, {5*2,  0, 5*22}, {5*2,  0, 5*23}, {5*2,  0, 5*24}},
//            {{5*3,  0, 5*0}, {5*3,  0, 5*1}, {5*3,  0, 5*2}, {5*3,  0, 5*3}, {5*3,  0, 5*4}, {5*3,  0, 5*5}, {5*3,  0, 5*6}, {5*3,  0, 5*7}, {5*3,  0, 5*8}, {5*3,  0, 5*9}, {5*3,  0, 5*10}, {5*3,  0, 5*11}, {5*3,  0, 5*12}, {5*3,  0, 5*13}, {5*3,  0, 5*14}, {5*3,  0, 5*15}, {5*3,  0, 5*16}, {5*3,  0, 5*17}, {5*3,  0, 5*18}, {5*3,  0, 5*19}, {5*3,  0, 5*20}, {5*3,  0, 5*21}, {5*3,  0, 5*22}, {5*3,  0, 5*23}, {5*3,  0, 5*24}},
//            {{5*4,  0, 5*0}, {5*4,  0, 5*1}, {5*4,  0, 5*2}, {5*4,  0, 5*3}, {5*4,  0, 5*4}, {5*4,  0, 5*5}, {5*4,  0, 5*6}, {5*4,  0, 5*7}, {5*4,  0, 5*8}, {5*4,  0, 5*9}, {5*4,  0, 5*10}, {5*4,  0, 5*11}, {5*4,  0, 5*12}, {5*4,  0, 5*13}, {5*4,  0, 5*14}, {5*4,  0, 5*15}, {5*4,  0, 5*16}, {5*4,  0, 5*17}, {5*4,  0, 5*18}, {5*4,  0, 5*19}, {5*4,  0, 5*20}, {5*4,  0, 5*21}, {5*4,  0, 5*22}, {5*4,  0, 5*23}, {5*4,  0, 5*24}},
//            {{5*5,  0, 5*0}, {5*5,  0, 5*1}, {5*5,  0, 5*2}, {5*5,  0, 5*3}, {5*5,  0, 5*4}, {5*5,  0, 5*5}, {5*5,  0, 5*6}, {5*5,  0, 5*7}, {5*5,  0, 5*8}, {5*5,  0, 5*9}, {5*5,  0, 5*10}, {5*5,  0, 5*11}, {5*5,  0, 5*12}, {5*5,  0, 5*13}, {5*5,  0, 5*14}, {5*5,  0, 5*15}, {5*5,  0, 5*16}, {5*5,  0, 5*17}, {5*5,  0, 5*18}, {5*5,  0, 5*19}, {5*5,  0, 5*20}, {5*5,  0, 5*21}, {5*5,  0, 5*22}, {5*5,  0, 5*23}, {5*5,  0, 5*24}},
//            {{5*6,  0, 5*0}, {5*6,  0, 5*1}, {5*6,  0, 5*2}, {5*6,  0, 5*3}, {5*6,  0, 5*4}, {5*6,  0, 5*5}, {5*6,  0, 5*6}, {5*6,  0, 5*7}, {5*6,  0, 5*8}, {5*6,  0, 5*9}, {5*6,  0, 5*10}, {5*6,  0, 5*11}, {5*6,  0, 5*12}, {5*6,  0, 5*13}, {5*6,  0, 5*14}, {5*6,  0, 5*15}, {5*6,  0, 5*16}, {5*6,  0, 5*17}, {5*6,  0, 5*18}, {5*6,  0, 5*19}, {5*6,  0, 5*20}, {5*6,  0, 5*21}, {5*6,  0, 5*22}, {5*6,  0, 5*23}, {5*6,  0, 5*24}},
//            {{5*7,  0, 5*0}, {5*7,  0, 5*1}, {5*7,  0, 5*2}, {5*7,  0, 5*3}, {5*7,  0, 5*4}, {5*7,  0, 5*5}, {5*7,  0, 5*6}, {5*7,  0, 5*7}, {5*7,  0, 5*8}, {5*7,  0, 5*9}, {5*7,  0, 5*10}, {5*7,  0, 5*11}, {5*7,  0, 5*12}, {5*7,  0, 5*13}, {5*7,  0, 5*14}, {5*7,  0, 5*15}, {5*7,  0, 5*16}, {5*7,  0, 5*17}, {5*7,  0, 5*18}, {5*7,  0, 5*19}, {5*7,  0, 5*20}, {5*7,  0, 5*21}, {5*7,  0, 5*22}, {5*7,  0, 5*23}, {5*7,  0, 5*24}},
//            {{5*8,  0, 5*0}, {5*8,  0, 5*1}, {5*8,  0, 5*2}, {5*8,  0, 5*3}, {5*8,  0, 5*4}, {5*8,  0, 5*5}, {5*8,  0, 5*6}, {5*8,  0, 5*7}, {5*8,  0, 5*8}, {5*8,  0, 5*9}, {5*8,  0, 5*10}, {5*8,  0, 5*11}, {5*8,  0, 5*12}, {5*8,  0, 5*13}, {5*8,  0, 5*14}, {5*8,  0, 5*15}, {5*8,  0, 5*16}, {5*8,  0, 5*17}, {5*8,  0, 5*18}, {5*8,  0, 5*19}, {5*8,  0, 5*20}, {5*8,  0, 5*21}, {5*8,  0, 5*22}, {5*8,  0, 5*23}, {5*8,  0, 5*24}},
//            {{5*9,  0, 5*0}, {5*9,  0, 5*1}, {5*9,  0, 5*2}, {5*9,  0, 5*3}, {5*9,  0, 5*4}, {5*9,  0, 5*5}, {5*9,  0, 5*6}, {5*9,  0, 5*7}, {5*9,  0, 5*8}, {5*9,  0, 5*9}, {5*9,  0, 5*10}, {5*9,  0, 5*11}, {5*9,  0, 5*12}, {5*9,  0, 5*13}, {5*9,  0, 5*14}, {5*9,  0, 5*15}, {5*9,  0, 5*16}, {5*9,  0, 5*17}, {5*9,  0, 5*18}, {5*9,  0, 5*19}, {5*9,  0, 5*20}, {5*9,  0, 5*21}, {5*9,  0, 5*22}, {5*9,  0, 5*23}, {5*9,  0, 5*24}},
//            {{5*10, 0, 5*0}, {5*10, 0, 5*1}, {5*10, 0, 5*2}, {5*10, 0, 5*3}, {5*10, 0, 5*4}, {5*10, 0, 5*5}, {5*10, 0, 5*6}, {5*10, 0, 5*7}, {5*10, 0, 5*8}, {5*10, 0, 5*9}, {5*10, 0, 5*10}, {5*10, 0, 5*11}, {5*10, 0, 5*12}, {5*10, 0, 5*13}, {5*10, 0, 5*14}, {5*10, 0, 5*15}, {5*10, 0, 5*16}, {5*10, 0, 5*17}, {5*10, 0, 5*18}, {5*10, 0, 5*19}, {5*10, 0, 5*20}, {5*10, 0, 5*21}, {5*10, 0, 5*22}, {5*10, 0, 5*23}, {5*10, 0, 5*24}},
//            {{5*11, 0, 5*0}, {5*11, 0, 5*1}, {5*11, 0, 5*2}, {5*11, 0, 5*3}, {5*11, 0, 5*4}, {5*11, 0, 5*5}, {5*11, 0, 5*6}, {5*11, 0, 5*7}, {5*11, 0, 5*8}, {5*11, 0, 5*9}, {5*11, 0, 5*10}, {5*11, 0, 5*11}, {5*11, 0, 5*12}, {5*11, 0, 5*13}, {5*11, 0, 5*14}, {5*11, 0, 5*15}, {5*11, 0, 5*16}, {5*11, 0, 5*17}, {5*11, 0, 5*18}, {5*11, 0, 5*19}, {5*11, 0, 5*20}, {5*11, 0, 5*21}, {5*11, 0, 5*22}, {5*11, 0, 5*23}, {5*11, 0, 5*24}},
//            {{5*12, 0, 5*0}, {5*12, 0, 5*1}, {5*12, 0, 5*2}, {5*12, 0, 5*3}, {5*12, 0, 5*4}, {5*12, 0, 5*5}, {5*12, 0, 5*6}, {5*12, 0, 5*7}, {5*12, 0, 5*8}, {5*12, 0, 5*9}, {5*12, 0, 5*10}, {5*12, 0, 5*11}, {5*12, 0, 5*12}, {5*12, 0, 5*13}, {5*12, 0, 5*14}, {5*12, 0, 5*15}, {5*12, 0, 5*16}, {5*12, 0, 5*17}, {5*12, 0, 5*18}, {5*12, 0, 5*19}, {5*12, 0, 5*20}, {5*12, 0, 5*21}, {5*12, 0, 5*22}, {5*12, 0, 5*23}, {5*12, 0, 5*24}},
//            {{5*13, 0, 5*0}, {5*13, 0, 5*1}, {5*13, 0, 5*2}, {5*13, 0, 5*3}, {5*13, 0, 5*4}, {5*13, 0, 5*5}, {5*13, 0, 5*6}, {5*13, 0, 5*7}, {5*13, 0, 5*8}, {5*13, 0, 5*9}, {5*13, 0, 5*10}, {5*13, 0, 5*11}, {5*13, 0, 5*12}, {5*13, 0, 5*13}, {5*13, 0, 5*14}, {5*13, 0, 5*15}, {5*13, 0, 5*16}, {5*13, 0, 5*17}, {5*13, 0, 5*18}, {5*13, 0, 5*19}, {5*13, 0, 5*20}, {5*13, 0, 5*21}, {5*13, 0, 5*22}, {5*13, 0, 5*23}, {5*13, 0, 5*24}},
//            {{5*14, 0, 5*0}, {5*14, 0, 5*1}, {5*14, 0, 5*2}, {5*14, 0, 5*3}, {5*14, 0, 5*4}, {5*14, 0, 5*5}, {5*14, 0, 5*6}, {5*14, 0, 5*7}, {5*14, 0, 5*8}, {5*14, 0, 5*9}, {5*14, 0, 5*10}, {5*14, 0, 5*11}, {5*14, 0, 5*12}, {5*14, 0, 5*13}, {5*14, 0, 5*14}, {5*14, 0, 5*15}, {5*14, 0, 5*16}, {5*14, 0, 5*17}, {5*14, 0, 5*18}, {5*14, 0, 5*19}, {5*14, 0, 5*20}, {5*14, 0, 5*21}, {5*14, 0, 5*22}, {5*14, 0, 5*23}, {5*14, 0, 5*24}},
//            {{5*15, 0, 5*0}, {5*15, 0, 5*1}, {5*15, 0, 5*2}, {5*15, 0, 5*3}, {5*15, 0, 5*4}, {5*15, 0, 5*5}, {5*15, 0, 5*6}, {5*15, 0, 5*7}, {5*15, 0, 5*8}, {5*15, 0, 5*9}, {5*15, 0, 5*10}, {5*15, 0, 5*11}, {5*15, 0, 5*12}, {5*15, 0, 5*13}, {5*15, 0, 5*14}, {5*15, 0, 5*15}, {5*15, 0, 5*16}, {5*15, 0, 5*17}, {5*15, 0, 5*18}, {5*15, 0, 5*19}, {5*15, 0, 5*20}, {5*15, 0, 5*21}, {5*15, 0, 5*22}, {5*15, 0, 5*23}, {5*15, 0, 5*24}},
//            {{5*16, 0, 5*0}, {5*16, 0, 5*1}, {5*16, 0, 5*2}, {5*16, 0, 5*3}, {5*16, 0, 5*4}, {5*16, 0, 5*5}, {5*16, 0, 5*6}, {5*16, 0, 5*7}, {5*16, 0, 5*8}, {5*16, 0, 5*9}, {5*16, 0, 5*10}, {5*16, 0, 5*11}, {5*16, 0, 5*12}, {5*16, 0, 5*13}, {5*16, 0, 5*14}, {5*16, 0, 5*15}, {5*16, 0, 5*16}, {5*16, 0, 5*17}, {5*16, 0, 5*18}, {5*16, 0, 5*19}, {5*16, 0, 5*20}, {5*16, 0, 5*21}, {5*16, 0, 5*22}, {5*16, 0, 5*23}, {5*16, 0, 5*24}},
//            {{5*17, 0, 5*0}, {5*17, 0, 5*1}, {5*17, 0, 5*2}, {5*17, 0, 5*3}, {5*17, 0, 5*4}, {5*17, 0, 5*5}, {5*17, 0, 5*6}, {5*17, 0, 5*7}, {5*17, 0, 5*8}, {5*17, 0, 5*9}, {5*17, 0, 5*10}, {5*17, 0, 5*11}, {5*17, 0, 5*12}, {5*17, 0, 5*13}, {5*17, 0, 5*14}, {5*17, 0, 5*15}, {5*17, 0, 5*16}, {5*17, 0, 5*17}, {5*17, 0, 5*18}, {5*17, 0, 5*19}, {5*17, 0, 5*20}, {5*17, 0, 5*21}, {5*17, 0, 5*22}, {5*17, 0, 5*23}, {5*17, 0, 5*24}},
//            {{5*18, 0, 5*0}, {5*18, 0, 5*1}, {5*18, 0, 5*2}, {5*18, 0, 5*3}, {5*18, 0, 5*4}, {5*18, 0, 5*5}, {5*18, 0, 5*6}, {5*18, 0, 5*7}, {5*18, 0, 5*8}, {5*18, 0, 5*9}, {5*18, 0, 5*10}, {5*18, 0, 5*11}, {5*18, 0, 5*12}, {5*18, 0, 5*13}, {5*18, 0, 5*14}, {5*18, 0, 5*15}, {5*18, 0, 5*16}, {5*18, 0, 5*17}, {5*18, 0, 5*18}, {5*18, 0, 5*19}, {5*18, 0, 5*20}, {5*18, 0, 5*21}, {5*18, 0, 5*22}, {5*18, 0, 5*23}, {5*18, 0, 5*24}},
//            {{5*19, 0, 5*0}, {5*19, 0, 5*1}, {5*19, 0, 5*2}, {5*19, 0, 5*3}, {5*19, 0, 5*4}, {5*19, 0, 5*5}, {5*19, 0, 5*6}, {5*19, 0, 5*7}, {5*19, 0, 5*8}, {5*19, 0, 5*9}, {5*19, 0, 5*10}, {5*19, 0, 5*11}, {5*19, 0, 5*12}, {5*19, 0, 5*13}, {5*19, 0, 5*14}, {5*19, 0, 5*15}, {5*19, 0, 5*16}, {5*19, 0, 5*17}, {5*19, 0, 5*18}, {5*19, 0, 5*19}, {5*19, 0, 5*20}, {5*19, 0, 5*21}, {5*19, 0, 5*22}, {5*19, 0, 5*23}, {5*19, 0, 5*24}},
//            {{5*20, 0, 5*0}, {5*20, 0, 5*1}, {5*20, 0, 5*2}, {5*20, 0, 5*3}, {5*20, 0, 5*4}, {5*20, 0, 5*5}, {5*20, 0, 5*6}, {5*20, 0, 5*7}, {5*20, 0, 5*8}, {5*20, 0, 5*9}, {5*20, 0, 5*10}, {5*20, 0, 5*11}, {5*20, 0, 5*12}, {5*20, 0, 5*13}, {5*20, 0, 5*14}, {5*20, 0, 5*15}, {5*20, 0, 5*16}, {5*20, 0, 5*17}, {5*20, 0, 5*18}, {5*20, 0, 5*19}, {5*20, 0, 5*20}, {5*20, 0, 5*21}, {5*20, 0, 5*22}, {5*20, 0, 5*23}, {5*20, 0, 5*24}},
//            {{5*21, 0, 5*0}, {5*21, 0, 5*1}, {5*21, 0, 5*2}, {5*21, 0, 5*3}, {5*21, 0, 5*4}, {5*21, 0, 5*5}, {5*21, 0, 5*6}, {5*21, 0, 5*7}, {5*21, 0, 5*8}, {5*21, 0, 5*9}, {5*21, 0, 5*10}, {5*21, 0, 5*11}, {5*21, 0, 5*12}, {5*21, 0, 5*13}, {5*21, 0, 5*14}, {5*21, 0, 5*15}, {5*21, 0, 5*16}, {5*21, 0, 5*17}, {5*21, 0, 5*18}, {5*21, 0, 5*19}, {5*21, 0, 5*20}, {5*21, 0, 5*21}, {5*21, 0, 5*22}, {5*21, 0, 5*23}, {5*21, 0, 5*24}},
//            {{5*22, 0, 5*0}, {5*22, 0, 5*1}, {5*22, 0, 5*2}, {5*22, 0, 5*3}, {5*22, 0, 5*4}, {5*22, 0, 5*5}, {5*22, 0, 5*6}, {5*22, 0, 5*7}, {5*22, 0, 5*8}, {5*22, 0, 5*9}, {5*22, 0, 5*10}, {5*22, 0, 5*11}, {5*22, 0, 5*12}, {5*22, 0, 5*13}, {5*22, 0, 5*14}, {5*22, 0, 5*15}, {5*22, 0, 5*16}, {5*22, 0, 5*17}, {5*22, 0, 5*18}, {5*22, 0, 5*19}, {5*22, 0, 5*20}, {5*22, 0, 5*21}, {5*22, 0, 5*22}, {5*22, 0, 5*23}, {5*22, 0, 5*24}},
//            {{5*23, 0, 5*0}, {5*23, 0, 5*1}, {5*23, 0, 5*2}, {5*23, 0, 5*3}, {5*23, 0, 5*4}, {5*23, 0, 5*5}, {5*23, 0, 5*6}, {5*23, 0, 5*7}, {5*23, 0, 5*8}, {5*23, 0, 5*9}, {5*23, 0, 5*10}, {5*23, 0, 5*11}, {5*23, 0, 5*12}, {5*23, 0, 5*13}, {5*23, 0, 5*14}, {5*23, 0, 5*15}, {5*23, 0, 5*16}, {5*23, 0, 5*17}, {5*23, 0, 5*18}, {5*23, 0, 5*19}, {5*23, 0, 5*20}, {5*23, 0, 5*21}, {5*23, 0, 5*22}, {5*23, 0, 5*23}, {5*23, 0, 5*24}},
//            {{5*24, 0, 5*0}, {5*24, 0, 5*1}, {5*24, 0, 5*2}, {5*24, 0, 5*3}, {5*24, 0, 5*4}, {5*24, 0, 5*5}, {5*24, 0, 5*6}, {5*24, 0, 5*7}, {5*24, 0, 5*8}, {5*24, 0, 5*9}, {5*24, 0, 5*10}, {5*24, 0, 5*11}, {5*24, 0, 5*12}, {5*24, 0, 5*13}, {5*24, 0, 5*14}, {5*24, 0, 5*15}, {5*24, 0, 5*16}, {5*24, 0, 5*17}, {5*24, 0, 5*18}, {5*24, 0, 5*19}, {5*24, 0, 5*20}, {5*24, 0, 5*21}, {5*24, 0, 5*22}, {5*24, 0, 5*23}, {5*24, 0, 5*24}}
    glm::vec3 controlPoints[25][25];
    BezierPatch bezier = BezierPatch(controlPoints);
public:
    /*!
     * Create a new player
     */
    Ocean();

    /*!
     * Update player position considering keyboard inputs
     * @param scene Scene to update
     * @param dt Time delta
     * @return true to delete the object
     */
    bool update(Scene &scene, float dt) override;

    /*!
     * Render player
     * @param scene Scene to render in
     */
    void render(Scene &scene) override;


    /*!
     * Ocean click event
     * @param scene
     */
    void onClick(Scene &scene) override;

    glm::vec3 globalPosition = {0, 0, 0};
};

