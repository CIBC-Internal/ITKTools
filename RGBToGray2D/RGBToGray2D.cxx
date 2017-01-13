#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkRGBToLuminanceImageFilter.h"

#include <iostream>
#include <sstream>


int
main(int argc, char *argv[] )
{
  if ( argc < 3 )
  {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0] << " input output" << std::endl;
    return EXIT_FAILURE;
  }
  
  typedef long int IndexType;
  typedef unsigned long int UIndexType;
  
  std::string inputPath = argv[1];
  std::string outputPath = argv[2];
  
  std::cout << argv[0] << " "
  << inputPath << " "
  << outputPath << std::endl;
  
  const UIndexType Dimension = 2;
  typedef unsigned char InputComponentType;
  typedef itk::RGBPixel< InputComponentType > InputPixelType;

  typedef float OutputPixelType;
  
  typedef itk::Image< InputPixelType, Dimension> InputImageType;
  typedef itk::Image< OutputPixelType, Dimension > OutputImageType;
  
  typedef itk::ImageFileReader< InputImageType > ImageReaderType;
  typedef itk::ImageFileWriter< OutputImageType > WriterType;
  
  typedef itk::RGBToLuminanceImageFilter< InputImageType, OutputImageType > GrayscaleConvertFilterType;
  
  // Read images
  ImageReaderType::Pointer reader = ImageReaderType::New();
  reader->SetFileName( inputPath );
  
  try
  {
    reader->Update();
  }
  catch(itk::ExceptionObject& e)
  {
    std::cerr << e << std::endl;
    return EXIT_FAILURE;
  }

  GrayscaleConvertFilterType::Pointer grayFilter = GrayscaleConvertFilterType::New();
  grayFilter->SetInput( reader->GetOutput() );
  
  // Write the output image
  WriterType::Pointer writer = WriterType::New();
  writer->SetNumberOfStreamDivisions(10);
  writer->SetFileName( outputPath );
  writer->SetInput( grayFilter->GetOutput() );
  
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
