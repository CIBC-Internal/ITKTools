
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkConstantPadImageFilter.h"
#include "itkImageFileWriter.h"

#include <string>
#include <sstream>


template<class T>
int PadImage(const std::string& input, const std::string& output,
             const int lowerRegion0, const int lowerRegion1, const int upperRegion0, const int upperRegion1,
             const int pixelValue)
{
  
  const unsigned int Dimension = 2;
  //typedef uint32_t OutputPixelType;
  typedef T PixelType;
  typedef itk::Image< PixelType, Dimension > ImageType;
  
  typename ImageType::SizeType lowerExtendRegion;
  typename ImageType::SizeType upperExtendRegion;

  lowerExtendRegion[0] = lowerRegion0;
  lowerExtendRegion[1] = lowerRegion1;
  upperExtendRegion[0] = upperRegion0;
  upperExtendRegion[1] = upperRegion1;

  typename ImageType::PixelType constantPixel = pixelValue;
  
  //  std::cerr << lowerExtendRegion[0] << " " << lowerExtendRegion[1] << " " << upperExtendRegion[0] << " " << upperExtendRegion[1] << " " << constantPixel << std::endl;
  
  typedef typename itk::ImageFileReader< ImageType > ReaderType;
  typename ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( input );
  
  //  typedef itk::CastImageFilter< InputImageType, OutputImageType > CastFilterType;
  //  CastFilterType::Pointer castFilter = CastFilterType::New();
  //  castFilter->SetInput( reader->GetOutput() );
  
  typedef typename itk::ConstantPadImageFilter < ImageType, ImageType > ConstantPadImageFilterType;
  typename ConstantPadImageFilterType::Pointer padFilter = ConstantPadImageFilterType::New();
  padFilter->SetInput( reader->GetOutput() );
  padFilter->SetPadLowerBound(lowerExtendRegion);
  padFilter->SetPadUpperBound(upperExtendRegion);
  padFilter->SetConstant(constantPixel);
  padFilter->Update();
  
  typedef typedef itk::ImageFileWriter< ImageType > WriterType;
  typename WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( output );
  
  writer->SetInput( padFilter->GetOutput() );
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

int
main(int argc, char *argv[])
{
  if ( argc < 9 )
  {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0]
              << " input output pad_lower_bound0 pad_lower_bound1 pad_upper_bound0 pad_upper_bound1 pixel_value ushort|uchar"
              << std::endl;
    return EXIT_FAILURE;
  }

  std::string input( argv[1] );
  std::string output( argv[2] );

  int lowerRegion0 = 0, lowerRegion1 = 0, upperRegion0 = 0, upperRegion1 = 0, pixelValue = 0;
  std::istringstream iss_argv3( argv[3] );
  iss_argv3 >> lowerRegion0;
  std::istringstream iss_argv4( argv[4] );
  iss_argv4 >> lowerRegion1;
  std::istringstream iss_argv5( argv[5] );
  iss_argv5 >> upperRegion0;
  std::istringstream iss_argv6( argv[6] );
  iss_argv6 >> upperRegion1;

  std::istringstream iss_argv7( argv[7] );
  iss_argv7 >> pixelValue;

  std::string type( argv[8] );

  if (type == "ushort")
  {
    return PadImage<unsigned short>(input, output, lowerRegion0, lowerRegion1, upperRegion0, upperRegion1, pixelValue);
  }
  else
  {
    return PadImage<unsigned char>(input, output, lowerRegion0, lowerRegion1, upperRegion0, upperRegion1, pixelValue);
  }
//
////  std::cerr << lowerExtendRegion[0] << " " << lowerExtendRegion[1] << " " << upperExtendRegion[0] << " " << upperExtendRegion[1] << " " << constantPixel << std::endl;
//
//  typedef itk::ImageFileReader< ImageType > ReaderType;
//  ReaderType::Pointer reader = ReaderType::New();
//  reader->SetFileName( input );
//
////  typedef itk::CastImageFilter< InputImageType, OutputImageType > CastFilterType;
////  CastFilterType::Pointer castFilter = CastFilterType::New();
////  castFilter->SetInput( reader->GetOutput() );
//
//  typedef itk::ConstantPadImageFilter < ImageType, ImageType > ConstantPadImageFilterType;
//  ConstantPadImageFilterType::Pointer padFilter = ConstantPadImageFilterType::New();
//  padFilter->SetInput( reader->GetOutput() );
//  padFilter->SetPadLowerBound(lowerExtendRegion);
//  padFilter->SetPadUpperBound(upperExtendRegion);
//  padFilter->SetConstant(constantPixel);
//  padFilter->Update();
//
//  typedef itk::ImageFileWriter< ImageType > WriterType;
//  WriterType::Pointer writer = WriterType::New();
//  writer->SetFileName( output );
//
//  writer->SetInput( padFilter->GetOutput() );
//  writer->UseCompressionOn();
//  writer->SetNumberOfStreamDivisions(10);
//
//  try
//  {
//    // update by streaming
//    writer->Update();
//  }
//  catch( itk::ExceptionObject & err )
//  {
//    std::cerr << "ExceptionObject caught !" << std::endl;
//    std::cerr << err << std::endl;
//    return EXIT_FAILURE;
//  }
//
//  return EXIT_SUCCESS;
}
