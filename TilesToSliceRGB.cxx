#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkTileImageFilter.h"
#include "itkPNGImageIO.h"

#include <iostream>
#include <iomanip>
#include <sstream>

int
main(int argc, char *argv[] )
{
  if ( argc != 5 )
  {
    std::cerr << "Usage: "<< std::endl;
    std::cerr << argv[0];
    std::cerr << " inputPath outputFileName startIndex tilesLayout (> 2)";
    std::cerr << std::endl;
    return EXIT_FAILURE;
  }

  typedef int IndexType;
  typedef unsigned int UIndexType;

  std::string inputPath = argv[1];
  std::string outputFileName = argv[2];

  UIndexType start;
  std::istringstream issArgv3(argv[3]);
  issArgv3 >> start;

  UIndexType tilesLayout;
  std::istringstream issArgv4(argv[4]);
  issArgv4 >> tilesLayout;
  if (tilesLayout < 2)
  {
    std::cerr << "tilesLayout > 2" << std::endl;
  }

  // unix
  UIndexType pathLen = inputPath.size();
  if (inputPath[pathLen-1] != '/')
  {
    inputPath.append("/");
  }
  
  const IndexType StartIndex = start, EndIndex = start + tilesLayout - 1;

  const UIndexType Dimension = 2;
  typedef unsigned char ComponentType;
  typedef itk::RGBPixel< ComponentType > PixelType;
  typedef itk::Image< PixelType, Dimension> ImageType;
  
  // create a ImageIO
  typedef itk::PNGImageIO ImageIOType;
  ImageIOType::Pointer pngImageIO = ImageIOType::New();
  
  typedef itk::TileImageFilter< ImageType, ImageType > TileFilterType;
  TileFilterType::Pointer tileFilter = TileFilterType::New();
  
  itk::FixedArray< UIndexType, Dimension > layout;
  layout[0] = tilesLayout;
  layout[1] = tilesLayout;
  
  tileFilter->SetLayout( layout );

  UIndexType counter = 0;
  for (IndexType y = EndIndex; y >= StartIndex; --y)
  {
    for (IndexType x = StartIndex; x <= EndIndex; ++x)
    {      
      std::ostringstream inputFileFormat;
      inputFileFormat << inputPath
      << "X" << std::setfill('0') << std::setw(3) << x
      << "_Y" << std::setfill('0') << std::setw(3) << y
      << ".png";

      // Read images
      typedef itk::ImageFileReader< ImageType > ImageReaderType ;
      ImageReaderType::Pointer reader = ImageReaderType::New();
      reader->SetFileName( inputFileFormat.str().c_str() );
      reader->SetImageIO(pngImageIO);
 
      try
      {
        reader->Update();
      }
      catch( itk::ExceptionObject & error )
      {
        std::cerr << "Read error: " << error << std::endl;
        return EXIT_FAILURE;
      }
      
      tileFilter->SetInput(counter++, reader->GetOutput());
    }
  }

  // Set the value of output pixels which are created by mismatched size input images.
  // If the two images are the same height, this will not be used (shouldn't be needed)
//  unsigned char fillerValue = 0;
//  tileFilter->SetDefaultPixelValue( fillerValue );
  
  tileFilter->Update();
  
  // Write the output image
  typedef itk::ImageFileWriter< ImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  if (tilesLayout > 7)
  {
    writer->SetNumberOfStreamDivisions(2);
  }
  else if (tilesLayout > 15)
  {
    writer->SetNumberOfStreamDivisions(8);
  }
  else if (tilesLayout > 31)
  {
    writer->SetNumberOfStreamDivisions(16);
  }
  writer->SetInput( tileFilter->GetOutput() );
  writer->SetFileName( outputFileName );

  try
  {
    writer->Update();
  }
  catch( itk::ExceptionObject & error )
  {
    std::cerr << "Write error: " << error << std::endl;
    return EXIT_FAILURE;
  }
  
  return EXIT_SUCCESS;
}
