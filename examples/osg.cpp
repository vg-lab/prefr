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

#include <shaderPath.h>

#include <prefr/prefr.h>

#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/Viewer>
#include <osgGA/TrackballManipulator>
#include <osg/ShapeDrawable>
#include <osg/PolygonMode>

#include <osgDB/ReadFile>
#include <osgDB/FileUtils>


int main( int argc, char** argv )
{
  prefr::OSGManager* manager;
  prefr::ParticleSystem* particleSystem;

  prefr::Cluster* cluster;
  prefr::Model* model;
  prefr::PointSource* source;

  prefr::Updater* updater;
  prefr::Sorter* sorter;
  prefr::Renderer* renderer;

  unsigned int maxParticles = 10;
  unsigned int maxClusters = 1;

  std::string vertPath;
  std::string fragPath;

  if( argc >= 2 )
    maxParticles = atoi( argv[ 1 ] );

  if( argc >= 3 )
    maxClusters = atoi( argv[ 2 ] );

  particleSystem = new prefr::ParticleSystem( maxParticles );

  // Configure particle system

  // Create model and define particles behavior
  model = new prefr::Model( 3.0f, 5.0f );
  model->color.Insert( 0.0f, (glm::vec4(0, 0, 1, 0.2)));
  model->color.Insert( 0.65f, (glm::vec4(0, 1, 0, 0.2)));
  model->color.Insert( 1.0f, (glm::vec4(0, 0.5, 0.5, 0)));

  model->velocity.Insert( 0.0f, 3.0f );
  model->velocity.Insert( 1.0f, 5.0f );

  model->size.Insert( 0.0f, 1.0f );
  std::cout << "Created model." << std::endl;

  particleSystem->AddModel( model );

  updater = new prefr::Updater( );
  std::cout << "Created updater." << std::endl;

  sorter = new prefr::Sorter( );
  std::cout << "Created sorter." << std::endl;

  renderer = new prefr::OSGRenderer( );
  std::cout << "Created OSG renderer." << std::endl;

  particleSystem->AddUpdater(updater);
  particleSystem->sorter(sorter);
  particleSystem->renderer(renderer);


  unsigned int particlesPerCluster = maxParticles / maxClusters;

  std::cout << "Creating " << maxClusters
            << " clusters with " << particlesPerCluster << std::endl;

  for ( unsigned int i = 0; i < maxClusters; i++ )
  {
    source = new prefr::PointSource( 1.f, glm::vec3( i * 10, 0, 0 ));
    particleSystem->AddSource(source);

    cluster = new prefr::Cluster( );
    cluster->source( source );
    cluster->updater( updater );
    cluster->model( model );

    particleSystem->AddCluster( cluster,
                                i * particlesPerCluster,
                                particlesPerCluster );
  }



  // Configure OSG viewer and camera
  osg::ref_ptr< osgViewer::Viewer > viewer = new osgViewer::Viewer;
  viewer->setUpViewInWindow( 100, 100, 800, 600 );

  // Get window and set name
  osgViewer::ViewerBase::Windows windows;
  viewer->getWindows(windows);
  windows[ 0 ]->setWindowName( "OpenSceneGraph Instancing Example" );

  // Get context to determine max number of uniforms in vertex shader
  osgViewer::ViewerBase::Contexts contexts;
  viewer->getContexts( contexts );

  if( !viewer->getCameraManipulator( ))
    viewer->setCameraManipulator( new osgGA::TrackballManipulator, true );

  viewer->addEventHandler( new osgViewer::StatsHandler );

  osg::State* cameraState =
      viewer->getCamera( )->getGraphicsContext( )->getState( );

  cameraState->setUseModelViewAndProjectionUniforms( true );
//  cameraState->setUseVertexAttributeAliasing(true);

  // Configure OSG Manager
  manager = new prefr::OSGManager( *particleSystem );

  // Set camera manipulator for automatically consider OSG camera
  manager->setCameraManipulator( viewer );

  // Configure scene
  osg::Group* groupNode = new osg::Group;
  std::cout << manager->node( ) << std::endl;
  groupNode->addChild( manager->node( ));
  viewer->setSceneData( groupNode );

  // Adjust camera to content
  viewer->getCameraManipulator( )->setAutoComputeHomePosition( true );
  viewer->getCameraManipulator( )->home( 0.0 );

  // Load shaders
  fragPath = vertPath = std::string( PREFR_LIBRARY_BASE_PATH );
  vertPath.append( "OSG/shd/osg-vert.glsl" );
  fragPath.append( "OSG/shd/osg-frag.glsl" );
  manager->configureProgram( vertPath, fragPath );

  particleSystem->Start( );

  return viewer->run();

}
