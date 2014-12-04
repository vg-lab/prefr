/*
 * osg.cpp
 *
 *  Created on: 12/11/2014
 *      Author: sgalindo
 */


#include <shaderPath.h>

#include <prefr/ParticleSystem.h>
#include <prefr/ParticlePrototype.h>
#include <prefr/ParticleEmitter.h>
#include <prefr/ParticleUpdater.h>
#include <prefr/OSG/OSGDefaultParticleSystem.h>
#include <prefr/OSG/OSGDefaultParticleRenderer.h>
#include <prefr/ParticleSorter.h>

#if (PREFR_WITH_CUDA)
#include <prefr/cuda/ThrustParticleSorter.cuh>
//  #include <prefr/cuda/CUDAParticleSystem.cuh>
//  #include <prefr/cuda/GLCUDAParticleRenderer.cuh>
#else
//  #include <prefr/OSG/OSGDefaultParticleSystem.h>
//  #include <prefr/OSG/OSGDefaultParticleSorter.h>
//  #include <prefr/OSG/OSGDefaultParticleRenderer.h>
#endif

#include <osgViewer/View>
#include <osgViewer/Viewer>
#include <osgViewer/CompositeViewer>
#include <osgGA/TrackballManipulator>
#include <osg/ShapeDrawable>
#include <osg/PolygonMode>

#include <osgGA/StateSetManipulator>

#include <osgViewer/ViewerEventHandlers>

#include <osgDB/ReadFile>
#include <osgDB/FileUtils>

using namespace prefr;

//#if (particles_WITH_CUDA)
//  CUDAParticleSystem* ps;
//#else
//  OSGDefaultParticleSystem* ps;
//#endif

OSGDefaultParticleSystem* ps;

void initOpenGL(osg::GraphicsContext* context,
                GLint& maxNumUniforms,
                GLint& maxUniformBlockSize)
{

  context->realize();
  context->makeCurrent();
  maxNumUniforms = 0;
  glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &maxNumUniforms);
  maxUniformBlockSize = 0;
  glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &maxUniformBlockSize);

  // init glew
  glewInit();
  context->releaseContext();

}

osgViewer::CompositeViewer* createCompositeViewer()
{
  osgViewer::CompositeViewer* viewer = new osgViewer::CompositeViewer;

  osg::GraphicsContext::WindowingSystemInterface* wsi = osg::GraphicsContext::getWindowingSystemInterface();
  if (!wsi)
  {
      osg::notify(osg::NOTICE)<<"Error, no WindowSystemInterface available, cannot create windows."<<std::endl;
      return nullptr;
  }

  unsigned int width, height;
  wsi->getScreenResolution(osg::GraphicsContext::ScreenIdentifier(0), width, height);

  osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
  traits->x = 100;
  traits->y = 100;
  traits->width = 1000;
  traits->height = 800;
  traits->windowDecoration = true;
  traits->doubleBuffer = true;
  traits->sharedContext = 0;

  osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());
  if (gc.valid())
  {
      osg::notify(osg::INFO)<<"  GraphicsWindow has been created successfully."<<std::endl;

      // need to ensure that the window is cleared make sure that the complete window is set the correct colour
      // rather than just the parts of the window that are under the camera's viewports
      gc->setClearColor(osg::Vec4f(0.2f,0.2f,0.6f,1.0f));
      gc->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }
  else
  {
      osg::notify(osg::NOTICE)<<"  GraphicsWindow has not been created successfully."<<std::endl;
  }

  // view one
  {
      osgViewer::View* view = new osgViewer::View;
      view->setName("View one");
      viewer->addView(view);

      view->getCamera()->setName("Cam one");
      view->getCamera()->setViewport(new osg::Viewport(0,0, traits->width, traits->height));
      view->getCamera()->setGraphicsContext(gc.get());
      view->setCameraManipulator(new osgGA::TrackballManipulator);

      // add the state manipulator
      osg::ref_ptr<osgGA::StateSetManipulator> statesetManipulator = new osgGA::StateSetManipulator;
      statesetManipulator->setStateSet(view->getCamera()->getOrCreateStateSet());

      view->addEventHandler( statesetManipulator.get() );

      view->addEventHandler( new osgViewer::StatsHandler );
//      view->addEventHandler( new osgViewer::HelpHandler );
//      view->addEventHandler( new osgViewer::WindowSizeHandler );
      view->addEventHandler( new osgViewer::ThreadingHandler );
//      view->addEventHandler( new osgViewer::RecordCameraPathHandler );
  }

  return viewer;

}


