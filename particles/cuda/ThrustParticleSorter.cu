#include "../Definitions.h"
#include "ThrustParticleSorter.cuh"


namespace particles
{
    namespace defaultParticleSystem
    {

      ThrustParticleSorter::ThrustParticleSorter(ParticleCollection* arrayParticles, DistanceArray* distanceArray)
      : DefaultParticleSorter(arrayParticles, distanceArray)
      {}

      void ThrustParticleSorter::Sort(SortOrder order)
      {
        thrust::host_vector<int> hostID(distances->ids->begin(), distances->ids->end());//at(distances->ids->size()-1));
        thrust::device_vector<int> devID = hostID;

        thrust::host_vector<double> hostDistance(distances->distances->begin(), distances->distances->end());//at(distances->distances->size()-1));
        thrust::device_vector<double> devDistance = hostDistance;

        thrust::stable_sort_by_key(devDistance.begin(), devDistance.end(), devID.begin(), thrust::greater<double>());

        thrust::copy(devID.begin(), devID.end(), hostID.begin());
      }

//      void ThrustParticleSorter::UpdateCameraDistance(vec3 cameraPosition)
//      {
//
//      }
//      void ThrustParticleSorter::UpdateCameraDistance(unsigned int i, vec3 cameraPosition){}


    }

}
