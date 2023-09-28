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
#include "write_off.h"
#include "read_off.h"
#include "tmesh_tiling.h"

namespace OOCTriTileLib {

namespace TilingAlgorithms {

void create_triangle_mesh_tiling(const std::vector<std::string> input_filenames,
                                 const std::string              out_directory,
                                 const int                      max_vtx_per_tile,
                                 std::vector<std::string>     & tile_filenames)
{
    int bufferzone_size = 0;

    std::vector<std::vector<std::string>> bufferzone_filenames;

    create_triangle_mesh_tiling(input_filenames, out_directory, max_vtx_per_tile, bufferzone_size, tile_filenames, bufferzone_filenames);
}

void create_triangle_mesh_tiling (const std::vector<std::string> input_filenames,
                                  const std::string              out_directory,
                                  const int                      max_vtx_per_tile,
                                  const int                      bufferzone_size,
                                  std::vector<std::string>     & tile_filenames,
                                  std::vector<std::vector<std::string>>     & bufferzone_filenames)
{
#ifndef STXXL
    std::cerr << "[ERROR] STXXL library is necessary to run triangle mesh tiling algorithm." << std::endl;
    exit(1);
#else

    Vtx bb_min;
    Vtx bb_max;

    std::string downsample_filename = out_directory + "/V_downsample";
    std::string binary_filename     = out_directory + "/V_binary";

    stxxl::uint64 n_vertices = 0, n_triangles = 0;
    int n_sample_vertices = 0 ;

    int percentage =  1000;
    stxxl::uint64 stop = max_vtx_per_tile / percentage;

    // Init by reading the input mesh and computing both bounding box and a vertex downsample
    get_bounding_box_and_downsample_and_binary_OFF(input_filenames, downsample_filename, binary_filename, percentage,
                                                   n_vertices, n_triangles, n_sample_vertices,
                                                   bb_min, bb_max);

    // Create BSP root by exploiting the vertex downsample
    BspCell root (bb_min, bb_max);
    root.is_bsp_root = true;
    root.n_inner_vertices = n_sample_vertices;
    root.filename_inner_v = downsample_filename;

    // Create BSP starting from the root and exploiting the vertex downsample
    BinarySpacePartition bsp (root);
    bsp.create(stop, out_directory);

    // Fill the BSP cells by reading the original input (both vertices and triangles)
    bsp.fill(binary_filename, input_filenames.size());

    std::cout << "[OUTPUT] Writing output tiles in " << out_directory << " ... " << std::endl;

    // Write the output according to selected output format
    write_bsp_OFF(bsp, out_directory);

    std::cout << "[OUTPUT] Writing output tiles in " << out_directory << " ... COMPLETED. " << std::endl;

    for (int leaf = 0; leaf < bsp.get_n_leaves(); leaf++)
    {
        tile_filenames.push_back(bsp.get_leaf(leaf)->filename_mesh);
    }

    if (bufferzone_size > 0)
    {
        std::cout << std::endl << "[BUFFERZONES] Creating Bufferzones of size " << bufferzone_size << std::endl;

        bufferzone_filenames.resize(tile_filenames.size());

        for (int leaf = 0; leaf < bsp.get_n_leaves(); leaf++)
        {
            bsp.create_bufferzone(leaf, bufferzone_size, out_directory);

            for (std::string filename : bsp.get_leaf(leaf)->bufferzone_filenames)
                bufferzone_filenames.at(leaf).push_back(filename);
        }
    }

    std::string bsp_filename = out_directory + "/bsp.json";
    std::ofstream bsp_file (bsp_filename);

    std::cout << "[OUTPUT] Writing BSP in " << bsp_filename << " ... " << std::endl;

    cereal::JSONOutputArchive ar( bsp_file );
    bsp.serialize(ar);

    std::cout << "[OUTPUT] Writing BSP in " << bsp_filename << " ... COMPLETED." << std::endl;

#endif
}

}

}
