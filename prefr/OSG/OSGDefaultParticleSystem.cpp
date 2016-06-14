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
#include "OSGDefaultParticleSystem.h"

#include "../Log.h"

#ifdef PREFR_USE_OPENSCENEGRAPH

namespace prefr
{



  OSGDefaultParticleSystem::OSGDefaultParticleSystem()
  : ParticleSystem( 0 )
  , osg::Drawable()
  , cameraManipulator( nullptr )
  , rootNode( nullptr )
  , blendFunctionSrc( osg::BlendFunc::SRC_ALPHA )
  , blendFunctionDst( osg::BlendFunc::ONE_MINUS_CONSTANT_ALPHA )
  {
    setUseDisplayList(false);
    setUseVertexBufferObjects(true);
  }

  OSGDefaultParticleSystem::OSGDefaultParticleSystem(
const OSGDefaultParticleSystem& other,
const osg::CopyOp& copyOp)
  : ParticleSystem( other.maxParticles )
  , osg::Drawable(other, copyOp)
  , cameraManipulator( nullptr )
  , rootNode( nullptr )
  , blendFunctionSrc( osg::BlendFunc::SRC_ALPHA )
  , blendFunctionDst( osg::BlendFunc::ONE_MINUS_CONSTANT_ALPHA )
  {
    setUseDisplayList(false);
    setUseVertexBufferObjects(true);
  }

  OSGDefaultParticleSystem::OSGDefaultParticleSystem(
    unsigned int initialParticlesNumber,
    unsigned int _maxParticles, bool  _loop,
    osg::BlendFunc::BlendFuncMode blendFuncSrc,
    osg::BlendFunc::BlendFuncMode blendFuncDst)
  : ParticleSystem( _maxParticles )
  , osg::Drawable()
  , cameraManipulator( nullptr )
  , rootNode( nullptr )
  , blendFunctionSrc( blendFuncSrc )
  , blendFunctionDst( blendFuncDst )
  {

    rootNode = new osg::Geode;
    rootNode->addDrawable(this);

    rootNode->setCullCallback(new OSGPSNodeCallBack);

     setUseDisplayList(false);
     setUseVertexBufferObjects(true);

  }

  OSGDefaultParticleSystem::~OSGDefaultParticleSystem()
  {}



  void OSGDefaultParticleSystem::Update(float deltaTime)
  {
    ParticleSystem::Update(deltaTime);

    UpdateUniformVariables(deltaTime);

    UpdateRender();
  }

  void OSGDefaultParticleSystem::UpdateUnified(float deltaTime)
  {
    ParticleSystem::Update(deltaTime);

    UpdateUniformVariables(deltaTime);

    UpdateRender();
  }



  void OSGDefaultParticleSystem::UpdateRender()
  {
    this->_sorter->Sort();

    PREFR_DEBUG_CHECK( static_cast<OSGRenderer*>(this->_renderer),
                       "casting failed" );

    static_cast<OSGRenderer*>(this->_renderer)->SetupRender(
      this->_aliveParticles);

    dirtyBound();
  }

  void OSGDefaultParticleSystem::Render() const
  {
    PREFR_DEBUG_CHECK( static_cast<OSGRenderer*>(this->_renderer),
                       "casting failed" );

    static_cast<OSGRenderer*>(this->_renderer)->Paint(_aliveParticles);
  }

  // OSG



  void OSGDefaultParticleSystem::SetAlphaBlendingFunction(
    osg::BlendFunc::BlendFuncMode src, osg::BlendFunc::BlendFuncMode dst)
  {
    blendFunctionSrc = src;
    blendFunctionDst = dst;
    osg::StateSet* psState = rootNode->getOrCreateStateSet();

    osg::BlendFunc* bf = new osg::BlendFunc();
    bf->setSource(blendFunctionSrc);
    bf->setDestination(blendFunctionDst);

    psState->setAttributeAndModes(bf, osg::StateAttribute::ON);

  }

}
#endif
