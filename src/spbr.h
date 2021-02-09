//////////////////
///// spbr.h /////
//////////////////

#if !defined  SPBR_HH
#define       SPBR_HH

#include <kvs/PointObject>
#include <kvs/Type>
#include <kvs/Vector3>
#include <kvs/Camera>
#include <kvs/MersenneTwister>
#include <kvs/ValueArray>

#include <fstream>
#include<cstddef> // size_t 

#include "file_format.h" //TANAKA 2017/11/05
#include "loop_image_param.h" //TANAKATANAKA

//---- constants
#include "const_spbr.h" 
#include "shading_parameters.h" 

const int  SPBR_BUF_SIZE = 256 ;

//typedef kvs::ValueArray<kvs::Real32> KVS_FLOAT ;


//--------------------//
class SPBR  : public kvs::PointObject {
//--------------------//

  typedef kvs::PointObject SuperClass;

 public:

  // Constructor for SPBR ASCII data
  SPBR ( const char* input_file, SPBR_ASCII_FORMAT file_format );

  //---------- METHODS ----------//

  // Camera control
  void   setCameraPosition ( double Ex , double Ey , double Ez  );
  void   setLookAt         ( double LAx, double LAy, double LAz );
  void   setViewAngle      ( double angle_deg );
  void   setFlagCameraFar  ( int flag ) ; 
  void   setCameraZoom     ( double f_zoom ); 
  double viewAngle ( void ) { return m_viewAngle ;}
  bool   isCameraFar      ( void ) { return m_flagCameraFar; } 
  kvs::Vector3f  cameraPosition ( void ){ return m_cameraPosition ;}
  kvs::Vector3f  lookAt         ( void ){ return m_lookAt         ;}

  void   setOrthogonalCamera (void) 
         { 
	   std::cout << "** Orthogonal camera is set." << std::endl;
           m_flagCameraProjectionType = kvs::Camera::Orthogonal; 
         }
  void   setPerspectiveCamera (void) 
         { 
	   std::cout << "** Perspective camera is set." << std::endl;
           m_flagCameraProjectionType = kvs::Camera::Perspective; 
         }
  kvs::Camera::ProjectionType CameraProjectionType( void ) 
         { return m_flagCameraProjectionType;} 

       // axis_name: X, +X, x, +x, Y, +Y, y, +y, Z, +Z, z, +z 
  bool setCameraAngleByAxis ( char* axis_name, double* zrot_angle_deg, double* xrot_angle_deg ) ;


  // Repeat-level control
  void          setRepeatLevel   ( unsigned int level );
  unsigned int  repeatLevel  ( void ) { return m_repeatLevel  ; }

  // Wireframe-box control
  void setWireframeBox   ( double xmin, double ymin, double zmin, 
                           double xmax, double ymax, double zmax  );
  bool isDrawWireframeBox ( void )  { return m_flagDrawWireframeBox;}
  kvs::Vector3d wireframeBoxMinPosition ( void )  { return m_minWBPosition; }
  kvs::Vector3d wireframeBoxMaxPosition ( void )  { return m_maxWBPosition; }

  // Particle-attribute control
  void setColor ( double R , double G , double B  );
  void setColor ( unsigned int Rb, unsigned int Gb, unsigned int Bb );
  void setFlagReverseNormals ( int flag ) { m_flagReverseNormals = flag; }
  void setFlagUseNormals ( int flag ) ;
  void setFlagForcedColor ( int flag ) { m_flagForcedColor = flag; }//FORCED_COLOR
  void setFlagParticleZoom ( int flag ) ; 
  void setPointSize      ( unsigned int size ) ; // for OPBR
  bool isNormalsReversed   ( void ) { return m_flagReverseNormals;}
  bool isShading           ( void ) { return m_flagUseNormals;}
  bool isForcedColor ( void ) { return m_flagForcedColor;}//FORCED_COLOR
  bool isParticleZoomOn ( void ) { return m_flagParticleZoom; } 

