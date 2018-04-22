//
//  OBJLoader.h
//  OBJRenderer
//
//  Created by Neil on 26/9/15.
//  Copyright (c) 2015 Neil. All rights reserved.
//

#ifndef __OBJRenderer__OBJLoader__
#define __OBJRenderer__OBJLoader__

#include <vector>

bool LoadOBJ(const char* path, std::vector<float> &out_vertices, std::vector<float> &out_uvs, std::vector<float> &out_normals, std::vector<unsigned int> &indices
             );

#endif /* defined(__OBJRenderer__OBJLoader__) */
