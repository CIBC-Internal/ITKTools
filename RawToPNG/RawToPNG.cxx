
#include "itkRawImageIO.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkExtractImageFilter.h"

int
main(int argc, char *argv[])
{
  if ( argc < 3 )
  {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0] << " rawImagePath outputImagePath" << std::endl;
    return EXIT_FAILURE;
  }

  std::string rawImagePath = argv[1];
  std::string outputImagePath = argv[2];
  const unsigned int startIndex = 1, endIndex = 2;

  const unsigned int OutputDimension = 2;
  const unsigned int InputDimension = 3;
  const unsigned int SliceDimXY = 131072;
  typedef unsigned char PixelType;
  typedef itk::Image< PixelType, OutputDimension > OutputImageType;
  typedef itk::Image< PixelType, InputDimension > InputImageType;
  
  // create a ImageIO
  typedef itk::RawImageIO< PixelType, InputDimension > ImageIOType;
  ImageIOType::Pointer rawImageIO = ImageIOType::New();
  
  typedef itk::ImageFileReader< InputImageType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( rawImagePath );
  reader->SetImageIO( rawImageIO );  
  
  rawImageIO->SetDimensions( 0, SliceDimXY );
  rawImageIO->SetDimensions( 1, SliceDimXY );
  rawImageIO->SetDimensions( 2, 1 );
  rawImageIO->SetSpacing( 0, 1.0 );
  rawImageIO->SetSpacing( 1, 1.0 );
  rawImageIO->SetSpacing( 2, 1.0 );
  rawImageIO->SetOrigin(0, 0);
  rawImageIO->SetOrigin(1, 0);
  rawImageIO->SetOrigin(2, 0);
  rawImageIO->SetHeaderSize(0);
  rawImageIO->SetByteOrderToLittleEndian();
  rawImageIO->SetComponentType(itk::ImageIOBase::UCHAR);
  rawImageIO->SetNumberOfComponents(1);

  reader->Update();

  InputImageType::RegionType inputRegion = reader->GetOutput()->GetLargestPossibleRegion();
  InputImageType::SizeType size = inputRegion.GetSize();
  // collapse z direction
  size[2] = 0;
  InputImageType::IndexType start = inputRegion.GetIndex();
  start[2] = 0;
  InputImageType::RegionType desiredRegion;
  desiredRegion.SetSize(size);
  desiredRegion.SetIndex(start);

  // collapse 3D to 2D
  typedef itk::ExtractImageFilter< InputImageType, OutputImageType > ExtractFilterType;
  ExtractFilterType::Pointer extract = ExtractFilterType::New();
  extract->InPlaceOn();
  extract->SetDirectionCollapseToSubmatrix();
  extract->SetExtractionRegion(desiredRegion);
  extract->SetInput( reader->GetOutput() );
  
  typedef itk::ImageFileWriter< OutputImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( outputImagePath );

  writer->SetInput( extract->GetOutput() );
  writer->UseCompressionOn();
  writer->SetNumberOfStreamDivisions(400);
  
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
