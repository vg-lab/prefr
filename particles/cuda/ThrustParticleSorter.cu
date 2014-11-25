#include "ThrustParticleSorter.cuh"


namespace particles
{
  namespace defaultParticleSystem
  {

    namespace CUDATHRUST
    {
    
      ThrustParticleSorter::ThrustParticleSorter(ParticleCollection* arrayParticles)
      : ParticleSorter(arrayParticles)
      {}

      void ThrustParticleSorter::InitDistanceArray()
      {
        distances = new CUDADistanceArray( particles->size );

        CUDADistanceArray* cda = static_cast<CUDADistanceArray*>(distances);

        cda->deviceID.resize( particles->size );
        cda->deviceDistances.resize( particles->size );
      }


      void ThrustParticleSorter::Sort(SortOrder order)
      {

        CUDADistanceArray* cda = static_cast<CUDADistanceArray*>(distances);


        thrust::copy(cda->ids->begin(), cda->ids->end(), cda->deviceID.begin());
        thrust::copy(cda->distances->begin(), cda->distances->end(), cda->deviceDistances.begin());

        if (order == SortOrder::Ascending)
          thrust::sort_by_key(cda->deviceDistances.begin(), cda->deviceDistances.end()
                              , cda->deviceID.begin(), thrust::less<float>());
        else
          thrust::sort_by_key(cda->deviceDistances.begin(), cda->deviceDistances.end()
                              , cda->deviceID.begin(), thrust::greater<float>());

        thrust::copy(cda->deviceID.begin(), cda->deviceID.end(), cda->ids->begin());

      }

      void ThrustParticleSorter::UpdateCameraDistance(const glm::vec3& cameraPosition)
      {
        for (tparticleContainer::iterator it = particles->start; it != particles->end; it++)
        {
          UpdateCameraDistance((*it)->id, cameraPosition);
        }
      }
      void ThrustParticleSorter::UpdateCameraDistance(unsigned int i,  const glm::vec3& cameraPosition)
      {

        tparticle_ptr current = particles->elements->at(i);
        distances->ids->at(i) = current->id;
        distances->distances->at(i) = current->Alive() ?  glm::length(current->position - cameraPosition) : -1;

      }

    }

  }

}
