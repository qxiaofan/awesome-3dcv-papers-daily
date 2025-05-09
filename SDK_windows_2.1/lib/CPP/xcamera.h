#pragma once
#ifndef __CAMERA_XCAMERA_H__
#define __CAMERA_XCAMERA_H__
#include<vector>
#include<string>

extern "C" {
    namespace CAMERA {

        using namespace std;
#ifdef _WIN32 
#define CAMERA_API __declspec(dllexport)

#elif __linux
#define CAMERA_API 
#endif
		  
  
            //相机标定参数结构体
        typedef struct CalibrationParam
        {
            //相机内参
            float intrinsic[3 * 3];
            //相机外参
            float extrinsic[4 * 4];
            //相机畸变，只用前5个
            float distortion[1 * 12];//<k1,k2,p1,p2,k3,k4,k5,k6,s1,s2,s3,s4>

        }CalibrationParam;

		enum class Engine
		{
			Normal = 0,
			Reflect = 1,
			Black = 2,
		};

		enum class Color
		{
			Rgb = 0,
			Bgr = 1,
			Bayer = 2,
			Gray= 3,
		};

        class XCamera
        {
        public:
            XCamera() = default;
            virtual ~XCamera() = default;
            XCamera(const XCamera&) = delete;
            XCamera& operator=(const XCamera&) = delete;
			 
			//功能： 连接相机
			//输入参数： camera_id（相机ip地址）
			//输出参数： 无
			//返回值： 类型（int）:返回0表示连接成功;返回-1表示连接失败.
			virtual int connect(const char* camera_id) = 0;
			 

			//函数名： DfGetParamJson
			//功能： 获取Json配置文件
			//输入参数：无
			//输出参数： config_json（配置文件字符）、config_size（配置文本输出大小）、
			//status_json（输出设置状态）、status_size（状态文本大小）
			//返回值： 类型（int）:返回0表示设置参数成功;否则失败。
			virtual int getParamJson(char* config_json, char* status_json) = 0;


			//函数名： DfSaveJson
			//功能： 保存Json配置文件
			//输入参数：config_json（配置文件字符）
			//输出参数： 无 
			//返回值： 类型（int）:返回0表示设置参数成功;否则失败。
			virtual int saveJson(const char* config_json, const char* path) = 0;


			//函数名： DfReadJson
			//功能： 读取Json配置文件
			//输入参数：config_json（配置文件字符）、config_size（配置文本大小）、
			//输出参数： 无 
			//返回值： 类型（int）:返回0表示设置参数成功;否则失败。
			virtual int readJson(char* config_json, const char* path) = 0;

			//函数名： DfSetParamJson
			//功能： 设置Json配置文件里的参数
			//输入参数：config_json（输入配置文件字符）、config_size（输入文本出大小）
			//输出参数：out_status_json（输出设置状态）、out_size（输出文本大小）、maxnum(填入采集captureData接口）
			//返回值： 类型（int）:返回0表示设置参数成功;否则失败。
			virtual int setParamJson(char* config_json, char* status_json, int& maxnum) = 0;

			//功能： 获取相机分辨率
			//输入参数： 无
			//输出参数： width(图像宽)、height(图像高)
			//返回值： 类型（int）:返回0表示获取参数成功;返回-1表示获取参数失败.
			virtual  int  getCameraResolution(int* width, int* height) = 0;

			//函数名： DfGetCameraChannels
			//功能： 获取相机图像通道数
			//输入参数： 无
			//输出参数： channels(通道数)
			//返回值： 类型（int）:返回0表示获取参数成功;返回-1表示获取参数失败.
			virtual int  getCameraChannels(int* channels) = 0;

			//函数名： DfSetCaptureEngine
			//功能： 设置采集引擎
			//输入参数：engine
			//输出参数：  
			//返回值： 类型（int）:返回0表示设置参数成功;返回-1表示设置参数失败。
			virtual int setCaptureEngine(Engine engine) = 0;

			//函数名： DfGetCaptureEngine
			//功能： 设置采集引擎
			//输入参数：
			//输出参数：engine
			//返回值： 类型（int）:返回0表示设置参数成功;返回-1表示设置参数失败。
			virtual int getCaptureEngine(Engine& engine) = 0;
			 
			//功能： 采集一帧数据并阻塞至返回状态
			//输入参数： exposure_num（曝光次数）：设置值为1为单曝光，大于1为多曝光模式（具体参数在相机gui中设置）.
			//输出参数： timestamp(时间戳)
			//返回值： 类型（int）:返回0表示获取采集数据成功;返回-1表示采集数据失败.
			virtual  int captureData(int exposure_num, char* timestamp) = 0;
			 
			//功能： 获取深度图
			//输入参数：无
			//输出参数： depth(深度图)
			//返回值： 类型（int）:返回0表示获取数据成功;返回-1表示采集数据失败.
			virtual  int getDepthData(float* depth) = 0;

			//功能： 获取去畸变深度图
			//输入参数：无
			//输出参数： undistort_depth(深度图)
			//返回值： 类型（int）:返回0表示获取数据成功;返回-1表示采集数据失败.
			virtual  int getUndistortDepthData(float* undistort_depth) = 0;
			
			//功能： 获取点云
			//输入参数：无
			//输出参数： point_cloud(点云)
			//返回值： 类型（int）:返回0表示获取数据成功;返回-1表示采集数据失败.
			virtual  int getPointcloudData(float* point_cloud) = 0;
			 
			//功能： 获取亮度图
			//输入参数：无
			//输出参数： brightness(亮度图)
			//返回值： 类型（int）:返回0表示获取数据成功;返回-1表示采集数据失败.
			virtual  int getBrightnessData(unsigned char* brightness)  = 0;

			//功能： 获取去畸变亮度图
			//输入参数：无
			//输出参数： undistort_brightness(亮度图)
			//返回值： 类型（int）:返回0表示获取数据成功;返回-1表示采集数据失败.
			virtual  int getUndistortBrightnessData(unsigned char* undistort_brightness) = 0;

			//函数名： getColorBrightnessData
			//功能： 获取亮度图
			//输入参数：无
			//输出参数： brightness(亮度图),color(亮度图颜色类型)
			//返回值： 类型（int）:返回0表示获取数据成功;返回-1表示采集数据失败.
			virtual int getColorBrightnessData(unsigned char* brightness, Color color) = 0;

			//函数名： getUndistortColorBrightnessData
			//功能： 获取去畸变后的彩色亮度图
			//输入参数：无
			//输出参数： brightness(亮度图)
			//返回值： 类型（int）:返回0表示获取数据成功;返回-1表示采集数据失败.
			virtual int getUndistortColorBrightnessData(unsigned char* brightness, Color color) = 0;
			 
			//功能： 获取校正到基准平面的高度映射图
			//输入参数：无  
			//输出参数： height_map(高度映射图)
			//返回值： 类型（int）:返回0表示获取数据成功;返回-1表示采集数据失败.
			virtual  int getHeightMapData(float* height_map)  = 0;
			 
			//功能： 获取基准平面参数
			//输入参数：无
			//输出参数： R(旋转矩阵：3*3)、T(平移矩阵：3*1)
			//返回值： 类型（int）:返回0表示获取数据成功;返回-1表示采集数据失败.
			virtual  int getStandardPlaneParam(float* R, float* T)  = 0;
			 
			//功能： 获取校正到基准平面的高度映射图
			//输入参数：R(旋转矩阵)、T(平移矩阵)
			//输出参数： height_map(高度映射图)
			//返回值： 类型（int）:返回0表示获取数据成功;返回-1表示采集数据失败.
			virtual  int getHeightMapDataBaseParam(float* R, float* T, float* height_map)  = 0;
			  
			//功能： 断开相机连接
			//输入参数： camera_id（相机ip地址）
			//输出参数： 无
			//返回值： 类型（int）:返回0表示断开成功;返回-1表示断开失败.
			virtual  int disconnect(const char* camera_id) = 0;
			 
			//功能： 获取相机标定参数
			//输入参数： 无
			//输出参数： calibration_param（相机标定参数结构体）
			//返回值： 类型（int）:返回0表示获取标定参数成功;返回-1表示获取标定参数失败.
			virtual  int getCalibrationParam(struct CalibrationParam* calibration_param) = 0;


			/***************************************************************************************************************************************************************/
			//参数设置
			 
			//功能： 设置LED电流
			//输入参数： led（电流值）
			//输出参数： 无
			//返回值： 类型（int）:返回0表示设置参数成功;否则失败。
			virtual  int setParamLedCurrent(int led) = 0;
			 
			//功能： 设置LED电流
			//输入参数： 无
			//输出参数： led（电流值）
			//返回值： 类型（int）:返回0表示获取参数成功;否则失败。
			virtual  int getParamLedCurrent(int& led) = 0;
			  
			 
			//功能： 设置基准平面的外参
			//输入参数：R(旋转矩阵：3*3)、T(平移矩阵：3*1)
			//输出参数： 无
			//返回值： 类型（int）:返回0表示设置参数成功;否则失败。
			virtual  int setParamStandardPlaneExternal(float* R, float* T) = 0;
			 
			//功能： 获取基准平面的外参
			//输入参数：无
			//输出参数： R(旋转矩阵：3*3)、T(平移矩阵：3*1)
			//返回值： 类型（int）:返回0表示获取参数成功;否则失败。
			virtual  int getParamStandardPlaneExternal(float* R, float* T) = 0;
			 
			//功能： 设置生成亮度图参数
			//输入参数：model(1:与条纹图同步连续曝光、2：单独发光曝光、3：不发光单独曝光)、exposure(亮度图曝光时间)
			//输出参数： 无
			//返回值： 类型（int）:返回0表示设置参数成功;否则失败。
			virtual  int setParamGenerateBrightness(int model, float exposure) = 0;
			 
			//功能： 获取生成亮度图参数
			//输入参数： 无
			//输出参数：model(1:与条纹图同步连续曝光、2：单独发光曝光、3：不发光单独曝光)、exposure(亮度图曝光时间)
			//返回值： 类型（int）:返回0表示获取参数成功;否则失败。
			virtual  int getParamGenerateBrightness(int& model, float& exposure) = 0;
			 
			//功能： 设置相机曝光时间
			//输入参数：exposure(相机曝光时间)
			//输出参数： 无
			//返回值： 类型（int）:返回0表示设置参数成功;否则失败。
			virtual  int setParamCameraExposure(float exposure) = 0;
			 
			//功能： 获取相机曝光时间
			//输入参数： 无
			//输出参数：exposure(相机曝光时间)
			//返回值： 类型（int）:返回0表示获取参数成功;否则失败。
			virtual  int getParamCameraExposure(float& exposure) = 0;
			 
			//功能： 设置混合多曝光参数（最大曝光次数为6次）
			//输入参数： num（曝光次数）、exposure_param[6]（6个曝光参数、前num个有效）、led_param[6]（6个led亮度参数、前num个有效）
			//输出参数： 无
			//返回值： 类型（int）:返回0表示设置参数成功;否则失败。
			virtual  int setParamMixedHdr(int num, int exposure_param[6], int led_param[6]) = 0;
			 
			//功能： 获取混合多曝光参数（最大曝光次数为6次）
			//输入参数： 无
			//输出参数： num（曝光次数）、exposure_param[6]（6个曝光参数、前num个有效）、led_param[6]（6个led亮度参数、前num个有效）
			//返回值： 类型（int）:返回0表示获取参数成功;否则失败。
			virtual  int getParamMixedHdr(int& num, int exposure_param[6], int led_param[6]) = 0;
			 
			//功能： 设置相机曝光时间
			//输入参数：confidence(相机置信度)
			//输出参数： 无
			//返回值： 类型（int）:返回0表示设置参数成功;否则失败。
			virtual  int setParamCameraConfidence(float confidence) = 0;
			 
			//功能： 获取相机曝光时间
			//输入参数： 无
			//输出参数：confidence(相机置信度)
			//返回值： 类型（int）:返回0表示获取参数成功;否则失败。
			virtual  int getParamCameraConfidence(float& confidence) = 0;
			 
			//功能： 设置相机增益
			//输入参数：gain(相机增益)
			//输出参数： 无
			//返回值： 类型（int）:返回0表示设置参数成功;否则失败。
			virtual  int setParamCameraGain(float gain) = 0;
			 
			//功能： 获取相机增益
			//输入参数： 无
			//输出参数：gain(相机增益)
			//返回值： 类型（int）:返回0表示获取参数成功;否则失败。
			virtual  int getParamCameraGain(float& gain) = 0;
			 
			//功能： 设置点云平滑参数
			//输入参数：smoothing(0:关、1-5:平滑程度由低到高)
			//输出参数： 无
			//返回值： 类型（int）:返回0表示设置参数成功;否则失败。
			virtual  int setParamSmoothing(int smoothing) = 0;
			 
			//功能： 设置点云平滑参数
			//输入参数：无
			//输出参数：smoothing(0:关、1-5:平滑程度由低到高)
			//返回值： 类型（int）:返回0表示获取参数成功;否则失败。
			virtual  int getParamSmoothing(int& smoothing) = 0;
			 
			//功能： 设置点云半径滤波参数
			//输入参数：use(开关：1开、0关)、radius(半径）、num（有效点）
			//输出参数： 无
			//返回值： 类型（int）:返回0表示设置参数成功;否则失败。
			virtual  int setParamRadiusFilter(int use, float radius, int num) = 0;
			 
			//功能： 获取点云半径滤波参数
			//输入参数：无
			//输出参数：use(开关：1开、0关)、radius(半径）、num（有效点）
			//返回值： 类型（int）:返回0表示获取参数成功;否则失败。
			virtual  int getParamRadiusFilter(int& use, float& radius, int& num) = 0;

			//函数名： setParamDepthFilter
			//功能： 设置深度图滤波参数
			//输入参数：use(开关：1开、0关)、depth_filterthreshold(深度图在1000mm距离过滤的噪声阈值)
			//输出参数： 无
			//返回值： 类型（int）:返回0表示设置参数成功;否则失败。
			virtual int setParamDepthFilter(int use, float depth_filter_threshold) = 0;

			//函数名： getParamDepthFilter
			//功能： 设置深度图滤波参数
			//输入参数：use(开关：1开、0关)、depth_filterthreshold(深度图在1000mm距离过滤的噪声阈值)
			//输出参数： 无
			//返回值： 类型（int）:返回0表示获取参数成功;否则失败。
			virtual int getParamDepthFilter(int& use, float& depth_filter_threshold) = 0;
			 
			//功能： 设置外点过滤阈值
			//输入参数：threshold(阈值0-100)
			//输出参数： 无
			//返回值： 类型（int）:返回0表示设置参数成功;否则失败。
			virtual  int setParamOutlierFilter(float threshold) = 0;
			 
			//功能： 获取外点过滤阈值
			//输入参数： 无
			//输出参数：threshold(阈值0-100)
			//返回值： 类型（int）:返回0表示获取参数成功;否则失败。
			virtual  int getParamOutlierFilter(float& threshold) = 0;
			 
			//功能： 设置多曝光模式
			//输入参数： model(1：HDR(默认值)、2：重复曝光)
			//输出参数：无
			//返回值： 类型（int）:返回0表示设置参数成功;否则失败。
			virtual  int setParamMultipleExposureModel(int model) = 0;
			 
			//功能： 设置重复曝光数
			//输入参数： num(2-10)
			//输出参数：无
			//返回值： 类型（int）:返回0表示设置参数成功;否则失败。
			virtual  int setParamRepetitionExposureNum(int num) = 0;

			//函数名： setParamGrayRectify
			//功能： 设置点云灰度补偿参数
			//输入参数：use(开关：1开、0关)、radius(半径：3、5、7、9）、sigma（补偿强度，范围0-100）
			//输出参数： 无
			//返回值： 类型（int）:返回0表示设置参数成功;否则失败。
			virtual int setParamGrayRectify(int use, int radius, float sigma) = 0;

			//函数名： getParamGrayRectify
			//功能： 获取点云灰度补偿参数
			//输入参数：无
			//输出参数：use(开关：1开、0关)、radius(半径：3、5、7、9）、sigma（补偿强度，范围0-100）
			//返回值： 类型（int）:返回0表示获取参数成功;否则失败。
			virtual int getParamGrayRectify(int& use, int& radius, float& sigma) = 0;

			//函数名： setParamBrightnessHdrExposure
			//功能： 设置亮度图多曝光参数（最大曝光次数为10次）
			//输入参数： num（曝光次数）、exposure_param[6]（6个曝光参数、前num个有效））
			//输出参数： 无
			//返回值： 类型（int）:返回0表示设置参数成功;否则失败。
			virtual int setParamBrightnessHdrExposure(int num, int exposure_param[10]) = 0;

			//函数名：getParamBrightnessHdrExposure
			//功能： 设置亮度图多曝光参数（最大曝光次数为10次）
			//输入参数：无 
			//输出参数：num（曝光次数）、exposure_param[10]（10个曝光参数、前num个有效））
			//返回值： 类型（int）:返回0表示设置参数成功;否则失败。
			virtual int getParamBrightnessHdrExposure(int& num, int exposure_param[10]) = 0;

			//函数名： setParamBrightnessExposureModel
			//功能： 设置亮度图曝光模式
			//输入参数： model（1：单曝光、2：曝光融合）
			//输出参数： 无
			//返回值： 类型（int）:返回0表示设置参数成功;否则失败。
			virtual int setParamBrightnessExposureModel(int model) = 0;

			//函数名： getParamBrightnessExposureModel
			//功能： 获取亮度图曝光模式
			//输入参数： 无
			//输出参数： model（1：单曝光、2：曝光融合）
			//返回值： 类型（int）:返回0表示设置参数成功;否则失败。
			virtual int getParamBrightnessExposureModel(int& model) = 0;

			//函数名： setParamBrightnessGain
			//功能： 设置亮度图增益
			//输入参数：gain(亮度图增益)
			//输出参数： 无
			//返回值： 类型（int）:返回0表示设置参数成功;否则失败。
			virtual int setParamBrightnessGain(float gain) = 0;

			//函数名： getParamBrightnessGain
			//功能： 获取亮度图增益
			//输入参数：无
			//输出参数：gain(亮度图增益)
			//返回值： 类型（int）:返回0表示设置参数成功;否则失败。
			virtual int getParamBrightnessGain(float& gain) = 0;

			//功能： 获取多曝光模式
			//输入参数： model(1：HDR(默认值)、2：重复曝光)
			//输出参数：无
			//返回值： 类型（int）:返回0表示设置参数成功;否则失败。
			virtual  int getParamMultipleExposureModel(int& model) = 0;

			//功能： 获取重复曝光数
			//输入参数： num(2-10)
			//输出参数：无
			//返回值： 类型（int）:返回0表示设置参数成功;否则失败。
			virtual  int getParamRepetitionExposureNum(int& num) = 0;

			//函数名： getSdkVersion
			//功能： 获取sdk版本
			//输入参数：无
			//输出参数：version(版本)
			//返回值： 类型（int）:返回0表示设置参数成功;否则失败。
			virtual int getSdkVersion(char version[64]) = 0;

			//函数名： getFirmwareVersion
			//功能： 获取固件版本
			//输入参数：无
			//输出参数：version(版本)
			//返回值： 类型（int）:返回0表示设置参数成功;否则失败。
			virtual int getFirmwareVersion(char version[64]) = 0;

			//函数名： DfCaptureBrightnessData
			//功能： 获取亮度图
			//输入参数： color(图像颜色类型)
			//输出参数： brightness(亮度图)
			//返回值： 类型（int）:返回0表示获取数据成功;返回-1表示采集数据失败.
			virtual int captureBrightnessData(unsigned char* brightness, Color color) = 0;

			//函数名： setParamReflectFilter
			//功能： 设置亮度图增益
			//输入参数：use(开关：1开、0关)、param_b（过滤系数：范围0-100）
			//输出参数： 无
			//返回值： 类型（int）:返回0表示设置参数成功;否则失败。
			virtual int setParamReflectFilter(int use, float param_b) = 0;

			//函数名： getParamReflectFilter
			//功能： 获取亮度图增益
			//输入参数：无
			//输出参数：use(开关：1开、0关)、param_b（过滤系数：范围0-100）
			//返回值： 类型（int）:返回0表示设置参数成功;否则失败。
			virtual int getParamReflectFilter(int& use, float& param_b) = 0;

			//函数名： savePointcloudToPcd
			//功能： 保存pcd点云
			//输入参数：pointcloud(点云)、brightness（亮度图）、channels（亮度图通道数）、path(路径)
			//输出参数：gain(亮度图增益)
			//返回值： 类型（int）:返回0表示设置参数成功;否则失败。
			virtual int savePointcloudToPcd(float* pointcloud, unsigned char* brightness, int channels, const char* path) = 0;

			//函数名： savePointcloudToPly
			//功能： 保存ply点云
			//输入参数：pointcloud(点云)、brightness（亮度图）、channels（亮度图通道数）、path(路径)
			//输出参数：gain(亮度图增益)
			//返回值： 类型（int）:返回0表示设置参数成功;否则失败。
			virtual int savePointcloudToPly(float* pointcloud, unsigned char* brightness, int channels, const char* path) = 0;
        };

        CAMERA_API void* createXCamera();
        CAMERA_API void destroyXCamera(void*);

    }
}
#endif