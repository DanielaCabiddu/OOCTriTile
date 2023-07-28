/********************************************************************************
*  This file is part of OOCTriTile                                              *
*  Copyright(C) 2023: Daniela Cabiddu                                           *
*                                                                               *
*  Author(s):                                                                   *
*                                                                               *
*     Daniela Cabiddu (daniela.cabiddu@cnr.it)                                  *
*                                                                               *
*     Italian National Research Council (CNR)                                   *
*     Institute for Applied Mathematics and Information Technologies (IMATI)    *
*     Via de Marini, 6                                                          *
*     16149 Genoa,                                                              *
*     Italy                                                                     *
*                                                                               *
*  This program is free software: you can redistribute it and/or modify it      *
*  under the terms of the GNU General Public License as published by the        *
*  Free Software Foundation, either version 3 of the License, or (at your       *
*  option) any later version.                                                   *
*                                                                               *
*  This program is distributed in the hope that it will be useful, but          *
*  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY   *
*  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for  *
*  more details.                                                                *
*                                                                               *
*  You should have received a copy of the GNU General Public License along      *
*  with this program. If not, see <https://www.gnu.org/licenses/>.              *
*                                                                               *
*********************************************************************************/
#ifndef TMESH_TILING_H
#define TMESH_TILING_H

#include <string>
#include <vector>

namespace OOCTriTileLib {

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

#ifndef OOCTriTileLib_STATIC
#include "tmesh_tiling.cpp"
#endif

#endif // TMESH_TILING_H
