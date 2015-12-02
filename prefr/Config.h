/**
 * @file    Config.h
 * @brief
 * @date
 * @remarks Copyright (c) GMRV/URJC. All rights reserved.
 * Do not distribute without further notice.
 */
#ifndef __PREFR_CONFIG__
#define __PREFR_CONFIG__

#include <prefr/api.h>

namespace prefr
{
  class PREFR_API Config
  {

  public:
    static void init( void );
    static bool isInitialized( void );

  private:
    static bool _initialized;

  };

} // namespace prefr

#endif //__PREFR_CONFIG__
