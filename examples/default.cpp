// PREFR includes
#include <prefr/prefr.h>
#include <shaderPath.h>



#include "CShader.h"

#include <iostream>

#define degreesToRadians( degrees ) ( ( degrees ) / 180.0 * M_PI )
#define radiansToDegrees( radians ) ( ( radians ) * ( 180.0 / M_PI ) )

#define WINDOW_TITLE "Particle Engine - Splatting"
#define OGLVER 3
#define OGLSUBVER 3
#define BG_COLOR 0.86f, 0.823f, 0.823f, 1.0f
#define SCREEN_SIZE 800, 600

#define fov 1.05f
#define DEFAULT_ASPECT 1.0f
#define nearPlane 0.3f
#define farPlane 20000.0f

using namespace prefr;

glm::mat4 projectionM;
glm::mat4 viewM;
glm::mat4 modelViewProjM;

glm::vec3 position = glm::vec3(0.0f, 4.0f, 10.0f);
glm::vec3 vforward = glm::vec3(0.0f,0.0f,-1.0f);
glm::vec3 vup = glm::vec3(0.0f,1.0f,0.0f);

glm::vec3 worldUp = glm::vec3(0.0f,1.0f,0.0f);

int resolution[] = {SCREEN_SIZE};

float aspectRatio=DEFAULT_ASPECT;

float deltaTime = 0.1f;

int mouseX, mouseY;

// Mouse sensitivity
float mouseSensitivity = degreesToRadians(100.0f);

// Mouse threshold in the borders of the screen to move constantly
float mouseThreshold = 0.15f;

// Calculated threshold depending on the resolution
float mouseXThreshold;
float mouseYThreshold;

CShader* particlesShader;


ParticleSystem* ps;

bool emit = true;

void initShaders()
{
  std::string vertPath, fragPath;
  fragPath = vertPath = std::string( PREFR_LIBRARY_BASE_PATH );
  vertPath.append("GL/shd/GL-vert.glsl");
  fragPath.append("GL/shd/GL-frag.glsl");
  particlesShader = new CShader(false, false,
                                vertPath.c_str() ,
                                fragPath.c_str()
    );
}

// Camera Movement
void MoveForward() {position += vforward;}
void MoveBackwards() {position -= vforward;}
void MoveUpwards() {position += worldUp;}
void MoveDownwards() {position -= worldUp;}
void MoveRight() {position += glm::cross(vforward, worldUp);}
void MoveLeft() {position -= glm::cross(vforward, worldUp);}

// Camera turning
void MouseTurnLeft ()
{
  glm::quat q=glm::angleAxis((float)(-mouseSensitivity*deltaTime),worldUp);
  vforward = glm::normalize(vforward*q);
}

void MouseTurnRight ()
{
  glm::quat q=glm::angleAxis((float)(mouseSensitivity*deltaTime),worldUp);
  vforward= glm::normalize(vforward*q);
}

void MouseTurnDown()
{
  glm::vec3 right=glm::cross(vforward,worldUp);
  glm::quat q=glm::angleAxis((float)(-mouseSensitivity*deltaTime),right);
  vforward=glm::normalize(vforward*q);
  vup=glm::normalize(vup*q);
}

void MouseTurnUp()
{
  glm::vec3 right=glm::cross(vforward,worldUp);
  glm::quat q=glm::angleAxis((float)(mouseSensitivity*deltaTime),right);
  vforward=glm::normalize(vforward*q);
  vup=glm::normalize(vup*q);
}


void makeProjectionMatrix( void )
{
  projectionM = glm::perspective(fov,aspectRatio,nearPlane,farPlane);
}

void makeModelViewProj(){
  //glm::mat4 modelM=glm::mat4(1.0f);
  viewM = glm::lookAt(position, position+vforward,worldUp);
  modelViewProjM = projectionM * viewM;
}



void mouseFunc (int /* button */, int /* state */,
                int  /* x */, int /* y */ )
{
}

void keyboardFunc(unsigned char key, int /* x */, int /* y */)
{
  switch (key)
  {
  case 'a':
    MoveLeft();
    break;

  case 'd':
    MoveRight();
    break;

  case 'w':
    MoveForward();
    break;

  case 's':
    MoveBackwards();
    break;

  case 'm':
    emit = !emit;
    break;

  case ' ':
    position = glm::vec3(0.0f, 4.0f, 10.0f);
    vforward = glm::vec3(0.0f,0.0f,-1.0f);
    vup = glm::vec3(0.0f,1.0f,0.0f);
    break;
  }

}

