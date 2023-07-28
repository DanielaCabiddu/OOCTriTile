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
#ifndef TRIMESH_H
#define TRIMESH_H

#include "stxxl.h"

#include <vector>

class Vertex
{
public:

    double x;
    double y;
    double z;

    stxxl::uint64 global_id;

    Vertex() {}

    Vertex (const double x, const double y, const double z, const stxxl::uint64 id)
    {
        this->x = x;
        this->y = y;
        this->z = z;

        this->global_id = id;
    }
};

class Edge
{
public:

    std::pair<int, int> endpoints;

    Edge() {}

    Edge (const int v0, const int v1)
    {
        endpoints.first = v0;
        endpoints.second = v1;
    }
};

class Triangle
{
public:

//    int vertex_0;
//    int vertex_1;
//    int vertex_2;

    std::vector<int> vertices;

    Triangle() {}

    Triangle (const int vertex_0, const int vertex_1, const int vertex_2)
    {
//        this->vertex_0 = vertex_0;
//        this->vertex_1 = vertex_1;
//        this->vertex_2 = vertex_2;

        vertices.push_back(vertex_0);
        vertices.push_back(vertex_1);
        vertices.push_back(vertex_2);
    }
};

class SubTrimesh
{
public:

    std::vector<Vertex>   vertices;
    std::vector<Triangle> triangles;
    std::vector<Edge>     edges;

    std::vector< std::vector<int> > vtx2vtx;
    std::vector< std::vector<int> > vtx2edg;
    std::vector< std::vector<int> > vtx2tri;
    std::vector< std::vector<int> > edg2tri;
    std::vector< std::vector<int> > tri2edg;
    std::vector< std::vector<int> > tri2tri;

    std::map<stxxl::uint64, int> vtx_global2local;

    SubTrimesh (const std::string filename, const std::string index_filename)
    {
        load (filename, index_filename);
        update_adjacency();
    }

    ~SubTrimesh ()
    {
        vertices.clear();
        triangles.clear();
        edges.clear();

        vtx2vtx.clear();
        vtx2edg.clear();
        vtx2tri.clear();
        edg2tri.clear();
        tri2edg.clear();
        tri2tri.clear();

        vtx_global2local.clear();
    }

    void load (const std::string filename, const std::string index_filename);

    void load_OFF (const std::string filename, const std::string index_filename);

    void update_adjacency();

    void local_vid_n_rings  (const int local_vid, const int n, std::set<int> &vids, std::set<int> &tids) const;
    void global_vid_n_rings (const int global_vid, const int n, std::set<int> &vids, std::set<int> &tids) const ;

    bool is_boundary_vertex (const int vid) const;

    const std::vector<int> boundary_vertices () const ;
};

#ifndef OOCTRITILELIB_STATIC
#include "trimesh.cpp"
#endif

#endif // TRIMESH_H
