#pragma once
#include "MathTypes.h"
#include "Exports.h"
#include "pch.h"

class ENGINE_API Transform
{
public:
	Transform(Vector3 pos, Quaternion rot, Vector3 scale);
	Transform();

	void SetPosition(const Vector3& pos);
	void SetScale(const Vector3& scale);
	void SetMatrixRotate(const Matrix& rotation);
	void SetQuaternionRotate(const Quaternion& quaternion);
	void SetWorldMatrix(const Matrix& matrix);
	void SetParent(Transform* transform);

	void SetEulerRotate(const Vector3& eulerAngle);
	void SetRadianRotate(const Vector3& radianAngle);

	void AdjustPosition(const Vector3& pos);
	void AdjustPosition(float x, float y, float z);
	void AdjustEulerRotation(const Vector3& eulerAngle);
	void AdjustEulerRotation(float roll, float pitch, float yaw);

	void Update();

	Vector3 GetWorldPosition();
	Vector3 GetScale();
	Quaternion GetQuaternionRotate();
	Vector3 GetEulerAngles();

	Vector3 GetRightVector();
	Vector3 GetUpVector();
	Vector3 GetForwardVector();

	Matrix GetWorldMatrix();

	bool IsDirty();


private:
	Matrix CalculateWorldMatrix();

	Vector3 localEulerAngles = Vector3::Zero;
	Vector3 localPosition = Vector3::Zero;
	Vector3 localScale = Vector3::One;
	Quaternion localRotate = Quaternion::Identity;

	Matrix world = Matrix::Identity;
	Matrix worldTranspose;

	Transform* parent = nullptr;
	bool isDirty = true;
};