int main(int argc, char** argv)
{
  osg::ref_ptr<osgViewer::CompositeViewer> viewer = createCompositeViewer();

  // get window and set name
  osgViewer::ViewerBase::Windows windows;
  viewer->getWindows(windows);
  windows[0]->setWindowName("OpenSceneGraph Instancing Example");

  // get context to determine max number of uniforms in vertex shader
  osgViewer::ViewerBase::Contexts contexts;
  viewer->getContexts(contexts);
//  GLint maxNumUniforms = 0;
//  GLint maxUniformBlockSize = 0;
//  initOpenGL(contexts[0], maxNumUniforms, maxUniformBlockSize);


  int maxParticles = 10;
  unsigned int maxEmitters = 1;

  if (argc >= 2)
    maxParticles = atoi(argv[1]);

  if (argc >= 3)
    maxEmitters = atoi(argv[2]);


//#if (particles_WITH_CUDA == 1)
//  ps = new CUDAParticleSystem(10, maxParticles, true);
//#else
//  ps = new OSGDefaultParticleSystem(10, maxParticles, true);

  ps = new OSGDefaultParticleSystem(10, maxParticles, true);

  std::cout << "No se ha encontrado el manipulador de cámara" << std::endl;


  ps->SetCameraManipulator(viewer);



  ParticlePrototype* prototype = new ParticlePrototype(3.0f, 5.0f, ParticleCollection(ps->particles, 0, maxParticles / 2));
  prototype->color.Insert(0.0f, (glm::vec4(0, 0, 1, 0.2)));
  prototype->color.Insert(0.65f, (glm::vec4(0, 1, 0, 0.2)));
  prototype->color.Insert(1.0f, (glm::vec4(0, 0.5, 0.5, 0)));

  prototype->velocity.Insert(0.0f, 3.0f);
  prototype->velocity.Insert(1.0f, 5.0f);

  prototype->size.Insert(0.0f, 1.0f);

  ps->AddPrototype(prototype);

  prototype = new ParticlePrototype(3.0f, 5.0f, ParticleCollection(ps->particles, maxParticles / 2, maxParticles));

  prototype->color.Insert(0.0f, (glm::vec4(1, 1, 0, 0.2)));
  prototype->color.Insert(0.75f, (glm::vec4(1, 0, 0, 0.2)));
  prototype->color.Insert(1.0f, (glm::vec4(1, 1, 1, 0)));

  prototype->velocity.Insert(0.0f, 3.0f);
  prototype->velocity.Insert(1.0f, 5.0f);

  prototype->size.Insert(0.0f, 1.0f);

  ps->AddPrototype(prototype);

  std::cout << "Created prototype." << std::endl;


  PointEmissionNode* emissionNode;

  int particlesPerEmitter = maxParticles / maxEmitters;

  std::cout << "Creating " << maxEmitters << " emitters with " << particlesPerEmitter << std::endl;

  for (unsigned int i = 0; i < maxEmitters; i++)
  {
    std::cout << "Creating emission node " << i << " from " << i * particlesPerEmitter << " to " << i * particlesPerEmitter + particlesPerEmitter << std::endl;

    emissionNode =
        new PointEmissionNode(ParticleCollection(ps->particles,
                                                 i * particlesPerEmitter,
                                                 i * particlesPerEmitter + particlesPerEmitter),
                              glm::vec3(i * 10, 0, 0));

    ps->AddEmissionNode(emissionNode);
  }

  ParticleEmitter* emitter = new ParticleEmitter(*ps->particles, 0.3f, true);
  ps->AddEmitter(emitter);
  emitter->UpdateConfiguration();

  std::cout << "Created emitter" << std::endl;
  ParticleUpdater* updater = new ParticleUpdater(*ps->particles);
  std::cout << "Created updater" << std::endl;

  ParticleSorter* sorter;

#if (particles_WITH_CUDA)
  sorter = new ThrustParticleSorter(*ps->particles);
#else
  sorter = new ParticleSorter(*ps->particles);
#endif

  std::cout << "Created sorter" << std::endl;

  OSGDefaultParticleRenderer* renderer = new OSGDefaultParticleRenderer(*ps->particles);

  std::cout << "Created systems" << std::endl;

  ps->AddUpdater(updater);
  ps->SetSorter(sorter);
  ps->SetRenderer(renderer);

  std::string vertPath, fragPath;
  fragPath = vertPath = std::string(PREFR_LIBRARY_BASE_PATH);
  vertPath.append("OSG/shd/osg-vert.glsl");
  fragPath.append("OSG/shd/osg-frag.glsl");
  ps->ConfigureProgram(vertPath, fragPath);

  ps->Start();

  osg::ShapeDrawable* sd = new osg::ShapeDrawable(new osg::Box(osg::Vec3(0,0,0), 1));
  osg::Geode* sdg = new osg::Geode;
  sdg->addDrawable(sd);
  sd->setColor(osg::Vec4(1,1,1,1));

  osg::StateSet* ss = sdg->getOrCreateStateSet();
  ss->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
  ss->setAttributeAndModes(
    new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK,
                         osg::PolygonMode::LINE));



  osg::Group* groupNode = new osg::Group;

  groupNode->addChild(ps->rootNode);
  groupNode->addChild(sdg);

  if (argc >= 4)
  {
    std::string filespath = std::string(argv[3]);
    osgDB::DirectoryContents files = osgDB::getDirectoryContents(filespath);

    osg::Group* meshes = new osg::Group;
    osg::Node* node;

    for (unsigned int i = 2; i < 10 /*files.size()/2*/; i++)
    {
      node = osgDB::readNodeFile(osgDB::findFileInDirectory(files[i], filespath));

      std::cout << files[i] << std::endl;
      if (!node)
        std::cout << "null node" << std::endl;
      else
        meshes->addChild(node);
    }

    ss = meshes->getOrCreateStateSet();
    ss->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    ss->setAttributeAndModes(
      new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK,
                           osg::PolygonMode::LINE));

    groupNode->addChild(meshes);
  }

    std::cout << "Finished loading." << std::endl;

//  osg::Geode* geode = new osg::Geode;
//  geode->addDrawable(ps);

#define PRINT_VEC3F( VEC )                                              \
  std::cout << "(" <<  VEC.x() << "," << VEC.y() << "," << VEC.z() << ")" << std::endl;

  osgViewer::View* view = viewer->getView(0);

  view->setSceneData(groupNode);

  view->getCameraManipulator()->setAutoComputeHomePosition(true);
  view->getCameraManipulator()->home(0.0);

//  viewer->setThreadingModel( osgViewer::Viewer::ThreadPerContext );

//  osg::State* cameraState = view->getCamera()->getGraphicsContext()->getState();
//  cameraState->setUseModelViewAndProjectionUniforms(true);
//  cameraState->setUseVertexAttributeAliasing(true);

  return viewer->run();

}

