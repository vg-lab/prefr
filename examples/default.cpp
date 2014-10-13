#include <particles/ParticleSystem.h>

using namespace particles;

int main(int argc, char** argv)
{
  ParticleSystem ps(10, 10, 10, true);

  delete (&ps);
}