void processSpecialKeys (int key, int /* x */, int /* y */)
{
  if(key == GLUT_KEY_UP)
    MoveForward();
  else if(key== GLUT_KEY_DOWN)
    MoveBackwards();
  else if(key== GLUT_KEY_RIGHT)
    MoveRight();
  else if(key== GLUT_KEY_LEFT)
    MoveLeft();
}

void motionFunc (int x, int y)
{
  if (mouseX > x)
  {
    MouseTurnLeft();
  }
  else if (mouseX < x)
  {
    MouseTurnRight();
  }

  mouseX = x;

  if (mouseY > y)
  {
    MouseTurnDown();
  }
  else if (mouseY < y)
  {
    MouseTurnUp();
  }

  mouseY = y;
}

void ResolveMouseThreshold(int /* value */ )
{
  if (mouseX < mouseXThreshold)
    MouseTurnLeft();
  else if (mouseX > (resolution[0] - mouseXThreshold))
    MouseTurnRight();
  if (mouseY < mouseYThreshold)
    MouseTurnDown();
  if (mouseY > (resolution[1] - mouseYThreshold))
    MouseTurnUp();
}

void idleFunc (){

  glutPostRedisplay();

}

void rescaleFunc (GLsizei w, GLsizei h)
{
  glViewport (0, 0, w, h);
  aspectRatio=(float)w/h;
}

void sceneRender (void)
{
  ps->UpdateUnified(deltaTime);
  ps->UpdateCameraDistances(position);
  ps->UpdateRender();

  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glClearColor(BG_COLOR);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA);

  makeModelViewProj();
  glFrontFace(GL_CCW);

  unsigned int shader;
  unsigned int uModelViewProjM, cameraUp, cameraRight;

  particlesShader->activate();

  shader = particlesShader->getID();

  uModelViewProjM = glGetUniformLocation(shader, "modelViewProjM");
  glUniformMatrix4fv(uModelViewProjM,1,GL_FALSE,glm::value_ptr(modelViewProjM));

  cameraUp = glGetUniformLocation(shader, "cameraUp");
  cameraRight = glGetUniformLocation(shader, "cameraRight");

//  vec3 right = normalize(cross(vforward, worldUp));

  glUniform3f(cameraUp, viewM[0][1], viewM[1][1], viewM[2][1]);
  glUniform3f(cameraRight, viewM[0][0], viewM[1][0], viewM[2][0]);


  ps->Render();

  ResolveMouseThreshold(0);

  glutSwapBuffers();
}

