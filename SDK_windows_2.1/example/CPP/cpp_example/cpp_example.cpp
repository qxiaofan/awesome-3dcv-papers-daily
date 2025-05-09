// example.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <string.h>
#include "xcamera.h"
#include "enumerate.h"
#include<opencv2/opencv.hpp>
using namespace CAMERA;

int main()
{
	/*****************************************************************************************************/
	int ret_code = 0;

	//创建相机
	XCamera* p_camera = (XCamera*)createXCamera();

	//连接相机 
	ret_code = p_camera->connect("169.254.5.240");

	int width = 0, height = 0;
	int channels = 1;

	if (0 == ret_code)
	{
		//必须连接相机成功后，才可获取相机分辨率
		ret_code = p_camera->getCameraResolution(&width, &height);
		std::cout << "Width: " << width << "    Height: " << height << std::endl;

		ret_code = p_camera->getCameraChannels(&channels);
		std::cout<< "channels: " << channels << std::endl;
	}
	else
	{
		std::cout << "Connect Camera Error!";
		return -1;
	}

	//分配内存保存采集结果
	float* point_cloud_data = (float*)malloc(sizeof(float) * width * height * 3);
	memset(point_cloud_data, 0, sizeof(float) * width * height * 3);

	float* height_map_data = (float*)malloc(sizeof(float) * width * height);
	memset(height_map_data, 0, sizeof(float) * width * height);

	float* depth_data = (float*)malloc(sizeof(float) * width * height);
	memset(depth_data, 0, sizeof(float) * width * height);

	char* timestamp_data = (char*)malloc(sizeof(char) * 30);
	memset(timestamp_data, 0, sizeof(char) * 30);

	unsigned char* brightness_data = (unsigned char*)malloc(sizeof(unsigned char) * width * height);
	memset(brightness_data, 0, sizeof(unsigned char) * width * height);

	unsigned char* color_brightness_data = (unsigned char*)malloc(sizeof(unsigned char) * width * height * 3);
	memset(color_brightness_data, 0, sizeof(unsigned char) * width * height * 3);

	int capture_num = 0;

	if (0 == ret_code)
	{
		//set patam json
		char status_json[20480];
		char config_json[20480];
		int num = 0;
		p_camera->readJson(config_json, "camera_config.json");
		p_camera->setParamJson(config_json, status_json, num);

		ret_code=p_camera->captureData(num, timestamp_data);

		if (0 == ret_code)
		{
			if (1 == channels)
			{
				//获取亮度图数据并保存
				ret_code = p_camera->getBrightnessData(brightness_data);
				if (0 == ret_code)
				{
					cv::Mat bright=cv::Mat(height, width, CV_8U, brightness_data);
					cv::imwrite("bright.bmp", bright);
		
				}
				//获取深度图数据并保存
				ret_code = p_camera->getDepthData(depth_data);

				if (0 == ret_code)
				{
					cv::Mat depth = cv::Mat(height, width, CV_32F, depth_data);
					cv::imwrite("depth.tiff", depth);
					std::cout << "Get Depth!" << std::endl;
				}

				//获取点云数据并保存（2选1）
				ret_code = p_camera->getPointcloudData(point_cloud_data);
				if (0 == ret_code)
				{
					std::cout << "Get Pointcloud!" << std::endl;
					const char* pcd = "1.pcd";
					const char* ply = "1.ply";
					p_camera->savePointcloudToPcd(point_cloud_data, brightness_data, channels, pcd);
					p_camera->savePointcloudToPly(point_cloud_data, brightness_data, channels, ply);
				}
			}
			else if(3 == channels)
			{
				//获取亮度图数据并保存
				ret_code = p_camera->getColorBrightnessData(color_brightness_data,Color::Rgb);
				if (0 == ret_code)
				{
					cv::Mat bright = cv::Mat(height, width, CV_8UC3, color_brightness_data);
					cv::imwrite("bright.bmp", bright);
					std::cout << "Get color Brightness!" << std::endl;
				}
				//获取深度图数据并保存
				ret_code = p_camera->getDepthData(depth_data);

				if (0 == ret_code)
				{
					cv::Mat depth = cv::Mat(height, width, CV_32F, depth_data);
					cv::imwrite("depth.tiff", depth);
					std::cout << "Get Depth!" << std::endl;
				}

				//获取点云数据并保存（2选1）
				ret_code = p_camera->getPointcloudData(point_cloud_data);
				if (0 == ret_code)
				{
					std::cout << "Get Pointcloud!" << std::endl;
					const char* pcd = "point_cloud.pcd";
					const char* ply = "point_cloud.ply";
					p_camera->savePointcloudToPcd(point_cloud_data, color_brightness_data, channels, pcd);
					p_camera->savePointcloudToPly(point_cloud_data, color_brightness_data, channels, ply);
				}
			}
			capture_num++;
			std::cout << "Capture num: " << capture_num << std::endl;

		}
		else
		{
			std::cout << "Capture Data Error!" << std::endl;
		}

	}

	free(brightness_data);
	free(depth_data);
	free(point_cloud_data);
	free(height_map_data);
	free(timestamp_data); 

	p_camera->disconnect("169.254.5.240");
	 
	destroyXCamera(p_camera);
	  
}


