/*
 * ParticleData.h
 *
 *  Created on: 12 de ene. de 2016
 *      Author: sgalindo
 */

#ifndef PREFR_PARTICLES_H_
#define PREFR_PARTICLES_H_

#include <boost/noncopyable.hpp>

#include "types.h"
#include "ElementCollection.hpp"

#include <vector>

#define STRINGIZE( cad ) #cad

#define PREFR_ATRIB( name, type ) \
  protected: \
    std::vector< type > _##name##Vector; \
  public: \
    type& p##name( unsigned int i ){ return _##name##Vector[ i ]; } \
    void p##name( unsigned int i, const type& value ){ _##name##Vector[ i ] = value; }

#define PREFR_ATRIB_BOOL( name, type ) \
  protected: \
    std::vector< type > _##name##Vector; \
  public: \
    type p##name( unsigned int i ){ return _##name##Vector[ i ]; } \
    void p##name( unsigned int i, const type& value ){ _##name##Vector[ i ] = value; }

#define PREFR_HANDLER( name, type ) \
    protected: \
      std::vector< type* > _##name##s; \
      std::vector< unsigned int > _##particles##name; \
    public: \
      type* name( unsigned int i ){ return _##name##s[ i ]; } \
      void add##name( type* handler ){ _##name##s.push_back( handler ); } \
      void remove##name( unsigned int i ) \
        { _##name##s.erase( _##name##s.begin( ) + i, \
                            _##name##s.begin( ) + i + 1 );}


#define PREFR_ATRIB_IT( name, type ) \
    protected: \
      std::vector< type >::iterator name##Iterator; \
    public: \
      type name( void ) const { return *name##Iterator; } \
      void name( const type& value ){ *name##Iterator = value; }

#define PREFR_ATRIB_BOOL_IT( name, type ) \
    protected: \
      std::vector< type >::iterator name##Iterator; \
    public: \
      type name( void ) const { return *name##Iterator; } \
      void name( const type& value ){ *name##Iterator = value; }

#define PREFR_ATRIB_CONST_IT( name, type ) \
    protected: \
      std::vector< type >::const_iterator name##ConstIterator; \
    public: \
      type name( void ) const { return *name##ConstIterator; }

namespace prefr
{
  class Particles;
}

typedef utils::ElementCollection< prefr::Particles > ParticleRange;
typedef utils::ElementCollection< prefr::Particles > ParticleCollection;

namespace prefr
{


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
      ALIVE,
      NEWBORN,
      DISTANCE
    };


    class iterator;
    class const_iterator;

    Particles( );
    Particles( unsigned int size );

    ~Particles( );

    unsigned int numParticles( void ) const;
    void resize( unsigned int newSize );
    void clear( void );

    iterator begin( void );
    const_iterator begin( void ) const;

    iterator end( void );
    const_iterator end( void ) const;

    ParticleRange range( void );

    iterator at( unsigned int i );
    const_iterator at( unsigned int i) const;

  protected:

    class base_iterator;

    iterator CreateIterator( unsigned int i );
    const_iterator CreateConstIterator( unsigned int i ) const;

//    std::vector< unsigned int > idVector;
//    std::vector< float > lifeVector;
//    std::vector< float > sizeVector;
//    std::vector< glm::vec3 > positionVector;
//    std::vector< glm::vec4 > colorVector;
//    std::vector< glm::vec3 > velocityVector;
//    std::vector< glm::vec3 > accelerationVector;
//
//    std::vector< bool > aliveVector;
//    std::vector< bool > newbornVector;
//
//    std::vector< float > distanceVector;

    PREFR_ATRIB( id, unsigned int )
    PREFR_ATRIB( life, float )
    PREFR_ATRIB( size, float )
    PREFR_ATRIB( position, glm::vec3 )
    PREFR_ATRIB( color, glm::vec4 )
    PREFR_ATRIB( velocityModule, float )
    PREFR_ATRIB( velocity, glm::vec3 )
    PREFR_ATRIB( acceleration, glm::vec3 )

    PREFR_ATRIB_BOOL( alive, bool )
    PREFR_ATRIB_BOOL( newborn, bool )



    unsigned int _size;

  };

//  class Particles::base_iterator
//  {
//
//  protected:
//
//    base_iterator( void );
//    base_iterator( const base_iterator& other );
//    virtual ~base_iterator( );
//
//    virtual base_iterator& operator++( void ) = 0;
//    virtual base_iterator& operator--( void ) = 0;
//
//    virtual base_iterator operator+( int increase ) = 0;
//    virtual base_iterator operator-( int decrease ) = 0;
//
//    int operator+( const base_iterator& other );
//    int operator-( const base_iterator& other );
//
//
//
//    virtual bool operator== ( const base_iterator& other );
//    virtual bool operator!= ( const base_iterator& other );
//
//    long _position;
//    unsigned int _size;
//
//    const Particles* _data;
//
//  };

  class Particles::iterator
  : public std::iterator<std::bidirectional_iterator_tag, Particles>
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

//    iterator* operator->( void ) const;
//    iterator& operator*( void ) const;

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
    PREFR_ATRIB_BOOL_IT( newborn, bool )

  };

  class Particles::const_iterator
  : public std::iterator<std::bidirectional_iterator_tag, Particles>
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

//    const_iterator* operator->( void );
//    const_iterator& operator*( void );

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
    PREFR_ATRIB_CONST_IT( newborn, bool )

  };

  typedef Particles::iterator tparticle;
  typedef tparticle* tparticle_ptr;

}
#endif /* PREFR_PARTICLES_H_ */
