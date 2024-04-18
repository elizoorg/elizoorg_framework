#include "Camera.h"

#include <iostream>

#include "Application.h"

Camera::Camera()
	: mPosition(0.0f, 0.0f, 0.0f),
	mRight(1.0f, 0.0f, 0.0f),
	mUp(0.0f, 1.0f, 0.0f),
	mLook(0.0f, 0.0f, 1.0f)
{
	SetLens(103 , 0.5, 0.1f, 1000.0f);
}
Camera::~Camera()
{
}

DirectX::XMVECTOR Camera::GetPositionXM() const
{
	using namespace DirectX;
	return XMLoadFloat3(&mPosition);
}

Vector3 Camera::GetPosition() const
{
	return mPosition;
}

void Camera::SetPosition(float x, float y, float z)
{
	mPosition = Vector3(x, y, z);
}

void Camera::SetPosition(const Vector3& v)
{
	mPosition = v;
}

DirectX::XMVECTOR Camera::GetRightXM() const
{
	return XMLoadFloat3(&mRight);
}

Vector3 Camera::GetRight() const
{
	return mRight;
}

Vector4 Camera::GetUpXM() const
{
	return XMLoadFloat3(&mUp);
}

Vector3 Camera::GetUp() const
{
	return mUp;
}

DirectX::XMVECTOR Camera::GetLookXM() const
{
	return XMLoadFloat3(&mLook);
}

Vector3 Camera::GetLook() const
{
	return mLook;
}

float Camera::GetNearZ() const
{
	return mNearZ;
}

float Camera::GetFarZ() const
{
	return mFarZ;
}

float Camera::GetAspect() const
{
	return mAspect;
}

float Camera::GetFovY() const
{
	return mFovY;
}

float Camera::GetFovX() const
{
	float halfWidth = 0.5f * GetNearWindowWidth();
	return 2.0f * atan(halfWidth / mNearZ);
}

float Camera::GetNearWindowWidth() const
{
	return mAspect * mNearWindowHeight;
}

float Camera::GetNearWindowHeight() const
{
	return mNearWindowHeight;
}

float Camera::GetFarWindowWidth() const
{
	return mAspect * mFarWindowHeight;
}

float Camera::GetFarWindowHeight() const
{
	return mFarWindowHeight;
}


void Camera::SetLens(float fovY, float aspect, float zn, float zf)
{
	mFovY = fovY;
	mAspect = aspect;
	mNearZ = zn;
	mFarZ = zf;

	mNearWindowHeight = 2.0f * mNearZ * tanf(0.5f * mFovY);
	mFarWindowHeight = 2.0f * mFarZ * tanf(0.5f * mFovY);

	mProj = Matrix::CreatePerspectiveFieldOfView(Math::Radians(mFovY), mAspect, mNearZ, mFarZ);
	//mProj = DirectX::XMMatrixOrthographicLH(1600, 800, 0.1f, 1.0f);
	
	//mProj = Matrix::CreateOrthographic(800, 800, mNearZ, mFarZ);
}

void Camera::LookAt(Vector3 pos, Vector3 target, Vector3 worldUp)
{
	
}

void Camera::LookAt(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	Matrix::CreateLookAt(pos, target, up);
}

Matrix Camera::View() const
{
	return mView;
}

Matrix Camera::Proj() const
{
	return mProj;
}

Matrix Camera::ViewProj() const
{
	return mView * mProj;
}

void Camera::Strafe(float d)
{
	// TODO: fix movement, it must depend by rotation quaternion
	Vector3 s(d);
	Vector3 r = mRight;
	Vector3 p = mPosition;
	mPosition = p + (s * r);
}

void Camera::Walk(float d)
{
	Vector3 s(d);
	Vector3 l = mLook;
	Vector3 p = mPosition;
	mPosition = p + (s * l);
}

void Camera::Fly(float d)
{
	Vector3 s(d);
	Vector3 u = mUp;
	Vector3 p = mPosition;
	mPosition = p + (s * u);
}

void Camera::Pitch(float angle)
{
	std::cout << "RotateY " << angle << std::endl;
	Matrix R = Matrix::CreateFromAxisAngle(mRight, angle);
	Vector3::TransformNormal(mUp, R, mUp);
	Vector3::TransformNormal(mLook, R, mLook);
}

void Camera::RotateY(float angle)
{
	std::cout << "RotateX " << angle << std::endl;
	Matrix R = Matrix::CreateRotationY(angle);
	Vector3::TransformNormal(mRight, R, mRight);
	Vector3::TransformNormal(mUp ,R, mUp);
	Vector3::TransformNormal(mLook, R, mLook);
}
using namespace DirectX;
inline XMVECTOR GMathFV(XMFLOAT3& val)
{
	return XMLoadFloat3(&val);
}

inline XMFLOAT3 GMathVF(XMVECTOR& vec)
{
	XMFLOAT3 val;
	XMStoreFloat3(&val, vec);
	return val;
}

void Camera::Rotate(Vector2 offset)
{
	//std::cout << _app->deltaTime << std::endl;
	//std::cout << (float)offset.y * (_app->deltaTime) << " " <<(float)offset.x * (_app->deltaTime) << std::endl;
	//offset.x *= SENSITIVITY;
	//offset.y *= SENSITIVITY;
	//angle_Pitch += offset.x * _app->deltaTime;
	//angle_Yaw += offset.y * _app->deltaTime;
	//std::cout << angle_Pitch << " " << angle_Yaw << "\n";
	//std::cout << _app->deltaTime << " \n";
	rotation_ *= Quaternion::CreateFromAxisAngle(Vector3::Up, static_cast<float>(offset.x) * _app->deltaTime);
	rotation_ *= Quaternion::CreateFromAxisAngle(Vector3::Right, static_cast<float>(-offset.y) * _app->deltaTime);


	//Pitch(Math::Radians(angle_Pitch));
	//RotateY(Math::Radians(angle_Yaw));
}

void Camera::UpdateViewMatrix()
{
	const auto rot = rotation_.ToEuler();
	const auto rotation = Quaternion::CreateFromYawPitchRoll(-rot.y, rot.x, 0);
	const auto target = Vector3::Transform(Vector3::Forward, rotation);
	const auto up = Vector3::Transform(Vector3::Up, rotation);
	mView = XMMatrixLookAtLH(mPosition, mPosition + target, up);
	//angle_Yaw += 1;
	//Rotate(Vector2(0.0f,0.0f));
	/*Vector3 R = mRight;
	Vector3 U = mUp;
	Vector3 L = mLook;
	Vector3 P = mPosition;

	L.Normalize();

	L.Cross(R, U);
	
	U.Normalize();

	mLook = U;

	U.Cross(L, R);

	mRight = R;

	float x = -P.Vector3::Dot(R);
	float y = -P.Vector3::Dot(U);
	float z = -P.Vector3::Dot(L);
	
	mRight = R;
	mUp = U;
	mLook = L;

	mView = Matrix(
		mRight.x, mUp.x, mLook.x, 0.0f,
		mRight.y, mUp.y, mLook.y, 0.0f,
		mRight.z, mUp.z, mLook.z, 0.0f,
		x,y,z,1.0f
	);*/
}

void Camera::Bind()
{
	_app->getInput()->DOnMouseMove.AddRaw(this, &Camera::Rotate);
}
