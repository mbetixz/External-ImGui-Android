#ifndef Math_h
#define Math_h

#include "Structs.h"

FMatrix RotatorToMatrix(FRotator rotation) {
    float radPitch = rotation.Pitch * ((float) PI / 180.0f);
    float radYaw = rotation.Yaw * ((float) PI / 180.0f);
    float radRoll = rotation.Roll * ((float) PI / 180.0f);
    
    float SP = sinf(radPitch);
    float CP = cosf(radPitch);
    float SY = sinf(radYaw);
    float CY = cosf(radYaw);
    float SR = sinf(radRoll);
    float CR = cosf(radRoll);
    
    FMatrix matrix;
    
    matrix[0][0] = (CP * CY);
    matrix[0][1] = (CP * SY);
    matrix[0][2] = (SP);
    matrix[0][3] = 0;
    
    matrix[1][0] = (SR * SP * CY - CR * SY);
    matrix[1][1] = (SR * SP * SY + CR * CY);
    matrix[1][2] = (-SR * CP);
    matrix[1][3] = 0;
    
    matrix[2][0] = (-(CR * SP * CY + SR * SY));
    matrix[2][1] = (CY * SR - CR * SP * SY);
    matrix[2][2] = (CR * CP);
    matrix[2][3] = 0;
    
    matrix[3][0] = 0;
    matrix[3][1] = 0;
    matrix[3][2] = 0;
    matrix[3][3] = 1;
    
    return matrix;
}

Vector2 World2Screen(Vector3 ActorPos, MinimalViewInfo LocalCamInfo, float ScreenWidth, float ScreenHeight) {
    float radPitch = LocalCamInfo.Rotation.Pitch * (PI / 180.0);
    float radYaw = LocalCamInfo.Rotation.Yaw * (PI / 180.0);
    float radRoll = LocalCamInfo.Rotation.Roll * (PI / 180.0);

    float SP = sin(radPitch);
    float CP = cos(radPitch);
    float SY = sin(radYaw);
    float CY = cos(radYaw);
    float SR = sin(radRoll);
    float CR = cos(radRoll);

    Vector3 AxisX;
    AxisX.X = (CP * CY);
    AxisX.Y = (CP * SY);
    AxisX.Z = (SP);
    Vector3 AxisY;
    AxisY.X = (SR * SP * CY - CR * SY);
    AxisY.Y = (SR * SP * SY + CR * CY);
    AxisY.Z = (-SR * CP);
    Vector3 AxisZ;
    AxisZ.X = (-(CR * SP * CY + SR * SY));
    AxisZ.Y = (CY * SR - CR * SP * SY);
    AxisZ.Z = (CR * CP);
    Vector3 vDelta;
    vDelta.X = (ActorPos.X - LocalCamInfo.Location.X);
    vDelta.Y = (ActorPos.Y - LocalCamInfo.Location.Y);
    vDelta.Z = (ActorPos.Z - LocalCamInfo.Location.Z);
    Vector3 vTransformed;
    vTransformed.X = Vector3::Dot(vDelta, AxisY);
    vTransformed.Y = Vector3::Dot(vDelta, AxisZ);
    vTransformed.Z = Vector3::Dot(vDelta, AxisX);

    if (vTransformed.Z < 1.0) {
        vTransformed.Z = 1.0;
    }
    Vector2 ScreenCenter;
    ScreenCenter.X = (ScreenWidth / 2.0);
    ScreenCenter.Y = (ScreenHeight / 2.0);
    Vector2 results;
    results.X = (ScreenCenter.X + vTransformed.X * (ScreenCenter.X / tan(LocalCamInfo.FOV * (M_PI / 360.0))) / vTransformed.Z);
    results.Y = (ScreenCenter.Y - vTransformed.Y * (ScreenCenter.X / tan(LocalCamInfo.FOV * (M_PI / 360.0))) / vTransformed.Z);
    return results;
}

Vector3 MatrixToVector(FMatrix matrix) {
	return Vector3(matrix.M[3][0], matrix.M[3][1], matrix.M[3][2]);
}

FMatrix MatrixMulti(FMatrix& m1, FMatrix& m2) {
	FMatrix matrix = FMatrix();
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				matrix.M[i][j] += m1.M[i][k] * m2.M[k][j];
			}
		}
	}
	return matrix;
}

FMatrix TransformToMatrix(FTransform transform)
{
	FMatrix matrix;
	matrix.M[3][0] = transform.Translation.X;
	matrix.M[3][1] = transform.Translation.Y;
	matrix.M[3][2] = transform.Translation.Z;
	float x2 = transform.Rotation.X + transform.Rotation.X;
	float y2 = transform.Rotation.Y + transform.Rotation.Y;
	float z2 = transform.Rotation.Z + transform.Rotation.Z;
	float xx2 = transform.Rotation.X * x2;
	float yy2 = transform.Rotation.Y * y2;
	float zz2 = transform.Rotation.Z * z2;
	matrix.M[0][0] = (1 - (yy2 + zz2)) * transform.Scale3D.X;
	matrix.M[1][1] = (1 - (xx2 + zz2)) * transform.Scale3D.Y;
	matrix.M[2][2] = (1 - (xx2 + yy2)) * transform.Scale3D.Z;
	float yz2 = transform.Rotation.Y * z2;
	float wx2 = transform.Rotation.W * x2;
	matrix.M[2][1] = (yz2 - wx2) * transform.Scale3D.Z;
	matrix.M[1][2] = (yz2 + wx2) * transform.Scale3D.Y;
	float xy2 = transform.Rotation.X * y2;
	float wz2 = transform.Rotation.W * z2;
	matrix.M[1][0] = (xy2 - wz2) * transform.Scale3D.Y;
	matrix.M[0][1] = (xy2 + wz2) * transform.Scale3D.X;
	float xz2 = transform.Rotation.X * z2;
	float wy2 = transform.Rotation.W * y2;
	matrix.M[2][0] = (xz2 + wy2) * transform.Scale3D.Z;
	matrix.M[0][2] = (xz2 - wy2) * transform.Scale3D.X;
	matrix.M[0][3] = 0;
	matrix.M[1][3] = 0;
	matrix.M[2][3] = 0;
	matrix.M[3][3] = 1;
	return matrix;
}

#endif