#pragma once
#include "MathTypes.h"
#include "Exports.h"
#include "pch.h"

class ENGINE_API Transform
{
	Vector3 Position;
	Vector3 RotationVector;

	Vector3 RotationAngles;
	Vector3 RotationSpeed;

	Vector3 PointToRotate;
	Vector3 Scale;

	Vector3 Speed;
	Vector3 Velocity;

	Vector3 LocalPosition;
	Vector3 LocalRotation;

	Transform* parent;
	std::vector<Transform*> Childs;
public:
	void setPosition(Vector3 position);
	void setLocalPosition(Vector3 position);
	void setScale(Vector3 scale);
	void setRotationVector(Vector3 rotation);
	void setRotationAngle(Vector3 angle);
	void setSpeed(Vector3 speed);
	void setRotationSpeed(Vector3 speed);
	void setRotationSpeed(float speed);
	void Move(Vector3 offset);

	Vector3 getPosition() { return Position; }
	Vector3 getRotationVector() { return RotationVector; }
	Vector3 getScale() { return Scale; }
	Vector3 getRotationAngle() { return RotationAngles; }

	void ApplyVelocity(Vector3 velocity);

	void UpdateChilds();
	void Update();

};

