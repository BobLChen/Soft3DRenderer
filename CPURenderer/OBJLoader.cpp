//
//  OBJLoader.cpp
//  MyOpenGL
//
//  Created by Neil on 26/9/15.
//  Copyright (c) 2015 Neil. All rights reserved.
//

#include "OBJLoader.h"
#include <stdio.h>
#include <string>

struct V3 {
    float x;
    float y;
    float z;
};

struct V2 {
    float x;
    float y;
};

bool LoadOBJ(const char* path,
             std::vector<float> &out_vertices,
             std::vector<float> &out_uvs,
             std::vector<float> &out_normals,
             std::vector<unsigned int> &out_indices
             ) {
    
    printf("Loading OBJ file %s...\n", path);
    
    std::vector<unsigned int> vertIdxs, uvIdxs, nrmIdxs;
    std::vector<V3> verteices;
    std::vector<V2> uvs;
    std::vector<V3> normals;
    
    FILE* file = fopen(path, "r");
    if (!file) {
        printf("Not in the right path\n");
        return false;
    }
    
    while (true) {
        char lineHeader[128];
        int res = fscanf(file, "%s", lineHeader);
        
        if (res == EOF) {
            break;
        }
        if (strcmp(lineHeader, "v") == 0) {
            V3 vert;
            fscanf(file, "%f %f %f\n", &vert.x, &vert.y, &vert.z);
            verteices.push_back(vert);
        } else if (strcmp(lineHeader, "vt") == 0) {
            V2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y);
            uvs.push_back(uv);
        } else if (strcmp(lineHeader, "vn") == 0) {
            V3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            normals.push_back(normal);
        } else if (strcmp(lineHeader, "f") == 0) {
            unsigned int vertIndex[3], uvIndex[3], normalIndex[3];
            
            int math = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
                              &vertIndex[0], &uvIndex[0], &normalIndex[0],
                              &vertIndex[1], &uvIndex[1], &normalIndex[1],
                              &vertIndex[2], &uvIndex[2], &normalIndex[2]
                              );
            if (math != 9) {
                printf("File can't be read. math = %d\n", math);
                return false;
            }
            vertIdxs.push_back(vertIndex[0]);
            vertIdxs.push_back(vertIndex[1]);
            vertIdxs.push_back(vertIndex[2]);
            
            uvIdxs.push_back(uvIndex[0]);
            uvIdxs.push_back(uvIndex[1]);
            uvIdxs.push_back(uvIndex[2]);
            
            nrmIdxs.push_back(normalIndex[0]);
            nrmIdxs.push_back(normalIndex[1]);
            nrmIdxs.push_back(normalIndex[2]);
        } else {
            char stupidBuffer[1000];
            fgets(stupidBuffer, 1000, file);
        }
    }
    
    for (unsigned int i = 0; i < vertIdxs.size(); i++) {
        unsigned int vertIndex  = vertIdxs[i];
        unsigned int uvIndex    = uvIdxs[i];
        unsigned int normalIndex= nrmIdxs[i];
        
        V3 vert = verteices[vertIndex - 1];
        V2 uv   = uvs[uvIndex - 1];
        V3 nrm  = normals[normalIndex - 1];
        
        out_vertices.push_back(vert.x);
        out_vertices.push_back(vert.y);
        out_vertices.push_back(vert.z);
        
        out_uvs.push_back(uv.x);
        out_uvs.push_back(uv.y);
        
        out_normals.push_back(nrm.x);
        out_normals.push_back(nrm.y);
        out_normals.push_back(nrm.z);
        
        out_indices.push_back(i);
    }
    
    return true;
}