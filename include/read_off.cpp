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
#include "read_off.h"

#include <inttypes.h>
#include <iostream>
#include <limits.h>

void get_bounding_box_and_downsample_and_binary_OFF(const std::vector<std::string> & mesh_filenames,
                                                    const std::string downsample_filename,
                                                    const std::string binary_filename,
                                                    const int percentage,
                                                    stxxl::uint64 &mesh_n_vertices,
                                                    stxxl::uint64 &mesh_n_triangles,
                                                    int &mesh_sample_vertices,
                                                    Vtx & bb_min,
                                                    Vtx & bb_max)
{
    bb_min.x = bb_min.y = bb_min.z = FLT_MAX;
    bb_max.x = bb_max.y = bb_max.z = -FLT_MAX;

    char s[256], *line;

    std::cout << "[OPENING] Sample file " << downsample_filename << std::endl;

    FILE *sample_fp = fopen (downsample_filename.c_str(), "wb");

    if (sample_fp == NULL)
    {
        std::cerr << "[ERROR] Opening file " << downsample_filename << std::endl;
        exit(1);
    }

    std::cout << "[OPENING] Binary file " << binary_filename << std::endl;

    std::ofstream binary_mesh (binary_filename.c_str(), std::ios::out | std::ios::binary);

    if (!binary_mesh.is_open())
    {
        std::cerr << "[ERROR] Opening file " << binary_filename << std::endl;
        exit(1);
    }


    stxxl::uint64 managed_v = 0;
    stxxl::uint64 managed_t = 0;

    for (int file = 0; file < mesh_filenames.size(); file++)
    {

        std::string mesh_filename = mesh_filenames.at(file);

        std::cout << "[OPENING] Mesh file " << mesh_filename << std::endl;

        FILE *fp = fopen(mesh_filename.c_str(), "r");

        if (fp == NULL)
        {
            std::cerr << "[ERROR] Opening file " << mesh_filename << std::endl;
            exit(1);
        }

        fscanf(fp,"%255s",s);

        if (std::strcmp(s,"OFF") || feof(fp))
        {
            std::cerr << "[ERROR] Input Format Error -- End of File" << std::endl;
            exit(1);
        }

        do
        {
            line = read_line_from_file(fp);
        }
        while (line[0] == '#' || line[0] == '\0' || !sscanf(line,"%256s",s));

        int n = 0;
        stxxl::uint64 n_v, n_t;

        if (sscanf(line,"%" SCNu64 " %" SCNu64 " %d", &n_v, &n_t, &n) < 3)
        {
            std::cerr << "[ERROR] Input Format Error" << std::endl;
            exit(1);
        }

        std::cout << "[INFO] " << n_v << " vertices and " << n_t << " triangles." << std::endl;

        binary_mesh.write(reinterpret_cast<const char*>(&n_v), sizeof n_v);
        binary_mesh.write(reinterpret_cast<const char*>(&n_t), sizeof n_t);

        mesh_n_vertices = n_v;
        mesh_n_triangles = n_t;
        mesh_sample_vertices = 0;

        stxxl::uint64 perc = (stxxl::uint64)(n_v / 10);

        stxxl::uint64 start = percentage /2;
        stxxl::uint64 sample_ptr = start;

        int delta = -start + (rand() % percentage);

        double coord_buffer[3];

        for (stxxl::uint64 i = 0; i < n_v; i++)
        {
            if ((i%perc) == 0)
                std::cout << " --- --- Reading Vertices .. " << i << " \\ " << n_v << " ( " << (i / perc) * 10 << "% )" << std::endl;

            if (i == n_v - 1)
                std::cout << " --- --- Reading Vertices .. " << i + 1 << " \\ " << n_v << " -- COMPLETED" << std::endl;


            if (fscanf(fp, "%lf %lf %lf", &coord_buffer[0], &coord_buffer[1], &coord_buffer[2]) != 3) {

                std::cerr << "[ERROR] Error reading vertex " << i << std::endl;
                exit(1);
            }

            binary_mesh.write(reinterpret_cast<const char*>(&coord_buffer[0]), sizeof coord_buffer[0]);
            binary_mesh.write(reinterpret_cast<const char*>(&coord_buffer[1]), sizeof coord_buffer[1]);
            binary_mesh.write(reinterpret_cast<const char*>(&coord_buffer[2]), sizeof coord_buffer[2]);

            // x
            if (coord_buffer[0] < bb_min.x)
                bb_min.x = coord_buffer[0];

            if (coord_buffer[0] > bb_max.x)
                bb_max.x = coord_buffer[0];

            // y
            if (coord_buffer[1] < bb_min.y)
                bb_min.y = coord_buffer[1];

            if (coord_buffer[1] > bb_max.y)
                bb_max.y = coord_buffer[1];

            // z
            if (coord_buffer[2] < bb_min.z)
                bb_min.z = coord_buffer[2];

            if (coord_buffer[2] > bb_max.z)
                bb_max.z = coord_buffer[2];

            if (i == sample_ptr + delta)
            {
                fwrite((void *) coord_buffer, sizeof(double), 3, sample_fp);

                sample_ptr+= percentage;
                delta = -start + rand() % percentage;

                mesh_sample_vertices++;
            }
        }

        stxxl::uint64 v1 = UINT64_MAX, v2 = UINT64_MAX, v3 = UINT64_MAX;
        stxxl::uint64 perc_t = (stxxl::uint64)(n_t / 10);

        for (stxxl::uint64 i = 0; i < n_t; i++)
        {
            if ((i%perc_t) == 0)
                std::cout << " --- --- Reading Triangles .. " << i << " \\ " << n_t << " ( " << (i / perc_t) * 10 << "% )" << std::endl;

            if (i == n_t - 1)
                std::cout << " --- --- Reading Triangle .. " << i + 1 << " \\ " << n_t << " -- COMPLETED" << std::endl;


            if (fscanf(fp, "%d %" SCNu64 " %" SCNu64 " %" SCNu64, &n, &v1, &v2, &v3) != 4) {

                std::cerr << "[ERROR] Error reading vertex " << i << std::endl;
                exit(1);
            }

            assert (v1 != UINT64_MAX); assert (v2 != UINT64_MAX); assert (v3 != UINT64_MAX);

            v1 += managed_v;
            v2 += managed_v;
            v3 += managed_v;

            binary_mesh.write(reinterpret_cast<const char*>(&v1), sizeof v1);
            binary_mesh.write(reinterpret_cast<const char*>(&v2), sizeof v2);
            binary_mesh.write(reinterpret_cast<const char*>(&v3), sizeof v3);

        }

        managed_v += n_v;
        managed_t += n_t;

        fclose(fp);

    }

    fclose(sample_fp);

    binary_mesh.close();
}

// Read one line (max 1024 chars) and exit if EOF
char * read_line_from_file (FILE *in, const bool exit_on_eof)
{
#define MAX_READLINE_CHARS	1024

    static char line[MAX_READLINE_CHARS];
    int i=0;
    char c;

    while ((c = fgetc(in)) != '\n' && i<(MAX_READLINE_CHARS-1))
        if (c==EOF) {
            if (exit_on_eof)
                return ("\nUnexpected end of file!\n");
            else return NULL;
        } else if (c != '\r')
            line[i++] = c;

    line[i] = '\0';

    if (i==MAX_READLINE_CHARS-1)
        return ("readLineFromFile: Line is too int. Truncated !\n");

    return line;
}
