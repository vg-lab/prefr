#ifndef THRUSTPARTICLESORTER_H_
#define THRUSTPARTICLESORTER_H_

#include "../Sorter.h"
#include "../types.h"
#include "CUDADistanceArray.cuh"

namespace prefr
{

  class ThrustSorter : public Sorter
  {
  public:

    PREFR_API ThrustSorter(  );
    PREFR_API virtual ~ThrustSorter();

    PREFR_API void Sort(SortOrder order);

    virtual void UpdateCameraDistance( const glm::vec3& cameraPosition,
                                       bool discardDeadParticles = true );

    virtual void UpdateCameraDistance( const tparticle_ptr current,
                                       const glm::vec3& cameraPosition,
                                       bool discardDeadParticles = true );

    virtual void InitDistanceArray();
  };

} // namespace prefr


#endif
