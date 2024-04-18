#include "Transform.h"

void Transform::setPosition(Vector3 position)
{
	Position = position;
}

void Transform::setLocalPosition(Vector3 position)
{
	LocalPosition = position;
}

void Transform::setScale(Vector3 scale)
{
	Scale = scale;
}

void Transform::setRotationVector(Vector3 rotation)
{
	RotationVector = rotation;
}

void Transform::setRotationAngle(Vector3 angle)
{
	RotationAngles = angle;
}

void Transform::setSpeed(Vector3 speed)
{
	Velocity = speed;
}

void Transform::setRotationSpeed(Vector3 speed)
{
	RotationSpeed = speed;
}

void Transform::setRotationSpeed(float speed)
{
	RotationSpeed = Vector3(speed);
}

void Transform::Move(Vector3 offset)
{
	Position += offset;
}

void Transform::ApplyVelocity(Vector3 velocity)
{
	Speed = velocity;
}

void Transform::UpdateChilds()
{
}

void Transform::Update()
{
	Position += Velocity;
	RotationAngles += RotationSpeed;
}
