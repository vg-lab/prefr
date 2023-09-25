/*
 * Copyright (c) 2014-2020 VG-Lab/URJC.
 *
 * Authors: Sergio E. Galindo <sergio.galindo@urjc.es>
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

#ifndef __PREFR__ICAMERA__
#define __PREFR__ICAMERA__

#include <prefr/api.h>
#include <glm/fwd.hpp>

namespace prefr
{

  /*! \class ICamera
   *
   * This class provides an interface to integrate external camera
   * implementations within PReFr by simply implementing its methods.
   *
   */
  class PREFR_API ICamera
  {
  public:

    /*! \brief Default constructor.
     *
     * Default constructor.
     *
     */
    ICamera( ){ }

    /*! \brief Default destructor.
     *
     * Default destructor.
     *
     */
    virtual ~ICamera( ){ }

    /*! \brief This method returns the current camera position.
     *
     * This method returns the current camera position. Implement to be used
     * within the particle system.
     *
     * @return Current camera position.
     */
    virtual glm::vec3 PReFrCameraPosition( void ) = 0;

    /*! \brief This method returns the current view matrix.
     *
     * This method returns the current view matrix. Implement to be used
     * within the particle system.
     *
     * @return Current view matrix.
     */
    virtual glm::mat4x4 PReFrCameraViewMatrix( void ) = 0;

    /*! \brief This method returns the current view projection matrix.
     *
     * This method returns the current view projection matrix.
     *
     * @return Current view projection matrix.
     */
    virtual glm::mat4x4 PReFrCameraViewProjectionMatrix( void ) = 0;
  };
}

#endif /* __PREFR__ICAMERA__ */
