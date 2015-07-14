#include "itkNrrdImageIO.h"
#include "itkImageFileReader.h"
#include "itkImageSeriesWriter.h"
#include "itkNumericSeriesFileNames.h"

#include <iostream>

int
main(int argc, char *argv[])
{
  if ( argc < 4 )
  {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0] << " nrrdImagePath outputPrefix outputExtension" << std::endl;
    return EXIT_FAILURE;
  }

  std::string nrrdImagePath = argv[1];
  std::string format = argv[2];
  format += "%03d.";
  format += argv[3];   // filename extension

  const unsigned int OutputDimension = 2;
  const unsigned int InputDimension = 3;
  typedef unsigned char PixelType;
  typedef itk::Image< PixelType, OutputDimension > OutputImageType;
  typedef itk::Image< PixelType, InputDimension > InputImageType;

  itk::NrrdImageIO::Pointer nrrdImageIO = itk::NrrdImageIO::New();

  typedef itk::ImageFileReader< InputImageType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( nrrdImagePath );
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

//std::cerr << nrrdImageIO->GetNumberOfDimensions() << ", " << nrrdImageIO->GetPixelTypeAsString(nrrdImageIO->GetPixelType() ) << std::endl;
//std::cerr << reader->GetOutput()->GetLargestPossibleRegion() << std::endl;

  InputImageType::RegionType region = reader->GetOutput()->GetLargestPossibleRegion();
  InputImageType::IndexType start = region.GetIndex();
  InputImageType::SizeType size  = region.GetSize();

  if (region.GetImageDimension() < 3)
  {
    std::cerr << "Error: expecting 3D data." << std::endl;
    return EXIT_FAILURE;
  }
std::cerr << start[0] << ", " << start[1] << ", " << start[2] << std::endl;
std::cerr << size[0] << ", " << size[1] << ", " << size[2] << std::endl;

  unsigned int firstSlice = start[ 2 ];
  unsigned int lastSlice = start[ 2 ] + size[ 2 ] - 1;
std::cerr << firstSlice << ", " << lastSlice << std::endl;

  typedef itk::ImageSeriesWriter< InputImageType, OutputImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();

  typedef itk::NumericSeriesFileNames NamesGeneratorType;
  NamesGeneratorType::Pointer nameGenerator = NamesGeneratorType::New();

  nameGenerator->SetSeriesFormat( format.c_str() );
  nameGenerator->SetStartIndex( firstSlice );
  nameGenerator->SetEndIndex( lastSlice );
  nameGenerator->SetIncrementIndex( 1 );

  writer->SetInput( reader->GetOutput() );
  writer->SetFileNames( nameGenerator->GetFileNames() );
  writer->UseCompressionOn();

  try
  {
    writer->Update();
  }
  catch (itk::ExceptionObject& e)
  {
    std::cerr << e << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}