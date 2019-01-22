/*
 * Copyright (c) 2014-2018 GMRV/URJC.
 *
 * Authors: Sergio Galindo <sergio.galindo@urjc.es>
 *
 * This file is part of PReFr <https://gmrv.gitlab.com/nsviz/prefr>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3.0 as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include <GL/glew.h>

#ifdef Darwin
  #define __gl_h_
  #define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
  #include <OpenGL/gl.h>
  #include <OpenGL/glu.h>
  #include <GL/freeglut.h>
#else
  #include <GL/gl.h>
  #include <GL/freeglut.h>
#endif

#include <cmath>

#include <prefr/prefr.h>
#include <shaderPath.h>

#ifdef PREFR_USE_RETO

#include <reto/reto.h>
#include <Eigen/Dense>



// GLUT Functions
void renderFunc( void );
void resizeFunc( int width, int height );
void idleFunc( void );
void mouseFunc( int button, int state, int x, int y );
void mouseMoveFunc( int xCoord, int yCoord );
void initContext( int argc, char** argv );
void initOGL( void );
void initShader( const char *vname, const char *fname );
void initShader2( const char *vname, const char *fname );
void InitParticleSystem( unsigned int maxParticles, unsigned int maxClusters );

glm::vec3 floatPtrToVec3( float* floatPos )
{
  return glm::vec3( floatPos[ 0 ],
                    floatPos[ 1 ],
                    floatPos[ 2 ]);
}

glm::mat4x4 floatPtrToMat4( float* floatPos )
{
  return glm::mat4x4( floatPos[ 0 ], floatPos[ 1 ],
                      floatPos[ 2 ], floatPos[ 3 ],
                      floatPos[ 4 ], floatPos[ 5 ],
                      floatPos[ 6 ], floatPos[ 7 ],
                      floatPos[ 8 ], floatPos[ 9 ],
                      floatPos[ 10 ], floatPos[ 11 ],
                      floatPos[ 12 ], floatPos[ 13 ],
                      floatPos[ 14 ], floatPos[ 15 ]);
}


// PReFr Interface classes inheritance

class Camera : public prefr::ICamera, public reto::Camera
{
  glm::vec3 PReFrCameraPosition( void )
  {
    return floatPtrToVec3( position( ));
  }

  glm::mat4x4 PReFrCameraViewMatrix( void )
  {
    return floatPtrToMat4( viewMatrix( ));
  }

  glm::mat4x4 PReFrCameraViewProjectionMatrix( void )
  {
    return floatPtrToMat4( viewProjectionMatrix( ));
  }
};

class RenderProgram : public prefr::IGLRenderProgram,
                      public reto::ShaderProgram
{
public:

  RenderProgram( )
  : prefr::IGLRenderProgram( )
  , reto::ShaderProgram( )
  {
    _viewProjectionMatrixAlias = std::string( "modelViewProjM" );
    _viewMatrixUpComponentAlias = std::string( "cameraUp" );
    _viewMatrixRightComponentAlias = std::string( "cameraRight" );
  }

  virtual ~RenderProgram( ){ }

  void prefrActivateGLProgram( void ){ use( );}

  unsigned int prefrGLProgramID( void ){ return program( ); }
};


Camera camera;
RenderProgram program;
RenderProgram pickProgram;

prefr::ParticleSystem* particleSystem;

bool rotation = false;
bool translation = false;

bool paintSoma = true;
bool paintNeurites = true;

int mxCoord, myCoord;


int pickX, pickY;
bool checkPicking = false;


void initContext( int argc, char** argv )
{
  glutInit( &argc,argv );
  glutInitContextVersion( 3,3 );
  glutInitContextFlags( GLUT_FORWARD_COMPATIBLE );
  glutInitContextProfile( GLUT_CORE_PROFILE );

  glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
  glutInitWindowSize( 500, 500 );
  glutInitWindowPosition( 0, 0 );
  glutCreateWindow( "GLUT example" );

  glewExperimental = GL_TRUE;
  GLenum err = glewInit( );
  if ( GLEW_OK != err ) {
    std::cout << "Error: " << glewGetErrorString( err ) << std::endl;
    exit ( -1 );
  }
  const GLubyte *oglVersion = glGetString( GL_VERSION );
  std::cout << "This system supports OpenGL Version: "
            << oglVersion << std::endl;

  glutReshapeFunc( resizeFunc );
  glutDisplayFunc( renderFunc );
  glutIdleFunc( idleFunc );
  glutMouseFunc( mouseFunc );
  glutMotionFunc( mouseMoveFunc );
}

void initOGL( void )
{
  glEnable( GL_DEPTH_TEST );
  glClearColor( 1.0f, 1.f, 1.f, 0.0f );

  glFrontFace( GL_CCW );
  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
  glEnable( GL_CULL_FACE );

}

void initShader( const char *vname, const char *fname )
{
  program.load( vname, fname );
  program.create( );
  program.link( );
}

void initShader2( const char *vname, const char *fname )
{
  pickProgram.load( vname, fname );
  pickProgram.create( );
  pickProgram.link( );
}

void expandBoundingBox( Eigen::Vector3f& minBounds,
                        Eigen::Vector3f& maxBounds,
                        const glm::vec3& position )
{
  for( unsigned int i = 0; i < 3; ++i )
  {
    minBounds( i ) = std::min( minBounds( i ), position[ i ]);
    maxBounds( i ) = std::max( maxBounds( i ), position[ i ]);
  }
}

prefr::GLPickRenderer* renderer;
prefr::GLRenderer::BlendFunc alphaBlendFunc;

void renderFunc( void )
{
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  camera.anim( );

  particleSystem->update( 0.1f );
  particleSystem->updateCameraDistances( );
  particleSystem->updateRender( );
  particleSystem->render( );


  if( checkPicking )
  {
    prefr::tparticle particle = renderer->pick( pickX, pickY );
    
    checkPicking = false;
  }

  glutSwapBuffers( );

}

void resizeFunc( int width, int height )
{
  camera.ratio((( double ) width ) / height );
  glViewport( 0, 0, width, height );
}

void idleFunc( void )
{

  static float angle = 0.0f;
  angle = ( angle > 2.0f * float( M_PI )) ? 0 : angle + 0.01f;
  glutPostRedisplay( );

}
void mouseFunc( int button, int state, int xCoord, int yCoord )
{
  if( button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN )
  {
    int height = 500;
    printf("Click at %d, %d\n", xCoord, height - yCoord);
    checkPicking = true;
    pickX = xCoord;
    pickY = height - yCoord;
  }

  if( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
  {
    rotation = true;
    mxCoord = xCoord;
    myCoord = yCoord;
  }

  if( button == GLUT_LEFT_BUTTON && state == GLUT_UP )
  {
    rotation=false;
  }

  if( button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN )
  {
    translation = true;
    mxCoord = xCoord;
    myCoord = yCoord;
  }

  if( button == GLUT_MIDDLE_BUTTON && state == GLUT_UP )
  {
    translation = false;

    if( alphaBlendFunc == prefr::GLRenderer::ONE_MINUS_CONSTANT_ALPHA )
    {
      alphaBlendFunc = prefr::GLRenderer::ONE_MINUS_SRC_ALPHA;
      glClearColor( 1.0f, 1.f, 1.f, 0.0f );
    }
    else
    {
      alphaBlendFunc = prefr::GLRenderer::ONE_MINUS_CONSTANT_ALPHA;
      glClearColor( 0.0f, 0.f, 0.f, 0.0f );
    }

    renderer->alphaBlendingFunc( alphaBlendFunc );
  }

  if( button == 3 && state == GLUT_DOWN )
  {
    camera.radius( camera.radius( ) / 1.1f );
  }

  if( button == 4 && state == GLUT_DOWN )
  {
    camera.radius( camera.radius( ) * 1.1f );
  }
}

void mouseMoveFunc( int xCoord, int yCoord )
{
  if( rotation )
  {
    camera.localRotation( - ( mxCoord - xCoord ) * 0.01,
                          - ( myCoord - yCoord ) * 0.01 );
    mxCoord = xCoord;
    myCoord = yCoord;
  }
  if( translation )
  {
    mxCoord = xCoord;
    myCoord = yCoord;
  }
}

using namespace prefr;
void InitParticleSystem( unsigned int maxParticles, unsigned int maxClusters )
{
  particleSystem = new ParticleSystem( maxParticles, &camera );

  /**/
  Model* model1 = new Model( 500.0f, 500.0f );
  model1->color.Insert( 0.0f, glm::vec4( 1.0f, 1.0f, 0.0f, 0.65f ));
  model1->color.Insert( 0.70f, glm::vec4( 1.0f, 0.0f, 0.0f, 0.05f ));
  model1->color.Insert( 1.0f, glm::vec4( 1.0f, 0.0f, 0.0f, 0.0f ));

  model1->size.Insert( 0.0f, 15.0f );
  model1->size.Insert( 1.0f, 3.0f );

  model1->velocity.Insert( 0.0f, 0.0f );
  //model1->velocity.Insert( 1.0f, 8.0f );
  particleSystem->addModel( model1 );

  Model* model2 = new Model( 500.0f, 500.0f );
  model2->color.Insert( 0.0f, glm::vec4( 0.0f, 1.0f, 1.0f, 0.65f ));
  model2->color.Insert( 1.0f, glm::vec4( 0.0f, 0.0f, 1.0f, 0.05f ));

  model2->size.Insert( 0.0f, 5.0f );
  model2->size.Insert( 1.0f, 3.0f );

  model2->velocity.Insert( 0.0f, 0.0f );
  //model2->velocity.Insert( 1.0f, 30.0f );
  particleSystem->addModel( model2 );
  /**/
  
  /**
  Model* model1 = new Model( 3.0f, 10.0f );
  model1->color.Insert( 0.0f, glm::vec4( 1.0f, 1.0f, 0.0f, 0.65f ));
  model1->color.Insert( 0.70f, glm::vec4( 1.0f, 0.0f, 0.0f, 0.05f ));
  model1->color.Insert( 1.0f, glm::vec4( 1.0f, 0.0f, 0.0f, 0.0f ));

  model1->size.Insert( 0.0f, 15.0f );
  model1->size.Insert( 1.0f, 3.0f );

  model1->velocity.Insert( 0.0f, 5.0f );
  model1->velocity.Insert( 1.0f, 8.0f );
  particleSystem->addModel( model1 );

  Model* model2 = new Model( 5.0f, 10.0f );
  model2->color.Insert( 0.0f, glm::vec4( 0.0f, 1.0f, 1.0f, 0.65f ));
  model2->color.Insert( 1.0f, glm::vec4( 0.0f, 0.0f, 1.0f, 0.05f ));

  model2->size.Insert( 0.0f, 10.0f );
  model2->size.Insert( 1.0f, 3.0f );

  model2->velocity.Insert( 0.0f, 5.0f );
  model2->velocity.Insert( 1.0f, 30.0f );
  particleSystem->addModel( model2 );
  **/

  Updater* updater = new Updater( );
  particleSystem->addUpdater( updater );

  Cluster* cluster;
  Source* source;
  Sampler* sampler = new SphereSampler( 200.0f, 360 );

  unsigned int particlesPerCluster = maxParticles / maxClusters;

  Eigen::Vector3f boundingBoxMin( std::numeric_limits< float >::max( ),
                                  std::numeric_limits< float >::max( ),
                                  std::numeric_limits< float >::max( ));
  Eigen::Vector3f boundingBoxMax( std::numeric_limits< float >::min( ),
                                  std::numeric_limits< float >::min( ),
                                  std::numeric_limits< float >::min( ));

  for( unsigned int i = 0; i < maxClusters; ++i )
  {
    glm::vec3 position( 100 * sinf(( i ) / 10.0f ),
                        100 * sinf(( i + 5 ) / 20.0f ),
                        i * 10 );

    cluster = new Cluster( );
    source = new Source( 0.3f, position, sampler );

    ParticleIndices indices;
    indices.reserve( particlesPerCluster );

    unsigned int start = particlesPerCluster * i;
    unsigned int end = start + particlesPerCluster;

    for( unsigned int index = start; index < end; ++index )
    {
      indices.push_back( index );
    }

    particleSystem->addSource( source, indices );
    particleSystem->addCluster( cluster, indices );

    cluster->setModel( i % 2 == 0 ? model1 : model2 );
    cluster->setUpdater( updater );

    expandBoundingBox( boundingBoxMin, boundingBoxMax, position );
  }

  Eigen::Vector3f center = ( boundingBoxMax + boundingBoxMin ) * 0.5f;
  float radius = ( boundingBoxMax - center ).norm( );
  radius += 50;

  camera.targetPivotRadius( center, radius );

  Sorter* sorter = new Sorter( );
  particleSystem->sorter( sorter );

  renderer = new GLPickRenderer( );
  renderer->glRenderProgram( &program );
  renderer->glPickProgram( &pickProgram );
  particleSystem->renderer( renderer );

