//#include "Rain.h"
//#include <glm/gtc/random.hpp>
//#include "Ocean.h"
//#include <shaders/color_vert_glsl.h>
//#include <shaders/color_frag_glsl.h>
//#include <shaders/texture_vert_glsl.h>
//#include <shaders/texture_frag_glsl.h>
//#include <glm/gtx/euler_angles.hpp>
//
//const float GRAVITATIONAL_CONSTANT = 0.000667f;
//// Static resources shared between all particles
//static std::unique_ptr<ppgso::Mesh> mesh;
//static std::unique_ptr<ppgso::Shader> shader;
//
//Rain::Rain() {
//
//    // TODO: add more parameters as needed
//    glm::vec3 speed;
//    glm::vec3 color;
//    glm::vec3 scale;
//
//    float mass;
//
//    Scene *scene;
//
//    float time = 0;
//    float lifetime = 10;
//public:
//    glm::vec3 position;
//    bool isGravityAffected;
//
//    /// Construct a new Particle
//    /// \param p - Initial position
//    /// \param s - Initial speed
//    /// \param c - Color of particle
//    Particle(glm::vec3 p, glm::vec3 s, glm::vec3 c, glm::vec3 scale, Scene *scene, bool isGravityAffected, float mass) {
//        // First particle will initialize resources
//        if (!shader) shader = std::make_unique<ppgso::Shader>(color_vert_glsl, color_frag_glsl);
//        if (!mesh) mesh = std::make_unique<ppgso::Mesh>("sphere.obj");
//
//        position = p;
//        speed = s;
//        color = c;
//        this->scale = scale;
//        this->mass = mass;
//        this->scene = scene;
//        this->isGravityAffected = isGravityAffected;
//    }
//
//    void calculateGravitySpeed() {
////        iterate through other objects and their mass
////        calculate gravity speed
////        add to speed
//
//        auto gravitySpeed = glm::vec3(0.0f, 0.0f, 0.0f);
//
//        for (auto &object: *scene) {
//            auto otherParticle = dynamic_cast<Particle *>(object.get());
//            if (otherParticle == nullptr) continue;
//            if (otherParticle == this) continue;
//
//            auto distance = glm::distance(position, otherParticle->position);
//
////            https://web.fscj.edu/Milczanowski/psc/lect/Ch2/sl13.htm
//            auto force = GRAVITATIONAL_CONSTANT * mass * otherParticle->mass / (distance * distance);
//            auto direction = glm::normalize(otherParticle->position - position);
//            gravitySpeed += direction * force;
//        }
//
//        speed += gravitySpeed;
//    }
//
//    bool update(float dTime, Scene &scene) override {
//        // TODO: Animate position using speed and dTime.
//        // - Return true to keep the object alive
//        // - Returning false removes the object from the scene
//        // - hint: you can add more particles to the scene here also
//
//        if (isGravityAffected) calculateGravitySpeed();
//
//        position += speed * dTime;
//
//        time += dTime;
//
//        if (time > lifetime) {
//            return false;
//        }
//
//        return true;
//    }
//
//    void render(const Camera &camera) override {
//        // TODO: Render the object
//        // - Use the shader
//        // - Setup all needed shader inputs
//        // - hint: use OverallColor in the color_vert_glsl shader for color
//        // - Render the mesh
//        shader->use();
//
//        shader->setUniform("OverallColor", color);
//        shader->setUniform("ProjectionMatrix", camera.projectionMatrix);
//        shader->setUniform("ViewMatrix", camera.viewMatrix);
//        shader->setUniform("ModelMatrix", glm::translate(position) * glm::scale(scale));
//        mesh->render();
//
//    }
//};
