/*
 * osg.cpp
 *
 *  Created on: 12/11/2014
 *      Author: sgalindo
 */

#include <particles/config.h>

#include <particles/ParticleSystem.h>

#include <particles/ParticlePrototype.h>

#include <particles/ParticleEmitter.h>
#include <particles/ParticleUpdater.h>

#include <particles/OSG/OSGDefaultParticleSystem.h>
#include <particles/OSG/OSGDefaultParticleRenderer.h>

#include <particles/ParticleSorter.h>

#if (particles_WITH_CUDA)
  #include <particles/cuda/ThrustParticleSorter.cuh>
//  #include <particles/cuda/CUDAParticleSystem.cuh>
//  #include <particles/cuda/GLCUDAParticleRenderer.cuh>
#else
//  #include <particles/OSG/OSGDefaultParticleSystem.h>
//  #include <particles/OSG/OSGDefaultParticleSorter.h>
//  #include <particles/OSG/OSGDefaultParticleRenderer.h>
#endif

#include <osgViewer/Viewer>
#include <osgGA/TrackballManipulator>
#include <osg/ShapeDrawable>
#include <osg/PolygonMode>

using namespace particles;

//using namespace particles::defaultParticleSystem::OSGParticleSystem;

#if (particles_WITH_CUDA)
  using namespace particles::defaultParticleSystem::CUDATHRUST;
#else

#endif

//#if (particles_WITH_CUDA)
//  CUDAParticleSystem* ps;
//#else
//  OSGDefaultParticleSystem* ps;
//#endif

  OSGDefaultParticleSystem* ps;

void initOpenGL(osg::GraphicsContext* context, GLint& maxNumUniforms, GLint& maxUniformBlockSize)
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

