/*
 * @file	collection.cpp
 * @brief
 * @author Sergio E. Galindo <sergio.galindo@urjc.es> 
 * @date
 * @remarks Copyright (c) GMRV/URJC. All rights reserved.
 *					Do not distribute without further notice.
 */

#include <prefr/prefr.h>
#include <iostream>

using namespace prefr;

int main( int /*argc*/, char** /*argv*/ )
{

  ParticleSystem* ps = new ParticleSystem( 200 );

  Cluster* c = new Cluster( );
  ps->addCluster( c, 0, 100 );

  ParticleCollection pc1 = c->particles( );

  for( ParticleCollection::iterator it = pc1.begin( ); it != pc1.end( ); it++ )
  {
    std::cout << " " << it.id( );
  }
  std::cout << std::endl;

  c = new Cluster( );
  ps->addCluster( c, 100, 100 );

  ParticleCollection pc2 = c->particles( );

  for( ParticleCollection::iterator it = pc2.begin( ); it != pc2.end( ); it++ )
  {
    std::cout << " " << it.id( );
  }
  std::cout << std::endl;

  delete ps;


}
