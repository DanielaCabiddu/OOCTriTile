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

#ifndef GEOMETRY_ITEMS_H
#define GEOMETRY_ITEMS_H

#include <stdio.h>

#include <stxxl.h>

class Point {

public:
    double x;
    double y;
    double z;

    Point () {}

};

class Vtx : Point {

public:

    int cell; // position of the grid cell (in the graph) where the vertex is located

    stxxl::uint64 index;

    double x;
    double y;
    double z;

    Vtx () {}

    Vtx(double xx, double yy, double zz)
    {
        x = xx; y = yy; z = zz;
        cell = - 1;
        index = UINT64_MAX;
    }

};

class Plane {
public:

    Vtx min;
    Vtx max;

    int axis; // 0 = x; 1 = y; 2 = z

    Plane () {}

};

typedef struct TriangleStruct {

    int index;
    int v1;
    int v2;
    int v3;

} TriangleStruct;


#endif // GEOMETRY_ITEMS_H
