/*
 * OSGDefaultParticleSystem.cpp
 *
 *  Created on: 28/10/2014
 *      Author: sgalindo
 */

#include "OSGDefaultParticleSystem.h"

namespace particles
{
  namespace defaultParticleSystem
  {
    namespace OSGParticleSystem
    {

      void OSGPSNodeCallBack::update(osg::NodeVisitor* nv, osg::Drawable* node)
      {
        osg::ref_ptr<OSGDefaultParticleSystem> osgps =
            static_cast<OSGDefaultParticleSystem*>( node );

        if (osgps)
        {
          osgps->Update(0.1f);
        }

//          traverse(node, nv);
      }


      OSGDefaultParticleSystem::OSGDefaultParticleSystem()
      : DefaultParticleSystem(0, 0, 0, false)
      , osg::Drawable()
      , cameraManipulator( nullptr )
      , rootNode( nullptr )
      {
        setUseDisplayList(false);
        setUseVertexBufferObjects(true);

        distances = new distanceArray(this->maxParticles);
        renderConfig = new RenderConfig();
      }
      OSGDefaultParticleSystem::OSGDefaultParticleSystem(const OSGDefaultParticleSystem& other, const osg::CopyOp& copyOp)
      : DefaultParticleSystem(0, 0, 0, false)
      , osg::Drawable(other, copyOp)
      , cameraManipulator( nullptr )
      , rootNode( nullptr )
      {
        setUseDisplayList(false);
        setUseVertexBufferObjects(true);

        distances = new distanceArray(this->maxParticles);
        renderConfig = new RenderConfig();
      }
      OSGDefaultParticleSystem::OSGDefaultParticleSystem(int initialParticlesNumber, int _maxParticles
														, float _emissionRate, bool _loop
                                       , osgGA::StandardManipulator* cam)
      : DefaultParticleSystem(initialParticlesNumber, _maxParticles, _emissionRate, loop)
      , osg::Drawable()
      , cameraManipulator( cam )
      , rootNode( nullptr )
      {
        distances = new distanceArray(this->maxParticles);
        renderConfig = new RenderConfig();


        this->setUpdateCallback(new OSGPSNodeCallBack);

        LoadProgram();

         //TODO Render bins, etc

         setUseDisplayList(false);
         setUseVertexBufferObjects(true);

      }

      void OSGDefaultParticleSystem::LoadProgram()
      {
        rootNode = new osg::Geode;

        osg::StateSet* psState = rootNode->getOrCreateStateSet();

        osg::Program* program = new osg::Program;
        osg::Shader* vertexShader =
          new osg::Shader( osg::Shader::VERTEX );
        osg::Shader* fragmentShader =
          new osg::Shader( osg::Shader::FRAGMENT );

        std::string fullPath;

        // Load vertex shader
        fullPath = osgDB::findDataFile( "shd/particle.vert" );

        if ( !fullPath.empty() )
          assert(vertexShader->loadShaderSourceFromFile( fullPath ));

        // Load fragment shader
        fullPath = osgDB::findDataFile( "shd/particle.frag" );

        if ( !fullPath.empty() )
          assert(fragmentShader->loadShaderSourceFromFile( fullPath ));

        program->addShader( vertexShader );
        program->addShader( fragmentShader );

        renderConfig->uCameraUp = new osg::Uniform("cameraUp", osg::Vec3f());
        renderConfig->uCameraRight = new osg::Uniform("cameraRight", osg::Vec3f());

        psState->addUniform(renderConfig->uCameraUp);
        psState->addUniform(renderConfig->uCameraRight);

        program->addBindAttribLocation( "vertexPosition", 0 );
        program->addBindAttribLocation( "particlePosition", 1 );
        program->addBindAttribLocation( "particleColor", 2 );

        psState->setRenderingHint(osg::StateSet::RenderingHint::TRANSPARENT_BIN);

        psState->setAttributeAndModes(program, osg::StateAttribute::ON);

        rootNode->addDrawable(this);
      }

      void OSGDefaultParticleSystem::SetCameraManipulator(osgGA::StandardManipulator* cam)
      {
        cameraManipulator = cam;
      }

      void OSGDefaultParticleSystem::SetRenderer(ParticleRenderer* renderConfig)
      {
        ParticleSystem::SetRenderer(renderConfig);


      }

      void OSGDefaultParticleSystem::Update(float deltaTime)
      {
        DefaultParticleSystem::Update(deltaTime);

        assert(cameraManipulator != nullptr);

//        if (!cameraManipulator)
//          std::cout << "No hay manipulador de cámara." << std::endl;


        // Get camera position to calculate distances
        osg::Vec3d eye, center, up;
        cameraManipulator->getTransformation(eye, center, up);
        renderConfig->eye = osg::Vec3f(eye);
        renderConfig->center = osg::Vec3f(center);
        renderConfig->up = osg::Vec3f(up);
        UpdateCameraDistances(glm::vec3(renderConfig->eye.x(), renderConfig->eye.y(), renderConfig->eye.z()));

//        std::cout << center.x() << ", " << center.y() << ", " << center.z() << ", " << std::endl;

        renderConfig->right = renderConfig->up ^ renderConfig->center;

        renderConfig->uCameraUp->set( renderConfig->up );

        renderConfig->uCameraRight->set( renderConfig->right );

        UpdateRender();

        std::cout << renderConfig->boundingBox.xMin() << ", " << renderConfig->boundingBox.xMax() << " "
                  << renderConfig->boundingBox.yMin() << ", " << renderConfig->boundingBox.yMax() << " "
                  << renderConfig->boundingBox.zMin() << ", " << renderConfig->boundingBox.zMax()
                  << std::endl;
      }


      void OSGDefaultParticleSystem::UpdateCameraDistances(const glm::vec3& cameraPosition)
      {

        unsigned int i = 0;
        for (tparticleContainer::iterator it = particles->start; it != particles->end; it++)
        {
         i = ((tparticle_ptr) *it)->id;
         static_cast<OSGDefaultParticleSorter*>(sorter)->UpdateCameraDistance(i, cameraPosition);
        }

      }

      void OSGDefaultParticleSystem::UpdateRender()
      {
        this->sorter->Sort();

        static_cast<OSGDefaultParticleRenderer*>(this->renderer)->SetupRender(this->aliveParticles);

        dirtyBound();
        dirtyDisplayList();
      }

      void OSGDefaultParticleSystem::Render() const
      {
        static_cast<OSGDefaultParticleRenderer*>(this->renderer)->Paint(aliveParticles);
      }

      osg::BoundingBox OSGDefaultParticleSystem::computeBound() const
      {
        return renderConfig->boundingBox;
      }


      void OSGDefaultParticleSystem::compileGLObjects( osg::RenderInfo& renderInfo ) const
      {
        static_cast<OSGDefaultParticleRenderer*>(this->renderer)->osgCompileGLObjects(renderInfo);
      }

      void OSGDefaultParticleSystem::drawImplementation(osg::RenderInfo& renderInfo) const
      {
        Render();
      }

      void OSGDefaultParticleSystem::accept(osg::PrimitiveFunctor& functor) const
      {
        if (!renderConfig->billboardVertices)
          return;

        functor.setVertexArray(aliveParticles, static_cast<const osg::Vec3*>(renderConfig->billboardVertices->getDataPointer()));
        renderConfig->billboardIndices->accept(functor);
      }

      void OSGDefaultParticleSystem::releaseGLObjects(osg::State* state) const
      {
        delete( renderConfig );
      }

    }
  }
}


