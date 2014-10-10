ITKTools
========

Useful (for me, at least) ITK-based tools.
Built on Linux with ITK 4.6.

- TilesToSlice: ITK filters to stitch 475x475 image tiles into a single image. Tiling is done in two steps to prevent overflowing the stack with the tiling filter. OpenMP is used to parallelize tiling operations, and [ITK IO streaming](http://www.kitware.com/media/html/IOStreamingInITK.html) is used to improve performance of the write filter.
