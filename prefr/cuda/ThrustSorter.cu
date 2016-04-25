
#include <thrust/sort.h>
#include <thrust/copy.h>
#include "ThrustSorter.cuh"

namespace prefr
{

      ThrustSorter::ThrustSorter( )
      {}

      void ThrustSorter::InitDistanceArray()
      {
        distances = new CUDADistanceArray( _particles.size );

        CUDADistanceArray* cda = static_cast<CUDADistanceArray*>(distances);

        cda->deviceID.resize( _particles.size );
        cda->deviceDistances.resize( _particles.size );
      }

      ThrustSorter::~ThrustSorter()
      {

      }

      void ThrustSorter::Sort(SortOrder order)
      {

        CUDADistanceArray* cda = static_cast<CUDADistanceArray*>(distances);

        std::vector<int>::iterator hostidbegin = distances->ids->begin();
        std::vector<int>::iterator hostidend = hostidbegin + _aliveParticles;
        std::vector<float>::iterator hostdistbegin = distances->distances->begin();
        std::vector<float>::iterator hostdistend = hostdistbegin  + _aliveParticles;
        thrust::device_vector<int>::iterator deviceidbegin = cda->deviceID.begin();
        thrust::device_vector<int>::iterator deviceidend = deviceidbegin + _aliveParticles;
        thrust::device_vector<float>::iterator devicedistbegin = cda->deviceDistances.begin();
        thrust::device_vector<float>::iterator devicedistend = devicedistbegin + _aliveParticles;


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

      void ThrustSorter::UpdateCameraDistance( const glm::vec3& cameraPosition,
                                                       bool renderDeadParticles )
      {
        SerializeAttributes( );

        _aliveParticles = 0;
        distances->ResetCounter();

    #ifdef DEBUG
        CUDADistanceArray* cda = static_cast<CUDADistanceArray*>(distances);
        for (auto it = cda->translatedIDs.begin(); it != cda->translatedIDs.end(); it++)
        {
          (*it) = -1;
        }
    #endif

        for( auto cluster : *_clusters )
        {
          if( cluster->active( ) || renderDeadParticles )
          {
            for( tparticle particle = _particles.begin( );
                 particle != _particles.end( );
                 particle++ )
            {
              UpdateCameraDistance( &particle, cameraPosition, renderDeadParticles );
              _aliveParticles++;
            }
          }
        }


      }
      void ThrustSorter::UpdateCameraDistance( const tparticle_ptr current, 
                                               const glm::vec3& cameraPosition,
                                               bool renderDeadParticles )
      {
        DistanceUnit& dist = distances->next();
        CUDADistanceArray* cda = static_cast<CUDADistanceArray*>(distances);
        cda->translatedIDs[distances->current] = current->id( );

        (*distances->distances)[distances->current] =
            current->alive( )  || renderDeadParticles ?
            length2(current->position( ) - cameraPosition) : -1;
    //        (*distances->distances)[current->id] = current->Alive() ?  length2(current->position - cameraPosition) : -1;

      }

}
