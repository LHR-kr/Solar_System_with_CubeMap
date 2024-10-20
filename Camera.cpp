#include "Camera.h"



using namespace std;
using namespace DirectX;

Matrix Camera::GetViewRow() {

    return Matrix::CreateTranslation(-this->m_position) *
           Matrix::CreateRotationY(-this->m_yaw) *
           Matrix::CreateRotationX(this->m_roll);
}

Vector3 Camera::GetEyePos() { return m_position; }

void Camera::UpdateMouse(float mouseNdcX, float mouseNdcY) {
    // 얼마나 회전할지 계산
    m_yaw = mouseNdcX * DirectX::XM_2PI;     // 좌우 360도
    m_roll = mouseNdcY * DirectX::XM_PIDIV2; // 위 아래 90도

    // 이동할 때 기준이 되는 정면/오른쪽 방향 계산
    m_viewDir = Vector3::Transform(Vector3(0.0f, 0.0f, 1.0f),
                                   Matrix::CreateRotationY(this->m_yaw));
    m_rightDir = m_upDir.Cross(m_viewDir);
}

void Camera::MoveForward(float dt) {
    // 이동후의_위치 = 현재_위치 + 이동방향 * 속도 * 시간차이;
    m_position += m_viewDir * m_speed * dt;
}

void Camera::MoveRight(float dt) { m_position += m_rightDir * m_speed * dt; }

void Camera::SetAspectRatio(float aspect) { m_aspect = aspect; }

Matrix Camera::GetProjRow() {
    return XMMatrixPerspectiveFovLH(XMConvertToRadians(m_projFovAngleY),
                                    m_aspect, m_nearZ, m_farZ);

}
