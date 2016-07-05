/*
 * Copyright (c) 2014-2016 GMRV/URJC.
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
#include <GL/gl.h>
#include <GL/freeglut.h>

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
void keyboardFunc( unsigned char key, int x, int y );
void mouseFunc( int button, int state, int x, int y );
void mouseMoveFunc( int xCoord, int yCoord );
void initContext( int argc, char** argv );
void initOGL( void );
void initShader( const char *vname, const char *fname );
void InitParticleSystem( unsigned int maxParticles, unsigned int maxClusters );
void destroy( void);


// PReFr Interface classes inheritance

class Camera : public prefr::ICamera, public reto::Camera
{
  glm::vec3 PReFrCameraPosition( void )
  {
    return glm::vec3( Position( )[ 0 ],
                      Position( )[ 1 ],
                      Position( )[ 2 ]);
  }

  glm::mat4x4 PReFrCameraViewMatrix( void )
  {
    return glm::mat4x4( ViewMatrix( )[ 0 ], ViewMatrix( )[ 1 ],
                        ViewMatrix( )[ 2 ], ViewMatrix( )[ 3 ],
                        ViewMatrix( )[ 4 ], ViewMatrix( )[ 5 ],
                        ViewMatrix( )[ 6 ], ViewMatrix( )[ 7 ],
                        ViewMatrix( )[ 8 ], ViewMatrix( )[ 9 ],
                        ViewMatrix( )[ 10 ], ViewMatrix( )[ 11 ],
                        ViewMatrix( )[ 12 ], ViewMatrix( )[ 13 ],
                        ViewMatrix( )[ 14 ], ViewMatrix( )[ 15 ]);
  }

  glm::mat4x4 PReFrCameraViewProjectionMatrix( void )
  {
    return glm::mat4x4( ViewProjectionMatrix( )[ 0 ],
                        ViewProjectionMatrix( )[ 1 ],
                        ViewProjectionMatrix( )[ 2 ],
                        ViewProjectionMatrix( )[ 3 ],
                        ViewProjectionMatrix( )[ 4 ],
                        ViewProjectionMatrix( )[ 5 ],
                        ViewProjectionMatrix( )[ 6 ],
                        ViewProjectionMatrix( )[ 7 ],
                        ViewProjectionMatrix( )[ 8 ],
                        ViewProjectionMatrix( )[ 9 ],
                        ViewProjectionMatrix( )[ 10 ],
                        ViewProjectionMatrix( )[ 11 ],
                        ViewProjectionMatrix( )[ 12 ],
                        ViewProjectionMatrix( )[ 13 ],
                        ViewProjectionMatrix( )[ 14 ],
                        ViewProjectionMatrix( )[ 15 ]);
  }
};

class RenderProgram : public prefr::IGLRenderProgram, public reto::ShaderProgram
{
  void PReFrActivateGLProgram( void ){ use( );}

  unsigned int PReFrGLProgramID( void ){ return program( ); }
};


Camera camera;
RenderProgram program;

prefr::ParticleSystem* particleSystem;

bool rotation = false;
bool translation = false;

bool paintSoma = true;
bool paintNeurites = true;

int mxCoord, myCoord;


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
  glutKeyboardFunc( keyboardFunc );
  glutMouseFunc( mouseFunc );
  glutMotionFunc( mouseMoveFunc );
}

void initOGL( void )
{
  glEnable( GL_DEPTH_TEST );
  glClearColor( 0.2f, 0.2f, 0.2f, 0.0f );

  glFrontFace( GL_CCW );
  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
  glEnable( GL_CULL_FACE );

}

void destroy( void )
{

}

void initShader( const char *vname, const char *fname )
{
  program.load( vname, fname );
  program.create( );
  program.link( );
}

void ExpandBoundingBox( Eigen::Vector3f& minBounds,
                        Eigen::Vector3f& maxBounds,
                        glm::vec3 position)
{
  for( unsigned int i = 0; i < 3; ++i )
  {
    minBounds( i ) = std::min( minBounds( i ), position[ i ] );
    maxBounds( i ) = std::max( maxBounds( i ), position[ i ] );
  }
}

using namespace prefr;
void InitParticleSystem( unsigned int maxParticles, unsigned int maxClusters )
{
  particleSystem = new ParticleSystem( maxParticles, &camera );

  Model model( 3.0f, 10.0f );
  model.color.Insert( 0.0f, glm::vec4( 1.0f, 1.0f, 0.0f, 0.15f ));
  model.color.Insert( 1.0f, glm::vec4( 1.0f, 0.0f, 0.0f, 0.05f ));

  model.size.Insert( 0.0f, 3.0f );
  model.size.Insert( 1.0f, 3.0f );

  model.velocity.Insert( 0.0f, 5.0f);
  model.velocity.Insert( 1.0f, 8.0f);
  particleSystem->AddModel( &model );


  Updater updater;
  particleSystem->AddUpdater( &updater );

  Cluster* cluster;
  SphereSource* source;

  unsigned int particlesPerCluster = maxParticles / maxClusters;



  Eigen::Vector3f boundingBoxMin( std::numeric_limits< float >::max( ),
                                  std::numeric_limits< float >::max( ),
                                  std::numeric_limits< float >::max( ));
  Eigen::Vector3f boundingBoxMax( std::numeric_limits< float >::min( ),
                                  std::numeric_limits< float >::min( ),
                                  std::numeric_limits< float >::min( ));

  for( unsigned int i = 0; i < maxClusters; ++i )
  {
    glm::vec3 position( sin( i ), cos( i ), i * 3 );

    cluster = new Cluster( );
    source = new SphereSource( 0.3f, position );
    cluster->source( source );
    cluster->updater( &updater );
    cluster->model( &model );

    particleSystem->AddSource( source );
    particleSystem->AddCluster( cluster,
                                particlesPerCluster * i,
                                particlesPerCluster );

    ExpandBoundingBox( boundingBoxMin, boundingBoxMax, position );
  }


  Eigen::Vector3f center = ( boundingBoxMax + boundingBoxMin ) * 0.5f;
  float radius = ( boundingBoxMax - center ).norm( );

  camera.TargetPivotRadius( center, radius );


  Sorter sorter;
  particleSystem->sorter( &sorter );

  GLRenderer renderer;
  renderer.glRenderProgram( &program );
  particleSystem->renderer( &renderer );

}

void renderFunc( void )
{
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  particleSystem->Update( 0.1f );
  particleSystem->UpdateCameraDistances( );
  particleSystem->UpdateRender( );
  particleSystem->Render( );


  glutSwapBuffers( );

}

void resizeFunc( int width, int height )
{
  camera.Ratio((( double ) width ) / height );
  glViewport( 0, 0, width, height );
}

void idleFunc( void )
{

  static float angle = 0.0f;
  angle = ( angle > 2.0f * float( M_PI )) ? 0 : angle + 0.01f;
  glutPostRedisplay( );

}

void keyboardFunc( unsigned char, int, int )
{
}

void mouseFunc( int button, int state, int xCoord, int yCoord )
{
  if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
  {
    rotation = true;
    mxCoord = xCoord;
    myCoord = yCoord;
  }
  if ( button == GLUT_LEFT_BUTTON && state == GLUT_UP )
  {
    rotation=false;
  }
  if ( button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN )
  {
    translation = true;
    mxCoord = xCoord;
    myCoord = yCoord;
  }
  if ( button == GLUT_MIDDLE_BUTTON && state == GLUT_UP )
  {
    translation = false;
  }
  if ( button == 3 && state == GLUT_DOWN )
  {
    camera.Radius( camera.Radius( ) / 1.1f );
  }
  if ( button == 4 && state == GLUT_DOWN )
  {
    camera.Radius( camera.Radius( ) * 1.1f );
  }
}

void mouseMoveFunc( int xCoord, int yCoord )
{
  if( rotation )
  {
    camera.LocalRotation( -( mxCoord - xCoord ) * 0.01, -( myCoord - yCoord ) * 0.01 );
    mxCoord = xCoord;
    myCoord = yCoord;
  }
  if( translation )
  {
    mxCoord = xCoord;
    myCoord = yCoord;
  }
}


int main( int argc, char** argv )
{
  initContext( argc, argv );
  initOGL( );

  unsigned int maxParticles = 1000;
  unsigned int maxClusters = 1;

  if (argc >= 2)
    maxParticles = atoi(argv[1]);

  if (argc >= 3)
    maxClusters = atoi(argv[2]);

  std::string vertPath;
  std::string fragPath;
  vertPath = fragPath = std::string( PREFR_LIBRARY_BASE_PATH );
  vertPath.append( "/GL/shd/GL-vert.glsl" );
  fragPath.append( "/GL/shd/GL-frag.glsl" );
  initShader( vertPath.c_str( ), fragPath.c_str( ));

  InitParticleSystem( maxParticles, maxClusters );

  glutMainLoop( );
  destroy( );

  return 0;
}

#endif
