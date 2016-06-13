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
#ifndef __PREFR__MODEL__
#define __PREFR__MODEL__

#include <prefr/api.h>
#include "Particles.h"
#include "InterpolationSet.hpp"


namespace prefr
{

  typedef ::utils::InterpolationSet<float> vectortfloat;
  typedef ::utils::InterpolationSet<glm::vec3> vectortvec3;
  typedef ::utils::InterpolationSet<glm::vec4> vectortvec4;

  class Model
  {
    friend class Updater;

  public:

    PREFR_API Model(void);
    PREFR_API Model(float min, float max );

    PREFR_API virtual ~Model();

    PREFR_API void SetLife(float min, float max);

protected:

    float minLife;
    float maxLife;

    float lifeInterval;
    float lifeNormalization;
    float dispersion;

    vectortfloat size;

    glm::vec3 positionOffset;

    vectortfloat velocity;
    vectortvec4 color;

  };

  typedef Model tprototype;
  typedef tprototype* tprototype_ptr;
  typedef std::vector<tprototype_ptr> ModelsArray;


}

#endif /* __PREFR__MODEL__ */
