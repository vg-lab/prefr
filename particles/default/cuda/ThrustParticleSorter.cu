#include "ThrustParticleSorter.cuh"


namespace particles
{
  namespace defaultParticleSystem
  {

    namespace CUDATHRUST
    {
    
      ThrustParticleSorter::ThrustParticleSorter(ParticleCollection* arrayParticles, distanceArray* distanceArray)
      : ParticleSorter(arrayParticles)
      , distances(distanceArray)
      {}

      void ThrustParticleSorter::Sort(SortOrder order)
      {
        //        thrust::host_vector<int> hostID(distances->ids->begin(), distances->ids->end());//at(distances->ids->size()-1));
        //        thrust::device_vector<int> devID = hostID;
        //
        //        thrust::host_vector<float> hostDistance(distances->distances->begin(), distances->distances->end());//at(distances->distances->size()-1));
        //        thrust::device_vector<float> devDistance = hostDistance;
        //
        //        thrust::sort_by_key(devDistance.begin(), devDistance.end(), devID.begin(), thrust::greater<float>());
        //
        //        thrust::copy(devID.begin(), devID.end(), hostID.begin());
        //        //thrust::copy(devDistance.begin(), devDistance.end(), hostDistance.begin());
        //
        ////        std::cout << "sorting..." << std::endl;
        //        for (int i = 0; i < distances->ids->size(); i++)
        //        {
        //          //std::cout << distances->ids->at(i) << ":" << distances->distances->at(i) << " " << hostID[i] << ":" << hostDistance[i] << std::endl;
        //          distances->ids->at(i) = hostID[i];
        //          distances->elements->at(i).id = &distances->ids->at(i);
        //          //distances->distances->at(i) = hostDistance[i];
        //          distances->elements->at(i).distance = &distances->distances->at(i);
        //        }

        //***************************************************



        thrust::copy(distances->hostID.begin(), distances->hostID.end(), distances->deviceID.begin());
        thrust::copy(distances->hostDistances.begin(), distances->hostDistances.end(), distances->deviceDistances.begin());

        thrust::sort_by_key(distances->deviceDistances.begin(), distances->deviceDistances.end()
                            , distances->deviceID.begin(), thrust::greater<float>());

        thrust::copy(distances->deviceID.begin(), distances->deviceID.end(), distances->hostID.begin());

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
//        tparticleptr current = particles->elements->at(i);
//        distances->ids->at(i) = current->id;
//        distances->distances->at(i) = current->Alive() ?  glm::length(current->position - cameraPosition) : -1;

        //************************


        tparticle_ptr current = particles->elements->at(i);
        distances->hostID[i] = current->id;
        distances->hostDistances[i] = current->Alive() ?  glm::length(current->position - cameraPosition) : -1;

      }

    }

  }

}
