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
#include <iostream>
#include <limits.h>       //For PATH_MAX

#include "dirent.h"

#include "tmesh_tiling.h"
#include "tclap/CmdLine.h"

using namespace std;

int main(int argc, char **argv)
{
    // Define the command line object.
    TCLAP::CmdLine cmd("Usage: trimesh_tiling [--file <filename> | --dir <directory] --out <directory> --verts <n_verts>", ' ', "0.9");

    TCLAP::ValueArg<std::string> dirArg("d","dir","input directory",false,"","string");
    cmd.add( dirArg );

    TCLAP::ValueArg<std::string> fileArg("f","file","filename",false,"","string");
    cmd.add( fileArg );

    TCLAP::ValueArg<std::string> outArg("o","out","output directory",true,"","string");
    cmd.add( outArg );

    TCLAP::ValueArg<std::string> maxvArg("v","verts","max number of vertex for tile",true,"","int");
    cmd.add( maxvArg );

    // Parse the args.
    cmd.parse( argc, argv );

    if (!fileArg.isSet() && !dirArg.isSet())
    {
        std::cerr << "At least one inbetween input file and input directory MUST be provided" << std::endl;
        return 1;
    }

    std::vector<std::string> filenames;

    if (fileArg.isSet())
        filenames.push_back(fileArg.getValue());

    if (dirArg.isSet())
    {
        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir (dirArg.getValue().c_str())) != NULL)
        {
            /* print all the files and directories within directory */
            while ((ent = readdir (dir)) != NULL)
            {
                std::string path = dirArg.getValue() + "/" + ent->d_name;

                int ext_pos = path.find_last_of(".");
                std::string ext = (ext_pos >= 0) ? path.substr(ext_pos) : "";

                if (ext.compare(".off") == 0)
                    filenames.push_back(path);
//                printf ("%s\n", path.c_str());
            }
            closedir (dir);
        } else {
            /* could not open directory */
            perror ("");
            return EXIT_FAILURE;
        }
    }

    const std::string output_directory = outArg.getValue();
    const int max_verts = std::atoi(maxvArg.getValue().c_str());

    std::vector<std::string> out_filenames;

    OOCTriTileLib::TilingAlgorithms::create_triangle_mesh_tiling(filenames, output_directory, max_verts, out_filenames);
    return 0;
}
