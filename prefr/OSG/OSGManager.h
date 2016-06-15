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

#ifndef __PREFR__OSG_PREFR_MANAGER__
#define __PREFR__OSG_PREFR_MANAGER__

#include <prefr/api.h>

#include "../ParticleSystem.h"
#include "OSGRenderConfig.h"

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

namespace prefr
{

#define ATRIB_ARRAY_POSITIONS 1

  class OSGNodeCallBack : public osg::NodeCallback
  {
  public:

    PREFR_API
    virtual void operator( )( osg::Node* node, osg::NodeVisitor* nv );

  };

  class OSGManager : public osg::Drawable
  {
  public:

    PREFR_API
    OSGManager( ParticleSystem& ps );
//    PREFR_API OSGManager( const OSGManager& other, const osg::CopyOp& copyop );

    PREFR_API virtual ~OSGManager( );


    // OSG Methods

    PREFR_API
    OSGManager( void );

    PREFR_API
    OSGManager( const OSGManager& other, const osg::CopyOp& copyop );

    META_Object( prefr::OSGManager, OSGManager )

    PREFR_API
    void compileGLObjects( osg::RenderInfo& renderInfo ) const;
    PREFR_API
    virtual void releaseGLObjects( osg::State* state ) const;

    PREFR_API
//    virtual osg::BoundingBox computeBound( ) const;
    virtual osg::BoundingSphere computeBound( ) const;

    PREFR_API
    virtual void accept(osg::PrimitiveFunctor& functor) const;

    PREFR_API
    virtual void drawImplementation( osg::RenderInfo& renderInfo ) const;





    PREFR_API
    virtual void ConfigureProgram( const std::string& shaderPathVert,
                                   const std::string& shaderPathFrag );

    PREFR_API
    virtual void SetCameraManipulator( osgViewer::ViewerBase* _viewer,
                                       unsigned int contextNumber = 0,
                                       unsigned int viewNumber = 0 );

    PREFR_API ParticleSystem* particleSystem( void ) const;

    PREFR_API osg::Geode* node( void ) const;

  protected:

    PREFR_API
    virtual void AcquireGraphicsContext( osg::GraphicsContext* context );

    PREFR_API
    virtual void UpdateUniformVariables(float deltaTime);

    ParticleSystem* _particleSystem;
    OSGRenderConfig* _renderConfig;

    osgGA::StandardManipulator* _cameraManipulator;

    osg::Geode* _geode;
  };

}

#endif

#endif /* __PREFR__OSG_PREFR_MANAGER__ */
