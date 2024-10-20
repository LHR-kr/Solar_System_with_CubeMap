#pragma once

#pragma once

#include <directxtk/SimpleMath.h>
#include <string>
#include <vector>

#include "Vertex.h"



using std::vector;

struct GeometryMeshData {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices; // uint32·Î º¯°æ
    std::string textureFilename;
};

