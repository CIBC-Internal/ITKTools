
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkConstantPadImageFilter.h"
#include "itkImageFileWriter.h"

#include <string>
#include <sstream>


int
main(int argc, char *argv[])
{
  if ( argc < 3 )
  {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0]
              << " input output pad_lower_bound0 pad_lower_bound1 pad_upper_bound0 pad_upper_bound1 pixel_value"
              << std::endl;
    return EXIT_FAILURE;
  }

  std::string input( argv[1] );
  std::string output( argv[2] );

  const unsigned int Dimension = 2;
  //typedef uint32_t OutputPixelType;
  typedef unsigned short PixelType;
  typedef itk::Image< PixelType, Dimension > ImageType;

  ImageType::SizeType lowerExtendRegion;
  ImageType::SizeType upperExtendRegion;

  ImageType::PixelType constantPixel;

  std::istringstream iss_argv3( argv[3] );
  iss_argv3 >> lowerExtendRegion[0];
  std::istringstream iss_argv4( argv[4] );
  iss_argv4 >> lowerExtendRegion[1];
  std::istringstream iss_argv5( argv[5] );
  iss_argv5 >> upperExtendRegion[0];
  std::istringstream iss_argv6( argv[6] );
  iss_argv6 >> upperExtendRegion[1];

  std::istringstream iss_argv7( argv[7] );
  iss_argv7 >> constantPixel;

//  std::cerr << lowerExtendRegion[0] << " " << lowerExtendRegion[1] << " " << upperExtendRegion[0] << " " << upperExtendRegion[1] << " " << constantPixel << std::endl;

  typedef itk::ImageFileReader< ImageType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( input );

//  typedef itk::CastImageFilter< InputImageType, OutputImageType > CastFilterType;
//  CastFilterType::Pointer castFilter = CastFilterType::New();
//  castFilter->SetInput( reader->GetOutput() );

  typedef itk::ConstantPadImageFilter < ImageType, ImageType > ConstantPadImageFilterType;
  ConstantPadImageFilterType::Pointer padFilter = ConstantPadImageFilterType::New();
  padFilter->SetInput( reader->GetOutput() );
  padFilter->SetPadLowerBound(lowerExtendRegion);
  padFilter->SetPadUpperBound(upperExtendRegion);
  padFilter->SetConstant(constantPixel);
  padFilter->Update();

  typedef itk::ImageFileWriter< ImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( output );

  writer->SetInput( padFilter->GetOutput() );
  writer->UseCompressionOn();
  writer->SetNumberOfStreamDivisions(10);

  try
  {
    // update by streaming
    writer->Update();
  }
  catch( itk::ExceptionObject & err )
  {
    std::cerr << "ExceptionObject caught !" << std::endl;
    std::cerr << err << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