  // Image-plane control
  void setFlagFPS     ( int flag ) ;
  bool isFPSDisplayed ( void ) { return m_flagFPS; }
  unsigned int imageResolution ( void ) { return m_imageResolution ;}

  void  setBackGroundColor ( unsigned int Rb,  
                             unsigned int Gb, 
                             unsigned int Bb)
  {
    m_BG_Rb = Rb;   m_BG_Gb = Gb;   m_BG_Bb = Bb;
  }

  unsigned int bg_Rb( void ) { return m_BG_Rb;}
  unsigned int bg_Gb( void ) { return m_BG_Gb;} 
  unsigned int bg_Bb( void ) { return m_BG_Bb;}
  kvs::RGBColor backGroundColor ( void ) 
  { 
    kvs::RGBColor bg_color( m_BG_Rb, m_BG_Gb, m_BG_Bb ); 
    return        bg_color;   
  }

  // local-coordinate control
  void          setBodyCoordOrigin     ( const kvs::Vector3d& origin ); 
  void          setBodyCoordBaseVector ( const kvs::Vector3d& e1, 
                                         const kvs::Vector3d& e2 ); 
  kvs::Vector3d doCoordTransformation( const kvs::Vector3d& P );  
  void          doCoordTransformation( double *x, double* y, double* z,  
                                       double *nx, double* ny, double* nz );  

  // LOD control
  void          setFlagLOD( int flag ) ;
  bool          isLOD     ( void      ) { return m_flagLOD ;}

  // Object rotation angle
  void   setObjZXRot ( double zrot_angle_deg, double xrot_angle_deg ) ;
  double objectZXRotAngle ( int index ) { return m_objZXRotAngle[ index ];  }
  bool   isZXRotation (void) { return m_flagObjZXRot ; }

  // Set min-max object coordinates
  void   setBoundingBox ( const kvs::Vector3f& minPoint, 
                          const kvs::Vector3f& maxPoint ) ;  
  bool   isForcedBoundingBox ( void ) { return m_flagForcedBoundingBox ;}
  kvs::Vector3f forcedMinPoint ( void ) { return m_forcedMinPoint ; } 
  kvs::Vector3f forcedMaxPoint ( void ) { return m_forcedMaxPoint ; } 

  // Set min-max object coordinates //ST_BOUNDING_BOX
  void   setStandardBoundingBox ( const kvs::Vector3f& minPoint, 
                                  const kvs::Vector3f& maxPoint ) ;  
  bool   isForcedStandardBoundingBox ( void ) { return m_flagForcedStandardBoundingBox;}
  kvs::Vector3f forcedStandardMinPoint ( void ) { return m_forcedStandardMinPoint; } 
  kvs::Vector3f forcedStandardMaxPoint ( void ) { return m_forcedStandardMaxPoint; } 

  //ST_BOUNDING_BOX
  // Set forced (standard) bounding box to the scene
  void setForcedMinMaxCoords( void );

  // Get bounding box info of the KVS point object
  kvs::Vector3f minCoord( void ) { return minObjectCoord();}
  kvs::Vector3f maxCoord( void ) { return maxObjectCoord();}


  // Shuffle control (1): Shuffle by the particle renderer
  void setFlagShuffle      ( int flag ) ;
  bool isParticleShuffleOn ( void ) { return m_flagShuffle; }

  // Shuffle control (2): Additional Shuffle in main()
  void setFlagForcedShuffle      ( int flag ) ;
  bool isForcedShuffleOn ( void ) { return m_flagForcedShuffle; }

  // Set shading type
  bool isLambertShading( void ) { return m_flagLambertShading; }
  bool isPhongShading  ( void ) { return !m_flagLambertShading; }
  double kd( void ) { return m_kd;}
  double ka( void ) { return m_ka;}
  double ks( void ) { return m_ks;}
  int    shininess ( void ) { return m_shininess; }
  void   setLambertShading ( double kd = DEFAULT_LAMBERT_KD, 
                             double ka = DEFAULT_LAMBERT_KA ) ; 
  void   setPhongShading   ( double kd = DEFAULT_PHONG_KD, 
                             double ka = DEFAULT_PHONG_KA, 
                             double ks = DEFAULT_PHONG_KS, 
                             int    shininess = DEFAULT_PHONG_SHININESS ); 


