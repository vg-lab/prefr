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
#ifndef __PREFR__OSG_DEFAULT_PARTICLE_SYSTEM__
#define __PREFR__OSG_DEFAULT_PARTICLE_SYSTEM__

#include <prefr/api.h>
#include "../ParticleSystem.h"

#ifdef PREFR_USE_OPENSCENEGRAPH

#include <osg/Array>
#include <osg/Geometry>

#include <osg/NodeVisitor>

#include <osgGA/StandardManipulator>
#include <osgGA/TrackballManipulator>

#include <osgDB/FileUtils>

#include <osgViewer/View>
#include <osgViewer/Viewer>
#include <osgViewer/CompositeViewer>
#include <osg/State>
#include <osg/GL>
#include <osg/BlendFunc>

#include "OSGRenderer.h"

namespace prefr
{

  #define ATRIB_ARRAY_POSITIONS 1

  class OSGPSNodeCallBack : public osg::NodeCallback
  {
  public:

    PREFR_API virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);

  };

  class OSGDefaultParticleSystem  : public ParticleSystem
                                  , public osg::Drawable
  {
  public:

    osgGA::StandardManipulator* cameraManipulator;

    osg::Geode* rootNode;

    osg::BlendFunc::BlendFuncMode blendFunctionSrc, blendFunctionDst;

    PREFR_API OSGDefaultParticleSystem();
    PREFR_API ~OSGDefaultParticleSystem();

    PREFR_API OSGDefaultParticleSystem(const OSGDefaultParticleSystem& other,
				       const osg::CopyOp& copyOp);

    PREFR_API OSGDefaultParticleSystem(
      unsigned int initialParticlesNumber,
      unsigned int _maxParticles, bool _loop = true,
      osg::BlendFunc::BlendFuncMode blendFuncSrc =
      osg::BlendFunc::/*BlendFuncMode::*/SRC_ALPHA,
      osg::BlendFunc::BlendFuncMode blendFuncDst =
      osg::BlendFunc::/*BlendFuncMode::*/ONE_MINUS_CONSTANT_ALPHA);

    META_Object(particles::defaultParticleSystem::OSGParticleSystem,
                OSGDefaultParticleSystem)


    PREFR_API virtual void ConfigureProgram(const std::string& shaderPathVert,
                                            const std::string& shaderPathFrag);

    PREFR_API virtual void Update(float deltaTime);
    PREFR_API virtual void UpdateUnified(float deltaTime);

    PREFR_API virtual void UpdateRender();
    PREFR_API virtual void Render() const;

    PREFR_API virtual void SetAlphaBlendingFunction(
      osg::BlendFunc::BlendFuncMode src, osg::BlendFunc::BlendFuncMode dst);

  protected:

    PREFR_API virtual void UpdateUniformVariables(float deltaTime);


  };

}
#endif

#endif /* __PREFR__OSG_DEFAULT_PARTICLE_SYSTEM__ */
