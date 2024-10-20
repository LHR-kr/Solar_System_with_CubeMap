#pragma once

#include <wrl.h>

#include "GeometryGenerator.h"
#include "Material.h"
#include "Vertex.h"

#include"Mesh.h"

using Microsoft::WRL::ComPtr;

class CubeMapMesh: public Mesh
{

 public:
    virtual void CreateTexture(const std::string filename,
                              ComPtr<ID3D11Device> device) override;



};
