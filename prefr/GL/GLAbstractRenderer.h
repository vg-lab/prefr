/*
 * Copyright (c) 2014-2022 VG-Lab/URJC.
 *
 * Authors:
 * - Sergio E. Galindo <sergio.galindo@urjc.es>
 * - Gael Rial Costas <g.rial.2018@alumnos.urjc.es>
 *
 * This file is part of PReFr <https://github.com/vg-lab/prefr>
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

//
// Created by gaeqs on 30/3/22.
//

#ifndef PREFR_GLABSTRACTRENDERER_H
#define PREFR_GLABSTRACTRENDERER_H


#include "prefr/core/Renderer.h"

namespace prefr
{

  class IGLRenderProgram;

  /**
   * Represents a renderer that uses an OpenGL context.
   */
  class GLAbstractRenderer : public prefr::Renderer
  {

  public:

    /**
     * Creates the renderer.
     * @param orderIndependent whether the renderer uses an order independent alpha render.
     */
    GLAbstractRenderer( bool orderIndependent )
      : Renderer( orderIndependent )
    { }

    /**
     * Returns the current shader program of this renderer.
     * @return the current shader program. May be null.
     */
    virtual IGLRenderProgram* getRenderProgram( ) = 0;

    /**
     * Sets the current shader program of this renderer.
     * @param program the new shader program. May be null.
     */
    virtual void setRenderProgram( IGLRenderProgram* program ) = 0;

  };

}


#endif //PREFR_GLABSTRACTRENDERER_H
