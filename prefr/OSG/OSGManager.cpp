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

#include "OSGManager.h"

namespace prefr
{
void OSGNodeCallBack::operator( )( osg::Node* node,
                                   osg::NodeVisitor* /* nv */ )
{
  osg::ref_ptr<OSGManager> osgps =
      static_cast<OSGManager*>(
        node->asGeode()->getDrawable(0) );

  if (osgps)
  {
    osgps->UpdateUnified(0.1f);
  }
}

void OSGManager::SetCameraManipulator(
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

  void OSGManager::AcquireGraphicsContext(
    osg::GraphicsContext* context)
  {
    context->realize();
    context->makeCurrent();

    // init glew
    glewInit();

    context->releaseContext();
  }


  void OSGManager::ConfigureProgram(
          const std::string& shaderPathVert,
          const std::string& shaderPathFrag)
  {
    OSGRenderConfig* osgrc =
      static_cast<OSGRenderConfig*>( renderConfig );


    osg::StateSet* psState = rootNode->getOrCreateStateSet();

    osg::Program* program = new osg::Program;
    osg::Shader* vertexShader = new osg::Shader( osg::Shader::VERTEX );
    osg::Shader* fragmentShader = new osg::Shader( osg::Shader::FRAGMENT );

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

  void OSGRenderer::UpdateUniformVariables(float /*deltaTime*/)
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

}
