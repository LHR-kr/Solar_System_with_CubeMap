#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxtk/SimpleMath.h>
#include <memory>



using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Vector4; 

struct Material {
    Vector3 ambient = Vector3(0.0f);  // 12
    float shininess = 256.0f;           // 4
    Vector3 diffuse = Vector3(0.0f);  // 12
    float dummy1;                     // 4
    Vector3 specular = Vector3(1.0f); // 12
    float dummy2;                     // 4
};



struct VertexViewProjConstantBufferData {
    Matrix view = Matrix::Identity;
    Matrix projection = Matrix::Identity;
};
static_assert((sizeof(VertexViewProjConstantBufferData) % 16) == 0,
              "Constant Buffer size must be 16-byte aligned");

struct VertexModelConstantBufferData {
    Matrix model = Matrix::Identity;
    Matrix invTranspose = Matrix::Identity;
};

static_assert((sizeof(VertexModelConstantBufferData) % 16) == 0,
              "Constant Buffer size must be 16-byte aligned");
// Α¶Έν
struct Light {
    Vector3 strength = Vector3(1.0f);              // 12
    float fallOffStart = 0.0f;                     // 4
    Vector3 direction = Vector3(0.0f, 0.0f, 1.0f); // 12
    float fallOffEnd = 10.0f;                      // 4
    Vector3 position = Vector3(0.0f, 0.0f, 0.0f);  // 12
    float spotPower = 100.0f;                      // 4
};

struct BasicPixelConstantBufferData {
    Vector3 eyeWorld;  // 12
    bool useTexture;   // 4
    Material material; // 48
    Light lights;      // 48 * MAX_LIGHTS
    Vector3 rimColor = Vector3(1.0f);
    float rimPower;
    float rimStrength = 0.0f;
    bool useSmoothstep = false;
    float dummy[2];
};

static_assert((sizeof(BasicPixelConstantBufferData) % 16) == 0,
              "Constant Buffer size must be 16-byte aligned");

struct BillboardConstantBufferData {
    Vector3 eyeWorld; // For geometry shader
    float width;     // For geometry shader
    Matrix model;    // For vertex shader
    Matrix view;     // For vertex shader
    Matrix proj;     // For vertex shader
    float time;
    Vector3 padding;
};

static_assert((sizeof(BasicPixelConstantBufferData) % 16) == 0,
              "Constant Buffer size must be 16-byte aligned");