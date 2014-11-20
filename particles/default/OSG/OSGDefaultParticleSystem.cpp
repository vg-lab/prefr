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

//      void OSGPSNodeCallBack::update(osg::NodeVisitor* nv, osg::Drawable* node)
//      {
//        osg::ref_ptr<OSGDefaultParticleSystem> osgps =
//            static_cast<OSGDefaultParticleSystem*>( node );
//
//        if (osgps)
//        {
//          osgps->Update(0.1f);
//        }
//
////          traverse(node, nv);
//      }

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
      : DefaultParticleSystem(0, 0, 0, false)
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

      OSGDefaultParticleSystem::OSGDefaultParticleSystem(
	int initialParticlesNumber, int _maxParticles,
	float _emissionRate, bool _loop, 
	osgGA::StandardManipulator* cam)
      : DefaultParticleSystem(initialParticlesNumber, 
			      _maxParticles, 
			      _emissionRate, 
			      loop)
      , osg::Drawable()
      , cameraManipulator( cam )
      , rootNode( nullptr )
      {

        distances = new distanceArray(this->maxParticles);
        renderConfig = new RenderConfig();


        LoadProgram();

        rootNode->setCullCallback(new OSGPSNodeCallBack);

         //TODO Render bins, etc

         setUseDisplayList(false);
         setUseVertexBufferObjects(true);

      }

      OSGDefaultParticleSystem::~OSGDefaultParticleSystem()
      {
	if ( distances ) 
	  delete distances;
	// if ( renderConfig ) 
	//   delete renderConfig;
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
        else
          std::cout << "Path vacío" << std::endl;

        // Load fragment shader
        fullPath = osgDB::findDataFile( "shd/particle.frag" );

        if ( !fullPath.empty() )
          assert(fragmentShader->loadShaderSourceFromFile( fullPath ));
        else
          std::cout << "Path vacío" << std::endl;

        program->addShader( vertexShader );
        program->addShader( fragmentShader );

        renderConfig->uCameraUp = 
	  new osg::Uniform("cameraUp", osg::Vec3f());

        renderConfig->uCameraRight = 
	  new osg::Uniform("cameraRight", osg::Vec3f());

        psState->addUniform(renderConfig->uCameraUp);
        psState->addUniform(renderConfig->uCameraRight);

        program->addBindAttribLocation( "vertexPosition", 0 );
        program->addBindAttribLocation( "particlePosition", 1 );
        program->addBindAttribLocation( "particleColor", 2 );

        psState->setRenderingHint(osg::StateSet::RenderingHint::TRANSPARENT_BIN);

        psState->setAttributeAndModes(program, osg::StateAttribute::ON);

        psState->setMode(GL_CULL_FACE, osg::StateAttribute::OFF);
        psState->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);

        psState->setMode(GL_BLEND, osg::StateAttribute::ON);

        osg::BlendFunc* blendFunc = new osg::BlendFunc();

        blendFunc->setSource(osg::BlendFunc::CONSTANT_ALPHA);
        blendFunc->setDestination(osg::BlendFunc::ONE_MINUS_CONSTANT_ALPHA);

        psState->setAttributeAndModes(blendFunc, osg::StateAttribute::ON);


        rootNode->addDrawable(this);
        psState->setMode( GL_BLEND, osg::StateAttribute::ON );


        setUseDisplayList(false);
        setUseVertexBufferObjects(true);



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

//        std::cout << renderConfig->boundingBox.xMin() << ", " << renderConfig->boundingBox.xMax() << " "
//                  << renderConfig->boundingBox.yMin() << ", " << renderConfig->boundingBox.yMax() << " "
//                  << renderConfig->boundingBox.zMin() << ", " << renderConfig->boundingBox.zMax()
//                  << std::endl;

        std::cout << "Update" << std::endl;
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
//        dirtyDisplayList();
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

        std::cout << "Compute bound" << std::endl;
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

        state->dirtyVertexAttribPointer(renderConfig->vboParticlesPositions);
        state->dirtyVertexAttribPointer(renderConfig->vboParticlesColor);

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
          std::cout << "Accept error" << std::endl;
          return;
        }
        functor.setVertexArray(renderConfig->billboardVertices->size(), static_cast<const osg::Vec3*>(renderConfig->billboardVertices->getDataPointer()));
        renderConfig->billboardIndices->accept(functor);
        std::cout << "Accept" << std::endl;
      }

      void OSGDefaultParticleSystem::releaseGLObjects(osg::State* state) const
      {
        delete renderConfig ;
      }

    }
  }
}


