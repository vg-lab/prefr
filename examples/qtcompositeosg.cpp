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
/*
 * prefrExample.cpp
 *
 *  Created on: 02/12/2014
 *      Author: sgalindo
 */

#include <shaderPath.h>

// This include has to be called before any other GL-dependent
#include <prefr/ParticleSystem.h>
#include <prefr/OSG/OSGDefaultParticleSystem.h>
#include "../prefr/Model.h"
#include "../prefr/OSG/OSGRenderer.h"
#include "../prefr/Sorter.h"
#include "../prefr/Source.h"
#include "../prefr/Updater.h"

#if (PREFR_USE_CUDA)
#include <prefr/cuda/ThrustParticleSorter.cuh>
//  #include <prefr/cuda/CUDAParticleSystem.cuh>
//  #include <prefr/cuda/GLCUDAParticleRenderer.cuh>
#else
//  #include <prefr/OSG/OSGDefaultParticleSystem.h>
//  #include <prefr/OSG/OSGDefaultParticleSorter.h>
//  #include <prefr/OSG/OSGDefaultParticleRenderer.h>
#endif

#include <QTimer>
#include <QApplication>
#include <QMainWindow>
#include <QGridLayout>

#include <osgViewer/View>
#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/Viewer>
#include <osgGA/TrackballManipulator>
#include <osg/ShapeDrawable>
#include <osg/PolygonMode>
#include <osg/Material>


#include <osgGA/TrackballManipulator>

#include <osgDB/ReadFile>

#include <osgQt/GraphicsWindowQt>

#include <iostream>


using namespace prefr;

class QtViewerWidget : public QWidget, public osgViewer::CompositeViewer
{
public:
    QtViewerWidget(osgViewer::ViewerBase::ThreadingModel threadingModel=osgViewer::CompositeViewer::SingleThreaded,
                   QWidget * parent_ = 0 ) : QWidget( parent_ )
    {
        setThreadingModel(threadingModel);

        // disable the default setting of viewer.done() by pressing Escape.
        setKeyEventSetsDone(0);

        rootNode = new osg::Group;

        QWidget* widget1 = addViewWidget( createGraphicsWindow(0,0,100,100), rootNode);

        QGridLayout* grid = new QGridLayout;
        grid->addWidget( widget1, 0, 0 );
        setLayout( grid );

        connect( &_timer, SIGNAL(timeout()), this, SLOT(update()) );
        _timer.start( 10 );
    }

    QWidget* addViewWidget( osgQt::GraphicsWindowQt* gw, osg::Node* scene )
    {
        osgViewer::View* view = new osgViewer::View;
        addView( view );

        osg::Camera* camera = view->getCamera();
        camera->setGraphicsContext( gw );

        const osg::GraphicsContext::Traits* traits = gw->getTraits();

        camera->setClearColor( osg::Vec4(0.2, 0.2, 0.6, 1.0) );
        camera->setViewport( new osg::Viewport(0, 0, traits->width, traits->height) );
        camera->setProjectionMatrixAsPerspective(30.0f, static_cast<double>(traits->width)/static_cast<double>(traits->height), 1.0f, 10000.0f );

        view->setSceneData( scene );
        view->addEventHandler( new osgViewer::StatsHandler );
        view->setCameraManipulator( new osgGA::TrackballManipulator );

        return gw->getGLWidget();
    }

    osgQt::GraphicsWindowQt* createGraphicsWindow( int _x, int _y, int _w, int _h, const std::string& name="", bool windowDecoration=false )
    {
        osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
        osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
        traits->windowName = name;
        traits->windowDecoration = windowDecoration;
        traits->x = _x;
        traits->y = _y;
        traits->width = _w;
        traits->height = _h;
        traits->doubleBuffer = true;
        traits->alpha = ds->getMinimumNumAlphaBits();
        traits->stencil = ds->getMinimumNumStencilBits();
        traits->sampleBuffers = ds->getMultiSamples();
        traits->samples = ds->getNumMultiSamples();

        return new osgQt::GraphicsWindowQt(traits.get());
    }

    void paintEvent( QPaintEvent* /*event*/ )
    { frame(); }

    osg::Group* rootNode;

protected:

    QTimer _timer;
};

//class QtMainWin: public QMainWindow
//{
//public:
//  Q_OBJECT
//
//  QtMainWin(QWidget *parent = 0);
//  ~QtMainWin();
//
//  QtViewerWidget osgMainViewer;
//};