#ifdef PREFR_USE_OPENMP
  particleSystem->parallel( true );
#endif

  particleSystem->run( true );

}


void usage( void )
{
  std::cout << "PReFr OpenGL-Glut example. " << std::endl
            << "Usage: " << std::endl
            << " prefrOGL [ Max_Particles ][ Num_Clusters ]" << std::endl
            << " where Max_Particles is the maximum number of the particles used on the example"
            << " and Num_Clusters is the total number of particle clusters. In this example "
            << " the number of total particles will be split uniformly over the clusters, as "
            << " for example 1000 particles and 20 clusters will lead to 50 particles per cluster."
            << " Note: by default example will use 1000 particles and a single cluster."
            << std::endl;
}

int main( int argc, char** argv )
{
  initContext( argc, argv );
  initOGL( );

  unsigned int maxParticles = 1000;
  unsigned int maxClusters = 1;

  if ( argc >= 2 )
  {
    if( strcmp( argv[ 1 ], "-h" ) == 0 )
    {
      usage( );
      exit( 0 );
    }
    else
      maxParticles = atoi( argv[ 1 ] );
  }


  if ( argc >= 3 )
    maxClusters = atoi( argv[ 2 ] );

  bool parallel = false;
  if( argc >= 4 && strcmp( argv[ 3 ], "-p" ) == 0 )
    parallel = true;

  {
    std::string vertPath;
    std::string fragPath;
    vertPath = fragPath = std::string( PREFR_LIBRARY_BASE_PATH );
    vertPath.append( "/GL/shd/GL-vert.glsl" );
    fragPath.append( "/GL/shd/GL-frag.glsl" );
    initShader( vertPath.c_str( ), fragPath.c_str( ));
  }
  {
    std::string vertPath;
    std::string fragPath;
    vertPath = fragPath = std::string( PREFR_LIBRARY_BASE_PATH );
    vertPath.append( "/GL/shd/GL-vert2.glsl" );
    fragPath.append( "/GL/shd/GL-frag2.glsl" );
    initShader2( vertPath.c_str( ), fragPath.c_str( ));
  }

  InitParticleSystem( maxParticles, maxClusters );

  particleSystem->parallel( parallel );

  glutMainLoop( );

  return 0;
}

#endif
