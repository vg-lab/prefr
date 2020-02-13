/*
 * Copyright (c) 2014-2020 GMRV/URJC.
 *
 * Authors: Sergio E. Galindo <sergio.galindo@urjc.es>
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

#ifndef __PREFR__MODEL__
#define __PREFR__MODEL__

#include <prefr/api.h>

#include "Particles.h"
#include "../utils/InterpolationSet.hpp"

namespace prefr
{

  typedef ::utils::InterpolationSet< float > vectortfloat;
  typedef ::utils::InterpolationSet< glm::vec3 > vectortvec3;
  typedef ::utils::InterpolationSet< glm::vec4 > vectortvec4;


  /*! \class Model
   * \brief This class provides a way to partially or totally define particles'
   *  behavior.
   *
   * This class provides a simple way to define partially or totally the
   * behavior of the particles. By default, this class will use InterpolationSet
   * objects to predefine several particles' properties variation such as life,
   * color, size or velocity module by simply inserting values to form a
   * piece-wise function on the desired property.
   *
   * Re-implement for adding new properties or behavior.
   *
   * @see Updater
   * @see Updater#Emit
   * @see Updater#Update
   * @see InterpolationSet
   * @see Source
   *
   */
  class Model
  {
    friend class Updater;

  public:

    /*! \brief Default constructor.
     *
     * Default constructor.
     */
    PREFR_API Model( void );

    /*! \brief Constructor initializing minimum and maximum life values.
     *
     *  Constructor initializing minimum and maximum life values for the
     *  particles using this Model object.
     *
     * @param min Minimum value for particles life range.
     * @param max Maximum value for particles life range.
     */
    PREFR_API Model( float min, float max );

    /*! \brief Default destructor.
     *
     * Default destructor.
     */
    PREFR_API virtual ~Model( );

    /*! \brief Defines minimum and maximum life values.
     *
     * This method sets the minimum and maximum life values for the
     * particles using this Model object.
     *
     * @param min Minimum value for particles life range.
     * @param max Maximum value for particles life range.
     */
    PREFR_API void setLife( float min, float max );

    /*! \brief Returns current minimum life value.
     *
     * This method returns current minimum life value.
     * @return Current minimum life value.
     */
    PREFR_API float minLife( void ) const;

    /*! \brief Returns current maximum life value.
     *
     * This method returns current maximum life value.
     * @return Current maximum life value.
     */
    PREFR_API float maxLife( void ) const;

    /*! \brief Returns the difference between max and min life values.
     *
     * Returns the difference between set maximum and minimum life values.
     * @return Difference between set maximum and minimum life values.
     */
    PREFR_API float lifeInterval( void );

    /*! \brief Returns the inverse of life range to avoid division operations..
     *
     * Returns the inverse of dividing one by the max life value, to
     * avoid later division operations when normalizing life values.
     * @return Inverse of life range.
     */
    PREFR_API float inverseMaxLife( void );

    /*! InterpolationSet defining size values. */
    vectortfloat size;

    /*! InterpolationSet defining velocity values. */
    vectortfloat velocity;

    /*! InterpolationSet defining color values. */
    vectortvec4 color;

protected:

    /*! Attribute storing minimum life value. */
    float _minLife;

    /*! Attribute storing maximum life value. */
    float _maxLife;

    /*! Attribute storing the difference between max and min life values. */
    float _lifeRange;

    /*! Attribute storing the inverse of the life range. */
    float _lifeNormalization;

  };

  typedef VectorizedSet< Model* > ModelsArray;


}

#endif /* __PREFR__MODEL__ */
