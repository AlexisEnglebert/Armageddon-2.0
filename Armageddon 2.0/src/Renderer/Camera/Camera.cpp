#include "Camera.h"

Armageddon::Camera::Camera()
{
    this->pos = DirectX::XMFLOAT3(0.0f, 0.0f, 3.0f);
    this->posVector = DirectX::XMLoadFloat3(&this->pos);
    this->rotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
    this->RotVector = DirectX::XMLoadFloat3(&this->rotation);
    UpdateViewMatrix();
}

void Armageddon::Camera::SetProjectionValues(float FovDegrees, float AspectRatio, float nearZ, float FarZ)
{
    FOV = FovDegrees;
    this->AspectRatio= AspectRatio;
    float fovRad = (FovDegrees / 360.0f) * DirectX::XM_2PI;
    this->NearZ = nearZ;
    this->FarZ = FarZ;
    this->ProjectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fovRad, AspectRatio, nearZ, FarZ);
}

const DirectX::XMMATRIX& Armageddon::Camera::GetViewMatrix() const
{
    return this->ViewMatrix;
}

const DirectX::XMMATRIX& Armageddon::Camera::GetProjectionMatrix() const
{
    return this->ProjectionMatrix;
}

const DirectX::XMFLOAT3& Armageddon::Camera::GetPos() const
{
    return this->pos;
}

const DirectX::XMVECTOR& Armageddon::Camera::GetVecPos() const
{
    return this->posVector;
}

const DirectX::XMFLOAT3& Armageddon::Camera::GetRotation() const
{
    return this->rotation;
}

void Armageddon::Camera::SetPosition(const DirectX::XMVECTOR& pos)
{
    DirectX::XMStoreFloat3(&this->pos, pos);
    this->posVector = pos;
    this->UpdateViewMatrix();
}

void Armageddon::Camera::SetPosition(float x, float y, float z)
{
    this->pos = DirectX::XMFLOAT3(x, y, z);
    this->posVector = DirectX::XMLoadFloat3(&this->pos);
    this->UpdateViewMatrix();
}

void Armageddon::Camera::AdjustPosition(const DirectX::XMVECTOR& pos)
{
    this->posVector += pos;
    DirectX::XMStoreFloat3(&this->pos, this->posVector);
    UpdateViewMatrix();
}

void Armageddon::Camera::AdjustPosition(float x, float y, float z)
{
    this->pos.x += x;
    this->pos.y += y;
    this->pos.z += z;
    this->posVector = DirectX::XMLoadFloat3(&this->pos);
    this->UpdateViewMatrix();
}

void Armageddon::Camera::SetRotation(const DirectX::XMVECTOR& pos)
{
    this->RotVector = pos;
    DirectX::XMStoreFloat3(&this->rotation, pos);
    this->UpdateViewMatrix();
}

void Armageddon::Camera::SetRotation(float x, float y, float z)
{

    this->rotation = DirectX::XMFLOAT3(x, y, z);
    this->RotVector = DirectX::XMLoadFloat3(&this->rotation);

    this->UpdateViewMatrix();
}

void Armageddon::Camera::AdjustRotation(const DirectX::XMVECTOR& pos)
{
    this->RotVector += pos;
    DirectX::XMStoreFloat3(&this->rotation, pos);
    this->UpdateViewMatrix();

}

void Armageddon::Camera::AdjustRotation(float x, float y, float z)
{
    this->rotation.x += x;
    this->rotation.y += y;
    this->rotation.z += z;
    this->RotVector = DirectX::XMLoadFloat3(&this->rotation);
    UpdateViewMatrix();
}

void Armageddon::Camera::SetLookAtPos(DirectX::XMFLOAT3 LookPos)
{
    // position ne peut pas être la même que la position de la camera 
    if (LookPos.x == this->pos.x && LookPos.y == this->pos.y && LookPos.z == this->pos.z)
    {
        return;
    }

    LookPos.x = this->pos.x - LookPos.x;
    LookPos.y = this->pos.z - LookPos.y;
    LookPos.z = this->pos.y - LookPos.y;

    float pitch = 0.0f;
    if (LookPos.y != 0.0f) // calcule l'amplitude de l'angle de la caméra dans l'axe Y (pythagore) 
    {
        const float Dist = sqrt(LookPos.x * LookPos.x + LookPos.z * LookPos.z);
        pitch = atan(LookPos.y / Dist);

    }

    this->SetRotation(pitch, 0.0f, 0.0f);
}

void Armageddon::Camera::SetZoomLevel(float windowWidth, float windowHeight, float ZoomLevel)
{
    float aspectRatio = windowWidth / windowHeight;
    float fovRad = (90.0f * ZoomLevel / 360.0f) * DirectX::XM_2PI;
}

const DirectX::XMVECTOR& Armageddon::Camera::GetForwardVec()
{
    return this->vec_forward;
}

const DirectX::XMVECTOR& Armageddon::Camera::GetBackwardVec()
{
    return this->vec_backward;
}

const DirectX::XMVECTOR& Armageddon::Camera::GetLeftVec()
{
    return this->vec_left;
}

const DirectX::XMVECTOR& Armageddon::Camera::GetRightVec()
{
    return this->vec_right;
}

const DirectX::XMVECTOR& Armageddon::Camera::GetUpVec()
{
    return vec_up;
}

const DirectX::XMVECTOR& Armageddon::Camera::GetDownVec()
{
    return this->vec_down;
}

void Armageddon::Camera::UpdateViewMatrix()
{
    // CALCULE LA ROTATION 
    DirectX::XMMATRIX CameraRotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(this->rotation.x, this->rotation.y, this->rotation.z);

    //CALCUL LA TARGET  
    DirectX::XMVECTOR CamTarget = DirectX::XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, CameraRotationMatrix);

    CamTarget += this->posVector;

    DirectX::XMVECTOR UpDir = DirectX::XMVector3TransformCoord(this->DEFAULT_UP_VECTOR, CameraRotationMatrix);
    this->ViewMatrix = DirectX::XMMatrixLookAtLH(this->posVector, CamTarget, UpDir);

    DirectX::XMMATRIX vecRotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(this->rotation.x, this->rotation.y, this->rotation.z);
    Armageddon::Log::GetLogger()->trace("Cam pos  X: {0}  Y: {1}  Z: {2}",this->pos.x, this->pos.y, this->pos.z);
  //  Armageddon::Log::GetLogger()->trace("Cam ROT  X: {0}  Y: {1}  Z: {2}",this->rotation.x, this->rotation.y, this->rotation.z);
    this->vec_backward = XMVector3TransformCoord(this->DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
    this->vec_forward = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
    this->vec_left = XMVector3TransformCoord(this->DEFAULT_LEFT_VECTOR, vecRotationMatrix);
    this->vec_right = XMVector3TransformCoord(this->DEFAULT_RIGHT_VECTOR, vecRotationMatrix);
    this->vec_down = XMVector3TransformCoord(this->DEFAULT_DOWN_VECTOR, vecRotationMatrix);
    this->vec_up = XMVector3TransformCoord(this->DEFAULT_UP_VECTOR, vecRotationMatrix);


}
