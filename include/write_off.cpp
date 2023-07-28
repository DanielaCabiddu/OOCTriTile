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

#include "write_off.h"

void write_bsp_OFF(const BinarySpacePartition &bsp, const std::string out_directory)
{
    for (int leaf=0; leaf < bsp.leaves.size(); leaf++)
    {
        BspCell *cell = bsp.leaves.at(leaf);

        if (cell->n_inner_vertices == 0)   
        {
            remove (cell->filename_inner_v.c_str());
            remove (cell->filename_boundary_v.c_str());
            remove (cell->filename_inner_t.c_str());

            continue;   
        }

        // read boundary vertices
        std::set<stxxl::uint64> added_vertices;

        std::ifstream cell_stream (cell->filename_boundary_v.c_str(), std::fstream::in | std::fstream::binary);

        if (!cell_stream.is_open())
        {
            std::cout << "[WARNING] No additional vertices." << std::endl;
        }
        else
        {
            stxxl::uint64 vertex;

            while (cell_stream.read (reinterpret_cast<char *>(&vertex),sizeof(vertex)) && !cell_stream.fail())
            {
                added_vertices.insert(vertex);
            }

            cell_stream.close();
        }

        cell_stream.open(cell->filename_inner_v.c_str());

        if (!cell_stream.is_open())
        {
            std::cout << "[ERROR] Opening file " << cell->filename_inner_v << std::endl;
            exit(1);
        }

        std::string out_filename = out_directory + "cell_" + std::to_string(leaf) + ".off";
        std::string local2global_filename = out_directory + "cell_" + std::to_string(leaf) + "_v_loc2glob";

        cell->filename_mesh = out_filename;
        cell->filename_local2global = local2global_filename;

        std::cout << "[OUTPUT] Writing " << out_filename << std::endl;

        std::map <stxxl::uint64, int> global_local_vertices;

        std::ofstream mesh_out_stream (out_filename.c_str(), std::fstream::out);
        std::ofstream local2global_out_stream (local2global_filename.c_str(), std::fstream::out);

        if (!mesh_out_stream.is_open() || !local2global_out_stream.is_open())
        {
            std::cout << "[ERROR] Opening file " << out_filename <<  " or " << local2global_filename << std::endl;
            exit(1);
        }

        mesh_out_stream << "OFF" << std::endl;
        mesh_out_stream << cell->n_inner_vertices + added_vertices.size() << " " << cell->n_inner_triangles << " 0" << std::endl;

        stxxl::uint64 id;
        double x,y,z;

        int vid = 0;

        for (; vid < cell->n_inner_vertices; vid++)
        {
            cell_stream.read (reinterpret_cast<char *>(&id),sizeof(id));

            cell_stream.read (reinterpret_cast<char *>(&x),sizeof(x));
            cell_stream.read (reinterpret_cast<char *>(&y),sizeof(y));
            cell_stream.read (reinterpret_cast<char *>(&z),sizeof(z));

            mesh_out_stream << x << " " << y << " " << z << std::endl;

            global_local_vertices[id] = vid;

            local2global_out_stream << id << std::endl;
        }

        cell_stream.close();

        for (int v : added_vertices)
        {
            mesh_out_stream << bsp.input_coords.at(v).x << " " << bsp.input_coords.at(v).y << " " << bsp.input_coords.at(v).z << std::endl;

            global_local_vertices[v] = vid++;

            local2global_out_stream << v << std::endl;
        }

        cell_stream.open(cell->filename_inner_t.c_str(), std::fstream::in | std::fstream::binary);

        if (!cell_stream.is_open())
        {
            std::cout << "[ERROR] Opening file " << cell->filename_inner_t << std::endl;
            exit(1);
        }

        stxxl::uint64 v1, v2, v3;

        for (int tid=0; tid < cell->n_inner_triangles; tid++)
        {
            cell_stream.read (reinterpret_cast<char *>(&v1),sizeof(v1));
            cell_stream.read (reinterpret_cast<char *>(&v2),sizeof(v2));
            cell_stream.read (reinterpret_cast<char *>(&v3),sizeof(v3));

            int local_v1 = global_local_vertices.at(v1);
            int local_v2 = global_local_vertices.at(v2);
            int local_v3 = global_local_vertices.at(v3);

            mesh_out_stream << "3 " << local_v1 << " " << local_v2 << " " << local_v3 << std::endl;
        }

        cell_stream.close();

        mesh_out_stream.close();
        local2global_out_stream.close();

        remove (cell->filename_inner_v.c_str());
        remove (cell->filename_boundary_v.c_str());
        remove (cell->filename_inner_t.c_str());
    }
}