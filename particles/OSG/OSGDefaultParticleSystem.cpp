/*
 * OSGDefaultParticleSystem.cpp
 *
 *  Created on: 28/10/2014
 *      Author: sgalindo
 */

#include "OSGDefaultParticleSystem.h"

#include <particles/config.h>

namespace particles
{
  namespace defaultParticleSystem
  {
    namespace OSGParticleSystem
    {

      void OSGPSNodeCallBack::operator()(osg::Node* node, osg::NodeVisitor* nv)
      {
        osg::ref_ptr<OSGDefaultParticleSystem> osgps =
            static_cast<OSGDefaultParticleSystem*>( node->asGeode()->getDrawable(0) );

        if (osgps)
        {
          osgps->Update(0.1f);
        }
      }

      OSGDefaultParticleSystem::OSGDefaultParticleSystem()
      : ParticleSystem(0, 0, false)
      , osg::Drawable()
      , cameraManipulator( nullptr )
      , rootNode( nullptr )
      {

	#ifdef DEBUG
	if ( (void * ) glDrawElementsInstanced == nullptr )
	  PARTICLES_THROW("glDrawElementsInstanced is nullptr");
        #endif

        setUseDisplayList(false);
        setUseVertexBufferObjects(true);

        distances = new distanceArray(this->maxParticles);
        renderConfig = new RenderConfig();
      }
      
      OSGDefaultParticleSystem::OSGDefaultParticleSystem(
	const OSGDefaultParticleSystem& other, 
	const osg::CopyOp& copyOp)
      : ParticleSystem(0, 0, false)
      , osg::Drawable(other, copyOp)
      , cameraManipulator( nullptr )
      , rootNode( nullptr )
      {
        setUseDisplayList(false);
        setUseVertexBufferObjects(true);

        distances = new distanceArray(this->maxParticles);
        renderConfig = new RenderConfig();
      }

      OSGDefaultParticleSystem::OSGDefaultParticleSystem(
      unsigned int initialParticlesNumber, unsigned int _maxParticles, bool _loop)
      : ParticleSystem(initialParticlesNumber,
			      _maxParticles, 
			      loop)
      , osg::Drawable()
      , cameraManipulator( nullptr )
      , rootNode( nullptr )
      {

        distances = new distanceArray(this->maxParticles);
        renderConfig = new RenderConfig();

        rootNode = new osg::Geode;
        rootNode->addDrawable(this);

        rootNode->setCullCallback(new OSGPSNodeCallBack);

         setUseDisplayList(false);
         setUseVertexBufferObjects(true);

      }

      OSGDefaultParticleSystem::~OSGDefaultParticleSystem()
      {
        delete( distances );

        delete( renderConfig );
      }

      void OSGDefaultParticleSystem::ConfigureProgram(const std::string& shaderPathVert, const std::string& shaderPathFrag)
      {

        osg::StateSet* psState = rootNode->getOrCreateStateSet();

        osg::Program* program = new osg::Program;
        osg::Shader* vertexShader =
          new osg::Shader( osg::Shader::VERTEX );
        osg::Shader* fragmentShader =
          new osg::Shader( osg::Shader::FRAGMENT );

        std::string fullPath;

        std::cout << "Loading vertex shader:" << shaderPathVert << std::endl;

        // Load vertex shader
        fullPath = osgDB::findDataFile( shaderPathVert );

        if ( fullPath.empty() )
          PARTICLES_THROW("Vertex file not found at: " + shaderPathVert)

        assert(vertexShader->loadShaderSourceFromFile( fullPath ));

        std::cout << "Loading vertex shader:" << shaderPathFrag << std::endl;

        // Load fragment shader
        fullPath = osgDB::findDataFile( shaderPathFrag );

        if ( fullPath.empty() )
          PARTICLES_THROW("Fragment shader file not found at: " + shaderPathVert)

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

        psState->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
//
        psState->setMode(GL_BLEND, osg::StateAttribute::ON);

        osg::BlendFunc* blendFunc = new osg::BlendFunc();
        blendFunc->setSource(osg::BlendFunc::SRC_ALPHA);
        blendFunc->setDestination(osg::BlendFunc::ONE_MINUS_CONSTANT_ALPHA);
//        blendFunc->setDestination(osg::BlendFunc::ONE_MINUS_SRC_ALPHA);

        psState->setAttributeAndModes(blendFunc, osg::StateAttribute::ON);

        psState->setAttributeAndModes(program, osg::StateAttribute::ON);

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
        ParticleSystem::Update(deltaTime);

        assert(cameraManipulator != nullptr);

        // Get camera position to calculate distances
        osg::Vec3d eye, center, up;
        cameraManipulator->getTransformation(eye, center, up);

        renderConfig->eye = osg::Vec3f(eye);
        renderConfig->center = osg::Vec3f(center);
        renderConfig->up = osg::Vec3f(up);

        UpdateCameraDistances(glm::vec3(renderConfig->eye.x(), renderConfig->eye.y(), renderConfig->eye.z()));

        osg::Vec3f forward = (renderConfig->center - renderConfig->eye);
        forward.normalize();
        renderConfig->right = renderConfig->up ^ forward;

        renderConfig->uCameraUp->set( renderConfig->up );

        renderConfig->uCameraRight->set( renderConfig->right );

        UpdateRender();
      }


