#ifndef PARTICLE_H_
#define PARTICLE_H_

#include <glm/glm.hpp>

using namespace glm;

namespace particles
{
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
    vec3 position, oldPosition, velocity;

    // Color
    vec4 color;

    // Distance to camera
    float distance;

    Particle(int _id)
      : id(_id), alive(true), life(0), size(1), velocityModule(0.f)
      , position(vec3zero), velocity(vec3zero), color(vec3one),  distance(-1) {}

    Particle (int _id, vec3 _pos, vec3 _vel, vec3 _dir, vec4 _color, float _life, float _size)
      : id(_id), alive(true), life(_life), size(_size), velocityModule(0.f), position(_pos)
      , velocity(_vel), color(_color),  distance(-1) {}

  };

}

#endif
