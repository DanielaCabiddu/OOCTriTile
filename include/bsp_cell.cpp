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

#include "bsp_cell.h"

BspCell::BspCell (const Vtx &v1, const Vtx &v2)
{
    this->v1 = v1;
    this->v2 = v2;
}

const Plane BspCell::getSubdivisionPlane() const {

    Plane plane;

    // get bounding box edge length
    double x_dim = v2.x - v1.x;
    double y_dim = v2.y - v1.y;
    double z_dim = v2.z - v1.z;

    // get the middle point of the largest dimension
    if (std::max(x_dim, std::max(y_dim, z_dim)) == x_dim ) {

        plane.min = Vtx(v1.x + x_dim/2, v1.y, v1.z);
        plane.max = Vtx(v2.x - x_dim/2, v2.y, v2.z);
        plane.axis = 0; // x

    } else if (std::max(x_dim, std::max(y_dim, z_dim)) == y_dim) {

        plane.min = Vtx(v1.x, v1.y + y_dim/2, v1.z);
        plane.max = Vtx(v2.x, v2.y - y_dim/2, v2.z);
        plane.axis = 1; // y

    } else {

        plane.min = Vtx(v1.x, v1.y, v1.z + z_dim/2);
        plane.max = Vtx(v2.x, v2.y, v2.z - z_dim/2);
        plane.axis = 2; // z

    }

    return plane;
}

const bool BspCell::hasPoint (const double x, const double y, const double z) const {

    double eps = 0.00000001;

    if (	(x > v1.x || abs(x - v1.x) < eps)
        &&	(x < v2.x || abs(x - v2.x) < eps)
        &&	(y > v1.y || abs(y - v1.y) < eps)
        &&	(y < v2.y || abs(y - v2.y) < eps)
        &&	(z > v1.z || abs(z - v1.z) < eps)
        &&	(z < v2.z || abs(z - v2.z) < eps) )

        return true;

    return false;
}

