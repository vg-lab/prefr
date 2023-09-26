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

#ifndef __PREFR__IRENDERPROGRAM___
#define __PREFR__IRENDERPROGRAM___

#include <prefr/api.h>

namespace prefr
{
  class PREFR_API IGLRenderProgram
  {
  public:

    virtual ~IGLRenderProgram( ){ }
    
    virtual void prefrActivateGLProgram( void ) = 0;
    
    virtual unsigned int prefrGLProgramID( void ) = 0;
    
    virtual inline
    const char* prefrViewProjectionMatrixAlias( void ) const
    {
      return _viewProjectionMatrixAlias.c_str( );
    }

    virtual inline
    const char* prefrViewMatrixUpComponentAlias( void ) const
    {
      return _viewMatrixUpComponentAlias.c_str( );
    }

    virtual inline
    const char* prefrViewMatrixRightComponentAlias( void ) const
    {
      return _viewMatrixRightComponentAlias.c_str( );
    }

  protected:

    std::string _viewProjectionMatrixAlias;
    std::string _viewMatrixUpComponentAlias;
    std::string _viewMatrixRightComponentAlias;
  };
}


#endif /* __PREFR__IRENDERPROGRAM___ */
