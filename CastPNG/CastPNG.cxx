
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkCastImageFilter.h"
#include "itkImageFileWriter.h"

int
main(int argc, char *argv[])
{
  if ( argc < 3 )
  {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0] << " input output" << std::endl;
    return EXIT_FAILURE;
  }

  std::string input = argv[1];
  std::string output = argv[2];

  const unsigned int Dimension = 2;
  typedef unsigned char InputPixelType;
  //typedef uint32_t OutputPixelType;
  typedef unsigned short OutputPixelType;
  typedef itk::Image< InputPixelType, Dimension > InputImageType;
  typedef itk::Image< OutputPixelType, Dimension > OutputImageType;
  
  typedef itk::ImageFileReader<InputImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( input );
  
  typedef itk::CastImageFilter< InputImageType, OutputImageType > CastFilterType;
  CastFilterType::Pointer castFilter = CastFilterType::New();
  castFilter->SetInput( reader->GetOutput() );
  
  typedef itk::ImageFileWriter< OutputImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( output );

  writer->SetInput( castFilter->GetOutput() );
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
