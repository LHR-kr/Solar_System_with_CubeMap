#pragma once

#include <directxtk/SimpleMath.h>
#include <vector>
#include <string>

#include "Vertex.h"
#include "GeometryMeshData.h"



class GeometryGenerator {
  public:
    //static vector<GeometryMeshData> ReadFromFile(std::string basePath,
    //                                     std::string filename);

    static GeometryMeshData MakeSquare();
    static GeometryMeshData MakeBox(const float scale = 1.0f);
    static GeometryMeshData MakeCylinder(const float bottomRadius,
                                 const float topRadius, float height,
                                 int numSlices);
    static GeometryMeshData MakeSphere(const float radius, const int numSlices,
                               const int numStacks);

    static GeometryMeshData SubdivideToSphere(const float radius, GeometryMeshData meshData);
    static GeometryMeshData MakeRing(const float innerRadius, const float outterRadius,
                             const int numSlices);
    static GeometryMeshData MakePoint();
};
