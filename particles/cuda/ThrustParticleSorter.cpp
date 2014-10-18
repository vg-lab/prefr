#include "ThrustParticleSorter.cu"


namespace particles
{
    namespace defaultParticleSystem
    {

      ThrustParticleSorter::ThrustParticleSorter(ParticleCollection* arrayParticles, DistanceArray* distanceArray)
      : DefaultParticleSorter(arrayParticles, distanceArray)
      {}

      void ThrustParticleSorter::Sort(SortOrder order)
      {
//        thrust::host_vector<int> hostID(distances->ids->at(0), distances->ids->at(distances->ids->size()-1));
//        thrust::device_vector<int> devID = hostID;
//
//        thrust::host_vector<float> hostDistance(distances->distances->at(0), distances->distances->at(distances->distances->size()-1));
//        thrust::device_vector<float> devDistance = hostDistance;
      }

//      void ThrustParticleSorter::UpdateCameraDistance(vec3 cameraPosition)
//      {
//
//      }
//      void ThrustParticleSorter::UpdateCameraDistance(unsigned int i, vec3 cameraPosition){}


    }

}
