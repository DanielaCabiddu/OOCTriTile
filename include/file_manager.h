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

#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "bsp.h"

#include "stxxl.h"

#include <fstream>
#include <vector>

class FileManager
{
public:

    BinarySpacePartition *bsp;

    std::vector<stxxl::uint64> vOuts_usage;
    std::vector<stxxl::uint64> tOuts_usage;
    std::vector<stxxl::uint64> bvOuts_usage;

    std::vector<std::ofstream *> vOuts;
    std::vector<std::ofstream *> tOuts;
    std::vector<std::ofstream *> bvOuts;

    stxxl::uint64 usage_indicator = 0;

    int n_open_files = 0;

    const int max_open_file = 200;

public:

    FileManager () {}

    FileManager (BinarySpacePartition *bsp)
    {
        this->bsp = bsp;

        for (int leaf = 0; leaf < bsp->leaves.size(); leaf++)
        {
            std::ofstream *os_v = new std::ofstream(bsp->leaves.at(leaf)->filename_inner_v.c_str(), std::ofstream::out | std::ofstream::binary);

            if (os_v->is_open())
            {
                vOuts.push_back(os_v);
                os_v->close();

                vOuts_usage.push_back(0);
            }

            std::ofstream * os_t = new std::ofstream (bsp->leaves.at(leaf)->filename_inner_t.c_str(), std::ofstream::out | std::ofstream::binary);

            if (os_t->is_open())
            {
                tOuts.push_back(os_t);
                os_t->close();

                tOuts_usage.push_back(0);
            }

            std::ofstream * os_bv = new std::ofstream (bsp->leaves.at(leaf)->filename_boundary_v.c_str(), std::ofstream::out | std::ofstream::binary);

            if (os_bv->is_open())
            {
                bvOuts.push_back(os_bv);
                os_bv->close();

                bvOuts_usage.push_back(0);
            }

        }

    }

    void close_oldest_file ();

    void close_all ();

    void guarantee_vOut_open  (const int position);
    void guarantee_tOut_open  (const int position);
    void guarantee_bvOut_open (const int position);

    void write_vertex           (const int position, const stxxl::uint64 vid, const double x, const double y, const double z);
    void write_boundary_vertex  (const int position, const stxxl::uint64 vid);
    void write_boundary_vertex  (const int position, const stxxl::uint64 vid, const double x, const double y, const double z);
    void write_triangle         (const int position, const stxxl::uint64 v1, const stxxl::uint64 v2, const stxxl::uint64 v3);


};

#ifndef IQLIB_STATIC
#include "file_manager.cpp"
#endif

#endif // FILE_MANAGER_H
