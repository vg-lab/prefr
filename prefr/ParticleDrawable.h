/*
 * ParticleDrawable.h
 *
 *  Created on: 17/11/2014
 *      Author: sgalindo
 */

#ifndef PARTICLEDRAWABLE_H_
#define PARTICLEDRAWABLE_H_

#include <osg/Drawable>

namespace prefr
{
  class ParticleDrawable : public osg::Drawable
  {
  public:

    std::vector<float> vertices;
    std::vector<float> color;

    virtual osg::BoundingBox computeBound() const;
    virtual void compileGLObjects(osg::RenderInfo& renderInfo) const;
    virtual void drawImplementation(osg::RenderInfo& renderInfo) const;
    virtual void accept(osg::PrimitiveFunctor& functor) const;
    virtual void releaseGLObjects(osg::State* state) const;



  };


}



#endif /* PARTICLEDRAWABLE_H_ */
