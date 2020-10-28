#include "World.h"
#include <algorithm>
//#include"Common.h"

//Vector2 camera = { world_max_x / 2 - (world_max_x / 2 / 2), world_max_y / 2 };

float Limit::Left_max = -100.0f;
float Limit::Right_max = 100.0f;
YR_Vector3 Limit::now_Scene_camera = { 0.0f,0.0f,0.0f };

void Limit::First_Camera_set(YR_Vector3 camera)
{
	now_Scene_camera = camera;
}


YR_Vector3 Limit::Set(YR_Vector3 p1, YR_Vector3 p2, YR_Vector3 start_eye)
{
	//最もカメラが引く位置
	//y=14.0f
	//z=-190.0f
	YR_Vector3 far_camera = YR_Vector3(0.0f, 14.0f, -190.0f);

	//デフォルト(最もカメラが寄る位置)
	//y=6.0f
	//z=-140.0f
	YR_Vector3 near_camera = YR_Vector3(0.0f, 6.0f, -140.0f);

	YR_Vector3 camera_pos;
	float distance_x = 0.0f;
	float distance_y = 0.0f;
	float up_meter = 0.0f;
	if (p1.x > p2.x)
	{
		distance_x = (p1.x - p2.x) / 2.0f;
		float correction = p2.x + distance_x;
		//カメラの調整
		camera_pos.x = correction;
		

		//camera_pos.z = start_eye.z - distance + adjustment_eye;
		//camera_pos.y = start_eye.y + (distance_x*0.1f) - (adjustment_eye*0.1f);
	}
	if (p1.x < p2.x)
	{
		distance_x = (p2.x - p1.x) / 2;
		float correction = p1.x + distance_x;
		camera_pos.x = correction;
		
		//camera_pos.z = start_eye.z - distance + adjustment_eye;
		//camera_pos.y = start_eye.y + (distance_x*0.1f) - (adjustment_eye * 0.1f);
	}

	if (p1.x == p2.x)
	{
		//float distance = (p2 - p1) / 2;
		float correction = p1.x;
		camera_pos.x = correction;
		
		//camera_pos.z = start_eye.z + adjustment_eye;
		camera_pos.y = start_eye.y - (adjustment_eye * 0.1f);
	}

	Left_max = camera_pos.x - limit_size;
	Right_max = camera_pos.x + limit_size;

	if (Left_max < -world_max_x)
	{
		Left_max = -world_max_x;
	}

	if (Right_max > world_max_x)
	{
		Right_max = world_max_x;
	}


	//y
	if (p1.y > p2.y)
	{
		up_meter = p1.y;
		distance_y = (p1.y - p2.y);
		/*distance_y = (p1.y - p2.y) / 2.0f;
		float correction = p2.y + distance_y;
		//カメラの調整
		camera_pos.y = correction;*/
	}
	if (p1.y < p2.y)
	{
		up_meter = p2.y;
		distance_y = (p2.y - p1.y);
	}

	if (p1.y == p2.y)
	{
		up_meter = p1.y;
	}

	float default_z = 12.0f;	//カメラが引き始めるプレイヤーの高さ
	float a = (start_eye.z - distance_x + adjustment_eye);
	float b = (start_eye.z - distance_x + adjustment_eye - (up_meter - default_z));

	float c = (start_eye.y + (distance_x * 0.1f) - (adjustment_eye * 0.1f));
	float d = (start_eye.y + (distance_x * 0.1f) - (adjustment_eye * 0.1f) + (up_meter - default_z));


	if (up_meter > default_z)
	{
		//プレイヤーの位置が既定の高さに達したらカメラを引く
		float t = 0.05f;
		if (distance_y > default_z)
		{
			//お互いの高さが同じ場合は画面を引かない
			DirectX::XMVECTOR now_z = DirectX::XMLoadFloat(&now_Scene_camera.z);
			DirectX::XMVECTOR far_z = DirectX::XMLoadFloat(&far_camera.z);

			DirectX::XMVECTOR lerp = DirectX::XMVectorLerp(now_z, far_z, t);
			DirectX::XMStoreFloat(&now_Scene_camera.z, lerp);
		}
		DirectX::XMVECTOR now_y = DirectX::XMLoadFloat(&now_Scene_camera.y);
		DirectX::XMVECTOR far_y = DirectX::XMLoadFloat(&far_camera.y);


		DirectX::XMVECTOR lerp = DirectX::XMVectorLerp(now_y, far_y, t);
		DirectX::XMStoreFloat(&now_Scene_camera.y, lerp);
	}
	else
	{
		//カメラをもとに戻す
		float t = 0.005f;
		DirectX::XMVECTOR now_z = DirectX::XMLoadFloat(&now_Scene_camera.z);
		DirectX::XMVECTOR near_z = DirectX::XMLoadFloat(&near_camera.z);

		DirectX::XMVECTOR lerp_z = DirectX::XMVectorLerp(now_z, near_z, t);
		DirectX::XMStoreFloat(&now_Scene_camera.z, lerp_z);

		DirectX::XMVECTOR now_y = DirectX::XMLoadFloat(&now_Scene_camera.y);
		DirectX::XMVECTOR near_y = DirectX::XMLoadFloat(&near_camera.y);


		DirectX::XMVECTOR lerp_y = DirectX::XMVectorLerp(now_y, near_y, t);
		DirectX::XMStoreFloat(&now_Scene_camera.y, lerp_y);
	}


	camera_pos.y = now_Scene_camera.y + (distance_x * 0.1f) - (adjustment_eye * 0.1f);
	camera_pos.z = now_Scene_camera.z - distance_x + adjustment_eye;

	//camera_pos.z = std::min((start_eye.z - distance_x + adjustment_eye), (start_eye.z - distance_x + adjustment_eye - ((up_meter - default_z) * 6.0f)));
	//camera_pos.y = std::max((start_eye.y + (distance_x * 0.1f) - (adjustment_eye * 0.1f)), (start_eye.y + (distance_x * 0.1f) - (adjustment_eye * 0.1f) + ((up_meter - default_z)*0.8f)));

	return camera_pos;
}

void Limit::Stop(float& p1, float& p2)
{
	//プレイヤーの移動制限
	if (p1 > Right_max)
	{
		p1 = Right_max;
	}

	if (p1 < Left_max)
	{
		p1 = Left_max;
	}

	if (p2 > Right_max)
	{
		p2 = Right_max;
	}

	if (p2 < Left_max)
	{
		p2 = Left_max;
	}
}