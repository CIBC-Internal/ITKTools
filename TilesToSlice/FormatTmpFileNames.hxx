#ifndef FORMAT_TMP_FILE_NAMES_H
#define FORMAT_TMP_FILE_NAMES_H

#include <boost/filesystem.hpp>

#include <iomanip>
#include <sstream>

namespace bfs = boost::filesystem;

bfs::path
formatTmpFileNames(const bfs::path& tmpPath, const unsigned int index)
{
  std::ostringstream tmpOutputFormat;
  tmpOutputFormat << "row_" << std::setfill('0') << std::setw(3) << index << ".png";
  return bfs::path( tmpPath / tmpOutputFormat.str().c_str() );
}

#endif
