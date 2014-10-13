#define particles_VERSION_MAJOR 0
#define particles_VERSION_MINOR 1
#define particles_VERSION_PATCH 0
#define particles_VERSION 0.1.0

#pragma once

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include <FreeImage.h>

#include <iostream>

#define degreesToRadians( degrees ) ( ( degrees ) / 180.0 * M_PI )
#define radiansToDegrees( radians ) ( ( radians ) * ( 180.0 / M_PI ) )

#define WINDOW_TITLE "Particle Engine - Splatting"
#define OGLVER 3
#define OGLSUBVER 3
#define BG_COLOR 0.86f, 0.823f, 0.823f, 1.0f
#define SCREEN_SIZE 800, 600

//#define fov 60.0f
#define fov 1.05f
#define defaultAspect 1.0f
#define nearPlane 0.3f
#define	farPlane 20000.0f

#include <vector>
#include "Particle.h"
#include "ElementCollection.hpp"


namespace particles
{
  typedef particles::Particle tparticle;
  typedef tparticle* tparticleptr;
  typedef std::vector<tparticleptr> tparticleContainer;
  typedef utils::ElementCollection<tparticle> ParticleCollection;


}

// Use this to debug a single particle (surprisely not working after all)
//#define PARTICLE_DEBUG
