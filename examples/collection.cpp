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

#include <prefr/prefr.h>
#include <iostream>

using namespace prefr;

void printIndices( const ParticleCollection& collection, const std::string& name )
{
  std::cout << name << ": ";
  for( auto const &it : collection )
  {
    std::cout << " " << it.id( );
  }
  std::cout << std::endl;

}

int main( int /*argc*/, char** /*argv*/ )
{

  ParticleSystem* ps = new ParticleSystem( 100 );

  // Create indices.
  ParticleIndices indices = { 0, 1, 5, 10, 20 };

  // Create collection from indices.
  ParticleCollection pc1 =  ps->createCollection( indices );

  printIndices( pc1, "Collection 1");

  // Copy collections.
  ParticleCollection pc2 = pc1;
  ParticleCollection pc3( pc2 );

  printIndices( pc2, "Collection 2");

  ParticleSet aux = { 21, 23, 50, 99 };

  // Add indices to collection
  pc2.addIndices( aux );

  printIndices( pc2, "Collection 2");
  printIndices( pc3, "Collection 3");

  // Transfer the given indices to the given collection.
  pc2.transferIndicesTo( pc3, aux );

  printIndices( pc2, "Collection 2");
  printIndices( pc3, "Collection 3");

  delete ps;
}
