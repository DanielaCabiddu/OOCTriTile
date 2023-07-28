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

#ifndef BSP_H
#define BSP_H

#include "bsp_cell.h"

#include <set>
#include <vector>

#define UNKNOWN_BOUNDARY_INFO -2
#define CONSTRAINED_BOUNDARY_VERTEX -1

struct ConstrainedVertex
{
    stxxl::uint64 vid;

    std::set<int> cells;

};

class BinarySpacePartition
{
public:

    BspCell root;                   // Sampled root node of the bsp. Generated be sampling the input mesh.

    int counter = 0;                // Used to se unique leaf ids

    std::vector<BspCell *> leaves;    // Bsp leaves. Each leaf refers to its files.

    stxxl::vector<int> vtx2cell;
    stxxl::vector<int> vtx2boundary;

    std::vector<Point> input_coords;

    std::map<stxxl::uint64, ConstrainedVertex> constrained_vertices;

    ///////////////////////////
    /// METHODS
    ///////////////////////////

    BinarySpacePartition () {}

    BinarySpacePartition (const BspCell &cell) { root = cell; leaves.push_back(&root);}

    void create (const int max_vtx_per_cell, const std::string out_directory);
    void fill   (const std::string input_binary_filename);

    void split_cell (BspCell &cell, const std::string out_directory);

    void create_bufferzone (const int leaf_pos, const int bufferzone_size, const std::string out_directory);

    const int get_largest_leaf_by_inner_vertices () const;

    const BspCell & get_minimum_cell (const BspCell &a, const BspCell &b) const; // by lexicographic order of ther barycenters

    void classify_triangle (const stxxl::uint64 v1, const stxxl::uint64 v2, const stxxl::uint64 v3,
                            std::pair<int, int> & selected_cell_vtx, std::pair<int, int> & neighbor_cell_vtx_1, std::pair<int, int> & neighbor_cell_vtx_2);
};

#ifndef IQLIB_STATIC
#include "bsp.cpp"
#endif

#endif // BSP_H
