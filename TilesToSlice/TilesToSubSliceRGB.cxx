#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkTileImageFilter.h"
#include "itkPNGImageIO.h"

#include "FormatTmpFileNames.hxx"

#include <boost/filesystem.hpp>

#include <iostream>
#include <sstream>

#include <omp.h>

namespace bfs = boost::filesystem;


int
main(int argc, char *argv[] )
{
  if ( argc != 6 )
  {
    std::cerr << "Usage: "<< std::endl;
    std::cerr << argv[0];
    std::cerr << " inputPath cachePath startRow rows tilesLayout (> 2)";
    std::cerr << std::endl;
    return EXIT_FAILURE;
  }

  typedef long int IndexType;
  typedef unsigned long int UIndexType;

  bfs::path inputPath(argv[1]);
  bfs::path tmpPath(argv[2]);

  if (! bfs::exists(tmpPath) )
  {
    if (! bfs::create_directory(tmpPath) )
    {
      std::cerr << "Could not create directory " << tmpPath.string() << std::endl;
      return EXIT_FAILURE;
    }
  }
  
  UIndexType startRow;
  {
    std::istringstream iss(argv[3]);
    iss >> startRow;
  }

  UIndexType rows;
  {
    std::istringstream iss(argv[4]);
    iss >> rows;
  }

  UIndexType tilesLayout;
  {
    std::istringstream iss(argv[5]);
    iss >> tilesLayout;
  }
  if (tilesLayout < 2)
  {
    std::cerr << "tilesLayout > 2" << std::endl;
    return EXIT_FAILURE;
  }

  const UIndexType StartXIndex = 0, EndXIndex = tilesLayout - 1, StartYIndex = startRow, EndYIndex = startRow + rows - 1;

  std::cout << argv[0] << " " << inputPath.string() << " "
            << tmpPath.string() << " "
            << StartXIndex << " " << EndXIndex << " "
            << StartYIndex << " " << EndYIndex << " "
            << tilesLayout << " " << std::endl;

  const UIndexType Dimension = 2;
  typedef unsigned char ComponentType;
  typedef itk::RGBPixel< ComponentType > PixelType;
  typedef itk::Image< PixelType, Dimension> ImageType;  
  typedef itk::TileImageFilter< ImageType, ImageType > TileFilterType;
  typedef itk::PNGImageIO ImageIOType;
  typedef itk::ImageFileWriter< ImageType > WriterType;
  typedef itk::ImageFileReader< ImageType > ImageReaderType;


  UIndexType exceptionCounterRead = 0, exceptionCounterWrite = 0;
  UIndexType counter = 0;

  #pragma omp parallel for \
    shared(exceptionCounterRead, exceptionCounterWrite), \
    firstprivate(counter), \
    num_threads(rows), \
    schedule(static, 1)
  for (UIndexType y = StartYIndex; y <= EndYIndex; ++y)
  {
    TileFilterType::Pointer tileFilter = TileFilterType::New();
    itk::FixedArray< UIndexType, Dimension > layout;
    layout[0] = tilesLayout;
    layout[1] = 0;
    tileFilter->SetLayout( layout );

    // Read images
    ImageReaderType::Pointer reader = ImageReaderType::New();
    // create a ImageIO
    ImageIOType::Pointer pngImageIO = ImageIOType::New();
    reader->SetImageIO(pngImageIO);
    
    bfs::path tmpFilePath = formatTmpFileNames(tmpPath, y); 

    for (UIndexType x = StartXIndex; x <= EndXIndex; ++x)
    {
      std::ostringstream inputFileFormat;
      inputFileFormat << "X" << std::setfill('0') << std::setw(3) << x
                      << "_Y" << std::setfill('0') << std::setw(3) << y
                      << ".png";
      bfs::path inputFile = inputPath / inputFileFormat.str();
      reader->SetFileName( inputFile.string() );

      try
      {
        // UpdateLargestPossibleRegion() used instead of Update() when reusing a reader
        reader->UpdateLargestPossibleRegion();
      }
      catch( itk::ExceptionObject & error )
      {
        #pragma omp critical
        {
          std::cerr << "Read error: " << error << std::endl;
          ++exceptionCounterRead;
        }
      }

      tileFilter->SetInput(counter++, reader->GetOutput());
    }
    // Set the value of output pixels which are created by mismatched size input images.
    // If the two images are the same height, this will not be used (shouldn't be needed)
    //unsigned char fillerValue = 0;
    //tileFilter->SetDefaultPixelValue( fillerValue );

    tileFilter->Update();

    // Write the output image
    WriterType::Pointer writer = WriterType::New();
    // create a ImageIO
    writer->SetImageIO(pngImageIO);
    writer->SetNumberOfStreamDivisions(300);
    writer->SetInput( tileFilter->GetOutput() );
    writer->SetFileName( tmpFilePath.string() );

    try
    {
      std::cerr << "Writing " << writer->GetFileName() << std::endl;
      writer->Update();
    }
    catch( itk::ExceptionObject & error )
    {
      #pragma omp critical
      {
        std::cerr << "Write error: " << error << std::endl;
        ++exceptionCounterWrite;
      }
    }
  }

  #pragma omp barrier
  
  if ( (exceptionCounterRead > 0) || (exceptionCounterWrite > 0) )
  {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
