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

#ifndef __PREFR__PARTICLES__
#define __PREFR__PARTICLES__

#include <boost/noncopyable.hpp>

#include "../utils/types.h"
#include "../utils/VectorizedSet.hpp"

#include <vector>
#include <tuple>
#include <memory>
#include <iostream>
#include <set>

#include <prefr/api.h>

#define STRINGIZE( cad ) #cad

#define PREFR_ATRIB( name, type ) \
  protected: \
    std::vector< type > _##name##Vector; /*! Vector storing name attribute. */ \
  public: \
    type& p##name( unsigned int i ){ return _##name##Vector[ i ]; } \
    void p##name( unsigned int i, const type& value ) \
      { _##name##Vector[ i ] = value; }

#define PREFR_CONST_IT_ATRIB( name, type ) \
  protected: \
    type* _##name##_ptr; \
  public: \
    type name( void ) const { return *_##name##_ptr; }
//    type name( void ) const { std::cout << "attrib " << STRINGIZE( name ) << " " << _##name##_ptr << std::endl; return *_##name##_ptr; }


#define PREFR_IT_ATRIB( name, type ) \
  public: \
    void set_##name( const type& value ){ *_##name##_ptr = value; }

namespace prefr
{
  class Particles;
  class ParticleCollection;

  typedef ParticleCollection ParticleRange;
  typedef prefr::VectorizedSet< unsigned int > ParticleSet;
  typedef std::vector< unsigned int > ParticleIndices;

//  typedef utils::ElementCollection< prefr::Particles > ParticleRange;
//  typedef utils::ElementCollection< prefr::Particles > ParticleCollection;

  typedef std::tuple< unsigned int*,
                      float*,
                      float*,
                      TVect3*,
                      TVect4*,
                      float*,
                      TVect3*,
                      float*,
                      TVect3*,
                      char*> TParticle;

  enum TParticleAttribEnum
  {
    ID = 0,
    LIFE,
    SIZE,
    POSITION,
    COLOR,
    VELOCITY_MODULE,
    VELOCITY,
    ACCELERATION_MODULE,
    ACCELERATION,
    PARTICLE_ALIVE
  };

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
  class PREFR_API Particles : public boost::noncopyable
  {
  public:

    class base_iterator;
    class base_const_iterator;

    class iterator;
    class const_iterator;

    friend class base_const_iterator;
    friend class ParticleCollection;

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
    ParticleRange range( void ) const;

    /*! \brief Returns an iterator pointing to the given nth position.
     *
     * Returns an iterator pointing to the given nth position. Throws
     * exception when i > n, where n is the actual number of total particles.
     *
     * @param i Nth position to be returned.
     * @return an Particles::iterator pointing to the given nth position.
     */
    iterator at( unsigned int i );

    /*! \brief Returns a constant iterator pointing to the given nth position.
     *
     * Returns a constant iterator pointing to the given nth position. Throws
     * exception when i > n, where n is the actual number of total particles.
     *
     * @param i Nth position to be returned.
     * @return an Particles::const_terator pointing to the given nth position.
     */
    const_iterator at( unsigned int i ) const;

    /*! \brief Returns an iterator pointing to the given nth position.
     *
     * Returns a iterator pointing to the given nth position.
     *
     * @param i Nth position to be returned.
     * @return an Particles::iterator pointing to the given nth position.
     */
    iterator operator[]( unsigned int i );

    /*! \brief Returns a constant iterator pointing to the given nth position.
     *
     * Returns a constant iterator pointing to the given nth position.
     *
     * @param i Nth position to be returned.
     * @return an Particles::const_terator pointing to the given nth position.
     */
    const_iterator operator[]( unsigned int i ) const;

    const TParticle& vectorReferences( void ) const;

  protected:

    iterator _createIterator( unsigned int i ) const;

    bool p_alive( unsigned int i ){ return _aliveVector[ i ]; } \
    void p_alive( unsigned int i, const bool& value ) \
      { _aliveVector[ i ] = value ? 1 : 0; }

    void initVectorReferences( void );

    PREFR_ATRIB( id, unsigned int )
    PREFR_ATRIB( life, float )
    PREFR_ATRIB( size, float )
    PREFR_ATRIB( position, glm::vec3 )
    PREFR_ATRIB( color, glm::vec4 )
    PREFR_ATRIB( velocityModule, float )
    PREFR_ATRIB( velocity, glm::vec3 )
    PREFR_ATRIB( accelerationModule, float )
    PREFR_ATRIB( acceleration, glm::vec3 )

    std::vector< char > _aliveVector;

    unsigned int _size;

    TParticle _vectorReferences;
  };


  class PREFR_API ParticleCollection
  {

  public:

    friend class Particles::iterator;
    friend class Particles::const_iterator;
    friend class Particles;
    friend class ParticleSystem;

    ParticleCollection( void );

    ParticleCollection( const Particles& data, const ParticleSet& indices_ );
    ParticleCollection( const Particles& data, const ParticleIndices& indices_ );

    const ParticleIndices& indices( void ) const;
    void indices( const ParticleSet& newIndices );
    void indices( const ParticleIndices& newIndices );

    size_t size( void ) const;
    bool empty( void ) const;

    Particles::iterator find( unsigned int particleId );

    bool hasElement( unsigned int idx ) const;

    Particles::iterator begin( void );
    Particles::const_iterator begin( void ) const;

    Particles::iterator end( void );
    Particles::const_iterator end( void ) const;

    /*! \brief Returns an iterator pointing to the given nth position.
     *
     * Returns an iterator pointing to the given nth position. Throws
     * exception when i > n, where n is the actual number of total particles.
     *
     * @param i Nth position to be returned.
     * @return an Particles::iterator pointing to the given nth position.
     */
    Particles::iterator at( unsigned int index_ );

    /*! \brief Returns a constant iterator pointing to the given nth position.
     *
     * Returns a constant iterator pointing to the given nth position. Throws
     * exception when i > n, where n is the actual number of total particles.
     *
     * @param i Nth position to be returned.
     * @return an Particles::const_terator pointing to the given nth position.
     */
    Particles::const_iterator at( unsigned int index_ ) const;

    /*! \brief Returns an iterator pointing to the given nth position.
     *
     * Returns a iterator pointing to the given nth position.
     *
     * @param i Nth position to be returned.
     * @return an Particles::iterator pointing to the given nth position.
     */
    Particles::iterator operator[]( unsigned int i );

    /*! \brief Returns a constant iterator pointing to the given nth position.
     *
     * Returns a constant iterator pointing to the given nth position.
     *
     * @param i Nth position to be returned.
     * @return an Particles::const_terator pointing to the given nth position.
     */
    Particles::const_iterator operator[]( unsigned int i ) const;

    void addIndex( unsigned int idx );
    void addIndices( const ParticleSet& idxVector );

    void removeIndex( unsigned int idx );
    void removeIndices( const ParticleSet& idxVector );

    void transferIndexTo( ParticleCollection& other, unsigned int idx );
    void transferIndicesTo( ParticleCollection& other, const ParticleSet& idxVector );

  protected:

    ParticleCollection( const Particles& data );

    ParticleCollection( const Particles& data,
                        unsigned int begin_,
                        unsigned int end_ );

    ParticleCollection( const Particles& data,
                        Particles::iterator begin_,
                        Particles::iterator end_ );

    Particles::iterator _createIterator( unsigned int index = 0, bool absolute = false ) const;

    ParticleSet _particleIndices;
    ParticleIndices _indices;
    TParticle _vectorReferences;

    unsigned int _size;

    const Particles* _data;

  };

  class PREFR_API Particles::base_const_iterator
  {

    virtual ~base_const_iterator( void );

    friend class Particles;
    friend class ParticleCollection;

  public:

    base_const_iterator( const base_const_iterator& other );

    void print( std::ostream& stream = std::cout ) const;

    bool alive( ) const { return *_alive_ptr; }

  protected:

    base_const_iterator( void );

    void set( unsigned int index_ = 0 );
    void increase( int inc );
    void decrease( int dec );

    bool compare( const base_const_iterator& other ) const ;
    int sum( const base_const_iterator& other ) const;
    int difference( const base_const_iterator& other ) const;

    TParticle currentValues( void );

    unsigned int _position;
    unsigned int _size;

    const Particles* _data;
    TParticle _vectorRef;

    unsigned int _indexPosition;
    const ParticleIndices* _particleIndices;

    PREFR_CONST_IT_ATRIB( id, unsigned int )
    PREFR_CONST_IT_ATRIB( life, float )
    PREFR_CONST_IT_ATRIB( size, float )
    PREFR_CONST_IT_ATRIB( position, glm::vec3 )
    PREFR_CONST_IT_ATRIB( color, glm::vec4 )
    PREFR_CONST_IT_ATRIB( velocityModule, float )
    PREFR_CONST_IT_ATRIB( velocity, glm::vec3 )
    PREFR_CONST_IT_ATRIB( accelerationModule, float )
    PREFR_CONST_IT_ATRIB( acceleration, glm::vec3 )

    char* _alive_ptr;

  };

  class PREFR_API Particles::base_iterator : public Particles::base_const_iterator
  {
  public:
    base_iterator( const base_const_iterator& other );
    virtual ~base_iterator( void );

    void set_alive(const bool value) {
      *_alive_ptr = value ? 1 : 0;
      *_size_ptr = std::abs(*_size_ptr) * (value ? 1.0f : -1.0f);
    }

  protected:
    base_iterator( void );

    PREFR_IT_ATRIB( id, unsigned int )
    PREFR_IT_ATRIB( life, float )
    PREFR_IT_ATRIB( size, float )
    PREFR_IT_ATRIB( position, glm::vec3 )
    PREFR_IT_ATRIB( color, glm::vec4 )
    PREFR_IT_ATRIB( velocityModule, float )
    PREFR_IT_ATRIB( velocity, glm::vec3 )
    PREFR_IT_ATRIB( accelerationModule, float )
    PREFR_IT_ATRIB( acceleration, glm::vec3 )

  };

  class PREFR_API Particles::iterator : public Particles::base_iterator
  {
    friend class ParticleCollection;
    friend class Particles::const_iterator;

  public:
    iterator( void );
    iterator( const iterator& other );
    iterator( const const_iterator& other );


    iterator& operator++( void );
    iterator operator++( int );
    iterator& operator--( void );
    iterator operator--( int );

    virtual int operator+( const iterator& other ) const;
    virtual int operator-( const iterator& other ) const;

    virtual iterator operator+( int increase ) const;
    virtual iterator operator-( int decrease ) const;

    virtual bool operator== ( const iterator& other ) const;
    virtual bool operator!= ( const iterator& other ) const;

    iterator operator*( void );
    iterator operator->( void );
  };

  class PREFR_API Particles::const_iterator : public Particles::base_const_iterator
  {
    friend class ParticleCollection;
    friend class Particles::iterator;

  public:
    const_iterator( void );
    const_iterator( const iterator& other );
    const_iterator( const const_iterator& other );

    const_iterator& operator++( void );
    const_iterator operator++( int );
    const_iterator& operator--( void );
    const_iterator operator--( int );

    const_iterator operator+( int increase ) const;
    const_iterator operator-( int decrease ) const;

    int operator+( const const_iterator& other ) const;
    int operator-( const const_iterator& other ) const;

    bool operator== ( const const_iterator& other ) const;
    bool operator!= ( const const_iterator& other ) const;

    const_iterator operator*( void );
    const_iterator operator->( void );
  };

  typedef Particles::iterator tparticle;
  typedef tparticle* tparticle_ptr;

  std::ostream& operator<<( std::ostream& stream,
                            const Particles::base_const_iterator& it );
}
#endif /* __PREFR__PARTICLES__ */