int main( int argc, char** argv )
{

  // Linux osg obj importer has a bug with non english lang.
#ifndef _WINDOWS
  setenv("LANG", "C", 1);
#endif


  osgViewer::ViewerBase::ThreadingModel threadingModel = 
    osgViewer::ViewerBase::/*ThreadingModel::*/ThreadPerContext; //SingleThreaded;



  QApplication app(argc, argv);

  QMainWindow w;
  w.setWindowTitle("main window");
  w.show( );

  //QtViewerWidget* viewWidget = new QtViewerWidget(threadingModel);
  QtViewerWidget viewWidget(threadingModel);

  viewWidget.setGeometry( 100, 100, 800, 600 );
  //viewWidget->show();

  //QtViewerWidget* viewWidget2 = new QtViewerWidget(threadingModel, &w);
  QtViewerWidget viewWidget2(threadingModel);
  viewWidget2.setGeometry( 100, 100, 800, 600 );
  //viewWidget2->show();
  //w.setCentralWidget( viewWidget2 );

  QHBoxLayout *layout = new QHBoxLayout;

  layout->addWidget( &viewWidget );
  layout->addWidget( &viewWidget2 );

  w.setCentralWidget(new QWidget);
  w.centralWidget()->setLayout(layout);


  unsigned int maxParticles = 50000;
  unsigned int maxEmitters = 1;

  OSGDefaultParticleSystem* ps;
  ps = new OSGDefaultParticleSystem(10, maxParticles, true);

  ps->SetCameraManipulator(&viewWidget);

  Model* prototype = new Model(3.0f, 5.0f, ParticleCollection(ps->_particles, 0, maxParticles / 2));
  prototype->color.Insert(0.0f, (glm::vec4(0, 0, 1, 0.2)));
  prototype->color.Insert(0.65f, (glm::vec4(0, 1, 0, 0.2)));
  prototype->color.Insert(1.0f, (glm::vec4(0, 0.5, 0.5, 0)));

  prototype->velocity.Insert(0.0f, 3.0f);
  prototype->velocity.Insert(1.0f, 5.0f);

  prototype->size.Insert(0.0f, 1.0f);

  ps->AddModel(prototype);

  prototype = new Model(3.0f, 5.0f, ParticleCollection(ps->_particles, maxParticles / 2, maxParticles));

  prototype->color.Insert(0.0f, (glm::vec4(1, 1, 0, 0.2)));
  prototype->color.Insert(0.75f, (glm::vec4(1, 0, 0, 0.2)));
  prototype->color.Insert(1.0f, (glm::vec4(1, 1, 1, 0)));

  prototype->velocity.Insert(0.0f, 3.0f);
  prototype->velocity.Insert(1.0f, 5.0f);

  prototype->size.Insert(0.0f, 1.0f);

  ps->AddModel(prototype);

  std::cout << "Created prototype." << std::endl;

  PointSource* emissionNode;

  int particlesPerEmitter = maxParticles / maxEmitters;

  std::cout << "Creating " << maxEmitters << " emitters with " << particlesPerEmitter << std::endl;

  for (unsigned int i = 0; i < maxEmitters; i++)
  {
    std::cout << "Creating emission node " << i << " from " << i * particlesPerEmitter << " to " << i * particlesPerEmitter + particlesPerEmitter << std::endl;

    emissionNode =
        new PointSource(ParticleCollection(ps->_particles,
                                                 i * particlesPerEmitter,
                                                 i * particlesPerEmitter + particlesPerEmitter),
                              glm::vec3(i * 10, 0, 0));

    ps->AddSource(emissionNode);
  }

  Emitter* emitter = new Emitter(*ps->_particles, 0.3f, true);
  ps->AddEmitter(emitter);

  std::cout << "Created emitter" << std::endl;
  Updater* updater = new Updater(*ps->_particles);
  std::cout << "Created updater" << std::endl;

  Sorter* sorter;

#if (PREFR_USE_CUDA)
  sorter = new ThrustParticleSorter(*ps->_particles);
#else
  sorter = new Sorter(*ps->_particles);
#endif

  std::cout << "Created sorter" << std::endl;

  OSGRenderer* renderer = new OSGRenderer(*ps->_particles);

  std::cout << "Created systems" << std::endl;



  ps->AddUpdater(updater);
  ps->Sorter(sorter);
  ps->renderer(renderer);

  std::string vertPath, fragPath;
  fragPath = vertPath = std::string(PREFR_LIBRARY_BASE_PATH);
  vertPath.append("OSG/shd/osg-vert.glsl");
  fragPath.append("OSG/shd/osg-frag.glsl");
  ps->ConfigureProgram(vertPath, fragPath);

  ps->Start();

  osg::ShapeDrawable* sd = new osg::ShapeDrawable(new osg::Box(osg::Vec3(0,0,0), 100));
  osg::Geode* sdg = new osg::Geode;
  sdg->addDrawable(sd);
  sd->setColor(osg::Vec4(1,1,1,1));

  osg::StateSet* ss = sdg->getOrCreateStateSet();
  ss->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
  ss->setAttributeAndModes(
    new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK,
                         osg::PolygonMode::LINE));



  osg::Group* groupNode = viewWidget.rootNode; //new osg::Group;


  groupNode->addChild(sdg);

  if (argc >= 4)
   {
     std::string filespath = std::string(argv[3]);
     osgDB::DirectoryContents files = osgDB::getDirectoryContents(filespath);

     osg::Group* meshes = new osg::Group;
     osg::Node* node;

     for (unsigned int i = 2; i < 1000 /*files.size()/2*/; i++)
     {
       node = osgDB::readNodeFile(osgDB::findFileInDirectory(files[i], filespath));

       std::cout << files[i] << std::endl;
       if (!node)
         std::cout << "null node" << std::endl;
       else
         meshes->addChild(node);
     }

     ss = meshes->getOrCreateStateSet();
     //ss->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
     ss->setAttributeAndModes(
       new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK,
                            osg::PolygonMode::FILL));
                            //osg::PolygonMode::LINE));

     groupNode->addChild(meshes);

     osg::ref_ptr<osg::Material> material = new osg::Material;

     material->setDiffuse(osg::Material::FRONT_AND_BACK,
                          osg::Vec4(1,0,0, 0.7));

     ss->setAttribute(material,
                      osg::StateAttribute::OVERRIDE);


   }
  groupNode->addChild(ps->rootNode);


  osgViewer::View* view = viewWidget.getView(0);

//    view->setSceneData(groupNode);
  //viewWidget.rootNode->addChild( groupNode );

    view->getCameraManipulator()->setAutoComputeHomePosition(true);
    view->getCameraManipulator()->home(0.0);

    osg::State* cameraState = view->getCamera()->getGraphicsContext()->getState();
    cameraState->setUseModelViewAndProjectionUniforms(true);



  return app.exec();
}
