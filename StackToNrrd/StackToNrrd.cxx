#include "itkNrrdImageIO.h"
#include "itkImage.h"
#include "itkImageSeriesReader.h"
#include "itkImageFileWriter.h"
#include "itkNumericSeriesFileNames.h"
#include "itkRGBPixel.h"

#include <string>

// see https://itk.org/Wiki/ITK/Examples/IO/VolumeFromSlices
int main( int argc, char * argv[] )
{
  // Verify the number of parameters in the command line
  if( argc < 5 )
  {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0]
    << " pattern firstSliceValue lastSliceValue nrrdPath"
    << std::endl;
    return EXIT_FAILURE;
  }
  
  typedef itk::RGBPixel< unsigned char >   PixelType;
  const unsigned int Dimension = 3;
  
  typedef itk::Image< PixelType, Dimension >  ImageType;
  typedef itk::ImageSeriesReader< ImageType > ReaderType;
  typedef itk::ImageFileWriter< ImageType > WriterType;

  itk::NrrdImageIO::Pointer nrrdImageIO = itk::NrrdImageIO::New();
  
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  writer->SetImageIO( nrrdImageIO );

  std::string pattern = argv[1];
  const unsigned int first = atoi( argv[2] );
  const unsigned int last  = atoi( argv[3] );
  std::string outputFilename = argv[4];
  
  typedef itk::NumericSeriesFileNames    NameGeneratorType;
  
  NameGeneratorType::Pointer nameGenerator = NameGeneratorType::New();
  
  nameGenerator->SetSeriesFormat( pattern );
  
  nameGenerator->SetStartIndex( first );
  nameGenerator->SetEndIndex( last );
  nameGenerator->SetIncrementIndex( 1 );
  std::vector<std::string> names = nameGenerator->GetFileNames();
  
  // List the files
  for ( auto &name : names )
  {
    std::cout << "File: " << name << std::endl;
  }
  
  reader->SetFileNames( names  );
  
  writer->SetFileName( outputFilename );
  writer->SetInput( reader->GetOutput() );
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