/*
 * Copyright (c) 2014-2022 VG-Lab/URJC.
 *
 * Authors:
 * - Sergio E. Galindo <sergio.galindo@urjc.es>
 * - Gael Rial Costas <g.rial.2018@alumnos.urjc.es>
 *
 * This file is part of PReFr <https://github.com/gmrvvis/prefr>
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

#ifndef __PREFR__OI_GL_RENDERER__
#define __PREFR__OI_GL_RENDERER__

#ifndef PREFR_SKIP_GLEW_INCLUDE

#include <GL/glew.h>

#endif

#include <prefr/core/Renderer.h>
#include "GLAbstractRenderer.h"

namespace prefr
{

  class IGLRenderProgram;

  class PREFR_API OIGLRenderer : public GLAbstractRenderer
  {

  public:

    explicit OIGLRenderer( IGLRenderProgram* program = nullptr );

    ~OIGLRenderer( ) override;

    void enableAccumulativeMode( bool accumulativeMode ) override;

    void updateRender( ParticleSystem& system ) override;

    void paint( const ParticleSystem& system ) const override;

    IGLRenderProgram* getRenderProgram( ) override;

    void setRenderProgram( IGLRenderProgram* program ) override;

  protected:

    void _init( unsigned int particleAmount ) override;

    void _dispose( ) override;

    IGLRenderProgram* _glRenderProgram;

    unsigned int _particleAmount;
    GLuint _vao;
    GLuint _vertexVBO;
    GLuint _sizeVBO;
    GLuint _positionVBO;
    GLuint _colorVBO;

    GLint _uniformMatrix;
    GLint _uniformCameraUp;
    GLint _uniformCameraRight;

    bool _accumulativeMode;

  };


}


#endif /* __PREFR__OI_GL_RENDERER__ */
