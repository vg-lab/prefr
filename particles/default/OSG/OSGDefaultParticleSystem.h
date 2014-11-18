/*
 * OSGDefaultParticleSystem.h
 *
 *  Created on: 28/10/2014
 *      Author: sgalindo
 */

#ifndef OSGDEFAULTPARTICLESYSTEM_H_
#define OSGDEFAULTPARTICLESYSTEM_H_

#include "../DefaultParticleSystem.h"

#include <osg/Array>

#include <osg/NodeVisitor>

#include <osgGA/StandardManipulator>


namespace particles
{
  namespace defaultParticleSystem
  {

    namespace OSGParticleSystem
    {

      class OSGPSNodeCallBack : public osg::NodeCallback
      {
      public:

        void operator()(osg::Node* node, osg::NodeVisitor* nv)
        {
//          osg::ref_ptr<OSGDefaultParticleSystem> osgps =
//              dynamic_cast<OSGDefaultParticleSystem*>( node->getUserData() );
//
//          if (osgps)
//          {
//            osgps->Update(0.1f);
//          }
//
//          traverse(node, nv);
        }

      };

      class OSGDefaultParticleSystem : public DefaultParticleSystem, public osg::Referenced
      {
      public:

        osg::ref_ptr<osg::Vec3Array> vertices;
        osg::ref_ptr<osg::Vec4Array> positions;
        osg::ref_ptr<osg::Vec4Array> colors;

        OSGDefaultParticleSystem(int initialParticlesNumber, int _maxParticles
                                 , float _emissionRate, bool _loop = true
                                 , osgGA::StandardManipulator* cam = nullptr);

        virtual void UpdateCameraDistances(const glm::vec3& cameraPosition);
        virtual void UpdateRender();
        virtual void Render();

      };
    }

  }
}


#endif /* OSGDEFAULTPARTICLESYSTEM_H_ */