  void  setPlyAsciiOutput ( unsigned int ncolumn )
        { 
          m_num_output_ply_column = ncolumn;
        }

  void  writePlyAscii ( kvs::ValueArray<kvs::Real32>& coords, 
                        kvs::ValueArray<kvs::Real32>& normals, 
                        kvs::ValueArray<kvs::UInt8>& colors );

  void  writePlyBinary(kvs::ValueArray<kvs::Real32>& coords,	//2018/8
					   kvs::ValueArray<kvs::Real32>& normals,
	                   kvs::ValueArray<kvs::UInt8>& colors);

  // Rotation speed control (mouse control)
  void   setMouseRotSpeed ( double mouse_rot_speed ); //ROTSPEED
  double mouseRotSpeed (void) { return m_mouse_rot_speed ;} //ROTSPEED

  // Zoom speed control (mouse control)
  void   setMouseZoomSpeed ( double mouse_zoom_speed ); //ZOOMSPEED
  double mouseZoomSpeed (void) { return m_mouse_zoom_speed ;} //ZOOMSPEED

 private:
  // Generate PointObject (called in the constructor)
  void generatePointObject( kvs::PointObject* point_object );

  // Methods called in GeneratePointObject()
  void readHeader_and_countParticles ( void ) ;
  void readPointData ( void ) ;
  
  // Utility methods
  //... normalize vector components
  void normalizeVector( double* nx_p, double* ny_p, double* nz_p );

  // GRAYSCALE
  // RGB color ==> grayscale color
  //   Return: true if m_grayscale_id is not zero
  bool getGrayscaleColor ( unsigned int Rb, 
                           unsigned int Gb, 
                           unsigned int Bb , 
                           unsigned int * pGb);

  //---------- DATA ----------//

  // IO control
  char          m_input_file [256] ;

  // Camera control 
  kvs::Vector3f m_cameraPosition;
  kvs::Vector3f m_lookAt;
  double        m_viewAngle     ;
  bool          m_flagCameraFar ; 

  // Repeat-level control
  unsigned int  m_repeatLevel    ;

  // Particle-attribute control
  unsigned int  m_Rb, m_Gb, m_Bb ;
  bool          m_flagReverseNormals ;
  bool          m_flagUseNormals ;
  bool          m_flagForcedColor ; //FORCED_COLOR
  bool          m_flagParticleZoom ; 

  // Number of particles
  unsigned long        m_numParticles  ; //TANAKA_181230
  //  size_t           m_numParticles  ; 
  //  int           m_numParticles     ;

  // Point size (available for OPBR)
  unsigned int  m_pointSize ; 

  // Wireframe-box control
  bool          m_flagDrawWireframeBox ;
  kvs::Vector3d m_minWBPosition, m_maxWBPosition ;

  // Image-plane control
  bool          m_flagFPS        ;
  unsigned int  m_BG_Rb, m_BG_Gb, m_BG_Bb ;
  unsigned int  m_imageResolution ;

  // local-coordinate control
  bool          m_flagCoordTransformed ;
  kvs::Vector3d m_O              ; // origin of body coord
  kvs::Vector3d m_e1, m_e2, m_e3 ; // base vectors of body coord

  // LOD control
  bool          m_flagLOD ;

  // Object rotation
  bool   m_flagObjZXRot ;
  double m_objZXRotAngle[2]; 

  // Min-max object coordinates
  bool m_flagForcedBoundingBox ;
  kvs::Vector3f m_forcedMinPoint, m_forcedMaxPoint ;

  // Min-max object coordinates //ST_BOUNDING_BOX
  bool m_flagForcedStandardBoundingBox ;
  kvs::Vector3f m_forcedStandardMinPoint, m_forcedStandardMaxPoint ;

  // Shuffling flags
  bool m_flagShuffle       ; // shuffle by the particle renderer
  bool m_flagForcedShuffle ; // additinal shuffle in main()

