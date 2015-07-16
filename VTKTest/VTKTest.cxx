#include "itkVTKImageIO.h"
#include "itkImageFileReader.h"

#include <iostream>

int
main(int argc, char *argv[])
{
  if ( argc < 2 )
  {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0] << " vtkFile" << std::endl;
    return EXIT_FAILURE;
  }

  std::string vtkImagePath = argv[1];

  const unsigned int InputDimension = 3;
  typedef unsigned char PixelType;
  typedef itk::Image< PixelType, InputDimension > InputImageType;

  itk::VTKImageIO::Pointer vtkImageIO = itk::VTKImageIO::New();

  typedef itk::ImageFileReader< InputImageType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( vtkImagePath );
  reader->SetImageIO( vtkImageIO );

  try
  {
    reader->Update();
  }
  catch(itk::ExceptionObject& e)
  {
    std::cerr << e << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
