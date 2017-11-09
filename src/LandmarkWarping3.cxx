// warp 3d image with landmarks
// modified LandmarkWarping2.cxx from ITK/Examples by Yang Yu, 10/10/2017

//
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkWarpImageFilter.h"
#include "itkVector.h"
#include "itkLandmarkDisplacementFieldSource.h"
#include <fstream>

//
int main( int argc, char * argv[] )
{

  if( argc < 3 )
    {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " landmarksFile fixedImage ";
    std::cerr << "movingImage deformedMovingImage" << std::endl;
    return EXIT_FAILURE;
    }

  const     unsigned int   Dimension = 3;
  typedef   float          VectorComponentType;

  typedef   itk::Vector< VectorComponentType, Dimension >    VectorType;

  typedef   itk::Image< VectorType, Dimension >   DisplacementFieldType;

  typedef   unsigned short                           PixelType;
  typedef   itk::Image< PixelType, Dimension >       FixedImageType;
  typedef   itk::Image< PixelType, Dimension >       MovingImageType;

  typedef   itk::ImageFileReader< FixedImageType  >  FixedReaderType;
  typedef   itk::ImageFileReader< MovingImageType >  MovingReaderType;

  typedef   itk::ImageFileWriter< MovingImageType >  MovingWriterType;


  FixedReaderType::Pointer fixedReader = FixedReaderType::New();
  fixedReader->SetFileName( argv[2] );

  try
    {
    fixedReader->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << "Exception thrown " << std::endl;
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }


  MovingReaderType::Pointer movingReader = MovingReaderType::New();
  MovingWriterType::Pointer movingWriter = MovingWriterType::New();

  movingReader->SetFileName( argv[3] );
  movingWriter->SetFileName( argv[4] );


  FixedImageType::ConstPointer fixedImage = fixedReader->GetOutput();

  typedef itk::LandmarkDisplacementFieldSource<
                                DisplacementFieldType
                                             >  DisplacementSourceType;

  DisplacementSourceType::Pointer deformer = DisplacementSourceType::New();

  deformer->SetOutputSpacing( fixedImage->GetSpacing() );
  deformer->SetOutputOrigin(  fixedImage->GetOrigin() );
  deformer->SetOutputRegion(  fixedImage->GetLargestPossibleRegion() );
  deformer->SetOutputDirection( fixedImage->GetDirection() );

  typedef DisplacementSourceType::LandmarkContainer LandmarkContainerType;
  typedef DisplacementSourceType::LandmarkPointType LandmarkPointType;

  LandmarkContainerType::Pointer sourceLandmarks =
    LandmarkContainerType::New();
  LandmarkContainerType::Pointer targetLandmarks =
    LandmarkContainerType::New();

  LandmarkPointType sourcePoint;
  LandmarkPointType targetPoint;

  std::ifstream pointsFile;
  pointsFile.open( argv[1] );

  unsigned int pointId = 0;

  pointsFile >> sourcePoint;
  pointsFile >> targetPoint;

  while( !pointsFile.fail() )
    {
    sourceLandmarks->InsertElement( pointId, sourcePoint );
    targetLandmarks->InsertElement( pointId, targetPoint );
    ++pointId;

    pointsFile >> sourcePoint;
    pointsFile >> targetPoint;

    }

  pointsFile.close();

  deformer->SetSourceLandmarks( sourceLandmarks.GetPointer() );
  deformer->SetTargetLandmarks( targetLandmarks.GetPointer() );

  try
    {
    deformer->UpdateLargestPossibleRegion();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << "Exception thrown " << std::endl;
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }

  DisplacementFieldType::ConstPointer displacementField = deformer->GetOutput();

  typedef itk::WarpImageFilter< MovingImageType,
                                MovingImageType,
                                DisplacementFieldType  >  FilterType;

  FilterType::Pointer warper = FilterType::New();

  typedef itk::LinearInterpolateImageFunction<
                       MovingImageType, double >  InterpolatorType;

  InterpolatorType::Pointer interpolator = InterpolatorType::New();

  warper->SetInterpolator( interpolator );


  warper->SetOutputSpacing( displacementField->GetSpacing() );
  warper->SetOutputOrigin(  displacementField->GetOrigin() );

  warper->SetDisplacementField( displacementField );

  warper->SetInput( movingReader->GetOutput() );

  movingWriter->SetInput( warper->GetOutput() );

  try
    {
    movingWriter->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << "Exception thrown " << std::endl;
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;

}