  // Projection Type (Perspective = 0, Orthogonal = 1, Frustum =2 )
  kvs::Camera::ProjectionType m_flagCameraProjectionType;

  // Lambert shading
  bool m_flagLambertShading ;
  double m_kd, m_ka ;
  double m_ks;
  int    m_shininess; 

  // GRAYSCALE 
  //  grayscale id
  int    m_grayscale_id ;

  // For particle proliferation
  kvs::MersenneTwister m_unirand   ; // Generator of uniform random number  

  // For controlling mouse response speed 
  double m_mouse_rot_speed ; //ROTSPEED
  double m_mouse_zoom_speed; //ZOOMSPEED


  //---- For SPBR Binary Data
 public:
  // Constructor for SPBR Binary Data
  SPBR ( const char* input_file, SPBR_BINARY_FORMAT file_format );

  void generatePointObject_Binary( kvs::PointObject* point_object );
  void readHeader_Binary();
  void readPointData_Binary();
 private:
  std::ifstream m_finBin;

 //---- For PLY ASCII file data
 public:
  // Constructor for PLY ASCII file data
  SPBR(const char* input_file, PLY_ASCII_FORMAT format_type);//2018/8

 private:
  void generatePLYPointObject( kvs::PointObject* point_object );
  void readPLYHeader_and_countParticles ( void );
  void readPLYPointData ( void ) ;
  void read_SPBR_ParameterFile( const char* filename );

 private:
  unsigned int m_num_output_ply_column ;


  //---- For PLY_Binary file data //2018/8
  public:
  // Constructor for PLY Binary file data
  SPBR(const char* input_file, PLY_BINARY_FORMAT format_type);

 private:
	 void generatePLYPointObject_binary(kvs::PointObject* point_object);
	 void readPLYHeader_and_countParticles_Binary(void);
	 void readPLYPointData_Binary(void);
	 void read_SPBR_ParameterFile_Binary(const char* filename);
 /*private:
	 unsigned int m_num_output_ply_column;*/


//---- For Brightness Adjustment
private:
    // bool m_flagBrightnessAdjustment; // UCHIDA 2020/10/03
    int     m_brightness_adjustment_id; // UCHIDA 2021/02/09

    // UCHIDA 2020/09/29
    // void   setFlagBrightnessAdjustment( int flag );
    // bool   isBrightnessAdjustment ( void ) { return m_flagBrightnessAdjustment; }

public:
    // UCHIDA 2021/02/09
    void    setBrightnessAdjustmentID( const int id );
    int     getBrightnessAdjustmentID( void ) { return m_brightness_adjustment_id; }


//---- For Loop Image Application //TANAKATANAKA
private:
    double    m_end_rot_angle  ;//Default: 360.0
    int       m_num_rot_images ;//Default: 180
    char      m_axis_name[2];// Default: "Y"
    char      m_loop_image_name_head[256];// Default: "./IMAGE_DATA/image"

    void setEndRotAngle ( double angle ) { m_end_rot_angle = angle ; }
    void setNumRotImages( int num      ) { m_num_rot_images = num; }
    void setRotAxis     ( const char axis[] ) {std::strcpy( m_axis_name, axis);}
    void setLoopImageNameHead ( const char name_head[] )
         { std::strcpy( m_loop_image_name_head, name_head ) ; }

    void setDefaultLoopImageParam (void)
    {
      setEndRotAngle ( DEFAULT_END_ROT_ANGLE ) ;
      setNumRotImages( DEFAULT_NUM_ROT_IMAGES) ;
      setRotAxis     ( DEFAULT_ROT_AXIS_NAME ) ;
      setLoopImageNameHead ( DEFAULT_LOOP_IMAGE_NAME_HEAD );
    }

public:
    double getEndRotAngle ( void ) { return m_end_rot_angle;}
    int    getNumRotImages( void ) { return m_num_rot_images ;}
    const char*  getRotAxis     ( void ) { return m_axis_name      ;}
    const char*  getLoopImageNameHead ( void ) { return m_loop_image_name_head ; }

};

#endif // end of spbr.h
