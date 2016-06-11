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

  void OSGPSNodeCallBack::operator()(osg::Node* node,
                                     osg::NodeVisitor* /* nv */ )
  {
    osg::ref_ptr<OSGDefaultParticleSystem> osgps =
        static_cast<OSGDefaultParticleSystem*>(
          node->asGeode()->getDrawable(0) );

    if (osgps)
    {
      osgps->UpdateUnified(0.1f);
    }
  }

  OSGDefaultParticleSystem::OSGDefaultParticleSystem()
  : ParticleSystem(0, 0, false)
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
  : ParticleSystem(0, 0, false)
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
  : ParticleSystem(initialParticlesNumber,
        _maxParticles,
        _loop)
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

  void OSGDefaultParticleSystem::ConfigureProgram(
    const std::string& shaderPathVert,
    const std::string& shaderPathFrag)
  {

    PREFR_DEBUG_CHECK( _renderer,
                       "Configure executed before adding a "
                       "renderer to the particle system.");
    OSGRenderConfig* osgrc =
      static_cast<OSGRenderConfig*>(_renderer->renderConfig);


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
      PREFR_THROW("Vertex file not found at: " + shaderPathVert)

    assert(vertexShader->loadShaderSourceFromFile( fullPath ));

    std::cout << "Loading vertex shader:" << shaderPathFrag << std::endl;

    // Load fragment shader
    fullPath = osgDB::findDataFile( shaderPathFrag );

    if ( fullPath.empty() )
      PREFR_THROW("Fragment shader file not found at: " + shaderPathVert)

    assert(fragmentShader->loadShaderSourceFromFile( fullPath ));

    program->addShader( vertexShader );
    program->addShader( fragmentShader );

    osgrc->uCameraUp = new osg::Uniform("cameraUp", osg::Vec3f());
    osgrc->uCameraRight = new osg::Uniform("cameraRight", osg::Vec3f());

    psState->addUniform(osgrc->uCameraUp);
    psState->addUniform(osgrc->uCameraRight);

    program->addBindAttribLocation( "vertexPosition", 0 );
    program->addBindAttribLocation( "particlePosition", 1 );
    program->addBindAttribLocation( "particleColor", 2 );

    psState->setRenderingHint(
      osg::StateSet::/*RenderingHint::*/TRANSPARENT_BIN);

    psState->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
//
    psState->setMode(GL_BLEND, osg::StateAttribute::ON);

    osg::BlendFunc* blendFunc = new osg::BlendFunc();
    blendFunc->setSource(blendFunctionSrc);
    blendFunc->setDestination(blendFunctionDst);
//        blendFunc->setDestination(osg::BlendFunc::ONE_MINUS_SRC_ALPHA);

    psState->setAttributeAndModes(blendFunc, osg::StateAttribute::ON);

    psState->setAttributeAndModes(program, osg::StateAttribute::ON);

  }

  void OSGDefaultParticleSystem::SetCameraManipulator(
    osgViewer::ViewerBase* _viewer,
    unsigned int contextNumber, unsigned int viewNumber)
  {
    osgViewer::ViewerBase::Contexts contexts;
    osgViewer::View* view;
    osgViewer::Viewer* viewer= dynamic_cast<osgViewer::Viewer*>(_viewer);

    view = dynamic_cast<osgViewer::View*>(viewer);



    if (!view)
      view =
        dynamic_cast<osgViewer::CompositeViewer*>(_viewer)->getView(viewNumber);

    PREFR_DEBUG_CHECK(view, "View is nullptr");

    cameraManipulator =
      dynamic_cast<osgGA::StandardManipulator*>(view->getCameraManipulator());

    PREFR_DEBUG_CHECK(cameraManipulator, "camera manipulator is nullptr");

    _viewer->getContexts(contexts, true);

    AcquireGraphicsContext(contexts[contextNumber]);
  }

  void OSGDefaultParticleSystem::AcquireGraphicsContext(
    osg::GraphicsContext* context)
  {
    context->realize();
    context->makeCurrent();

    // init glew
    glewInit();

    context->releaseContext();
  }

  void OSGDefaultParticleSystem::UpdateUniformVariables(float /*deltaTime*/)
  {
    assert(cameraManipulator != nullptr);

    OSGRenderConfig* osgrc =
      static_cast<OSGRenderConfig*>(_renderer->renderConfig);

    // Get camera position to calculate distances
    osg::Vec3d eye, center, up;
    cameraManipulator->getTransformation(eye, center, up);

    osgrc->eye = osg::Vec3f(eye);
    osgrc->center = osg::Vec3f(center);
    osgrc->up = osg::Vec3f(up);

    UpdateCameraDistances(glm::vec3(osgrc->eye.x(), osgrc->eye.y(),
                                    osgrc->eye.z()));

    osg::Vec3f forward = (osgrc->center - osgrc->eye);
    forward.normalize();
    osgrc->right = osgrc->up ^ forward;

    osgrc->uCameraUp->set( osgrc->up );

    osgrc->uCameraRight->set( osgrc->right );
  }

  void OSGDefaultParticleSystem::Update(float deltaTime)
  {
    ParticleSystem::Update(deltaTime);

    UpdateUniformVariables(deltaTime);

    UpdateRender();
  }

  void OSGDefaultParticleSystem::UpdateUnified(float deltaTime)
  {
    ParticleSystem::UpdateUnified(deltaTime);

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

  osg::BoundingBox OSGDefaultParticleSystem::computeBound() const
  {
    PREFR_DEBUG_CHECK( _renderer->renderConfig, "renderConfig is nullptr" );

    return static_cast<OSGRenderConfig*>(_renderer->renderConfig)->boundingBox;

  }


  void OSGDefaultParticleSystem::compileGLObjects(
    osg::RenderInfo& renderInfo ) const
  {
    PREFR_DEBUG_CHECK(
      static_cast<OSGRenderer*>(this->_renderer),
      "casting failed" );

    static_cast<OSGRenderer*>
      (this->_renderer)->osgCompileGLObjects(renderInfo);
  }

  void OSGDefaultParticleSystem::drawImplementation(
    osg::RenderInfo& renderInfo) const
  {
    osg::State* state = renderInfo.getState();
    state->setUseVertexAttributeAliasing(true);
    state->setUseModelViewAndProjectionUniforms(true);

    Render();
  }

  void OSGDefaultParticleSystem::releaseGLObjects(osg::State* /* state */ )
    const
  {
    OSGRenderConfig* osgrc =
      static_cast<OSGRenderConfig*>(_renderer->renderConfig);

    glDeleteBuffers(1, &osgrc->vboBillboardVertex);
    glDeleteBuffers(1, &osgrc->vboDrawElements);
    glDeleteBuffers(1, &osgrc->vboParticlesPositions);
    glDeleteBuffers(1, &osgrc->vboParticlesColors);
    glDeleteVertexArrays(1, &osgrc->vao);
  }

  void OSGDefaultParticleSystem::accept(osg::PrimitiveFunctor& functor) const
  {
    OSGRenderConfig* osgrc =
          static_cast<OSGRenderConfig*>(_renderer->renderConfig);

    if (!osgrc->vertexArray|| !osgrc->billboardIndices)
      return;

    // add drawable to the stats
    functor.setVertexArray(osgrc->vertexArray->size(),
                           static_cast<const osg::Vec3*>(
                             osgrc->vertexArray->getDataPointer()));
    osgrc->billboardIndices->accept(functor);
  }

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
