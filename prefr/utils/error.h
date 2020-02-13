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

#ifndef __PREFR_ERROR__
#define __PREFR_ERROR__

#include "Log.h"
#include <stdexcept>
#include <iostream>

#define PREFR_THROW( msg )                                       \
  {                                                              \
    prefr::Log::log( msg, prefr::LOG_LEVEL_ERROR );              \
    throw std::runtime_error( msg );                             \
  }


#define PREFR_CHECK_THROW( cond, errorMsg )                      \
  {                                                              \
    if ( ! (cond) ) PREFR_THROW( errorMsg );                     \
  }


#ifdef DEBUG
#define PREFR_DEBUG_CHECK( cond, errorMsg )                      \
  {                                                              \
    PREFR_CHECK_THROW( cond, errorMsg )                          \
  }
#else
#define PREFR_DEBUG_CHECK( cond, errorMsg )
#endif


#endif // __PREFR_ERROR__
