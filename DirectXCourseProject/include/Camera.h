#pragma once
#include "include/SimpleMath.h"
#include "MathTypes.h"
#include "Exports.h"
#include "iostream"


namespace Engine
{
	class Application;
}

class ENGINE_API Camera
{
public:
	Camera();
	~Camera();
	Camera(Engine::Application* app) : _app(app)
	{
		mPosition = Vector3(0.0f, 0.0f, 0.0f);
		mRight = Vector3(1.0f, 0.0f, 0.0f);
		mUp = Vector3(0.0f, 1.0f, 0.0f);
		mLook = Vector3(0.0f, 0.0f, 1.0f);
		SetLens(103, 0.5, 0.1f, 1000.0f);
	}
	friend class Engine::Application;
	Engine::Application* _app;


	// Get/Set world camera position.
	DirectX::XMVECTOR GetPositionXM()const;
	Vector3 GetPosition()const;
	void SetPosition(float x, float y, float z);
	void SetPosition(const Vector3& v);

	// Get camera basis vectors.
	DirectX::XMVECTOR GetRightXM()const;
	Vector3 GetRight()const;
	Vector4 GetUpXM()const;
	Vector3 GetUp()const;
	DirectX::XMVECTOR GetLookXM()const;
	Vector3 GetLook()const;

	// Get frustum properties.
	float GetNearZ()const;
	float GetFarZ()const;
	float GetAspect()const;
	float GetFovY()const;
	float GetFovX()const;

	// Get near and far plane dimensions in view space coordinates.
	float GetNearWindowWidth()const;
	float GetNearWindowHeight()const;
	float GetFarWindowWidth()const;
	float GetFarWindowHeight()const;

	// Set frustum.
	void SetLens(float fovY, float aspect, float zn, float zf);

	// Define camera space via LookAt parameters.
	void LookAt(Vector3 pos, Vector3 target, Vector3 worldUp);
	void LookAt(const Vector3& pos, const Vector3& target, const Vector3& up);

	// Get View/Proj matrices.
	Matrix View()const;
	Matrix Proj()const;
	Matrix ViewProj()const;

	// Strafe/Walk the camera a distance d.
	void Strafe(float d);
	void Walk(float d);
	void Fly(float d);

	// Rotate the camera.
	void Pitch(float angle);
	void RotateY(float angle);
	void Rotate(Vector2 offset);

	// After modifying camera position/orientation, call to rebuild the view matrix.
	void UpdateViewMatrix();


	void Bind();

private:

	// Camera coordinate system with coordinates relative to world space.
	Vector3 mPosition;
	Vector3 mRight;
	Vector3 mUp;
	Vector3 mLook;

	Vector2 prevMousePos;

	// Cache frustum properties.
	float mNearZ;
	float mFarZ;
	float mAspect;
	float mFovY;
	float mNearWindowHeight;
	float mFarWindowHeight;

	DirectX::SimpleMath::Quaternion rotation_;


	const double SENSITIVITY = 0.000000001f;

	float angle_Yaw=-180.0f;
	float angle_Pitch=0.0f;

	// Cache View/Proj matrices.
	Matrix mView;
	Matrix mProj;
};


