/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ImageAdaptor2.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif


// Software Guide : BeginLatex
//
// This example illustrates how to use the \doxygen{ImageAdaptor}
// to access the individual components of an RGB image. In this case, we 
// create an ImageAdaptor that will accept a RGB image as input and 
// presents it as a scalar image. The pixel data
// will be taken directly from the red channel of the original image.
// 
// \index{itk::ImageAdaptor!Instantiation}
// \index{itk::ImageAdaptor!Header}
//
// Software Guide : EndLatex 

#include "itkImage.h"
#include "itkImageAdaptor.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"


//  Software Guide : BeginLatex
//
//  As with the previous example, the bulk of the effort in creating the image 
//  adaptor is associated with the definition of the pixel accessor class. In 
//  this case, the accessor converts a RGB vector to a scalar containing the 
//  red channel component. Note that in the following, we do not need to define
//  the \code{Set()} method since we only expect the adaptor to be used for 
//  reading data from the image.
//
//  Software Guide : EndLatex 


// Software Guide : BeginCodeSnippet
class RedChannelPixelAccessor  
{
public:
  typedef itk::RGBPixel<float>   InternalType;
  typedef               float    ExternalType;

  static ExternalType Get( const InternalType & input ) 
    {
      return static_cast<ExternalType>( input.GetRed() );
    }
};
// Software Guide : EndCodeSnippet


//  Software Guide : BeginLatex
//
//  The \code{Get()} method simply calls the \code{GetRed()} method
//  defined in the \doxygen{RGBPixel} class.
//
//  Software Guide : EndLatex 


//-------------------------
//
//   Main code
//
//-------------------------

int main( int argc, char *argv[] ) 
{
  if( argc < 3 )
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << "ImageAdaptor2   inputRGBFileName outputRedChannelFileName" << std::endl;
    return -1;
    }


//  Software Guide : BeginLatex
//
//  Now we use the internal pixel type of the pixel accessor to define the
//  input image type, and then proceed to instantiate the ImageAdaptor type.
//
//  \index{PixelAccessor!RGB red channel}
//  \index{itk::ImageAdaptor!RGB red channel}
//  \index{ImageAdaptor!RGB red channel}
//
//  Software Guide : EndLatex 


// Software Guide : BeginCodeSnippet
  typedef RedChannelPixelAccessor::InternalType  InputPixelType;
  const   unsigned int   Dimension = 2;
  typedef itk::Image< InputPixelType, Dimension >   ImageType;
  typedef itk::ImageAdaptor<  ImageType, RedChannelPixelAccessor > 
    ImageAdaptorType;

  ImageAdaptorType::Pointer adaptor = ImageAdaptorType::New();
// Software Guide : EndCodeSnippet


//  Software Guide : BeginLatex
//
//  We create an image reader and connect the output to the adaptor
//  as before.
//
//  Software Guide : EndLatex 


// Software Guide : BeginCodeSnippet
  typedef itk::ImageFileReader< ImageType >   ReaderType;
  ReaderType::Pointer reader = ReaderType::New();  
// Software Guide : EndCodeSnippet

  reader->SetFileName( argv[1] );
  reader->Update();

// Software Guide : BeginCodeSnippet
  adaptor->SetImage( reader->GetOutput() );
// Software Guide : EndCodeSnippet
 

//  Software Guide : BeginLatex
//
//  We create an \doxygen{RescaleIntensityImageFilter} and an
//  \doxygen{ImageFileWriter} to rescale the dynamic range of the pixel values
//  and send the extracted channel to an image file. Note that the image type
//  used for the rescaling filter is the \code{ImageAdaptorType} itself. That
//  is, the adaptor type is used in the same context as an image type.
//
//  Software Guide : EndLatex 


// Software Guide : BeginCodeSnippet
  typedef itk::Image< unsigned char, Dimension >   OutputImageType;
  typedef itk::RescaleIntensityImageFilter< ImageAdaptorType, OutputImageType >
    RescalerType;

  RescalerType::Pointer rescaler = RescalerType::New();
  typedef itk::ImageFileWriter< OutputImageType >   WriterType;
  WriterType::Pointer writer = WriterType::New();
// Software Guide : EndCodeSnippet


  writer->SetFileName( argv[2] );


//  Software Guide : BeginLatex
//
//  Finally, we connect the adaptor as the input to the rescaler and invoke the
//  \code{Update()} method on the writer.
//
//  Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet
  rescaler->SetOutputMinimum(  0  );
  rescaler->SetOutputMaximum( 255 );

  rescaler->SetInput( adaptor );
  writer->SetInput( rescaler->GetOutput() );
  writer->Update();
// Software Guide : EndCodeSnippet


//  Software Guide : BeginLatex
//
//  ImageAdaptors for the green and blue channels can easily be implemented by
//  modifying the pixel accessor of the red channel and then using the
//  new pixel accessor for instantiating the type of an image adaptor. 
//  The following define a green channel pixel accessor.
//
//  \index{PixelAccessor!RGB green channel}
//  \index{itk::ImageAdaptor!RGB green channel}
//  \index{ImageAdaptor!RGB green channel}
//
//  Software Guide : EndLatex 


// Software Guide : BeginCodeSnippet
  class GreenChannelPixelAccessor  
  {
  public:
    typedef itk::RGBPixel<float>   InternalType;
    typedef               float    ExternalType;

    static ExternalType Get( const InternalType & input ) 
      {
        return static_cast<ExternalType>( input.GetGreen() );
      }
  };
// Software Guide : EndCodeSnippet


//  Software Guide : BeginLatex
//
// A blue channel pixel accessor is similarly defined.
//
//  \index{PixelAccessor!RGB blue channel}
//  \index{itk::ImageAdaptor!RGB blue channel}
//  \index{ImageAdaptor!RGB blue channel}
//
//  Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet
  class BlueChannelPixelAccessor  
  {
  public:
    typedef itk::RGBPixel<float>   InternalType;
    typedef               float    ExternalType;

    static ExternalType Get( const InternalType & input ) 
      {
        return static_cast<ExternalType>( input.GetBlue() );
      }
  };
// Software Guide : EndCodeSnippet


//  Software Guide : BeginLatex
//
// \begin{figure} \center
// \includegraphics[width=0.24\textwidth]{VisibleWomanEyeSlice.eps}
// \includegraphics[width=0.24\textwidth]{VisibleWomanEyeSliceRedComponent.eps}
// \includegraphics[width=0.24\textwidth]{VisibleWomanEyeSliceGreenComponent.eps}
// \includegraphics[width=0.24\textwidth]{VisibleWomanEyeSliceBlueComponent.eps}
// \itkcaption[Image Adaptor to RGB Image]{Using
// ImageAdaptor to extract the components of an RGB image. The
// image on the left is a subregion of the Visible Woman cryogenic data set. 
// The red, green and blue components are shown from left to right as scalar 
// images extracted with an ImageAdaptor.}
// \label{fig:ImageAdaptorToRGBImage}
// \end{figure}
//
//
//  Figure~\ref{fig:ImageAdaptorToRGBImage} shows the result
//  of extracting the red, green and blue components from a region of the
//  Visible Woman cryogenic data set. 
//
//  Software Guide : EndLatex 


  return 0;
}



