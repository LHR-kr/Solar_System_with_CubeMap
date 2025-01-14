﻿#pragma once

#include <directxtk/SimpleMath.h>



using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Vector3;

class Camera {
  public:
    Matrix GetViewRow();
    Matrix GetProjRow();
    Vector3 GetEyePos();

    void UpdateMouse(float mouseNdcX, float mouseNdcY);
    void MoveForward(float dt);
    void MoveRight(float dt);
    void SetAspectRatio(float aspect);

  private:


    Vector3 m_position = Vector3(0.0f, 0.4f, -5.0f);
    Vector3 m_viewDir = Vector3(0.0f, 0.0f, 1.0f);
    Vector3 m_upDir = Vector3(0.0f, 1.0f, 0.0f); 
    Vector3 m_rightDir = Vector3(1.0f, 0.0f, 0.0f);

    // roll, pitch, yaw
    float m_roll = 0.0f, m_yaw = 0.0f;

    float m_speed = 1.0f; // 움직이는 속도
    float m_projFovAngleY = 70.0f;
    float m_nearZ = 0.01f;
    float m_farZ = 100.0f;
    float m_aspect = 16.0f / 9.0f;
};
