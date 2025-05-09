using CameraCsharp;
using System;
using System.Runtime.InteropServices;
using System.Text;
using System.IO;

namespace example
{
    class Program
    {

        static void Main()
        {

            //string cameraId = args[1];
            CameraCls camera = new CameraCls();

            string cameraId = "192.168.3.69";
            int ret_code;
            ret_code = camera.DfConnect_Csharp(cameraId);

            int width=0 ;
            int height=0;
            int channels=0 ;
            if (ret_code == 0)
            {

                ret_code = camera.GetCameraResolution_Csharp(out width, out height);
                Console.WriteLine("width:{0}", width);
                Console.WriteLine("height:{0}", height);
                ret_code = camera.DfGetCameraChannels_Csharp(out channels);
                Console.WriteLine("channels:{0}", channels);
            }
            else
            {
                Console.WriteLine("Connect camera Error!!");
            }

            CalibrationParam calibrationParam;
            ret_code = camera.DfGetCalibrationParam_Csharp(out calibrationParam);
            if (ret_code == 0)
            {
                Console.WriteLine("内参：");
                for (int i = 0; i < calibrationParam.intrinsic.Length; i++)
                {
                    Console.WriteLine(calibrationParam.intrinsic[i]);
                }
                Console.WriteLine("外参：");
                for (int i = 0; i < calibrationParam.extrinsic.Length; i++)
                {
                    Console.WriteLine(calibrationParam.extrinsic[i]);
                }
                Console.WriteLine("畸变：");
                for (int i = 0; i < calibrationParam.distortion.Length; i++)
                {
                    Console.WriteLine(calibrationParam.distortion[i]);
                }
            }
            else
            {
                Console.WriteLine("Get Calibration Data Error!!");
            }

            //分配内存保存采集结果


            StringBuilder timestamp = new StringBuilder(30);

            byte[] brightnessArray = new byte[width * height];
            IntPtr brightnessPtr = Marshal.AllocHGlobal(brightnessArray.Length);



            float[] depthArray = new float[width * height];
            IntPtr depthPtr = Marshal.AllocHGlobal(depthArray.Length * sizeof(float));

            float[] point_cloud_Array = new float[width * height * 3];
            IntPtr point_cloud_Ptr = Marshal.AllocHGlobal(depthArray.Length * sizeof(float) * 3);

            if (ret_code == 0)
            {
                ret_code = camera.DfSetParamSmoothing_Csharp(1);
                if (ret_code != 0)
                {
                    Console.WriteLine("Set Camera Smooothing Error!!");
                }
                ret_code = camera.DfSetParamCameraConfidence_Csharp(2);
                if (ret_code != 0)
                {
                    Console.WriteLine("Set Camera Confidence Error!!");
                }
                ret_code = camera.DfSetParamCameraGain_Csharp(0);
                if (ret_code != 0)
                {
                    Console.WriteLine("Set Camera Gain Error!!");
                }
                ret_code = camera.DfSetParamDepthFilter_Csharp(1, 33);
                if (ret_code != 0)
                {
                    Console.WriteLine("Set Camera DepthFilter Error!!");
                }
                ret_code = camera.DfSetParamRadiusFilter_Csharp(1, 2f, 4);
                if (ret_code != 0)
                {
                    Console.WriteLine("Set Camera RadiusFilter Error!!");
                }
            }

            //单曝光模式
            if (false)
            {
                ret_code = camera.DfSetParamLedCurrent_Csharp(1023);
                if (ret_code != 0)
                {
                    Console.WriteLine("Set Camera LedCurrent Error!!");
                }
                ret_code = camera.DfSetParamCameraExposure_Csharp(20000);
                if (ret_code != 0)
                {
                    Console.WriteLine("Set Camera CameraExposure Error!!");
                }
                ret_code = camera.DfCaptureData_Csharp(1, timestamp);
                string timestampString = timestamp.ToString();
                Console.WriteLine("Success！timestamp:" + timestampString);

            }

            else
            {
                if (true)
                {
                    //采集HDR模式
                    int num = 2;
                    int model = 1;
                    int[] exposureParam = { 5000, 20000 };
                    int[] ledParam = { 1023, 1023 };

                    ret_code = camera.DfSetParamMixedHdr_Csharp(num, exposureParam, ledParam);
                    if (ret_code != 0)
                    {
                        Console.WriteLine("Set Camera HDR Error!!");
                    }

                    ret_code = camera.DfSetParamMultipleExposureModel_Csharp(model);
                    if (ret_code != 0)
                    {
                        Console.WriteLine("Set Camera Multiple Exposure Model Error!!");
                    }

                    Engine engine = Engine.Reflect;
                    ret_code = camera.DfSetCaptureEngine_Csharp(engine);
                    if (ret_code != 0)
                    {
                        Console.WriteLine("Set Camera Capture Engine Error!!");
                    }

                    ret_code = camera.DfCaptureData_Csharp(2, timestamp);
                    string timestampString = timestamp.ToString();
                    Console.WriteLine("Success！timestamp:" + timestampString);

                }
                else
                {
                    //采集重复曝光模式
                    ret_code = camera.DfSetParamLedCurrent_Csharp(1023);
                    if (ret_code != 0)
                    {
                        Console.WriteLine("Set Camera LedCurrent Error!!");
                    }
                    ret_code = camera.DfSetParamCameraExposure_Csharp(20000);
                    if (ret_code != 0)
                    {
                        Console.WriteLine("Set Camera CameraExposure Error!!");
                    }

                    Engine engine = Engine.Reflect;

                    ret_code = camera.DfSetParamRepetitionExposureNum_Csharp(5);
                    if (ret_code != 0)
                    {
                        Console.WriteLine("Set Camera Repetition Exposure Num Error!!");
                    }

                    ret_code = camera.DfSetParamMultipleExposureModel_Csharp(2);
                    if (ret_code != 0)
                    {
                        Console.WriteLine("Set Camera Multiple Exposure Model Error!!");
                    }

                    ret_code = camera.DfSetCaptureEngine_Csharp(engine);
                    if (ret_code != 0)
                    {
                        Console.WriteLine("Set Camera Capture Engine Error!!");
                    }

                    ret_code = camera.DfCaptureData_Csharp(5, timestamp);
                    string timestampString = timestamp.ToString();
                    Console.WriteLine("Success！timestamp:" + timestampString);
                }
            }

            if (ret_code == 0)
            {
                //获取亮度图数据
                ret_code = camera.DfGetBrightnessData_Csharp(brightnessPtr);
                if (ret_code == 0)
                {
                    Console.WriteLine("Get Brightness!!");
                    Marshal.Copy(brightnessPtr, brightnessArray, 0, brightnessArray.Length);
                    //for (int i = 0; i < brightnessArray.Length / 10; i++)
                    //{
                    //    Console.WriteLine("brightnessArray[{0}] = {1}", i, brightnessArray[i]);
                    //}
                    Marshal.FreeHGlobal(brightnessPtr);
                }

                //获取深度图数据
                ret_code = camera.DfGetDepthDataFloat_Csharp(depthPtr);
                if (ret_code == 0)
                {
                    Console.WriteLine("Get Depth!!");
                    Marshal.Copy(depthPtr, depthArray, 0, depthArray.Length);
                    //for (int i = 0; i < depthArray.Length / 2; i++)
                    //{
                    //    Console.WriteLine("deptharray[{0}] = {1}", i, depthArray[i]);
                    //}

                    Marshal.FreeHGlobal(depthPtr);
                }

                //获取点云数据
                ret_code = camera.DfGetPointcloudData_Csharp(point_cloud_Ptr);
                if (ret_code == 0)
                {
                    Console.WriteLine("Get PointCloud!!");
                    Marshal.Copy(point_cloud_Ptr, point_cloud_Array, 0, point_cloud_Array.Length);
                    //for (int i = 0; i < point_cloud_Array.Length / 2; i++)
                    //{
                    //    Console.WriteLine("point_cloud_Array[{0}] = {1}", i, point_cloud_Array[i]);
                    //}
                    Marshal.FreeHGlobal(point_cloud_Ptr);
                }
            }
            else
            {
                Console.WriteLine("Capture Data Error!!");
            }

            ret_code = camera.DfDisconnect_Csharp(cameraId);
            if (ret_code == 0)
            {
                Console.WriteLine("Camera Disconnect!!");
                Console.ReadKey();
            }


        }

    }
}