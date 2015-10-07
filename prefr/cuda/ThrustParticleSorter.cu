
#include "ThrustParticleSorter.cuh"

#include <thrust/sort.h>
#include <thrust/copy.h>

namespace prefr
{

      ThrustParticleSorter::ThrustParticleSorter( const ParticleCollection& arrayParticles)
      : ParticleSorter(arrayParticles)
      {}

      void ThrustParticleSorter::InitDistanceArray()
      {
        distances = new CUDADistanceArray( particles->size );

        CUDADistanceArray* cda = static_cast<CUDADistanceArray*>(distances);

        cda->deviceID.resize( particles->size );
        cda->deviceDistances.resize( particles->size );
      }

      ThrustParticleSorter::~ThrustParticleSorter()
      {

      }

      void ThrustParticleSorter::Sort(SortOrder order)
      {

        CUDADistanceArray* cda = static_cast<CUDADistanceArray*>(distances);

        std::vector<int>::iterator hostidbegin = distances->ids->begin();
        std::vector<int>::iterator hostidend = hostidbegin + aliveParticles;
        std::vector<float>::iterator hostdistbegin = distances->distances->begin();
        std::vector<float>::iterator hostdistend = hostdistbegin  + aliveParticles;
        thrust::device_vector<int>::iterator deviceidbegin = cda->deviceID.begin();
        thrust::device_vector<int>::iterator deviceidend = deviceidbegin + aliveParticles;
        thrust::device_vector<float>::iterator devicedistbegin = cda->deviceDistances.begin();
        thrust::device_vector<float>::iterator devicedistend = devicedistbegin + aliveParticles;


    //    thrust::copy(cda->ids->begin(), hostidend, deviceidbegin);
        thrust::sequence(deviceidbegin, deviceidend);
        thrust::copy(hostdistbegin, hostdistend, devicedistbegin);

        if (order == SortOrder::Ascending)
          thrust::sort_by_key(devicedistbegin, devicedistend
                              , deviceidbegin, thrust::less<float>());
        else
          thrust::sort_by_key(devicedistbegin, devicedistend
                              , deviceidbegin, thrust::greater<float>());

        thrust::copy(deviceidbegin, deviceidend, hostidbegin);

      }

      void ThrustParticleSorter::UpdateCameraDistance(const glm::vec3& cameraPosition)
      {
        aliveParticles = 0;
        distances->ResetCounter();

    #ifdef DEBUG
        CUDADistanceArray* cda = static_cast<CUDADistanceArray*>(distances);
        for (auto it = cda->translatedIDs.begin(); it != cda->translatedIDs.end(); it++)
        {
          (*it) = -1;
        }
    #endif

        for (EmissionNodesArray::iterator nodit = emissionNodes->begin();
                 nodit != emissionNodes->end();
                 nodit++)
        {
          if (!(*nodit) || !(*nodit)->Active())
            continue;

          for (tparticleContainer::iterator it = (*nodit)->particles->start;
               it != (*nodit)->particles->end;
               it++)
          {
            if ((*it)->Alive())
            {
              UpdateCameraDistance((*it), cameraPosition);
              aliveParticles++;
            }
          }
        }
      }
      void ThrustParticleSorter::UpdateCameraDistance(const tparticle_ptr current,  const glm::vec3& cameraPosition)
      {
        DistanceUnit& dist = distances->next();
        CUDADistanceArray* cda = static_cast<CUDADistanceArray*>(distances);
        cda->translatedIDs[distances->current] = current->id;

        (*distances->distances)[distances->current] = current->Alive() ?  length2(current->position - cameraPosition) : -1;
    //        (*distances->distances)[current->id] = current->Alive() ?  length2(current->position - cameraPosition) : -1;

      }

}
