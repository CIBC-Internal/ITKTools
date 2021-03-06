ITKTools
========

Useful (for me, at least) ITK-based tools.
Built against ITK 4.

- TilesToSlice: ITK filters to stitch 475x475 image tiles into a single image. Tiling is done in two steps to prevent overflowing the stack with the tiling filter. OpenMP is used to parallelize tiling operations, and [ITK IO streaming](http://www.kitware.com/media/html/IOStreamingInITK.html) is used to improve performance of the write filter.

- RawToPNG: ITK filters to convert 3D raw files to 2D png. Makes use of [ITK IO streaming](http://www.kitware.com/media/html/IOStreamingInITK.html).

- NrrdToStack: ITK filters to read a Nrrd and write an image stack (png, tiff etc.).
- StackToNrrd: ITK filters to read an image stack and write a Nrrd.

- ConvertNrrd: Convert Nrrd to another image format.

- VTKToNrrd: Convert VTK to Nrrd (and vice-versa).

- RGBToGray2D: Convert RGB pixel image to grayscale.
