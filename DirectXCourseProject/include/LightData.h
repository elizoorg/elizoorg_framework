#pragma once
#include "Mathtypes.h"
#include "Exports.h"

struct ENGINE_API LightData {
	Vector4 LightPos;
	Vector4 LightColor;
	Vector4 AmbientSpecularRowType;
	Matrix T;
	Matrix ViewMatrix;
};