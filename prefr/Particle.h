#ifndef PARTICLE_H_
#define PARTICLE_H_


#include <glm/glm.hpp>

namespace prefr
{

  class ParticlePrototype;

  static glm::vec3 vec3zero (0,0,0);
  static glm::vec3 vec3one (1,1,1);

  class Particle
  {
  public:

    int id;

    // Life properties
    bool alive;
    float life;
    float size;

    float velocityModule;

    // Position, velocity...
    glm::vec3 position, oldPosition, velocity;

    // Color
    glm::vec4 color;

    // Distance to camera
    float distance;

    Particle(int _id, bool _alive = true)
    : id(_id)
    , alive(_alive)
    , life(0)
    , size(1)
    , velocityModule(0.f)
    , position(vec3zero)
    , velocity(vec3zero)
    , color(glm::vec4(vec3one,1))
    , distance(-1)
    {}

    Particle (int _id, glm::vec3 _pos, glm::vec3 _vel, glm::vec3 /* _dir */,
              glm::vec4 _color, float _life, float _size)
    : id(_id)
    , alive(true)
    , life(_life)
    , size(_size)
    , velocityModule(0.f)
    , position(_pos)
    , velocity(_vel)
    , color(_color)
    , distance(-1)
    {}

    bool Alive(){return this->alive;}

  };

}

#endif
