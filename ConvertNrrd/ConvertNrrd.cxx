#include "itkNrrdImageIO.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include <iostream>
#include <sstream>

int
main(int argc, char *argv[])
{
  if ( argc < 3 )
  {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0] << " nrrdInput output" << std::endl;
    return EXIT_FAILURE;
  }

  typedef long int IndexType;
  typedef unsigned long int UIndexType;

  std::string inputPath = argv[1];
  std::string outputPath = argv[2];

  std::cout << argv[0] << " " << inputPath << " " << outputPath << std::endl;

  const UIndexType Dimension = 2;
  typedef float PixelType;

  typedef itk::Image< PixelType, Dimension > InputImageType;
  typedef itk::Image< PixelType, Dimension > OutputImageType;

  typedef itk::ImageFileReader< InputImageType > ImageReaderType;
  typedef itk::ImageFileWriter< OutputImageType > WriterType;

  itk::NrrdImageIO::Pointer nrrdImageIO = itk::NrrdImageIO::New();

  // Read images
  ImageReaderType::Pointer reader = ImageReaderType::New();
  reader->SetFileName( inputPath );
  reader->SetImageIO( nrrdImageIO );

  try
  {
    reader->Update();
  }
  catch(itk::ExceptionObject& e)
  {
    std::cerr << e << std::endl;
    return EXIT_FAILURE;
  }

  // Write the output image
  WriterType::Pointer writer = WriterType::New();
  writer->SetNumberOfStreamDivisions(10);
  writer->SetFileName( outputPath );
  writer->SetInput( reader->GetOutput() );

  try
  {
    std::cout << "Writing " << writer->GetFileName() << std::endl;
    writer->Update();
  }
  catch( itk::ExceptionObject & error )
  {
    std::cerr << "Write error: " << error << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
