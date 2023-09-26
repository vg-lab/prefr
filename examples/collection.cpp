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

#include <prefr/prefr.h>
#include <iostream>

using namespace prefr;

void printIndices( const ParticleCollection& collection, const std::string& name )
{
  std::cout << "--------------------- " << name << ": ";

  prefr::Particles::const_iterator begin = collection.begin( );
  prefr::Particles::const_iterator end = collection.end( );

  std::cout << std::endl;

  for( auto it : collection )
    std::cout << " " << it.id( );

  std::cout << std::endl;

  std::cout << "--------------------- " << std::endl;
}

int main( int /*argc*/, char** /*argv*/ )
{

  ParticleSystem* ps = new ParticleSystem( 100 );

  // Create indices.
  ParticleIndices indices = { 0, 1, 5, 10, 20 };

  // Create collection from indices.
  ParticleCollection one =  ps->createCollection( indices );

  printIndices( one, "one");

  // Copy collections.
  ParticleCollection two = one;
  ParticleCollection three( two );

  printIndices( two, "two");
  printIndices( three, "three");

  ParticleSet aux = { 21, 23, 50, 99 };

  // Add indices to collection
  two.addIndices( aux );

  printIndices( two, "two");

  // Transfer the given indices to the given collection.
  two.transferIndicesTo( three, aux );

  printIndices( two, "two");
  printIndices( three, "three");

  three.removeIndices( indices );

  printIndices( three, "three" );

  two.addIndices( indices );

  printIndices( two, "two");

  three.addIndices( two.indices( ));

  printIndices( three, "three" );

  three.removeIndices( indices );

  printIndices( three, "three" );

  three.addIndices( indices );

  printIndices( three, "three" );

  three.removeIndices( two.indices( ));

  printIndices( three, "three" );

  three.removeIndices( three.indices( ));

  printIndices( three, "three" );


  delete ps;
}