      void OSGDefaultParticleSystem::UpdateCameraDistances(const glm::vec3& cameraPosition)
      {

        unsigned int i = 0;
        for (tparticleContainer::iterator it = particles->start; it != particles->end; it++)
        {
          #ifdef DEBUG
	  if ( ! *it )
	    PARTICLES_THROW( "null pointer access" );
	  #endif 

         i = ((tparticle_ptr) *it)->id;

          #ifdef DEBUG
	  if ( ! static_cast<OSGDefaultParticleSorter*>(sorter) )
	    PARTICLES_THROW( "casting failed" );
	  #endif 

         static_cast<OSGDefaultParticleSorter*>(sorter)->UpdateCameraDistance(i, cameraPosition);
        }

      }

      void OSGDefaultParticleSystem::UpdateRender()
      {
        this->sorter->Sort();

        #ifdef DEBUG
	if ( ! static_cast<OSGDefaultParticleRenderer*>(this->renderer) )
	  PARTICLES_THROW( "casting failed" );
	#endif 

        static_cast<OSGDefaultParticleRenderer*>(this->renderer)->SetupRender(this->aliveParticles);

        dirtyBound();
      }

      void OSGDefaultParticleSystem::Render() const
      {
        #ifdef DEBUG
	if ( ! static_cast<OSGDefaultParticleRenderer*>(this->renderer) )
	  PARTICLES_THROW( "casting failed" );
	#endif 

        static_cast<OSGDefaultParticleRenderer*>(this->renderer)->Paint(aliveParticles);
      }

      osg::BoundingBox OSGDefaultParticleSystem::computeBound() const
      {
        #ifdef DEBUG
	if ( ! renderConfig )
	  PARTICLES_THROW( "renderConfig is nullptr" );
	#endif 

        return renderConfig->boundingBox;

      }


      void OSGDefaultParticleSystem::compileGLObjects( osg::RenderInfo& renderInfo ) const
      {
        #ifdef DEBUG
	if ( ! static_cast<OSGDefaultParticleRenderer*>(this->renderer) )
	  PARTICLES_THROW( "casting failed" );
	#endif 

        static_cast<OSGDefaultParticleRenderer*>(this->renderer)->osgCompileGLObjects(renderInfo);
      }

      void OSGDefaultParticleSystem::drawImplementation(osg::RenderInfo& renderInfo) const
      {
        osg::State* state = renderInfo.getState();
        state->setUseVertexAttributeAliasing(true);

        Render();
      }

      void OSGDefaultParticleSystem::accept(osg::PrimitiveFunctor& functor) const
      {
        #ifdef DEBUG
	if ( ! renderConfig )
	  PARTICLES_THROW( "renderConfig is nullptr" );
	#endif 

        if (!renderConfig->billboardVertices || !renderConfig->billboardIndices)
        {
          return;
        }
        functor.setVertexArray(renderConfig->billboardVertices->size(), static_cast<const osg::Vec3*>(renderConfig->billboardVertices->getDataPointer()));
        renderConfig->billboardIndices->accept(functor);
      }

      void OSGDefaultParticleSystem::releaseGLObjects(osg::State* state) const
      {
        glDeleteBuffers(1, &renderConfig->vboBillboardVertex);
        glDeleteBuffers(1, &renderConfig->vboDrawElements);
        glDeleteBuffers(1, &renderConfig->vboParticlesPositions);
        glDeleteBuffers(1, &renderConfig->vboParticlesColor);
        glDeleteVertexArrays(1, &renderConfig->vao);
      }

    }
  }
}


