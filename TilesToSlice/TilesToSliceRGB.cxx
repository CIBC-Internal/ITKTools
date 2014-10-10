#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkTileImageFilter.h"
#include "itkPNGImageIO.h"
#include "itkRGBToLuminanceImageFilter.h"

#include "FormatTmpFileNames.hxx"

#include <boost/filesystem.hpp>

#include <iostream>
#include <sstream>

#include <omp.h>

namespace bfs = boost::filesystem;

int
main(int argc, char *argv[] )
{
  if ( argc != 4 )
  {
    std::cerr << "Usage: "<< std::endl;
    std::cerr << argv[0];
    std::cerr << " outputFileName cachePath tilesLayout (> 2)";
    std::cerr << std::endl;
    return EXIT_FAILURE;
  }

  typedef long int IndexType;
  typedef unsigned long int UIndexType;

  bfs::path outputFilePath(argv[1]);
  bfs::path tmpPath(argv[2]);
  if (! boost::filesystem::exists(tmpPath) )
  {
    std::cerr << "Cache directory " << tmpPath.string() << " does not exist." << std::endl;
    return EXIT_FAILURE;
  }

  UIndexType tilesLayout;
  std::istringstream issArgv3(argv[3]);
  issArgv3 >> tilesLayout;
  if (tilesLayout < 2)
  {
    std::cerr << "tilesLayout > 2" << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << argv[0] << " "
            << outputFilePath.string() << " "
            << tmpPath.string() << " "
            << tilesLayout << std::endl;

  const UIndexType Dimension = 2;
  typedef unsigned char ComponentType;

  typedef itk::RGBPixel< ComponentType > InputPixelType;
  typedef ComponentType OutputPixelType;

  typedef itk::Image< InputPixelType, Dimension> InputImageType;  
  typedef itk::Image< OutputPixelType, Dimension > OutputImageType;

  typedef itk::PNGImageIO ImageIOType;
  typedef itk::ImageFileReader< InputImageType > ImageReaderType;
  typedef itk::ImageFileWriter< OutputImageType > WriterType;

  typedef itk::TileImageFilter< InputImageType, InputImageType > TileFilterType;
  typedef itk::RGBToLuminanceImageFilter< InputImageType, OutputImageType > GrayscaleConvertFilterType;

  TileFilterType::Pointer tileFilterTmp = TileFilterType::New();
  itk::FixedArray< UIndexType, Dimension > layoutTmp;
  layoutTmp[0] = 1; // tiling rows
  layoutTmp[1] = tilesLayout;
  tileFilterTmp->SetLayout( layoutTmp );
  UIndexType counterTmp = 0;
  const IndexType TileListBegin = tilesLayout-1, TileListEnd = 0;
  
  // Read images
  ImageReaderType::Pointer reader = ImageReaderType::New();
  // create a ImageIO
  ImageIOType::Pointer pngImageIO = ImageIOType::New();
  reader->SetImageIO(pngImageIO);
  
  for (IndexType r = TileListBegin; r >= TileListEnd; --r)
  {
    // Tried using array - failed due to possible scope issues in parallel block?
    boost::filesystem::path tmpFilePath = formatTmpFileNames(tmpPath, r);
    std::cout << "Read " << tmpFilePath.string() << std::endl;

    // Read images
//    ImageReaderType::Pointer reader = ImageReaderType::New();
//    // create a ImageIO
//    ImageIOType::Pointer pngImageIO = ImageIOType::New();
//    reader->SetImageIO(pngImageIO);
    reader->SetFileName(tmpFilePath.string());

    try
    {
      // UpdateLargestPossibleRegion() used instead of Update() when reusing a reader
      reader->UpdateLargestPossibleRegion();
    }
    catch( itk::ExceptionObject & error )
    {
      std::cerr << "Read error: " << error << std::endl;
      return EXIT_FAILURE;
    }

    tileFilterTmp->SetInput(counterTmp++, reader->GetOutput());
    tileFilterTmp->Update();
  }

  GrayscaleConvertFilterType::Pointer grayFilter = GrayscaleConvertFilterType::New();
  grayFilter->SetInput( tileFilterTmp->GetOutput() );

  // Write the output image
  WriterType::Pointer writer = WriterType::New();
//  // create a ImageIO
//  ImageIOType::Pointer pngImageIO = ImageIOType::New();
//  writer->SetImageIO(pngImageIO);
  writer->SetNumberOfStreamDivisions(400);
  writer->SetFileName( outputFilePath.string() );
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
