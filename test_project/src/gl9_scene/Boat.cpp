#include <glm/gtc/random.hpp>
#include "Boat.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>


// Static resources
std::unique_ptr<ppgso::Mesh> Boat::mesh;
std::unique_ptr<ppgso::Texture> Boat::texture;
std::unique_ptr<ppgso::Shader> Boat::shader;

Boat::Boat() {
    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("boat_diffuse.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("boat.obj");
}
/*
GLfloat controlPoints[4][3] = {
        {0.0,1,1},{10.0,1,1},{10.0,1,1},{10.0,1,1}
};
*/
std::vector<glm::vec3> controlPoints = {
        {0, 0, -10},
        {10, 0, -10},
        {10, 0, 0},
        {0, 0, 0},
        {-10, 0, 0},
        {-10, 0, 10},
        {0, 0, 10}
};
std::vector<glm::vec3> points;
glm::vec3 position;
// These numbers are used to pass buffer data to OpenGL
GLuint vao = 0, vbo = 0;

glm::vec2
bezierPoint(const glm::vec2 &p0, const glm::vec2 &p1, const glm::vec2 &p2, const glm::vec2 &p3, const float t) {
    // TODO: Compute point on the Bezier curve
    std::vector<glm::vec2> points;
    points.emplace_back(p0);
    points.emplace_back(p1);
    points.emplace_back(p2);
    points.emplace_back(p3);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3 - i; j++) {
            points[j] = (1 - t) * points[j] + t * points[j + 1];
        }
    }
    return points[0];
}

void bezierShape(int count) {
    //controlPoints = normalize(controlPoints);
    for (int i = 1; i < (int) controlPoints.size(); i += 3) {
        for (int j = 0; j <= count; j++) {
            // TODO: Generate points for each Bezier curve and insert them
            glm::vec2 point = bezierPoint(controlPoints[i - 1], controlPoints[i], controlPoints[i + 1],
                                          controlPoints[i + 2], (float) j / (float) count);
            points.emplace_back(point, 0);
        }
    }
}


bool Boat::update(Scene &scene, float dt) {
    bezierShape(15);
    // slow speed
    speed = 1.0f;
    //position.x += speed * dt;
    //speed += 0.01f;
    rotation.y = -ppgso::PI/4;

    // iterate throught points
    for (int i = 0; i < (int) controlPoints.size()-1; i++) {
        auto currentPoint = points[i];
        auto nextPoint = points[i+1];
        auto pos = currentPoint;
        float t = distance(pos, nextPoint) / distance(currentPoint, nextPoint) + dt * speed;
        auto newPos = (currentPoint + (nextPoint - currentPoint) * t);
        std::cout << t << std::endl;
        if (t > 1) {
            currentPoint = nextPoint;
            nextPoint = points[points.size() - 1];
            t = 0;
        }
    }
    position.x += ((1 - dt)*(1 - dt)*(1 - dt)*controlPoints[0][0]
             + (3 * dt*(1 - dt)*(1 - dt))* controlPoints[1][0]
             + (3 * dt*dt*(1 - dt))* controlPoints[2][0]
             + dt*dt*dt*controlPoints[3][0])
            /100;

    position.z += ((1 - dt)*(1 - dt)*(1 - dt)*controlPoints[0][1]
             + (3 * dt*(1 - dt)*(1 - dt))* controlPoints[1][1]
             + (3 * dt*dt*(1 - dt))* controlPoints[2][1]
             + dt*dt*dt*controlPoints[3][1])
            /100;

    if (position.x > -15) {
        position.x -= ((1 - dt)*(1 - dt)*(1 - dt)*controlPoints[0][0]
                       + (3 * dt*(1 - dt)*(1 - dt))* controlPoints[1][0]
                       + (3 * dt*dt*(1 - dt))* controlPoints[2][0]
                       + dt*dt*dt*controlPoints[3][0])
                      /100;
    }
    //position.x += speed * dt;
    //dt +=  (float) glfwGetTime();
    // make boat follow bezierShape
    // Move the boat
    //position.x += 0.5f * sin(dt);
    //position.y += 0.5f * cos(dt);
    // Generate modelMatrix from position, rotation and scale
    generateModelMatrix();

    return true;
}

void Boat::render(Scene &scene) {
    shader->use();
    // Generate a vertex array object
    // This keeps track of what attributes are associated with buffers
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Generate a vertex buffer object, this will feed data to the vertex shader
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // TODO: Pass the control points to the GPU
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), points.data(), GL_STATIC_DRAW);

    // Setup vertex array lookup, this tells the shader how to pick data for the "Position" input
    auto position_attrib = shader->getAttribLocation("Position");
    glVertexAttribPointer(position_attrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(position_attrib);

    glClearColor(1, 1, 1, 1);
    // Draw shape
    glBindVertexArray(vao);

    // TODO: Define the correct render mode
    glDrawArrays(GL_LINE_STRIP, 0, (GLsizei) points.size());


    // Set up light
    shader->setUniform("LightDirection", scene.lightDirection);

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    mesh->render();

}

void Boat::onClick(Scene &scene) {

    std::cout << "Boat clicked!" << std::endl;
    //explode(scene, position, {10.0f, 10.0f, 10.0f}, 0 );
}

