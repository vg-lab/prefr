#include <particles/ParticleSystem.h>
#include <particles/default/DefaultParticleSystem.h>

using namespace particles::defaultParticleSystem;

int main(int argc, char** argv)
{
  DefaultGLParticleSystem ps(10, 10, 10, true);

  delete (&ps);
}
