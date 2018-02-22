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

void printIndices( const ParticleCollection& collection, const std::string& name )
{
  std::cout << name << ": ";
  for( auto it : collection )
  {
    std::cout << " " << it.id( );
  }
  std::cout << std::endl;

}

int main( int /*argc*/, char** /*argv*/ )
{

  ParticleSystem* ps = new ParticleSystem( 100 );

  ParticleIndices indices = { 0, 1, 5, 10, 20 };

  ParticleCollection pc1 =  ps->createCollection( indices );

  auto begin = pc1.begin( );
  auto end = pc1.end( );

  printIndices( pc1, "Collection 1");

  ParticleCollection pc2 = pc1;
  ParticleCollection pc3( pc2 );

  printIndices( pc2, "Collection 2");

  ParticleIndices aux = { 21, 23, 50, 99 };
//  pc2.addIndex( 21 );
//  pc2.addIndex( 23 );
//  pc2.addIndex( 50 );
//  pc2.addIndex( 99 );

  pc2.addIndices( aux );

  printIndices( pc2, "Collection 2");
  printIndices( pc3, "Collection 3");

  pc2.transferIndicesTo( pc3, aux );

  printIndices( pc2, "Collection 2");
  printIndices( pc3, "Collection 3");


  delete ps;


}
