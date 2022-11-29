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
        modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
         modelMatrix = glm::rotate(modelMatrix, glm::radians(1.0f), glm::vec3(1, 0, 0));
        modelMatrix = glm::translate(modelMatrix, position);
        modelMatrix = glm::scale(modelMatrix, scale);

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
    glm::vec3 controlPoints[25][25]{
            {{0,  0, 0}, {0,  0, 1}, {0,  0, 2}, {0,  0, 3}, {0,  0, 4}, {0,  0, 5}, {0,  0, 6}, {0,  0, 7}, {0,  0, 8}, {0,  0, 9}, {0,  0, 10}, {0,  0, 11}, {0,  0, 12}, {0,  0, 13}, {0,  0, 14}, {0,  0, 15}, {0,  0, 16}, {0,  0, 17}, {0,  0, 18}, {0,  0, 19}, {0,  0, 20}, {0,  0, 21}, {0,  0, 22}, {0,  0, 23}, {0,  0, 24}},
            {{1,  0, 0}, {1,  0, 1}, {1,  0, 2}, {1,  0, 3}, {1,  0, 4}, {1,  0, 5}, {1,  0, 6}, {1,  0, 7}, {1,  0, 8}, {1,  0, 9}, {1,  0, 10}, {1,  0, 11}, {1,  0, 12}, {1,  0, 13}, {1,  0, 14}, {1,  0, 15}, {1,  0, 16}, {1,  0, 17}, {1,  0, 18}, {1,  0, 19}, {1,  0, 20}, {1,  0, 21}, {1,  0, 22}, {1,  0, 23}, {1,  0, 24}},
            {{2,  0, 0}, {2,  0, 1}, {2,  0, 2}, {2,  0, 3}, {2,  0, 4}, {2,  0, 5}, {2,  0, 6}, {2,  0, 7}, {2,  0, 8}, {2,  0, 9}, {2,  0, 10}, {2,  0, 11}, {2,  0, 12}, {2,  0, 13}, {2,  0, 14}, {2,  0, 15}, {2,  0, 16}, {2,  0, 17}, {2,  0, 18}, {2,  0, 19}, {2,  0, 20}, {2,  0, 21}, {2,  0, 22}, {2,  0, 23}, {2,  0, 24}},
            {{3,  0, 0}, {3,  0, 1}, {3,  0, 2}, {3,  0, 3}, {3,  0, 4}, {3,  0, 5}, {3,  0, 6}, {3,  0, 7}, {3,  0, 8}, {3,  0, 9}, {3,  0, 10}, {3,  0, 11}, {3,  0, 12}, {3,  0, 13}, {3,  0, 14}, {3,  0, 15}, {3,  0, 16}, {3,  0, 17}, {3,  0, 18}, {3,  0, 19}, {3,  0, 20}, {3,  0, 21}, {3,  0, 22}, {3,  0, 23}, {3,  0, 24}},
            {{4,  0, 0}, {4,  0, 1}, {4,  0, 2}, {4,  0, 3}, {4,  0, 4}, {4,  0, 5}, {4,  0, 6}, {4,  0, 7}, {4,  0, 8}, {4,  0, 9}, {4,  0, 10}, {4,  0, 11}, {4,  0, 12}, {4,  0, 13}, {4,  0, 14}, {4,  0, 15}, {4,  0, 16}, {4,  0, 17}, {4,  0, 18}, {4,  0, 19}, {4,  0, 20}, {4,  0, 21}, {4,  0, 22}, {4,  0, 23}, {4,  0, 24}},
            {{5,  0, 0}, {5,  0, 1}, {5,  0, 2}, {5,  0, 3}, {5,  0, 4}, {5,  0, 5}, {5,  0, 6}, {5,  0, 7}, {5,  0, 8}, {5,  0, 9}, {5,  0, 10}, {5,  0, 11}, {5,  0, 12}, {5,  0, 13}, {5,  0, 14}, {5,  0, 15}, {5,  0, 16}, {5,  0, 17}, {5,  0, 18}, {5,  0, 19}, {5,  0, 20}, {5,  0, 21}, {5,  0, 22}, {5,  0, 23}, {5,  0, 24}},
            {{6,  0, 0}, {6,  0, 1}, {6,  0, 2}, {6,  0, 3}, {6,  0, 4}, {6,  0, 5}, {6,  0, 6}, {6,  0, 7}, {6,  0, 8}, {6,  0, 9}, {6,  0, 10}, {6,  0, 11}, {6,  0, 12}, {6,  0, 13}, {6,  0, 14}, {6,  0, 15}, {6,  0, 16}, {6,  0, 17}, {6,  0, 18}, {6,  0, 19}, {6,  0, 20}, {6,  0, 21}, {6,  0, 22}, {6,  0, 23}, {6,  0, 24}},
            {{7,  0, 0}, {7,  0, 1}, {7,  0, 2}, {7,  0, 3}, {7,  0, 4}, {7,  0, 5}, {7,  0, 6}, {7,  0, 7}, {7,  0, 8}, {7,  0, 9}, {7,  0, 10}, {7,  0, 11}, {7,  0, 12}, {7,  0, 13}, {7,  0, 14}, {7,  0, 15}, {7,  0, 16}, {7,  0, 17}, {7,  0, 18}, {7,  0, 19}, {7,  0, 20}, {7,  0, 21}, {7,  0, 22}, {7,  0, 23}, {7,  0, 24}},
            {{8,  0, 0}, {8,  0, 1}, {8,  0, 2}, {8,  0, 3}, {8,  0, 4}, {8,  0, 5}, {8,  0, 6}, {8,  0, 7}, {8,  0, 8}, {8,  0, 9}, {8,  0, 10}, {8,  0, 11}, {8,  0, 12}, {8,  0, 13}, {8,  0, 14}, {8,  0, 15}, {8,  0, 16}, {8,  0, 17}, {8,  0, 18}, {8,  0, 19}, {8,  0, 20}, {8,  0, 21}, {8,  0, 22}, {8,  0, 23}, {8,  0, 24}},
            {{9,  0, 0}, {9,  0, 1}, {9,  0, 2}, {9,  0, 3}, {9,  0, 4}, {9,  0, 5}, {9,  0, 6}, {9,  0, 7}, {9,  0, 8}, {9,  0, 9}, {9,  0, 10}, {9,  0, 11}, {9,  0, 12}, {9,  0, 13}, {9,  0, 14}, {9,  0, 15}, {9,  0, 16}, {9,  0, 17}, {9,  0, 18}, {9,  0, 19}, {9,  0, 20}, {9,  0, 21}, {9,  0, 22}, {9,  0, 23}, {9,  0, 24}},
            {{10, 0, 0}, {10, 0, 1}, {10, 0, 2}, {10, 0, 3}, {10, 0, 4}, {10, 0, 5}, {10, 0, 6}, {10, 0, 7}, {10, 0, 8}, {10, 0, 9}, {10, 0, 10}, {10, 0, 11}, {10, 0, 12}, {10, 0, 13}, {10, 0, 14}, {10, 0, 15}, {10, 0, 16}, {10, 0, 17}, {10, 0, 18}, {10, 0, 19}, {10, 0, 20}, {10, 0, 21}, {10, 0, 22}, {10, 0, 23}, {10, 0, 24}},
            {{11, 0, 0}, {11, 0, 1}, {11, 0, 2}, {11, 0, 3}, {11, 0, 4}, {11, 0, 5}, {11, 0, 6}, {11, 0, 7}, {11, 0, 8}, {11, 0, 9}, {11, 0, 10}, {11, 0, 11}, {11, 0, 12}, {11, 0, 13}, {11, 0, 14}, {11, 0, 15}, {11, 0, 16}, {11, 0, 17}, {11, 0, 18}, {11, 0, 19}, {11, 0, 20}, {11, 0, 21}, {11, 0, 22}, {11, 0, 23}, {11, 0, 24}},
            {{12, 0, 0}, {12, 0, 1}, {12, 0, 2}, {12, 0, 3}, {12, 0, 4}, {12, 0, 5}, {12, 0, 6}, {12, 0, 7}, {12, 0, 8}, {12, 0, 9}, {12, 0, 10}, {12, 0, 11}, {12, 0, 12}, {12, 0, 13}, {12, 0, 14}, {12, 0, 15}, {12, 0, 16}, {12, 0, 17}, {12, 0, 18}, {12, 0, 19}, {12, 0, 20}, {12, 0, 21}, {12, 0, 22}, {12, 0, 23}, {12, 0, 24}},
            {{13, 0, 0}, {13, 0, 1}, {13, 0, 2}, {13, 0, 3}, {13, 0, 4}, {13, 0, 5}, {13, 0, 6}, {13, 0, 7}, {13, 0, 8}, {13, 0, 9}, {13, 0, 10}, {13, 0, 11}, {13, 0, 12}, {13, 0, 13}, {13, 0, 14}, {13, 0, 15}, {13, 0, 16}, {13, 0, 17}, {13, 0, 18}, {13, 0, 19}, {13, 0, 20}, {13, 0, 21}, {13, 0, 22}, {13, 0, 23}, {13, 0, 24}},
            {{14, 0, 0}, {14, 0, 1}, {14, 0, 2}, {14, 0, 3}, {14, 0, 4}, {14, 0, 5}, {14, 0, 6}, {14, 0, 7}, {14, 0, 8}, {14, 0, 9}, {14, 0, 10}, {14, 0, 11}, {14, 0, 12}, {14, 0, 13}, {14, 0, 14}, {14, 0, 15}, {14, 0, 16}, {14, 0, 17}, {14, 0, 18}, {14, 0, 19}, {14, 0, 20}, {14, 0, 21}, {14, 0, 22}, {14, 0, 23}, {14, 0, 24}},
            {{15, 0, 0}, {15, 0, 1}, {15, 0, 2}, {15, 0, 3}, {15, 0, 4}, {15, 0, 5}, {15, 0, 6}, {15, 0, 7}, {15, 0, 8}, {15, 0, 9}, {15, 0, 10}, {15, 0, 11}, {15, 0, 12}, {15, 0, 13}, {15, 0, 14}, {15, 0, 15}, {15, 0, 16}, {15, 0, 17}, {15, 0, 18}, {15, 0, 19}, {15, 0, 20}, {15, 0, 21}, {15, 0, 22}, {15, 0, 23}, {15, 0, 24}},
            {{16, 0, 0}, {16, 0, 1}, {16, 0, 2}, {16, 0, 3}, {16, 0, 4}, {16, 0, 5}, {16, 0, 6}, {16, 0, 7}, {16, 0, 8}, {16, 0, 9}, {16, 0, 10}, {16, 0, 11}, {16, 0, 12}, {16, 0, 13}, {16, 0, 14}, {16, 0, 15}, {16, 0, 16}, {16, 0, 17}, {16, 0, 18}, {16, 0, 19}, {16, 0, 20}, {16, 0, 21}, {16, 0, 22}, {16, 0, 23}, {16, 0, 24}},
            {{17, 0, 0}, {17, 0, 1}, {17, 0, 2}, {17, 0, 3}, {17, 0, 4}, {17, 0, 5}, {17, 0, 6}, {17, 0, 7}, {17, 0, 8}, {17, 0, 9}, {17, 0, 10}, {17, 0, 11}, {17, 0, 12}, {17, 0, 13}, {17, 0, 14}, {17, 0, 15}, {17, 0, 16}, {17, 0, 17}, {17, 0, 18}, {17, 0, 19}, {17, 0, 20}, {17, 0, 21}, {17, 0, 22}, {17, 0, 23}, {17, 0, 24}},
            {{18, 0, 0}, {18, 0, 1}, {18, 0, 2}, {18, 0, 3}, {18, 0, 4}, {18, 0, 5}, {18, 0, 6}, {18, 0, 7}, {18, 0, 8}, {18, 0, 9}, {18, 0, 10}, {18, 0, 11}, {18, 0, 12}, {18, 0, 13}, {18, 0, 14}, {18, 0, 15}, {18, 0, 16}, {18, 0, 17}, {18, 0, 18}, {18, 0, 19}, {18, 0, 20}, {18, 0, 21}, {18, 0, 22}, {18, 0, 23}, {18, 0, 24}},
            {{19, 0, 0}, {19, 0, 1}, {19, 0, 2}, {19, 0, 3}, {19, 0, 4}, {19, 0, 5}, {19, 0, 6}, {19, 0, 7}, {19, 0, 8}, {19, 0, 9}, {19, 0, 10}, {19, 0, 11}, {19, 0, 12}, {19, 0, 13}, {19, 0, 14}, {19, 0, 15}, {19, 0, 16}, {19, 0, 17}, {19, 0, 18}, {19, 0, 19}, {19, 0, 20}, {19, 0, 21}, {19, 0, 22}, {19, 0, 23}, {19, 0, 24}},
            {{20, 0, 0}, {20, 0, 1}, {20, 0, 2}, {20, 0, 3}, {20, 0, 4}, {20, 0, 5}, {20, 0, 6}, {20, 0, 7}, {20, 0, 8}, {20, 0, 9}, {20, 0, 10}, {20, 0, 11}, {20, 0, 12}, {20, 0, 13}, {20, 0, 14}, {20, 0, 15}, {20, 0, 16}, {20, 0, 17}, {20, 0, 18}, {20, 0, 19}, {20, 0, 20}, {20, 0, 21}, {20, 0, 22}, {20, 0, 23}, {20, 0, 24}},
            {{21, 0, 0}, {21, 0, 1}, {21, 0, 2}, {21, 0, 3}, {21, 0, 4}, {21, 0, 5}, {21, 0, 6}, {21, 0, 7}, {21, 0, 8}, {21, 0, 9}, {21, 0, 10}, {21, 0, 11}, {21, 0, 12}, {21, 0, 13}, {21, 0, 14}, {21, 0, 15}, {21, 0, 16}, {21, 0, 17}, {21, 0, 18}, {21, 0, 19}, {21, 0, 20}, {21, 0, 21}, {21, 0, 22}, {21, 0, 23}, {21, 0, 24}},
            {{22, 0, 0}, {22, 0, 1}, {22, 0, 2}, {22, 0, 3}, {22, 0, 4}, {22, 0, 5}, {22, 0, 6}, {22, 0, 7}, {22, 0, 8}, {22, 0, 9}, {22, 0, 10}, {22, 0, 11}, {22, 0, 12}, {22, 0, 13}, {22, 0, 14}, {22, 0, 15}, {22, 0, 16}, {22, 0, 17}, {22, 0, 18}, {22, 0, 19}, {22, 0, 20}, {22, 0, 21}, {22, 0, 22}, {22, 0, 23}, {22, 0, 24}},
            {{23, 0, 0}, {23, 0, 1}, {23, 0, 2}, {23, 0, 3}, {23, 0, 4}, {23, 0, 5}, {23, 0, 6}, {23, 0, 7}, {23, 0, 8}, {23, 0, 9}, {23, 0, 10}, {23, 0, 11}, {23, 0, 12}, {23, 0, 13}, {23, 0, 14}, {23, 0, 15}, {23, 0, 16}, {23, 0, 17}, {23, 0, 18}, {23, 0, 19}, {23, 0, 20}, {23, 0, 21}, {23, 0, 22}, {23, 0, 23}, {23, 0, 24}},
            {{24, 0, 0}, {24, 0, 1}, {24, 0, 2}, {24, 0, 3}, {24, 0, 4}, {24, 0, 5}, {24, 0, 6}, {24, 0, 7}, {24, 0, 8}, {24, 0, 9}, {24, 0, 10}, {24, 0, 11}, {24, 0, 12}, {24, 0, 13}, {24, 0, 14}, {24, 0, 15}, {24, 0, 16}, {24, 0, 17}, {24, 0, 18}, {24, 0, 19}, {24, 0, 20}, {24, 0, 21}, {24, 0, 22}, {24, 0, 23}, {24, 0, 24}}
    };
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

