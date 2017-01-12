#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkRGBPixel.h"

#include <string>

// see https://itk.org/Wiki/ITK/Examples/IO/VolumeFromSlices
int main( int argc, char * argv[] )
{
  // Verify the number of parameters in the command line
  if( argc < 3 )
  {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0]
    << " inputFile outputFile"
    << std::endl;
    return EXIT_FAILURE;
  }
  
  typedef itk::RGBPixel< unsigned char >      PixelType;
  const unsigned int Dimension = 2;
  
  typedef itk::Image< PixelType, Dimension >  ImageType;
  typedef itk::ImageFileReader< ImageType >   ReaderType;
  typedef itk::ImageFileWriter< ImageType >   WriterType;
  
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  
  std::string inputFile = argv[1];
  std::string outputFile = argv[2];

  reader->SetFileName( inputFile );
  reader->Update();

  writer->SetInput( reader->GetOutput() );
  writer->SetFileName( outputFile );

  try
  {
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