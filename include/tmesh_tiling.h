/****************************************************************************
* Italian National Research Council                                         *
* Institute for Applied Mathematics and Information Technologies, Genoa     *
* IMATI-GE / CNR                                                            *
*                                                                           *
* Author: Daniela Cabiddu (daniela.cabiddu@ge.imati.cnr.it)                 *
*                                                                           *
* Copyright(C) 2017                                                         *
* All rights reserved.                                                      *
*                                                                           *
* This file is part of IQlib.                                               *
*                                                                           *
* IQlib is free software; you can redistribute it and/or modify             *
* it under the terms of the GNU General Public License as published by      *
* the Free Software Foundation; either version 3 of the License, or         *
* (at your option) any later version.                                       *
*                                                                           *
* This program is distributed in the hope that it will be useful,           *
* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
* GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
* for more details.                                                         *
****************************************************************************/

#ifndef TMESH_TILING_H
#define TMESH_TILING_H

#include <string>
#include <vector>

namespace IQlib {

namespace TilingAlgorithms {

void create_triangle_mesh_tiling (const std::vector<std::string> input_filenames,
                                  const std::string          out_directory,
                                  const int                  max_vtx_per_tile,
                                  std::vector<std::string> & tile_filenames);

void create_triangle_mesh_tiling (const std::vector<std::string> input_filenames,
                                  const std::string          out_directory,
                                  const int                  max_vtx_per_tile,
                                  const int                  bufferzone_size,
                                  std::vector<std::string> & tile_filenames,
                                  std::vector<std::vector<std::string> > &bufferzone_filenames);

}

}

#ifndef IQLIB_STATIC
#include "tmesh_tiling.cpp"
#endif

#endif // TMESH_TILING_H
