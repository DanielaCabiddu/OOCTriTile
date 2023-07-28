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
#ifndef READ_OFF_H
#define READ_OFF_H

#include "geometry_items.h"

void get_bounding_box_and_downsample_and_binary_OFF (const std::vector<std::string> &filename,
                                                     const std::string downsample_filename,
                                                     const std::string binary_filename, const int percentage,
                                                     stxxl::uint64 &mesh_n_vertices,
                                                     stxxl::uint64 &mesh_n_triangles,
                                                     int &mesh_sample_vertices,
                                                     Vtx & bb_min,
                                                     Vtx & bb_max);

char *read_line_from_file(FILE *in, const bool exit_on_eof = true);

#ifndef OOCTRITILELIB_STATIC
#include "read_off.cpp"
#endif

#endif // READ_OFF_H
