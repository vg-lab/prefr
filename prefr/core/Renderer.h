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

#ifndef __PREFR__RENDERER__
#define __PREFR__RENDERER__

#include <prefr/api.h>
#include <memory>

namespace prefr
{
  class ParticleSystem;

  /**
   * Represents a particle render.
   *
   * A particle render grabs the current particles inside a ParticleSystem
   * and creates a frame in the current OpenGL context.
   *
   * A particle renderer may implement an order-independent transparency
   * algorithm. These renderers are much faster than order-dependent
   * algorithms, but the quality may be poorer.
   */
  class PREFR_API Renderer
  {
    friend class ParticleSystem;

  public:

    /**
     * Creates an empty particle renderer.
     * @param orderIndependent whether the renderer implements
     * an order-independent transparency algorithm.
     */
    explicit Renderer( bool orderIndependent );

    /**
     * The destroyer of the renderer.
     *
     * Do not release OpenGL resources here.
     * Use _dispose( ) instead.
     */
    virtual ~Renderer( ) = default;

    /**
     * Returns whether this renderer implements an order-independent
     * transparency algorithm.
     * @return whether this renderer implements an order-independent
     * transparency algorithm.
     */
    bool isOrderIndependent( ) const;

    /**
     * Enables or disables the accumulative mode of this renderer.
     * @param accumulativeMode whether the accumulative mode should be enabled.
     */
    virtual void enableAccumulativeMode( bool accumulativeMode ) = 0;

    /**
     *  Updates the render with the current particles' data.
     *
     *  This method must be overridden.
     *
     *  @param system the ParticleSystem requesting the update.
     */
    virtual void updateRender( ParticleSystem& system ) = 0;

    /**
     * Renders a frame.
     *
     * This method must be overridden.
     *
     * @param system the ParticleSystem requesting the update.
     */
    virtual void paint( const ParticleSystem& system ) const = 0;

  protected:

    /**
     * Initializes this renderer.
     *
     * Implementations must initialize their OpenGL resources in this method.
     *
     * This method MUST be called from a OpenGL context's thread.
     *
     * @param particleAmount the amount of particles the system can hold.
     */
    virtual void _init( unsigned int particleAmount ) = 0;

    /**
     * Disposes this renderer.
     *
     * Implementations must release all their OpenGL resources in this method.
     *
     * This method MUST be called from a OpenGL context's thread.
     */
    virtual void _dispose( ) = 0;

  private:

    bool _orderIndependent;

  };
}

#endif /* __PREFR__RENDERER__ */
