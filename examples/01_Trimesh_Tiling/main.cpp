#include <iostream>

#include "tmesh_tiling.h"
#include "tclap/CmdLine.h"

#include <filesystem>
namespace fs = std::filesystem;

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
        for (const auto & entry : fs::directory_iterator(dirArg.getValue()))
            filenames.push_back(entry.path());
    }

    const std::string output_directory = outArg.getValue();
    const int max_verts = std::atoi(maxvArg.getValue().c_str());


    std::vector<std::string> out_filenames;

    IQlib::TilingAlgorithms::create_triangle_mesh_tiling(filenames, output_directory, max_verts, out_filenames);
    return 0;
}
