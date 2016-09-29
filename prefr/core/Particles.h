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
#ifndef __PREFR__PARTICLES__
#define __PREFR__PARTICLES__

#include <boost/noncopyable.hpp>

#include "../utils/types.h"
#include "../utils/ElementCollection.hpp"

#include <vector>

#define STRINGIZE( cad ) #cad

#define PREFR_ATRIB( name, type ) \
  protected: \
    std::vector< type > _##name##Vector; /*! Vector storing name attribute. */ \
  public: \
    type& p##name( unsigned int i ){ return _##name##Vector[ i ]; } \
    void p##name( unsigned int i, const type& value ) \
      { _##name##Vector[ i ] = value; }

#define PREFR_ATRIB_BOOL( name, type ) \
  protected: \
    std::vector< type > _##name##Vector; \
  public: \
    type p##name( unsigned int i ){ return _##name##Vector[ i ]; } \
    void p##name( unsigned int i, const type& value ) \
      { _##name##Vector[ i ] = value; }

#define PREFR_ATRIB_IT( name, type ) \
  protected: \
    std::vector< type >::iterator _##name##Iterator; \
  public: \
    type name( void ) const { return *_##name##Iterator; } \
    void name( const type& value ){ *_##name##Iterator = value; }

#define PREFR_ATRIB_BOOL_IT( name, type ) \
  protected: \
    std::vector< type >::iterator _##name##Iterator; \
  public: \
    type name( void ) const { return *_##name##Iterator; } \
    void name( const type& value ){ *_##name##Iterator = value; }

#define PREFR_ATRIB_CONST_IT( name, type ) \
  protected: \
    std::vector< type >::const_iterator _##name##ConstIterator; \
  public: \
    type name( void ) const { return *_##name##ConstIterator; }

namespace prefr
{
  class Particles;
}

typedef utils::ElementCollection< prefr::Particles > ParticleRange;
typedef utils::ElementCollection< prefr::Particles > ParticleCollection;

namespace prefr
{

  /*! \class Particles
   *
   * \brief This class contains the set of attributes defining the particles
   * properties such as life, size, color, position and other values.
   *
   * This class contains the set of vectorized attributes defining particles
   * properties such as life, size, color, position, velocity and acceleration
   * values. Iterators generated from this class allow to iterate through the
   * whole set of particles in order to alter their properties' values.
   *
   *
   * @see ParticleSystem
   * @see Particles::iterator
   * @see Particles::const_iterator
   */
  class Particles : public boost::noncopyable
  {

  public:

    enum TAttribute
    {
      ID = 0,
      LIFE,
      SIZE,
      POSITION,
      COLOR,
      VELOCITY,
      ACCELERATION,
      ALIVE
    };


    class iterator;
    class const_iterator;

    /*! \brief Default constructor.
     *
     * Default constructor. Use Particles::resize method to set the number
     * of available particles.
     *
     */
    Particles( );

    /*! \brief Constructor specifying the number of available particles.
     *
     * Constructor specifying the number of available particles.
     *
     * @param size Number of available particles.
     */
    Particles( unsigned int size );

    /*! \brief Default destructor.
     *
     * Default destructor.
     *
     */
    ~Particles( );

    /*! \brief Returns the number of available particles.
     *
     * Returns the number of available particles.
     *
     * @return Number of available particles.
     */
    unsigned int numParticles( void ) const;

    /*! \brief Sets the number of available particles.
     *
     * Sets the number of available particles. Use this method when
     * initializing with the default constructor. This method might be used
     * for increasing or decreasing the number of available particles.
     *
     * Note: If the number of particles is decreased, check Cluster particles'
     * ranges, as behavior is undefined in this case.
     *
     * @param newSize New number of available particles to be set.
     */
    void resize( unsigned int newSize );

    /*! \brief Sets the number of available particles to zero
     *
     * Sets the number of available particles to zero, releasing current
     * resources from memory.
     */
    void clear( void );

    /*! \brief Creates and returns an iterator pointing to the first particle.
     *
     * Creates and returns an iterator pointing to the first particle.
     *
     * @return An iterator pointing to the first particle.
     */
    iterator begin( void );

    /*! \brief Creates and returns a constant iterator pointing to the first
     * particle.
     *
     * Creates and returns a constant iterator pointing to the first
     * particle.
     *
     * @return A constant iterator pointing to the first particle.
     */
    const_iterator begin( void ) const;

    /*! \brief Creates and returns an iterator pointing to the end of the set
     * of particles.
     *
     * Creates and returns an iterator pointing to the end of the set
     * of particles.
     *
     * @return An iterator pointing to the end of the set of particles.
     */
    iterator end( void );

    /*! \brief Creates and returns a constant iterator pointing to the end of
     * the set of particles.
     *
     * Creates and returns a constant iterator pointing to the end of
     * the set of particles.
     *
     * @return A constant iterator pointing to the end of the set of particles.
     */
    const_iterator end( void ) const;

    /*! \brief Returns a prefr::utils::InterpolationSet object with the
     * values set to the first and last positions.
     *
     * Returns a prefr::utils::InterpolationSet object with the
     * values set to the first and last positions.
     *
     * @return A prefr::utils::InterpolationSet object with the
     * values set to the first and last positions.
     */
    ParticleRange range( void );

    /*! \brief Returns an iterator pointing to the given nth position.
     *
     * Returns an iterator pointing to the given nth position.
     *
     * @param i Nth position to be returned.
     * @return an Particles::iterator pointing to the given nth position.
     */
    iterator at( unsigned int i );

    /*! \brief Returns a constant iterator pointing to the given nth position.
     *
     * Returns a constant iterator pointing to the given nth position.
     *
     * @param i Nth position to be returned.
     * @return an Particles::const_terator pointing to the given nth position.
     */
    const_iterator at( unsigned int i ) const;

  protected:

    class base_iterator;

    iterator CreateIterator( unsigned int i );
    const_iterator CreateConstIterator( unsigned int i ) const;

    PREFR_ATRIB( id, unsigned int )
    PREFR_ATRIB( life, float )
    PREFR_ATRIB( size, float )
    PREFR_ATRIB( position, glm::vec3 )
    PREFR_ATRIB( color, glm::vec4 )
    PREFR_ATRIB( velocityModule, float )
    PREFR_ATRIB( velocity, glm::vec3 )
    PREFR_ATRIB( acceleration, glm::vec3 )

    PREFR_ATRIB_BOOL( alive, bool )

    unsigned int _size;

  };

  class Particles::iterator
  : public std::iterator< std::bidirectional_iterator_tag, Particles >
  {
    friend class Particles;
    friend class Particles::const_iterator;

  public:

    iterator( void );
    iterator( const iterator& other );
    virtual ~iterator( ){ }

    iterator& operator++( void );
    iterator operator++( int );
    iterator& operator--( void );
    iterator operator--( int );

    iterator operator+( int increase );
    iterator operator-( int decrease );

    int operator+( const iterator& other );
    int operator-( const iterator& other );

    bool operator== ( const iterator& other );
    bool operator!= ( const iterator& other );

  protected:

    long _position;
    unsigned int _size;

    const Particles* _data;

    PREFR_ATRIB_IT( id, unsigned int )
    PREFR_ATRIB_IT( life, float )
    PREFR_ATRIB_IT( size, float )
    PREFR_ATRIB_IT( position, glm::vec3 )
    PREFR_ATRIB_IT( color, glm::vec4 )
    PREFR_ATRIB_IT( velocityModule, float )
    PREFR_ATRIB_IT( velocity, glm::vec3 )
    PREFR_ATRIB_IT( acceleration, glm::vec3 )

    PREFR_ATRIB_BOOL_IT( alive, bool )

  };

  class Particles::const_iterator
  : public std::iterator< std::bidirectional_iterator_tag, Particles >
  {
    friend class Particles;
    friend class Particles::iterator;

  public:

    const_iterator( void );
    const_iterator( const Particles::const_iterator& other );
    const_iterator( const Particles::iterator& other );
    ~const_iterator( ){ }

    const_iterator& operator++( void );
    const_iterator operator++( int );
    const_iterator& operator--( void );
    const_iterator operator--( int );

    const_iterator operator+( int increase );
    const_iterator operator-( int decrease );

    bool operator== ( const const_iterator& other );
    bool operator!= ( const const_iterator& other );

  protected:

    long _position;
    unsigned int _size;

    const Particles* _data;

    PREFR_ATRIB_CONST_IT( id, unsigned int )
    PREFR_ATRIB_CONST_IT( life, float )
    PREFR_ATRIB_CONST_IT( size, float )
    PREFR_ATRIB_CONST_IT( position, glm::vec3 )
    PREFR_ATRIB_CONST_IT( color, glm::vec4 )
    PREFR_ATRIB_CONST_IT( velocityModule, float )
    PREFR_ATRIB_CONST_IT( velocity, glm::vec3 )
    PREFR_ATRIB_CONST_IT( acceleration, glm::vec3 )

    PREFR_ATRIB_CONST_IT( alive, bool )

  };

  typedef Particles::iterator tparticle;
  typedef tparticle* tparticle_ptr;

}
#endif /* __PREFR__PARTICLES__ */
