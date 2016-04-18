// PREFR includes
#include <prefr/prefr.h>
#include <shaderPath.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>

#include "CShader.h"

#include "FPS.h"

#include <iostream>
#include <sstream>
#include <string>

// gettimeofday
#include <sys/time.h>

#if (PREFR_USE_CUDA)
  #include <prefr/cuda/ThrustParticleSorter.cuh>
#endif

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

glm::vec3 position = glm::vec3(0.0f, 4.0f, 50.0f);
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

unsigned int maxParticles = 10;
unsigned int maxEmitters = 1;

bool emit = true;

FPS fps;

unsigned int frameLimit, frameCounter;

float frameAcc;

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

bool allowRotation;

void mouseFunc (int  button , int  state ,
                int  /* x */, int /* y */ )
{
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    allowRotation = true;
  else
    allowRotation = false;
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

  case 'p':

    fps.print();

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
  if (!allowRotation)
    mouseX = x;
    mouseY = y;
    return;

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
  if (!allowRotation)
    return;

  if (mouseX < 0 || mouseX >= resolution[0] ||
      mouseY < 0 || mouseY >= resolution[1])
    return;

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

long double times[5];

void sceneRender (void)
{
  struct timeval startTime, endTime;
  long double totalTime;

  gettimeofday(&startTime, NULL);
  ps->UpdateUnified(deltaTime);
  gettimeofday(&endTime, NULL);

  totalTime =  (endTime.tv_sec - startTime.tv_sec); //* 1000000L;
  totalTime += (endTime.tv_usec - startTime.tv_usec) / 1000000.0f;
  times[0] += totalTime;

  gettimeofday(&startTime, NULL);
  ps->UpdateCameraDistances(position);
  gettimeofday(&endTime, NULL);

  totalTime =  (endTime.tv_sec - startTime.tv_sec); //* 1000000L;
  totalTime += (endTime.tv_usec - startTime.tv_usec) / 1000000.0f;
  times[1] += totalTime;

//  ps->UpdateRender();

  gettimeofday(&startTime, NULL);
  ps->sorter->Sort();
  gettimeofday(&endTime, NULL);

  totalTime =  (endTime.tv_sec - startTime.tv_sec); //* 1000000L;
  totalTime += (endTime.tv_usec - startTime.tv_usec) / 1000000.0f;
  times[2] += totalTime;

  gettimeofday(&startTime, NULL);
  ps->renderer->SetupRender(ps->aliveParticles);
  gettimeofday(&endTime, NULL);

  totalTime =  (endTime.tv_sec - startTime.tv_sec); //* 1000000L;
  totalTime += (endTime.tv_usec - startTime.tv_usec) / 1000000.0f;
  times[3] += totalTime;



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

  gettimeofday(&startTime, NULL);
  ps->Render();
  gettimeofday(&endTime, NULL);

  totalTime =  (endTime.tv_sec - startTime.tv_sec); //* 1000000L;
  totalTime += (endTime.tv_usec - startTime.tv_usec) / 1000000.0f;
  times[4] += totalTime;

  ResolveMouseThreshold(0);

//  fps.updateAndPrint();

  glutSwapBuffers();

  if (frameLimit > 0)
  {
    frameCounter++;

//    frameAcc += fps.fps;

    if (frameCounter >= frameLimit)
    {
//        std::cout << maxParticles << "," << maxEmitters << "," << frameLimit << "," << frameAcc / frameLimit << std::endl;

        totalTime = 0;
        for ( int i = 0; i < 5 ; i++ )
          totalTime += times[i];
        std::cout << totalTime;
        for ( int i = 0; i < 5 ; i++ )
          std::cout << "," << times[i];
        for ( int i = 0; i < 5 ; i++ )
          std::cout << "," << 100 * times[i] / totalTime;

        std::cout << std::endl;

        exit(0);
    }



//    std::stringstream ss;
//    ss << " " << std::to_string(frameCounter) << " out of " << std::to_string(frameLimit);
//
//    std::string title (WINDOW_TITLE);
//    title.append(ss.str());
//    glutSetWindowTitle(title.c_str());
  }

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

  for ( int i = 0; i < 5 ; i++ )
    times[i] = 0.0f;

  initGlut(&argc, argv);
  initGlew();
  initShaders();

  mouseXThreshold = mouseThreshold * resolution[0];
  mouseYThreshold = mouseThreshold * resolution[1];

  makeProjectionMatrix();

  frameCounter = 0;
  frameLimit = 1000;

  if (argc >= 2)
    maxParticles = atoi(argv[1]);

  if (argc >= 3)
    maxEmitters = atoi(argv[2]);

  if (argc >= 4)
    frameLimit = atoi(argv[3]);
  else
    frameLimit = 0;

  ps = new ParticleSystem(10, maxParticles, true);

  ParticlePrototype* prototype = new ParticlePrototype(3.0f, 5.0f, ParticleCollection(ps->particles, 0, maxParticles / 2));
  prototype->color.Insert(0.0f, (glm::vec4(0, 0, 1, 0.2)));
  prototype->color.Insert(0.65f, (glm::vec4(0, 1, 0, 0.2)));
  prototype->color.Insert(1.0f, (glm::vec4(0, 0.5, 0.5, 0)));

  prototype->velocity.Insert(0.0f, 3.0f);
  prototype->velocity.Insert(1.0f, 5.0f);

  prototype->size.Insert(0.0f, 1.0f);

  ps->AddPrototype(prototype);

  prototype = new ParticlePrototype(3.0f, 5.0f, ParticleCollection(ps->particles, maxParticles / 2, maxParticles));

  prototype->color.Insert(0.0f, (glm::vec4(1, 1, 0, 0.2)));
  prototype->color.Insert(0.75f, (glm::vec4(1, 0, 0, 0.2)));
  prototype->color.Insert(1.0f, (glm::vec4(1, 1, 1, 0)));

  prototype->velocity.Insert(0.0f, 3.0f);
  prototype->velocity.Insert(1.0f, 5.0f);

  prototype->size.Insert(0.0f, 1.0f);

  ps->AddPrototype(prototype);

//  std::cout << "Created prototype." << std::endl;

  PointEmissionNode* emissionNode;

  int particlesPerEmitter = maxParticles / maxEmitters;

//  std::cout << "Creating " << maxEmitters << " emitters with " << particlesPerEmitter << std::endl;

  for (unsigned int i = 0; i < maxEmitters; i++)
  {
//    std::cout << "Creating emission node " << i << " from " << i * particlesPerEmitter << " to " << i * particlesPerEmitter + particlesPerEmitter << std::endl;

    emissionNode =
        new PointEmissionNode(ParticleCollection(ps->particles,
                                                 i * particlesPerEmitter,
                                                 i * particlesPerEmitter + particlesPerEmitter),
                              glm::vec3(i * 10, 0, 0));

    ps->AddEmissionNode(emissionNode);
  }

  ParticleEmitter* emitter = new ParticleEmitter(*ps->particles, 0.3f, true);
  ps->AddEmitter(emitter);

//  std::cout << "Created emitter" << std::endl;
  ParticleUpdater* updater = new ParticleUpdater(*ps->particles);
//  std::cout << "Created updater" << std::endl;

  ParticleSorter* sorter;

#if (PREFR_USE_CUDA)
  sorter = new ThrustParticleSorter(*ps->particles);
#else
  sorter = new ParticleSorter(*ps->particles);
#endif

//  std::cout << "Created sorter" << std::endl;

  GLDefaultParticleRenderer* renderer =
    new GLDefaultParticleRenderer(*ps->particles);

//  std::cout << "Created systems" << std::endl;

  ps->AddUpdater(updater);
  ps->SetSorter(sorter);
  ps->SetRenderer(renderer);

  ps->Start();

  glutMainLoop();




  return 0;

}