void initGlut(int *argcp, char **argv)
{
  glutInit(argcp, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitContextVersion(OGLVER, OGLSUBVER);
  glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
  glutInitContextProfile(GLUT_CORE_PROFILE);
  glutInitWindowSize(SCREEN_SIZE);
  glutInitWindowPosition(0,0);
  glutCreateWindow(WINDOW_TITLE);
  glutReshapeFunc(rescaleFunc);
  glutDisplayFunc(sceneRender);
  glutIdleFunc(idleFunc);
  glutKeyboardFunc(keyboardFunc);
  glutMouseFunc(mouseFunc);
  glutSpecialFunc(processSpecialKeys);

  glutPassiveMotionFunc(motionFunc);

  //glutTimerFunc(300, ResolveMouseThreshold, 0);
}

void initGlew()
{
  glewExperimental = GL_TRUE;
  GLenum err=glewInit();
  if (GLEW_OK != err){
    printf("Error: %s\n", glewGetErrorString(err));
  }
}


int main(int argc, char** argv)
{

  initGlut(&argc, argv);
  initGlew();
  initShaders();

  mouseXThreshold = mouseThreshold * resolution[0];
  mouseYThreshold = mouseThreshold * resolution[1];

  makeProjectionMatrix();
//  float aspect=DEFAULT_ASPECT;

  unsigned int maxParticles = 10;
  unsigned int maxEmitters = 1;

  if (argc >= 2)
    maxParticles = atoi(argv[1]);

  if (argc >= 3)
    maxEmitters = atoi(argv[2]);


//#if (particles_WITH_CUDA == 1)
//  ps = new CUDAParticleSystem(10, maxParticles, true);
//#else
//  ps = new GLDefaultParticleSystem(10, maxParticles, true);
//#endif

  ps = new ParticleSystem(10, maxParticles, true);

  ParticleCollection* colProto = new ParticleCollection(ps->particles, 0, maxParticles / 2);

  ParticlePrototype* prototype = new ParticlePrototype(3.0f, 5.0f);
//  prototype->minLife = 3.0f;
//  prototype->maxLife = 5.0f;
  prototype->color.Insert(0.0f, /*particles::RGBToHSV*/(glm::vec4(0, 0, 1, 0.2)));
//  prototype->color.Insert(0.4f, particles::RGBToHSV(glm::vec4(0, 127, 127, 0)));
  prototype->color.Insert(0.65f, /*particles::RGBToHSV*/(glm::vec4(0, 1, 0, 0.2)));
  prototype->color.Insert(1.0f, /*particles::RGBToHSV*/(glm::vec4(0, 0.5, 0.5, 0)));

  prototype->velocity.Insert(0.0f, 3.0f);
  prototype->velocity.Insert(1.0f, 5.0f);

  prototype->size.Insert(0.0f, 1.0f);

  prototype->particles = colProto;

  ps->AddPrototype(prototype);

  colProto = new ParticleCollection(ps->particles, maxParticles / 2, maxParticles);

  prototype = new ParticlePrototype(3.0f, 5.0f);

  prototype->color.Insert(0.0f, /*particles::RGBToHSV*/(glm::vec4(1, 1, 0, 0.2)));
//  prototype->color.Insert(0.4f, particles::RGBToHSV(glm::vec4(0, 127, 127, 0)));
  prototype->color.Insert(0.75f, /*particles::RGBToHSV*/(glm::vec4(1, 0, 0, 0.2)));
  prototype->color.Insert(1.0f, /*particles::RGBToHSV*/(glm::vec4(1, 1, 1, 0)));

  prototype->velocity.Insert(0.0f, 3.0f);
  prototype->velocity.Insert(1.0f, 5.0f);

  prototype->size.Insert(0.0f, 1.0f);

  prototype->particles = colProto;

  ps->AddPrototype(prototype);

  std::cout << "Created prototype." << std::endl;

  ParticleCollection* colEmissionNode;

  ParticleCollection* colEmitter = new ParticleCollection(ps->particles, 0, maxParticles);
  ParticleCollection* colUpdater = new ParticleCollection(ps->particles, 0, maxParticles);
  ParticleCollection* colSorter = new ParticleCollection(ps->particles, 0, maxParticles);
  ParticleCollection* colRenderer = new ParticleCollection(ps->particles, 0, maxParticles);

  std::cout << "Created collections" << std::endl;


  PointEmissionNode* emissionNode;

  int particlesPerEmitter = maxParticles / maxEmitters;

  std::cout << "Creating " << maxEmitters << " emitters with " << particlesPerEmitter << std::endl;

  for (unsigned int i = 0; i < maxEmitters; i++)
  {
    colEmissionNode =
      new ParticleCollection(ps->particles,
                             i * particlesPerEmitter,
                             i * particlesPerEmitter + particlesPerEmitter);

    std::cout << "Creating emission node " << i << " from "
              << i * particlesPerEmitter << " to "
              << i * particlesPerEmitter + particlesPerEmitter << std::endl;

    emissionNode =
      new PointEmissionNode(colEmissionNode, glm::vec3(i * 10, 0, 0));

    ps->AddEmissionNode(emissionNode);
  }

  ParticleEmitter* emitter = new ParticleEmitter(colEmitter, 0.3f, true);
  ps->AddEmitter(emitter);
  emitter->UpdateConfiguration();

  std::cout << "Created emitter" << std::endl;
  ParticleUpdater* updater = new ParticleUpdater(colUpdater);
  std::cout << "Created updater" << std::endl;

  ParticleSorter* sorter;

#if (PREFR_WITH_CUDA)
  sorter = new ThrustParticleSorter(colSorter);
#else
  sorter = new ParticleSorter(colSorter);
#endif


  std::cout << "Created sorter" << std::endl;

  GLDefaultParticleRenderer* renderer =
    new GLDefaultParticleRenderer(colRenderer);

  std::cout << "Created systems" << std::endl;

  ps->AddUpdater(updater);
  ps->SetSorter(sorter);
  ps->SetRenderer(renderer);

  ps->Start();

  glutMainLoop();

  return 0;

}
