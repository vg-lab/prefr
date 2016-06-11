/*
 * Copyright (c) 2014-2016 GMRV/URJC.
 *
 * Authors: Sergio Galindo <sergio.galindo@urjc.es>
 *
 * This file is part of PReFr <https://gmrv.gitlab.com/nsviz/prefr>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3.0 as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */
#ifndef __PREFR__PARTICLE__
#define __PREFR__PARTICLE__


#include <glm/glm.hpp>

namespace prefr
{

  class Model;

  static glm::vec3 vec3zero (0,0,0);
  static glm::vec3 vec3one (1,1,1);

  class Particle
  {
  public:

    int id;

    // Life properties
    bool alive;
    bool newborn;
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
    : id( _id )
    , alive( _alive )
    , newborn( true )
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
    , alive( true )
    , newborn( true )
    , life(_life)
    , size(_size)
    , velocityModule(0.f)
    , position(_pos)
    , velocity(_vel)
    , color(_color)
    , distance(-1)
    {}

    inline bool Alive(){return this->alive;}
    inline bool Newborn(){return this->newborn;}

  };

}

#endif