int main(int argc, char** argv)
{
  osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;

  viewer->setUpViewInWindow(100, 100, 800, 600);

  // get window and set name
  osgViewer::ViewerBase::Windows windows;
  viewer->getWindows(windows);
  windows[0]->setWindowName("OpenSceneGraph Instancing Example");

  // get context to determine max number of uniforms in vertex shader
  osgViewer::ViewerBase::Contexts contexts;
  viewer->getContexts(contexts);
  GLint maxNumUniforms = 0;
  GLint maxUniformBlockSize = 0;
  initOpenGL(contexts[0], maxNumUniforms, maxUniformBlockSize);


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

  if (!viewer->getCameraManipulator())
    viewer->setCameraManipulator(new osgGA::TrackballManipulator, true);

    std::cout << "No se ha encontrado el manipulador de cámara" << std::endl;


  ps->SetCameraManipulator((osgGA::StandardManipulator*)viewer->getCameraManipulator());



  ParticleCollection* colProto = new ParticleCollection(ps->particles, 0, maxParticles / 2);

  ParticlePrototype* prototype = new ParticlePrototype(3.0f, 5.0f);
//  prototype->minLife = 3.0f;
//  prototype->maxLife = 5.0f;
  prototype->color.Insert(0.0f, /*particles::RGBToHSV*/(glm::vec4(0, 0, 1, 0.2)));
//  prototype->color.Insert(0.4f, particles::RGBToHSV(glm::vec4(0, 127, 127, 0)));
  prototype->color.Insert(0.65f, /*particles::RGBToHSV*/(glm::vec4(0, 1, 0, 0.2)));
  prototype->color.Insert(1.0f, /*particles::RGBToHSV*/(glm::vec4(0, 0.5, 0.5, 0)));

  prototype->velocity.Insert(0.0f, 3.0f);
  prototype->velocity.Insert(1.0f, 5.0f);

  prototype->size.Insert(0.0f, 1.0f);

  prototype->particles = colProto;

  ps->AddPrototype(prototype);

  colProto = new ParticleCollection(ps->particles, maxParticles / 2, maxParticles);

  prototype = new ParticlePrototype(3.0f, 5.0f);

  prototype->color.Insert(0.0f, /*particles::RGBToHSV*/(glm::vec4(1, 1, 0, 0.2)));
//  prototype->color.Insert(0.4f, particles::RGBToHSV(glm::vec4(0, 127, 127, 0)));
  prototype->color.Insert(0.75f, /*particles::RGBToHSV*/(glm::vec4(1, 0, 0, 0.2)));
  prototype->color.Insert(1.0f, /*particles::RGBToHSV*/(glm::vec4(1, 1, 1, 0)));

  prototype->velocity.Insert(0.0f, 3.0f);
  prototype->velocity.Insert(1.0f, 5.0f);

  prototype->size.Insert(0.0f, 1.0f);

  prototype->particles = colProto;

  ps->AddPrototype(prototype);

  std::cout << "Created prototype." << std::endl;

  ParticleCollection* colEmissionNode;

  ParticleCollection* colEmitter = new ParticleCollection(ps->particles, 0, maxParticles);
  ParticleCollection* colUpdater = new ParticleCollection(ps->particles, 0, maxParticles);
  ParticleCollection* colSorter = new ParticleCollection(ps->particles, 0, maxParticles);
  ParticleCollection* colRenderer = new ParticleCollection(ps->particles, 0, maxParticles);

  std::cout << "Created collections" << std::endl;


  PointEmissionNode* emissionNode;

  int particlesPerEmitter = maxParticles / maxEmitters;

  std::cout << "Creating " << maxEmitters << " emitters with " << particlesPerEmitter << std::endl;

  for (unsigned int i = 0; i < maxEmitters; i++)
  {
    colEmissionNode = new ParticleCollection(ps->particles, i * particlesPerEmitter, i * particlesPerEmitter + particlesPerEmitter);
    std::cout << "Creating emission node " << i << " from " << i * particlesPerEmitter << " to " << i * particlesPerEmitter + particlesPerEmitter << std::endl;

    emissionNode = new PointEmissionNode(colEmissionNode, glm::vec3(i * 10, 0, 0));
    ps->AddEmissionNode(emissionNode);
  }

  ParticleEmitter* emitter = new ParticleEmitter(colEmitter, 0.3f, true);
  ps->AddEmitter(emitter);
  emitter->UpdateConfiguration();

  std::cout << "Created emitter" << std::endl;
  ParticleUpdater* updater = new ParticleUpdater(colUpdater);
  std::cout << "Created updater" << std::endl;

  ParticleSorter* sorter;

#if (particles_WITH_CUDA)
  sorter = new ThrustParticleSorter(colSorter);
#else
  sorter = new ParticleSorter(colSorter);
#endif


  std::cout << "Created sorter" << std::endl;

//#if (particles_WITH_CUDA)
//  GLCUDAParticleRenderer* renderer = new GLCUDAParticleRenderer(colRenderer, ps->distances, ps->renderConfig);
//#else
//  OSGDefaultParticleRenderer* renderer = new OSGDefaultParticleRenderer(colRenderer, ps->distances, ps->renderConfig);
//#endif

  OSGDefaultParticleRenderer* renderer = new OSGDefaultParticleRenderer(colRenderer);

  std::cout << "Created systems" << std::endl;



  ps->AddUpdater(updater);
  ps->SetSorter(sorter);
  ps->SetRenderer(renderer);

  std::string vertPath, fragPath;
  fragPath = vertPath = std::string(particles_LIBRARY_BASE_PATH);
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

//  osg::Geode* geode = new osg::Geode;
//  geode->addDrawable(ps);

#define PRINT_VEC3F( VEC ) \
  std::cout << "(" <<  VEC.x() << "," << VEC.y() << "," << VEC.z() << ")" << std::endl;

  viewer->setSceneData(groupNode);

  viewer->getCameraManipulator()->setAutoComputeHomePosition(true);
  viewer->getCameraManipulator()->home(0.0);

  osg::State* cameraState = viewer->getCamera()->getGraphicsContext()->getState();
  cameraState->setUseModelViewAndProjectionUniforms(true);
//  cameraState->setUseVertexAttributeAliasing(true);

  return viewer->run();

}

