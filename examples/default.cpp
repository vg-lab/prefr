#include <particles/ParticleSystem.h>
#include <particles/default/DefaultParticleSystem.h>
#include <particles/default/DefaultParticleEmitter.h>
#include <particles/default/DefaultParticleUpdater.h>
#include <particles/default/DefaultParticleSorter.h>
#include <particles/default/DefaultParticleRenderer.h>
#if (particles_WITH_CUDA)
  #include <particles/cuda/ThrustParticleSorter.cuh>
#endif

#include <particles/default/CShader.h>

using namespace particles::defaultParticleSystem;

using namespace glm;

mat4 projectionM;
mat4 viewM;
mat4 modelViewProjM;

vec3 position = vec3(0.0f, 4.0f, 10.0f);
vec3 vforward = vec3(0.0f,0.0f,-1.0f);
vec3 up = vec3(0.0f,1.0f,0.0f);

vec3 worldUp = vec3(0.0f,1.0f,0.0f);

int resolution[] = {SCREEN_SIZE};
float aspect=defaultAspect;

float deltaTime = 0.01f;

int mouseX, mouseY;

// Mouse sensitivity
float mouseSensitivity = degreesToRadians(100.0f);

// Mouse threshold in the borders of the screen to move constantly
float mouseThreshold = 0.15f;

// Calculated threshold depending on the resolution
float mouseXThreshold;
float mouseYThreshold;

CShader* particlesShader;

DefaultGLParticleSystem* ps;

bool emit = true;

void initShaders()
{
  particlesShader = new CShader(false, false,
                                "./particle.vert",
                                "./particle.frag");
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
  vforward = normalize(vforward*q);
}

void MouseTurnRight ()
{
  glm::quat q=glm::angleAxis((float)(mouseSensitivity*deltaTime),worldUp);
  vforward= normalize(vforward*q);
}

void MouseTurnDown()
{
  glm::vec3 right=glm::cross(vforward,worldUp);
  glm::quat q=glm::angleAxis((float)(-mouseSensitivity*deltaTime),right);
  vforward=normalize(vforward*q);
  up=normalize(up*q);
}

void MouseTurnUp()
{
  glm::vec3 right=glm::cross(vforward,worldUp);
  glm::quat q=glm::angleAxis((float)(mouseSensitivity*deltaTime),right);
  vforward=normalize(vforward*q);
  up=normalize(up*q);
}


void makeProjectionMatrix(){
  projectionM=glm::perspective(fov,aspect,nearPlane,farPlane);
}

void makeModelViewProj(){
  //glm::mat4 modelM=glm::mat4(1.0f);
  viewM = glm::lookAt(position, position+vforward,worldUp);
  modelViewProjM = projectionM * viewM;
}



void mouseFunc (int button, int state, int x, int y){}

void keyboardFunc(unsigned char key, int x, int y)
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
    position = vec3(0.0f, 4.0f, 10.0f);
    vforward = vec3(0.0f,0.0f,-1.0f);
    up = vec3(0.0f,1.0f,0.0f);
    break;
  }

}

void processSpecialKeys (int key,int x, int y){
  if(key == GLUT_KEY_UP)  MoveForward();
  else if(key== GLUT_KEY_DOWN) MoveBackwards();
  else if(key== GLUT_KEY_RIGHT) MoveRight();
  else if(key== GLUT_KEY_LEFT) MoveLeft();
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

void ResolveMouseThreshold(int value)
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
  aspect=(float)w/h;
}

void sceneRender (void)
{
  ps->Update(deltaTime);
  ps->UpdateCameraDistances(position);
  ps->UpdateRender();

  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glClearColor(BG_COLOR);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

  vec3 right = normalize(cross(vforward, worldUp));

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

  int maxParticles = 10;
  int maxEmitters = 1;

  if (argc >= 2)
    maxParticles = atoi(argv[1]);

  if (argc >= 3)
    maxEmitters = atoi(argv[2]);



  ps = new DefaultGLParticleSystem(10, maxParticles, 0.5f, true);

  ParticlePrototype* prototype = new ParticlePrototype();
  prototype->minLife = 3.0f;
  prototype->maxLife = 5.0f;
  prototype->color.Insert(0.0f, particles::RGBToHSV(vec4(0, 0, 255, 0)));
  prototype->color.Insert(0.4f, particles::RGBToHSV(vec4(0, 127, 127, 0)));
  prototype->color.Insert(0.8f, particles::RGBToHSV(vec4(0, 255, 0, 0)));
  prototype->color.Insert(1.0f, particles::RGBToHSV(vec4(255, 0, 0, 0)));

  for (int i = 0; i < prototype->color.size; i++)
  {
    glm::vec4 c = prototype->color.values[i];
    std::cout << prototype->color.times[i] << " "  << c.x << " " << c.y << " " << c.z << " " << c.w << std::endl;
  }

  prototype->velocity.Insert(0.0f, 3.0f);
  prototype->velocity.Insert(0.0f, 15.0f);

  prototype->size.Insert(0.0f, 1.0f);

  std::cout << "Created prototype." << std::endl;

  ParticleCollection* colEmitter;

  ParticleCollection* colUpdater = new ParticleCollection(ps->particles, 0, maxParticles);
  ParticleCollection* colSorter = new ParticleCollection(ps->particles, 0, maxParticles);
  ParticleCollection* colRenderer = new ParticleCollection(ps->particles, 0, maxParticles);

  std::cout << "Created collections" << std::endl;

  PointParticleEmitter* emitter;


  int particlesPerEmitter = maxParticles / maxEmitters;

  std::cout << "Creating " << maxEmitters << " emitters with " << particlesPerEmitter << std::endl;

  for (unsigned int i = 0; i < maxEmitters; i++)
  {
    colEmitter = new ParticleCollection(ps->particles, i * particlesPerEmitter, i * particlesPerEmitter + particlesPerEmitter);
    std::cout << "Creating emitter " << i << " from " << i * particlesPerEmitter << " to " << i * particlesPerEmitter + particlesPerEmitter << std::endl;
    emitter = new PointParticleEmitter(colEmitter, prototype, 0.5f, true, vec3(i * 10, 0, 0));
    ps->AddEmitter(emitter);
  }


  std::cout << "Created emitter" << std::endl;
  DefaultParticleUpdater* updater = new DefaultParticleUpdater(colUpdater, prototype, ps->distances);
  std::cout << "Created updater" << std::endl;

#if (particles_WITH_CUDA == 1)
  ThrustParticleSorter* sorter = new ThrustParticleSorter(colSorter, ps->distances);
#else
  DefaultParticleSorter* sorter = new DefaultParticleSorter(colSorter, ps->distances);


#endif
  std::cout << "Created sorter" << std::endl;

  DefaultParticleRenderer* renderer = new DefaultParticleRenderer(colRenderer, ps->distances, ps->renderConfig);

  std::cout << "Created systems" << std::endl;



  ps->AddUpdater(updater);
  ps->SetSorter(sorter);
  ps->SetRenderer(renderer);

  ps->Start();

  glutMainLoop();

  return 0;

}
