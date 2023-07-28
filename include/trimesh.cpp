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
*********************************************************************************/#include "trimesh.h"

#include <fstream>
#include <iostream>

typedef std::pair<int,int> ipair;

ipair unique_pair(int v0, int v1)
{
    ipair e;
    e.first  = std::min(v0,v1);
    e.second = std::max(v0,v1);
    return e;
}

void SubTrimesh::load (const std::string filename, const std::string index_filename)
{
    load_OFF(filename, index_filename);
}

void SubTrimesh::load_OFF (const std::string filename, const std::string index_filename)
{
    std::ifstream mesh (filename.c_str(), std::fstream::in);

    if (!mesh.is_open())
    {
        std::cerr << "[ERROR] Opening mesh file " << filename << std::endl;
        exit(1);
    }

    std::ifstream index (index_filename.c_str(), std::fstream::in);

    if (!index.is_open())
    {
        std::cerr << "[ERROR] Opening index file " << index_filename << std::endl;
        exit(1);
    }

    std::string format;
    int n_vertices, n_triangles, n_edges;

    double x, y, z;
    stxxl::uint64 global_index;

    int v0, v1, v2, n;

    mesh >> format >> n_vertices >> n_triangles >> n_edges;

    for (int vid = 0; vid < n_vertices; vid ++)
    {
        mesh >> x >> y >> z;
        index >> global_index;

        Vertex vertex (x, y, z, global_index);
        vertices.push_back(vertex);

        vtx_global2local[global_index] = vertices.size()-1;
    }

    for (int tid=0; tid < n_triangles; tid++)
    {
        mesh >> n >> v0 >> v1 >> v2;

        Triangle triangle(v0, v1, v2);

        triangles.push_back(triangle);
    }

    mesh.close();
    index.close();
}

void SubTrimesh::update_adjacency()
{
    edges.clear();

    vtx2vtx.clear();
    vtx2edg.clear();
    edg2tri.clear();
    tri2tri.clear();
    tri2edg.clear();
    vtx2tri.clear();

    vtx2tri.resize(vertices.size());

    typedef std::map<ipair, std::vector<int> > mymap;
    mymap edge_tri_map;

    for(int tid=0; tid<triangles.size(); ++tid)
    {
        Triangle t = triangles.at(tid);

        for(int i=0; i<3; ++i)
        {
            int  vid0 = t.vertices.at(i);
            int  vid1 = t.vertices.at((i+1)%3);

            vtx2tri[vid0].push_back(tid);

            ipair e = unique_pair(vid0, vid1);
            edge_tri_map[e].push_back(tid);
        }
    }

    edg2tri.resize(edge_tri_map.size());
    tri2edg.resize(triangles.size());
    tri2tri.resize(triangles.size());
    vtx2vtx.resize(vertices.size());
    vtx2edg.resize(vertices.size());

    int fresh_id = 0;

    for(mymap::iterator it=edge_tri_map.begin(); it!=edge_tri_map.end(); ++it)
    {
        ipair e    = it->first;

        int  eid  = fresh_id++;
        int  vid0 = e.first;
        int  vid1 = e.second;

        edges.push_back(Edge (vid0, vid1));

        vtx2vtx[vid0].push_back(vid1);
        vtx2vtx[vid1].push_back(vid0);

        vtx2edg[vid0].push_back(eid);
        vtx2edg[vid1].push_back(eid);

        std::vector<int> tids = it->second;
        //assert(tids.size() <= 2 && "Non manifold edge!");
        //assert(tids.size() >= 1 && "Non manifold edge!");
        //if (tids.size() > 2 || tids.size() < 1)
        //    std::cerr << " --- [LOADING][WARNING] Non manifold edge [" << vid0 << " , " << vid1 << " ] ! " << std::endl;

        for(int tid : tids)
        {
            tri2edg[tid].push_back(eid);
            edg2tri[eid].push_back(tid);
        }

        if (tids.size() >= 2)
        {
            for (size_t i=0; i<tids.size()-1; i++)
            {
                tri2tri[tids[i]].push_back(tids[i+1]);
                tri2tri[tids[i+1]].push_back(tids[i]);
            }

            if (tids.size() > 2)
            {
                tri2tri[tids[0]].push_back(tids[tids.size()-1]);
                tri2tri[tids[tids.size()-1]].push_back(tids[0]);
            }
        }
    }
}

void SubTrimesh::local_vid_n_rings (const int local_vid, const int n, std::set<int> &vids, std::set<int> &tids) const
{
    if (n == 0)
        return;

    for (int t : vtx2tri.at(local_vid))
        tids.insert(t);

    for (int v : vtx2vtx.at(local_vid))
    {
        vids.insert(v);

        local_vid_n_rings(v, n-1, vids, tids);
    }
}

void SubTrimesh::global_vid_n_rings (const int global_vid, const int n, std::set<int> &vids, std::set<int> &tids) const
{
    if (n == 0)
        return;

    auto it = vtx_global2local.find(global_vid);

    if (it == vtx_global2local.end())
        return;

    int local_vid = (*it).second;

    local_vid_n_rings(local_vid, n, vids, tids);
}

bool SubTrimesh::is_boundary_vertex (const int vid) const
{
    for (int e : vtx2edg.at(vid))
    {
        if (edg2tri.at(e).size() < 2)
            return true;
    }

    return false;
}

const std::vector<int> SubTrimesh::boundary_vertices () const
{
    std::vector<int> bv;

    for (int vid=0; vid < vertices.size(); vid++)
        if (is_boundary_vertex(vid))
            bv.push_back(vid);

    return bv;
}

