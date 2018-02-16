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

#include <vector>
#include <tuple>
#include <memory>

#define STRINGIZE( cad ) #cad

#define PREFR_ATRIB( name, type ) \
  protected: \
    std::vector< type > _##name##Vector; /*! Vector storing name attribute. */ \
  public: \
    type& p##name( unsigned int i ){ return _##name##Vector[ i ]; } \
    void p##name( unsigned int i, const type& value ) \
      { _##name##Vector[ i ] = value; }

#define PREFR_ATRIB_BOOL( name ) \
  protected: \
    std::vector< char > _##name##Vector; \
  public: \
    bool p##name( unsigned int i ){ return _##name##Vector[ i ]; } \
    void p##name( unsigned int i, const bool& value ) \
      { _##name##Vector[ i ] = value; }

#define PREFR_CONST_IT_ATRIB( name, type ) \
  protected: \
    type* _##name##_ptr; \
  public: \
    type name( void ) const { return *_##name##_ptr; }

#define PREFR_CONST_IT_ATRIB_BOOL( name ) \
  protected: \
    char* _##name##_ptr; \
  public: \
    bool name( void ) const { return *_##name##_ptr; }


#define PREFR_IT_ATRIB( name, type ) \
  public: \
    void set_##name( const type& value ){ *_##name##_ptr = value; }


namespace prefr
{
  class Particles;
  class ParticleCollection;

  typedef glm::vec3 TVect3;
  typedef glm::vec4 TVect4;

  typedef ParticleCollection ParticleRange;
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
    ALIVE
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
  class Particles : public boost::noncopyable
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

    iterator _createIterator( unsigned int i ) const;

    const TParticle& vectorReferences( void ) const;

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

    PREFR_ATRIB_BOOL( alive )

    unsigned int _size;

    TParticle _vectorReferences;
  };

  class Particles::base_const_iterator
  {

    virtual ~base_const_iterator( void );

    friend class Particles;
    friend class ParticleCollection;

  public:

    base_const_iterator( const base_const_iterator& other );

//    virtual int operator+( const base_const_iterator& other ) const;
//    virtual int operator-( const base_const_iterator& other ) const;
//
//    virtual int operator+( const base_iterator& other ) const;
//    virtual int operator-( const base_iterator& other ) const;
//
//    virtual base_const_iterator operator+( int increase );
//    virtual base_const_iterator operator-( int decrease );
//
//    virtual bool operator== ( const base_const_iterator& other ) const;
//    virtual bool operator!= ( const base_const_iterator& other ) const;

  protected:

    base_const_iterator( void );

    void set( unsigned int index_ = 0 );
    void increase( int inc );
    void decrease( int dec );

    bool compare( const base_const_iterator& other ) const ;
    int difference( const base_const_iterator& other ) const;

    unsigned int _position;
    unsigned int _size;

    const Particles* _data;
    TParticle _vectorRef;

    PREFR_CONST_IT_ATRIB( id, unsigned int )
    PREFR_CONST_IT_ATRIB( life, float )
    PREFR_CONST_IT_ATRIB( size, float )
    PREFR_CONST_IT_ATRIB( position, glm::vec3 )
    PREFR_CONST_IT_ATRIB( color, glm::vec4 )
    PREFR_CONST_IT_ATRIB( velocityModule, float )
    PREFR_CONST_IT_ATRIB( velocity, glm::vec3 )
    PREFR_CONST_IT_ATRIB( accelerationModule, float )
    PREFR_CONST_IT_ATRIB( acceleration, glm::vec3 )
    PREFR_CONST_IT_ATRIB_BOOL( alive )
  };

  class Particles::base_iterator : public Particles::base_const_iterator
  {

  public:

    base_iterator( const base_const_iterator& other );
    virtual ~base_iterator( void );

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
    PREFR_IT_ATRIB( alive, bool )

  };

  class Particles::iterator : public Particles::base_iterator
  {
    friend class Particles;
    friend class Particles::const_iterator;

  public:

    iterator( void );
    iterator( const base_iterator& other );
    iterator( const Particles::const_iterator& other );

    ~iterator( ){ }

    iterator& operator++( void );
    iterator operator++( int );
    iterator& operator--( void );
    iterator operator--( int );

    virtual int operator+( const iterator& other ) const;
    virtual int operator-( const iterator& other ) const;

    virtual iterator operator+( int increase );
    virtual iterator operator-( int decrease );

    virtual bool operator== ( const iterator& other ) const;
    virtual bool operator!= ( const iterator& other ) const;


//    virtual auto operator*( void );
//    virtual auto operator->( void );

  };

  class Particles::const_iterator : public Particles::base_const_iterator
  {
    friend class Particles;
    friend class Particles::iterator;

  public:

    const_iterator( void );
    const_iterator( const Particles::base_const_iterator& other );
    const_iterator( const Particles::iterator& other );
    virtual ~const_iterator( ){ }

    const_iterator& operator++( void );
    const_iterator operator++( int );
    const_iterator& operator--( void );
    const_iterator operator--( int );

    virtual int operator+( const const_iterator& other ) const;
    virtual int operator-( const const_iterator& other ) const;

    virtual const_iterator operator+( int increase );
    virtual const_iterator operator-( int decrease );

    virtual bool operator== ( const const_iterator& other ) const;
    virtual bool operator!= ( const const_iterator& other ) const;

  };


  typedef std::vector< unsigned int > ParticleIndices;

  class ParticleCollection
  {

  public:

    class iterator;
    class const_iterator;

    typedef ParticleCollection::iterator iterator;

    friend class iterator;
    friend class const_iterator;
    friend class Particles;
    friend class ParticleSystem;

    ParticleCollection( void );
    ParticleCollection( const ParticleCollection& other );

    const ParticleIndices& indices( void ) const;
    void indices( const ParticleIndices& newIndices );

    unsigned int size( void );

    iterator begin( void );
    const_iterator begin( void ) const;

    iterator end( void );
    const_iterator end( void ) const;

    iterator at( unsigned int index_ );

  protected:

    ParticleCollection( const Particles& data );
    ParticleCollection( const Particles& data, const ParticleIndices& indices_ );
    ParticleCollection( const Particles& data,
                        unsigned int begin_,
                        unsigned int end_ );

    ParticleCollection( const Particles& data,
                        Particles::iterator begin_,
                        Particles::iterator end_ );

    iterator _createIterator( unsigned int index = 0 ) const;

    ParticleIndices _particleIndices;
    TParticle _vectorReferences;

    unsigned int _size;

    const Particles* _data;
  };

  class ParticleCollection::iterator : public Particles::base_iterator
  {

    friend class ParticleCollection;
    friend class ParticleCollection::const_iterator;

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

  protected:

    unsigned int _indexPosition;
    const ParticleIndices* _particleIndices;

  };

  class ParticleCollection::const_iterator : public Particles::base_const_iterator
  {

    friend class ParticleCollection;
    friend class ParticleCollection::iterator;

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

    ParticleCollection* collection( void ) const;

  protected:

    unsigned int _indexPosition;
    const ParticleIndices* _particleIndices;

  };


  typedef Particles::iterator tparticle;
  typedef tparticle* tparticle_ptr;

}
#endif /* __PREFR__PARTICLES__ */
