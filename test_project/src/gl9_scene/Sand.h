//#pragma once
//
//#include <ppgso/ppgso.h>
//#include <shaders/texture_vert_glsl.h>
//#include <shaders/texture_frag_glsl.h>
//#include "object.h"
//#include "scene.h"
//
//
//// Object to represent Bezier patch
//class BezierPatch {
//private:
//    // 3D vectors define points/vertices of the shape
//    std::vector<glm::vec3> vertices;
//
//    // Texture coordinates
//    std::vector<glm::vec2> texCoords;
//
//    // Define our face using indexes to 3 vertices
//    struct face {
//        GLuint v0, v1, v2;
//    };
//
//    // Define our mesh as collection of faces
//    std::vector<face> mesh;
//
//    // These will hold the data and object buffers
//    GLuint vao, vbo, tbo, ibo;
//    glm::mat4 modelMatrix{1.0f};
//
//    glm::vec3 bezierPoint(const glm::vec3 controlPoints[25], float t) {
//        // TODO: Compute 3D point on bezier curve
//        std::vector<glm::vec3> points;
//        for (int i = 0; i < 25; i++) {
//            points.push_back(controlPoints[i]);
//        }
//
//        for (int i = 0; i < 24; i++) {
//            for (int j = 0; j < 24 - i; j++) {
//                //points[j] = (1 - t) * points[j] + t * points[j + 1];
//                points[j] = glm::lerp(points[j], points[j + 1], t);
//            }
//        }
//        return points[0];
//    }
//
//    ppgso::Shader program{texture_vert_glsl, texture_frag_glsl};
//    ppgso::Texture texture{ppgso::image::loadBMP("Sand.bmp")};
//public:
//    // Public attributes that define position, color ..
//    glm::vec3 position{0, 0, 0};
//    glm::vec3 rotation{0, 0, 0};
//    glm::vec3 scale{1, 1, 1};
//
//    BezierPatch(const glm::vec3 controlPoints[25][25]) {
//        bezierPatch(controlPoints);
//    }
//
//    // Initialize object data buffers
//    void bezierPatch(const glm::vec3 controlPoints[25][25]) {
//        // Generate Bezier patch points and incidences
//        vertices = {};
//        mesh = {};
//        unsigned int PATCH_SIZE = 25;
//        for (unsigned int i = 0; i < PATCH_SIZE; i++) {
//            glm::vec3 points[25];
//            float t = i * (1.f / PATCH_SIZE);
//            for (unsigned int k = 0; k < 25; k++) {
//                points[k] = bezierPoint(controlPoints[k], t);
//            }
//            for (unsigned int j = 0; j < PATCH_SIZE; j++) {
//                // TODO: Compute points on the bezier patch
//                // HINT: Compute u, v coordinates
//                float t2 = j * (1.f / PATCH_SIZE);
//                vertices.push_back(bezierPoint(points, t2));
//                texCoords.emplace_back(t, 1 - t2);
//            }
//        }
//        // Generate indices
//        for (unsigned int i = 1; i < PATCH_SIZE; i++) {
//            for (unsigned int j = 1; j < PATCH_SIZE; j++) {
//                // TODO: Compute indices for triangle 1
//                //mesh.push_back({i + PATCH_SIZE * (j - 1), (i - 1) + PATCH_SIZE * j, (i - 1) + PATCH_SIZE * (j - 1)});
//                mesh.push_back({(i-1)*PATCH_SIZE + (j-1), (i-1)*PATCH_SIZE + j, i*PATCH_SIZE + (j-1)});
//
//                // TODO: Compute indices for triangle 2
//                mesh.push_back({(i-1)*PATCH_SIZE + j, i * PATCH_SIZE + j,i * PATCH_SIZE + (j - 1)});
//            }
//        }
//
//        // Copy data to OpenGL
//        glGenVertexArrays(1, &vao);
//        glBindVertexArray(vao);
//
//        // Copy positions to gpu
//        glGenBuffers(1, &vbo);
//        glBindBuffer(GL_ARRAY_BUFFER, vbo);
//        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
//
//        // Set vertex program inputs
//        auto position_attrib = program.getAttribLocation("Position");
//        glEnableVertexAttribArray(position_attrib);
//        glVertexAttribPointer(position_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
//
//        // Copy texture positions to gpu
//        glGenBuffers(1, &tbo);
//        glBindBuffer(GL_ARRAY_BUFFER, tbo);
//        glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(glm::vec2), texCoords.data(), GL_STATIC_DRAW);
//
//        // Set vertex program inputs
//        auto texCoord_attrib = program.getAttribLocation("TexCoord");
//        glEnableVertexAttribArray(texCoord_attrib);
//        glVertexAttribPointer(texCoord_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
//
//        // Copy indices to gpu
//        glGenBuffers(1, &ibo);
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
//        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.size() * sizeof(face), mesh.data(), GL_STATIC_DRAW);
//
//    };
//
//    // Clean up
//    ~BezierPatch() {
//        // Delete data from OpenGL
//        glDeleteBuffers(1, &ibo);
//        glDeleteBuffers(1, &tbo);
//        glDeleteBuffers(1, &vbo);
//        glDeleteVertexArrays(1, &vao);
//    }
//
//    // Set the object transformation matrix
//    void update() {
//        // TODO: Compute transformation by scaling, rotating and then translating the shape
//        modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
//        modelMatrix = glm::rotate(modelMatrix, glm::radians(1.0f), glm::vec3(1, 0, 0));
//        modelMatrix = glm::translate(modelMatrix, position);
//        modelMatrix = glm::scale(modelMatrix, scale);
//
//    }
//
//    // Draw polygons
//    void render(Scene &scene) {
//        // Update transformation and color uniforms in the shader
//        program.use();
//
//        // Initialize projection
//        // Create projection matrix (field of view (radians), aspect ratio, near plane distance, far plane distance)
//        // You can think of this as the camera objective settings
//        program.setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
//
//        // Create view matrix (translate camera a bit backwards, so we can see the geometry)
//        // This can be seen as the camera position/rotation in space
//
//        program.setUniform("ViewMatrix", scene.camera->viewMatrix);
//
//        // Set model position
//        program.setUniform("ModelMatrix", modelMatrix);
//
//        // Bind texture
//        program.setUniform("Texture", texture);
//
//        glBindVertexArray(vao);
//        // TODO: Use correct rendering mode to draw the result
//        glDrawElements(GL_TRIANGLES, mesh.size() * 3, GL_UNSIGNED_INT, nullptr);
//    };
//};
//
//class Sand final : public Object {
//private:
//    // Static resources (Shared between instances)
//    //static std::unique_ptr<ppgso::Mesh> mesh;
//    static std::unique_ptr<ppgso::Shader> shader;
//    static std::unique_ptr<ppgso::Texture> texture;
//    float speed = 0;
//    // generate controlPoints .. x up to 25, y = 0, z up to 25
//    glm::vec3 controlPoints[25][25]{
//            {{0,  -80, 0}, {0,  -80, 1}, {0,  -80, 2}, {0,  -80, 3}, {0,  -80, 4}, {0,  -80, 5}, {0,  -80, 6}, {0,  -80, 7}, {0,  -80, 8}, {0,  -80, 9}, {0,  -80, 10}, {0,  -80, 11}, {0,  -80, 12}, {0,  -80, 13}, {0,  -80, 14}, {0,  -80, 15}, {0,  -80, 16}, {0,  -80, 17}, {0,  -80, 18}, {0,  -80, 19}, {0,  -80, 20}, {0,  -80, 21}, {0,  -80, 22}, {0,  -80, 23}, {0,  -80, 24}},
//            {{1,  -80, 0}, {1,  -80, 1}, {1,  -80, 2}, {1,  -80, 3}, {1,  -80, 4}, {1,  -80, 5}, {1,  -80, 6}, {1,  -80, 7}, {1,  -80, 8}, {1,  -80, 9}, {1,  -80, 10}, {1,  -80, 11}, {1,  -80, 12}, {1,  -80, 13}, {1,  -80, 14}, {1,  -80, 15}, {1,  -80, 16}, {1,  -80, 17}, {1,  -80, 18}, {1,  -80, 19}, {1,  -80, 20}, {1,  -80, 21}, {1,  -80, 22}, {1,  -80, 23}, {1,  -80, 24}},
//            {{2,  -80, 0}, {2,  -80, 1}, {2,  -80, 2}, {2,  -80, 3}, {2,  -80, 4}, {2,  -80, 5}, {2,  -80, 6}, {2,  -80, 7}, {2,  -80, 8}, {2,  -80, 9}, {2,  -80, 10}, {2,  -80, 11}, {2,  -80, 12}, {2,  -80, 13}, {2,  -80, 14}, {2,  -80, 15}, {2,  -80, 16}, {2,  -80, 17}, {2,  -80, 18}, {2,  -80, 19}, {2,  -80, 20}, {2,  -80, 21}, {2,  -80, 22}, {2,  -80, 23}, {2,  -80, 24}},
//            {{3,  -80, 0}, {3,  -80, 1}, {3,  -80, 2}, {3,  -80, 3}, {3,  -80, 4}, {3,  -80, 5}, {3,  -80, 6}, {3,  -80, 7}, {3,  -80, 8}, {3,  -80, 9}, {3,  -80, 10}, {3,  -80, 11}, {3,  -80, 12}, {3,  -80, 13}, {3,  -80, 14}, {3,  -80, 15}, {3,  -80, 16}, {3,  -80, 17}, {3,  -80, 18}, {3,  -80, 19}, {3,  -80, 20}, {3,  -80, 21}, {3,  -80, 22}, {3,  -80, 23}, {3,  -80, 24}},
//            {{4,  -80, 0}, {4,  -80, 1}, {4,  -80, 2}, {4,  -80, 3}, {4,  -80, 4}, {4,  -80, 5}, {4,  -80, 6}, {4,  -80, 7}, {4,  -80, 8}, {4,  -80, 9}, {4,  -80, 10}, {4,  -80, 11}, {4,  -80, 12}, {4,  -80, 13}, {4,  -80, 14}, {4,  -80, 15}, {4,  -80, 16}, {4,  -80, 17}, {4,  -80, 18}, {4,  -80, 19}, {4,  -80, 20}, {4,  -80, 21}, {4,  -80, 22}, {4,  -80, 23}, {4,  -80, 24}},
//            {{5,  -80, 0}, {5,  -80, 1}, {5,  -80, 2}, {5,  -80, 3}, {5,  -80, 4}, {5,  -80, 5}, {5,  -80, 6}, {5,  -80, 7}, {5,  -80, 8}, {5,  -80, 9}, {5,  -80, 10}, {5,  -80, 11}, {5,  -80, 12}, {5,  -80, 13}, {5,  -80, 14}, {5,  -80, 15}, {5,  -80, 16}, {5,  -80, 17}, {5,  -80, 18}, {5,  -80, 19}, {5,  -80, 20}, {5,  -80, 21}, {5,  -80, 22}, {5,  -80, 23}, {5,  -80, 24}},
//            {{6,  -80, 0}, {6,  -80, 1}, {6,  -80, 2}, {6,  -80, 3}, {6,  -80, 4}, {6,  -80, 5}, {6,  -80, 6}, {6,  -80, 7}, {6,  -80, 8}, {6,  -80, 9}, {6,  -80, 10}, {6,  -80, 11}, {6,  -80, 12}, {6,  -80, 13}, {6,  -80, 14}, {6,  -80, 15}, {6,  -80, 16}, {6,  -80, 17}, {6,  -80, 18}, {6,  -80, 19}, {6,  -80, 20}, {6,  -80, 21}, {6,  -80, 22}, {6,  -80, 23}, {6,  -80, 24}},
//            {{7,  -80, 0}, {7,  -80, 1}, {7,  -80, 2}, {7,  -80, 3}, {7,  -80, 4}, {7,  -80, 5}, {7,  -80, 6}, {7,  -80, 7}, {7,  -80, 8}, {7,  -80, 9}, {7,  -80, 10}, {7,  -80, 11}, {7,  -80, 12}, {7,  -80, 13}, {7,  -80, 14}, {7,  -80, 15}, {7,  -80, 16}, {7,  -80, 17}, {7,  -80, 18}, {7,  -80, 19}, {7,  -80, 20}, {7,  -80, 21}, {7,  -80, 22}, {7,  -80, 23}, {7,  -80, 24}},
//            {{8,  -80, 0}, {8,  -80, 1}, {8,  -80, 2}, {8,  -80, 3}, {8,  -80, 4}, {8,  -80, 5}, {8,  -80, 6}, {8,  -80, 7}, {8,  -80, 8}, {8,  -80, 9}, {8,  -80, 10}, {8,  -80, 11}, {8,  -80, 12}, {8,  -80, 13}, {8,  -80, 14}, {8,  -80, 15}, {8,  -80, 16}, {8,  -80, 17}, {8,  -80, 18}, {8,  -80, 19}, {8,  -80, 20}, {8,  -80, 21}, {8,  -80, 22}, {8,  -80, 23}, {8,  -80, 24}},
//            {{9,  -80, 0}, {9,  -80, 1}, {9,  -80, 2}, {9,  -80, 3}, {9,  -80, 4}, {9,  -80, 5}, {9,  -80, 6}, {9,  -80, 7}, {9,  -80, 8}, {9,  -80, 9}, {9,  -80, 10}, {9,  -80, 11}, {9,  -80, 12}, {9,  -80, 13}, {9,  -80, 14}, {9,  -80, 15}, {9,  -80, 16}, {9,  -80, 17}, {9,  -80, 18}, {9,  -80, 19}, {9,  -80, 20}, {9,  -80, 21}, {9,  -80, 22}, {9,  -80, 23}, {9,  -80, 24}},
//            {{10, -80, 0}, {10, -80, 1}, {10, -80, 2}, {10, -80, 3}, {10, -80, 4}, {10, -80, 5}, {10, -80, 6}, {10, -80, 7}, {10, -80, 8}, {10, -80, 9}, {10, -80, 10}, {10, -80, 11}, {10, -80, 12}, {10, -80, 13}, {10, -80, 14}, {10, -80, 15}, {10, -80, 16}, {10, -80, 17}, {10, -80, 18}, {10, -80, 19}, {10, -80, 20}, {10, -80, 21}, {10, -80, 22}, {10, -80, 23}, {10, -80, 24}},
//            {{11, -80, 0}, {11, -80, 1}, {11, -80, 2}, {11, -80, 3}, {11, -80, 4}, {11, -80, 5}, {11, -80, 6}, {11, -80, 7}, {11, -80, 8}, {11, -80, 9}, {11, -80, 10}, {11, -80, 11}, {11, -80, 12}, {11, -80, 13}, {11, -80, 14}, {11, -80, 15}, {11, -80, 16}, {11, -80, 17}, {11, -80, 18}, {11, -80, 19}, {11, -80, 20}, {11, -80, 21}, {11, -80, 22}, {11, -80, 23}, {11, -80, 24}},
//            {{12, -80, 0}, {12, -80, 1}, {12, -80, 2}, {12, -80, 3}, {12, -80, 4}, {12, -80, 5}, {12, -80, 6}, {12, -80, 7}, {12, -80, 8}, {12, -80, 9}, {12, -80, 10}, {12, -80, 11}, {12, -80, 12}, {12, -80, 13}, {12, -80, 14}, {12, -80, 15}, {12, -80, 16}, {12, -80, 17}, {12, -80, 18}, {12, -80, 19}, {12, -80, 20}, {12, -80, 21}, {12, -80, 22}, {12, -80, 23}, {12, -80, 24}},
//            {{13, -80, 0}, {13, -80, 1}, {13, -80, 2}, {13, -80, 3}, {13, -80, 4}, {13, -80, 5}, {13, -80, 6}, {13, -80, 7}, {13, -80, 8}, {13, -80, 9}, {13, -80, 10}, {13, -80, 11}, {13, -80, 12}, {13, -80, 13}, {13, -80, 14}, {13, -80, 15}, {13, -80, 16}, {13, -80, 17}, {13, -80, 18}, {13, -80, 19}, {13, -80, 20}, {13, -80, 21}, {13, -80, 22}, {13, -80, 23}, {13, -80, 24}},
//            {{14, -80, 0}, {14, -80, 1}, {14, -80, 2}, {14, -80, 3}, {14, -80, 4}, {14, -80, 5}, {14, -80, 6}, {14, -80, 7}, {14, -80, 8}, {14, -80, 9}, {14, -80, 10}, {14, -80, 11}, {14, -80, 12}, {14, -80, 13}, {14, -80, 14}, {14, -80, 15}, {14, -80, 16}, {14, -80, 17}, {14, -80, 18}, {14, -80, 19}, {14, -80, 20}, {14, -80, 21}, {14, -80, 22}, {14, -80, 23}, {14, -80, 24}},
//            {{15, -80, 0}, {15, -80, 1}, {15, -80, 2}, {15, -80, 3}, {15, -80, 4}, {15, -80, 5}, {15, -80, 6}, {15, -80, 7}, {15, -80, 8}, {15, -80, 9}, {15, -80, 10}, {15, -80, 11}, {15, -80, 12}, {15, -80, 13}, {15, -80, 14}, {15, -80, 15}, {15, -80, 16}, {15, -80, 17}, {15, -80, 18}, {15, -80, 19}, {15, -80, 20}, {15, -80, 21}, {15, -80, 22}, {15, -80, 23}, {15, -80, 24}},
//            {{16, -80, 0}, {16, -80, 1}, {16, -80, 2}, {16, -80, 3}, {16, -80, 4}, {16, -80, 5}, {16, -80, 6}, {16, -80, 7}, {16, -80, 8}, {16, -80, 9}, {16, -80, 10}, {16, -80, 11}, {16, -80, 12}, {16, -80, 13}, {16, -80, 14}, {16, -80, 15}, {16, -80, 16}, {16, -80, 17}, {16, -80, 18}, {16, -80, 19}, {16, -80, 20}, {16, -80, 21}, {16, -80, 22}, {16, -80, 23}, {16, -80, 24}},
//            {{17, -80, 0}, {17, -80, 1}, {17, -80, 2}, {17, -80, 3}, {17, -80, 4}, {17, -80, 5}, {17, -80, 6}, {17, -80, 7}, {17, -80, 8}, {17, -80, 9}, {17, -80, 10}, {17, -80, 11}, {17, -80, 12}, {17, -80, 13}, {17, -80, 14}, {17, -80, 15}, {17, -80, 16}, {17, -80, 17}, {17, -80, 18}, {17, -80, 19}, {17, -80, 20}, {17, -80, 21}, {17, -80, 22}, {17, -80, 23}, {17, -80, 24}},
//            {{18, -80, 0}, {18, -80, 1}, {18, -80, 2}, {18, -80, 3}, {18, -80, 4}, {18, -80, 5}, {18, -80, 6}, {18, -80, 7}, {18, -80, 8}, {18, -80, 9}, {18, -80, 10}, {18, -80, 11}, {18, -80, 12}, {18, -80, 13}, {18, -80, 14}, {18, -80, 15}, {18, -80, 16}, {18, -80, 17}, {18, -80, 18}, {18, -80, 19}, {18, -80, 20}, {18, -80, 21}, {18, -80, 22}, {18, -80, 23}, {18, -80, 24}},
//            {{19, -80, 0}, {19, -80, 1}, {19, -80, 2}, {19, -80, 3}, {19, -80, 4}, {19, -80, 5}, {19, -80, 6}, {19, -80, 7}, {19, -80, 8}, {19, -80, 9}, {19, -80, 10}, {19, -80, 11}, {19, -80, 12}, {19, -80, 13}, {19, -80, 14}, {19, -80, 15}, {19, -80, 16}, {19, -80, 17}, {19, -80, 18}, {19, -80, 19}, {19, -80, 20}, {19, -80, 21}, {19, -80, 22}, {19, -80, 23}, {19, -80, 24}},
//            {{20, -80, 0}, {20, -80, 1}, {20, -80, 2}, {20, -80, 3}, {20, -80, 4}, {20, -80, 5}, {20, -80, 6}, {20, -80, 7}, {20, -80, 8}, {20, -80, 9}, {20, -80, 10}, {20, -80, 11}, {20, -80, 12}, {20, -80, 13}, {20, -80, 14}, {20, -80, 15}, {20, -80, 16}, {20, -80, 17}, {20, -80, 18}, {20, -80, 19}, {20, -80, 20}, {20, -80, 21}, {20, -80, 22}, {20, -80, 23}, {20, -80, 24}},
//            {{21, -80, 0}, {21, -80, 1}, {21, -80, 2}, {21, -80, 3}, {21, -80, 4}, {21, -80, 5}, {21, -80, 6}, {21, -80, 7}, {21, -80, 8}, {21, -80, 9}, {21, -80, 10}, {21, -80, 11}, {21, -80, 12}, {21, -80, 13}, {21, -80, 14}, {21, -80, 15}, {21, -80, 16}, {21, -80, 17}, {21, -80, 18}, {21, -80, 19}, {21, -80, 20}, {21, -80, 21}, {21, -80, 22}, {21, -80, 23}, {21, -80, 24}},
//            {{22, -80, 0}, {22, -80, 1}, {22, -80, 2}, {22, -80, 3}, {22, -80, 4}, {22, -80, 5}, {22, -80, 6}, {22, -80, 7}, {22, -80, 8}, {22, -80, 9}, {22, -80, 10}, {22, -80, 11}, {22, -80, 12}, {22, -80, 13}, {22, -80, 14}, {22, -80, 15}, {22, -80, 16}, {22, -80, 17}, {22, -80, 18}, {22, -80, 19}, {22, -80, 20}, {22, -80, 21}, {22, -80, 22}, {22, -80, 23}, {22, -80, 24}},
//            {{23, -80, 0}, {23, -80, 1}, {23, -80, 2}, {23, -80, 3}, {23, -80, 4}, {23, -80, 5}, {23, -80, 6}, {23, -80, 7}, {23, -80, 8}, {23, -80, 9}, {23, -80, 10}, {23, -80, 11}, {23, -80, 12}, {23, -80, 13}, {23, -80, 14}, {23, -80, 15}, {23, -80, 16}, {23, -80, 17}, {23, -80, 18}, {23, -80, 19}, {23, -80, 20}, {23, -80, 21}, {23, -80, 22}, {23, -80, 23}, {23, -80, 24}},
//            {{24, -80, 0}, {24, -80, 1}, {24, -80, 2}, {24, -80, 3}, {24, -80, 4}, {24, -80, 5}, {24, -80, 6}, {24, -80, 7}, {24, -80, 8}, {24, -80, 9}, {24, -80, 10}, {24, -80, 11}, {24, -80, 12}, {24, -80, 13}, {24, -80, 14}, {24, -80, 15}, {24, -80, 16}, {24, -80, 17}, {24, -80, 18}, {24, -80, 19}, {24, -80, 20}, {24, -80, 21}, {24, -80, 22}, {24, -80, 23}, {24, -80, 24}}
//    };
//    BezierPatch bezier = BezierPatch(controlPoints);
//public:
//    /*!
//     * Create a new player
//     */
//    Sand();
//
//    /*!
//     * Update player position considering keyboard inputs
//     * @param scene Scene to update
//     * @param dt Time delta
//     * @return true to delete the object
//     */
//    bool update(Scene &scene, float dt) override;
//
//    /*!
//     * Render player
//     * @param scene Scene to render in
//     */
//    void render(Scene &scene) override;
//
//
//    /*!
//     * Ocean click event
//     * @param scene
//     */
//    void onClick(Scene &scene) override;
//
//    glm::vec3 globalPosition = {0, 0, 0};
//};
















//
// Created by peter on 25. 11. 2022.
//

#ifndef PPGSO_SAND_H
#define PPGSO_SAND_H
#include <ppgso/ppgso.h>

#include "object.h"

/*!
 * Simple object representing the player
 * Reads keyboard status and manipulates its own position
 * On Update checks collisions with Boat objects in the scene
 */
class Sand final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;
    float speed = 0;

public:
    /*!
     * Create a new player
     */
    Sand();

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
    void onClick(Scene &scene) override ;
};


#endif //PPGSO_SAND_H
