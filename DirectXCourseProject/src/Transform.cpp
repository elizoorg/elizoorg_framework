#include "Transform.h"

Vector3 Transform::GetWorldPosition()
{
	return Vector3(world.m[3]);
}

Vector3 Transform::GetScale()
{
	return localScale;
}

Quaternion Transform::GetQuaternionRotate()
{
	return localRotate;
}

Vector3 Transform::GetEulerAngles()
{
	return localEulerAngles;
}

Vector3 Transform::GetRightVector()
{
	auto v = Vector3{ worldTranspose._11, worldTranspose._21, worldTranspose._31 };
	v.Normalize();
	return v * -1;
}

Vector3 Transform::GetUpVector()
{
	Vector3 v = Vector3{ worldTranspose._12, worldTranspose._22, worldTranspose._32 };
	v.Normalize();
	return v;
}

Vector3 Transform::GetForwardVector()
{
	Vector3 v = Vector3{ worldTranspose._13, worldTranspose._23, worldTranspose._33 };
	v.Normalize();
	return v;
}


Matrix Transform::GetWorldMatrix()
{
	return world;
}

bool Transform::IsDirty()
{
	return isDirty;
}

Matrix Transform::CalculateWorldMatrix()
{	
	Matrix result = Matrix::CreateScale(localScale) * Matrix::CreateFromQuaternion(localRotate) * 
		Matrix::CreateTranslation(localPosition);

	if (parent != nullptr) {
		result  = result * parent->GetWorldMatrix();

		/*Vector3 new_pos = Vector3::Transform(Vector3(localPosition.x,localPosition.y,localPosition.z), result);
		Vector3 new_rot = Vector3::TransformNormal(Vector3(localRotate.x, localRotate.y, localRotate.z), result);
		result = Matrix::CreateScale(localScale) * Matrix::CreateFromYawPitchRoll(new_rot) * Matrix::CreateTranslation(new_pos);*/

	}



	return result;
}

Transform::Transform(Vector3 pos, Quaternion rot, Vector3 scale): localPosition(pos),localRotate(rot), localScale(scale)
{
}

Transform::Transform():Transform(Vector3::Zero,Quaternion::Identity,Vector3::One)
{
}

void Transform::SetPosition(const Vector3& pos)
{
	localPosition = pos;
	world = CalculateWorldMatrix();
	isDirty = true;
}

void Transform::SetScale(const Vector3& scale)
{
	localScale = scale;
	world = CalculateWorldMatrix();
	isDirty = true;
}

void Transform::SetMatrixRotate(const Matrix& rotation)
{
	localRotate = Quaternion::CreateFromRotationMatrix(rotation);
	world = CalculateWorldMatrix();
	isDirty = true;
}

void Transform::SetQuaternionRotate(const Quaternion& quaternion)
{
	localRotate = quaternion;
	world = CalculateWorldMatrix();
	isDirty = true;
}

void Transform::SetWorldMatrix(const Matrix& matrix)
{
	world = matrix;
	isDirty = true;
}

void Transform::SetParent(Transform* transform)
{
	parent = transform;
}

void Transform::SetEulerRotate(const Vector3& eulerAngle)
{
	localEulerAngles = eulerAngle;
	localRotate = Quaternion::CreateFromYawPitchRoll(Math::Radians(eulerAngle.x), Math::Radians(eulerAngle.y), Math::Radians(eulerAngle.z));
	world = CalculateWorldMatrix();
	isDirty = true;
}

void Transform::SetRadianRotate(const Vector3& radianAngle)
{
	localEulerAngles = Vector3(Math::Degrees(radianAngle.x), Math::Degrees(radianAngle.y), Math::Degrees(radianAngle.z));
	localRotate = Quaternion::CreateFromYawPitchRoll(radianAngle.x,radianAngle.y,radianAngle.z);
	world = CalculateWorldMatrix();
	isDirty = true;
}

void Transform::AdjustPosition(const Vector3& pos)
{
	SetPosition(localPosition + pos);
}

void Transform::AdjustPosition(float x, float y, float z)
{
	SetPosition(localPosition + Vector3(x,y,z));
}

void Transform::AdjustEulerRotation(const Vector3& eulerAngle)
{
	SetEulerRotate(localEulerAngles + eulerAngle);
}

void Transform::AdjustEulerRotation(float roll, float pitch, float yaw)
{
	SetEulerRotate(localEulerAngles + Vector3(roll,pitch,yaw));
}

void Transform::Update()
{
	if (!parent) {
		if (isDirty) {
			worldTranspose = world.Transpose();
			isDirty = false;
		}
	}
	else {
		if (parent->IsDirty()) {
			world = CalculateWorldMatrix();
			worldTranspose = world.Transpose();
			isDirty = false;
		}
		else {
			worldTranspose = world.Transpose();
			isDirty = false;
		}
	}
	
}
