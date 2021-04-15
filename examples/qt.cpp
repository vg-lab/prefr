#include <qDebug>
#include <QGuiApplication>
#include <QSurfaceFormat>
#include <QOpenGLFunctions>
#include <QOpenGLContext>
#include <QOpenGLWindow>
#include <QTimer>
#include <iostream>

#define PREFR_SKIP_GLEW_INCLUDE 1
#include <prefr/prefr.h>
#include <shaderPath.h>

using namespace prefr;

//constexpr float rotationScale = 0.01f;

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

class Camera : public prefr::ICamera, public reto::OrbitalCameraController
{
  glm::vec3 PReFrCameraPosition( void )
  {
    return floatPtrToVec3( this->position().data() );
  }

  glm::mat4x4 PReFrCameraViewMatrix( void )
  {
    return floatPtrToMat4( this->camera()->viewMatrix());
  }

  glm::mat4x4 PReFrCameraViewProjectionMatrix( void )
  {
    return floatPtrToMat4( this->camera()->projectionViewMatrix( ));
  }
};

class MyRenderProgram : public prefr::IGLRenderProgram,
                        public reto::ShaderProgram
{
public:

  MyRenderProgram( )
  : prefr::IGLRenderProgram( )
  , reto::ShaderProgram( )
  {
    _viewProjectionMatrixAlias = std::string( "modelViewProjM" );
    _viewMatrixUpComponentAlias = std::string( "cameraUp" );
    _viewMatrixRightComponentAlias = std::string( "cameraRight" );
  }

  virtual ~MyRenderProgram( ){ }

  void prefrActivateGLProgram( void ){ use( );}

  unsigned int prefrGLProgramID( void ){ return program( ); }
};



class GLWindow : public QOpenGLWindow
{
public:
  GLWindow()
  {
    QTimer* timer = new QTimer(this);
    timer->start(0);
    //connect(timer,SIGNAL(timeout()),this,SLOT(idle()));
    connect( timer, &QTimer::timeout, [=](){update();} );
  }

public slots:

  void idle( void )
  {this->update();}

  void init (void )
  {
    makeCurrent();
    auto ctx = QOpenGLContext::currentContext();
    assert(ctx);
    qDebug() << ctx->isValid();

    unsigned int maxParticles = 1000;
    unsigned int maxClusters = 1;

    std::string vertPath;
    std::string fragPath;
    vertPath = fragPath = std::string( PREFR_LIBRARY_BASE_PATH );
    vertPath.append( "/GL/shd/GL-vert.glsl" );
    fragPath.append( "/GL/shd/GL-frag.glsl" );
    prefr::Config::init( );
    //   glCreateShader(GL_VERTEX_SHADER);
    initShader( vertPath.c_str( ), fragPath.c_str( ));

    // _shaderParticlesDefault = new reto::ShaderProgram( );
    std::cout << vertPath << std::endl;
    // program.loadVertexShaderFromText( vertPath.c_str( ));
    // program.loadFragmentShaderFromText( fragPath.c_str( ) );
    // program.compileAndLink( );
    // program.autocatching( );
    
    particleSystem = new ParticleSystem( maxParticles, &camera );

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

    Updater* updater = new Updater( );
    particleSystem->addUpdater( updater );

    Cluster* cluster;
    Source* source;
    Sampler* sampler = new SphereSampler( 1.0f, 360 );

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

    const Eigen::Vector3f center = ( boundingBoxMax + boundingBoxMin ) * 0.5f;
    const float radius = ( boundingBoxMax - center ).norm( ) + 50;

    camera.position(center);
    camera.radius(radius);

    Sorter* sorter = new Sorter( );
    particleSystem->sorter( sorter );

    renderer = new GLRenderer( );
    renderer->glRenderProgram( &program );
    particleSystem->renderer( renderer );

#ifdef PREFR_USE_OPENMP
    particleSystem->parallel( true );
#endif

    particleSystem->run( true );

  }
  ~GLWindow()
  {
  }
  void initializeGL()
  {

    std::cout << "init gl" << std::endl;
    glEnable( GL_DEPTH_TEST );
    glClearColor( 1.0f, 1.f, 1.f, 0.0f );

    glFrontFace( GL_CCW );
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glEnable( GL_CULL_FACE );

    QOpenGLWindow::initializeGL( );

    //init( );
  }
  void resizeGL(int width, int height)
  {
      camera.windowSize(width, height);
      glViewport( 0, 0, width, height );
  }

  
  void paintGL()
  {
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    camera.anim( );

    particleSystem->update( 0.1f );
    particleSystem->updateCameraDistances( );
    particleSystem->updateRender( );
    particleSystem->render( );

  }

  void initShader( const char *vname, const char *fname )
  {
    program.load( vname, fname );
    program.create( );
    program.link( );
  }

protected:
  Camera camera;
  MyRenderProgram program;

  prefr::ParticleSystem* particleSystem;

  bool rotation = false;
  bool translation = false;

  bool paintSoma = true;
  bool paintNeurites = true;

  int mxCoord, myCoord;

  prefr::GLRenderer* renderer;
  prefr::GLRenderer::BlendFunc alphaBlendFunc;

};
  
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QSurfaceFormat fmt;
    fmt.setDepthBufferSize(24);

    // Request OpenGL 3.3 core or OpenGL ES 3.0.
    if (QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL) {
        qDebug("Requesting 3.3 core context");
        fmt.setVersion(4, 0);
        fmt.setProfile(QSurfaceFormat::CoreProfile);
    } else {
        qDebug("Requesting 3.0 context");
        fmt.setVersion(4, 0);
    }

    fmt.setProfile( QSurfaceFormat::CoreProfile );
    QSurfaceFormat::setDefaultFormat(fmt);

    GLWindow glWindow;
    glWindow.show();
    glWindow.init();
    return app.exec();
}
