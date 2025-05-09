#pragma once

#ifdef _WIN32 
#define DF_SDK_API __declspec(dllexport)

#elif __linux
#define DF_SDK_API 
#endif

//设备基本信息结构体
struct DeviceBaseInfo
{
	//相机内参
	char mac[64];
	//相机外参
	char ip[64];
};

//函数名： DfUpdateDeviceList
//功能： 获取可连接设备数
//输入参数： device_num(设备数)
//输出参数： 无
//返回值： 类型（int）:返回0表示连接成功;返回-1表示连接失败.
DF_SDK_API int DfUpdateDeviceList(int& device_num);

//函数名： DfGetAllDeviceBaseInfo
//功能： 获取设备基本信息
//输入参数： pDeviceInfo(设备信息)、pBufferSize（设备结构体内存尺寸）
//输出参数： 无
//返回值： 类型（int）:返回0表示连接成功;返回-1表示连接失败.
DF_SDK_API int DfGetAllDeviceBaseInfo(DeviceBaseInfo* pDeviceInfo, int* pBufferSize);