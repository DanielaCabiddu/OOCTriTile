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

#ifndef IQLIB_STATIC
#include "read_off.cpp"
#endif

#endif // READ_OFF_H
