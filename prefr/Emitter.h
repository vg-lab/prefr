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
#ifndef __PREFR__EMITTER__
#define __PREFR__EMITTER__

#include <prefr/api.h>
#include "InterpolationSet.hpp"
#include "Model.h"
#include "Particles.h"
#include "Source.h"


namespace prefr
{

  class Emitter
  {

  public:

    PREFR_API Emitter( float _emissionRate, bool _loop);

    PREFR_API virtual ~Emitter();

//    PREFR_API virtual void StartEmission(float deltaTime);
//    PREFR_API virtual void EndEmission();
//    PREFR_API virtual int EmitSingle( const Cluster& cluster,
//                                      const tparticle_ptr particle );


//    PREFR_API virtual void Emit( const Cluster& cluster,
//                                         const tparticle_ptr current,
//                                         bool override = false );

  protected:

    int maxParticles;
    float particlesBudget;
    float emissionRate;
    bool loop;
    bool active;

  private:

    float normalizationFactor;

  };


}



#endif /* __PREFR__EMITTER__ */
