/**
 * @file    error.h
 * @brief
 * @author  Pablo Toharia <pablo.toharia@urjc.es>
 * @date
 * @remarks Copyright (c) GMRV/URJC. All rights reserved.
            Do not distribute without further notice.
 */
#ifndef __PREFR_ERROR__
#define __PREFR_ERROR__

#include <stdexcept>

#ifdef DEBUG
  #define PREFR_LOG( msg )                                       \
    std::cerr << "PREFR "                                        \
              << __FILE__ << "("                                 \
              << __LINE__ << "): "                               \
              << msg << std::endl;
#else
  #define PREFR_LOG( msg )
#endif


#define PREFR_THROW( msg )                                       \
  {                                                              \
    PREFR_LOG( msg );                                            \
    throw std::runtime_error( msg );                             \
  }


#define PREFR_CHECK_THROW( cond, errorMsg )                      \
    {                                                            \
      if ( ! (cond) ) PREFR_THROW( errorMsg );                   \
    }


#ifdef DEBUG
  #define PREFR_DEBUG_CHECK( cond, errorMsg )                    \
{                                                                \
  PREFR_CHECK_THROW( cond, errorMsg )                            \
}
#else
  #define PREFR_DEBUG_CHECK( cond, errorMsg )
#endif


#endif // __PREFR_ERROR__
