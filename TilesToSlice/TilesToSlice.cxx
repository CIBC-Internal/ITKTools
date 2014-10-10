#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkTileImageFilter.h"
#include "itkPNGImageIO.h"

#include <iostream>
#include <iomanip>
#include <sstream>

int main(int argc, char *argv[] )
{
  if( argc != 3 )
  {
    std::cerr << "Usage: "<< std::endl;
    std::cerr << argv[0];
    std::cerr << " inputPath outputFileName";
    std::cerr << std::endl;
    return EXIT_FAILURE;
  }

  std::string inputPath = argv[1];
  std::string outputFileName = argv[2];
  
  typedef unsigned int IndexType;
  const IndexType Dimension = 2;
  
  // unix
  {
    unsigned int pathLen = inputPath.size();
    if (inputPath[pathLen-1] != '/')
    {
      inputPath.append("/");
    }
  }
  
  const IndexType XStartIndex = 0, XEndIndex = 2,
  YStartIndex = 0, YEndIndex = 2;
  
  // create a ImageIO
  typedef itk::PNGImageIO ImageIOType;
  ImageIOType::Pointer pngImageIO = ImageIOType::New();
  
  typedef unsigned char PixelType;
  typedef itk::Image< PixelType, Dimension> ImageType;

  typedef itk::TileImageFilter< ImageType, ImageType > TileFilterType;
  
  TileFilterType::Pointer tileFilter = TileFilterType::New();
  
  itk::FixedArray< IndexType, Dimension > layout;
  
  layout[0] = 3;
  layout[1] = 3;
  
  tileFilter->SetLayout( layout );

  IndexType counter = 0;
  for (IndexType x = XStartIndex; x <= XEndIndex; ++x)
  {
    for (IndexType y = YStartIndex; y <= YEndIndex; ++y)
    {      
      std::ostringstream inputFileFormat;
      inputFileFormat << inputPath
      << "X" << std::setfill('0') << std::setw(3) << x
      << "_Y" << std::setfill('0') << std::setw(3) << y
      << "_gray.png";
std::cerr << "Read " << inputFileFormat.str() << std::endl;
      // Read images
      typedef itk::ImageFileReader< ImageType > ImageReaderType ;
      ImageReaderType::Pointer reader = ImageReaderType::New();
      reader->SetFileName( inputFileFormat.str().c_str() );
      reader->SetImageIO(pngImageIO);
      reader->Update();

      tileFilter->SetInput(counter++, reader->GetOutput());
    }
  }

  // Set the value of output pixels which are created by mismatched size input images.
  // If the two images are the same height, this will not be used (shouldn't be needed)
  unsigned char fillerValue = 0;
  tileFilter->SetDefaultPixelValue( fillerValue );
  
  tileFilter->Update();
  
  // Write the output image
  typedef itk::ImageFileWriter< ImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetNumberOfStreamDivisions(16);
  writer->SetInput( tileFilter->GetOutput() );
  writer->SetFileName( outputFileName );
  writer->Update();
  
  return EXIT_SUCCESS;
}
