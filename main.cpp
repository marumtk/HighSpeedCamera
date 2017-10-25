
#include "HSC/CameraUI.hpp"
#ifdef _DEBUG
	#pragma comment(lib, "opencv_world310d.lib")
#else
	#pragma comment(lib, "opencv_world310.lib")
#endif
#include "baslerClass.hpp"
#include <opencv2/opencv.hpp>
#include <conio.h>
#include <thread>
#include <Windows.h>
#include <iostream>
#include <string>
#include <math.h>
#include "HighSpeedProjector.h"
#include <vector>
#include <chrono>

#define PROJ_HEIGHT 768
#define PROJ_WIDTH 1024
#define CAMERA_HEIGHT 480
#define CAMERA_WIDTH 640

// チェッカーパターンを生成する関数達
void Square_paint(cv::Mat &paint_image, const int square_size, const cv::Point offset_point, bool flag)
{
	if (flag == true)
	{
		for (int y = 0; y<square_size; y++)
		{
			for (int x = 0; x<square_size; x++)
			{
				paint_image.at<unsigned char>(offset_point.y + y, offset_point.x + x) = 255;
			}
		}
	}
	else
	{
		for (int y = 0; y<square_size; y++)
		{
			for (int x = 0; x<square_size; x++)
			{
				paint_image.at<unsigned char>(offset_point.y + y, offset_point.x + x) = 0;
			}
		}
	}
};
void Calibration_pattern(cv::Mat &calib_image, const unsigned int cheker_width, const unsigned int cheker_height, bool color_flag)
{
	auto _width = cheker_width - 1;
	auto _height = cheker_height - 1;
	const unsigned int cheker_size = 80;//80pixelのチェッカーパターンを生成
	auto _cheker_width = (_width + 2) * cheker_size; // チェッカーパターン全体の横幅(pixel)、[+1]の理由は格子点の数+1チェスボードがあるから
	auto _cheker_height = (_height + 2) * cheker_size; // チェッカーパターン全体の縦幅(pixel)
	unsigned int _image_offset_width = (PROJ_WIDTH - _cheker_width) / 2; //オフセットの計算
	unsigned int _image_offset_height = (PROJ_HEIGHT - _cheker_height) / 2;  //オフセットの計算
	calib_image.create(PROJ_HEIGHT, PROJ_WIDTH, CV_8UC1);
	std::cout << "オフセットは(x, y) = (" << _image_offset_width << ", " << _image_offset_height << ")" << std::endl;
	if (color_flag == true)
	{
		for (int y = 0; y < _height + 2; y++)
		{
			for (int x = 0; x < _width + 2; x++)
			{
				if ((y % 2) == 0)
				{
					if ((x % 2) == 1)
					{
						//塗りつぶす領域が範囲を超えないかどうかのチェック
						if (((x + 1) * cheker_size + _image_offset_width) < PROJ_WIDTH && (y + 1) * cheker_size + _image_offset_height < PROJ_HEIGHT)
						{
							Square_paint(calib_image, cheker_size, cv::Point(x * cheker_size + _image_offset_width, y * cheker_size + _image_offset_height), true);
						}
					}
					else
					{
						if (((x + 1) * cheker_size + _image_offset_width) < PROJ_WIDTH && (y + 1) * cheker_size + _image_offset_height < PROJ_HEIGHT)
						{
							Square_paint(calib_image, cheker_size, cv::Point(x * cheker_size + _image_offset_width, y * cheker_size + _image_offset_height), false);
						}
					}
				}
				else
				{
					if ((x % 2) == 1)
					{
						if (((x + 1) * cheker_size + _image_offset_width) < PROJ_WIDTH && (y + 1) * cheker_size + _image_offset_height < PROJ_HEIGHT)
						{
							Square_paint(calib_image, cheker_size, cv::Point(x * cheker_size + _image_offset_width, y * cheker_size + _image_offset_height), false);
						}
					}
					else
					{
						if (((x + 1) * cheker_size + _image_offset_width) < PROJ_WIDTH && (y + 1) * cheker_size + _image_offset_height < PROJ_HEIGHT)
						{
							Square_paint(calib_image, cheker_size, cv::Point(x * cheker_size + _image_offset_width, y * cheker_size + _image_offset_height), true);
						}
					}
				}
			}
		}

	}
	else
	{
		{
			for (int y = 0; y < _height + 2; y++)
			{
				for (int x = 0; x < _width + 2; x++)
				{
					if ((y % 2) == 0)
					{
						if ((x % 2) == 1)
						{
							//塗りつぶす領域が範囲を超えないかどうかのチェック
							if (((x + 1) * cheker_size + _image_offset_width) < PROJ_WIDTH && (y + 1) * cheker_size + _image_offset_height < PROJ_HEIGHT)
							{
								Square_paint(calib_image, cheker_size, cv::Point(x * cheker_size + _image_offset_width, y * cheker_size + _image_offset_height), false);
							}
						}
						else
						{
							if (((x + 1) * cheker_size + _image_offset_width) < PROJ_WIDTH && (y + 1) * cheker_size + _image_offset_height < PROJ_HEIGHT)
							{
								Square_paint(calib_image, cheker_size, cv::Point(x * cheker_size + _image_offset_width, y * cheker_size + _image_offset_height), true);
							}
						}
					}
					else
					{
						if ((x % 2) == 1)
						{
							if (((x + 1) * cheker_size + _image_offset_width) < PROJ_WIDTH && (y + 1) * cheker_size + _image_offset_height < PROJ_HEIGHT)
							{
								Square_paint(calib_image, cheker_size, cv::Point(x * cheker_size + _image_offset_width, y * cheker_size + _image_offset_height), true);
							}
						}
						else
						{
							if (((x + 1) * cheker_size + _image_offset_width) < PROJ_WIDTH && (y + 1) * cheker_size + _image_offset_height < PROJ_HEIGHT)
							{
								Square_paint(calib_image, cheker_size, cv::Point(x * cheker_size + _image_offset_width, y * cheker_size + _image_offset_height), false);
							}
						}
					}
				}
			}

		}
	}
};
// チェッカーパターンを投影する関数
void create_image_check(std::vector<cv::Mat> &result){
	cv::Mat image1(cv::Size(PROJ_WIDTH, PROJ_HEIGHT), CV_8UC3, cv::Scalar::all(255));
	Calibration_pattern(image1, 7, 5, true);
	result.push_back(image1);
};
// テスト画像を投影する関数
void create_image_test(std::vector<cv::Mat> &result) {
	int num_image = 256;
	for (int i = 0; i<num_image; i++) {
		cv::Mat img(cv::Size(PROJ_WIDTH, PROJ_HEIGHT), CV_8UC1, cv::Scalar::all(i));
		result.push_back(img);
	}
}


int main() {

	//プロジェクタを使わない場合は以下9行をコメントアウト
	
	// プロジェクタ周りの設定
	HighSpeedProjector hsproj;
	// 投影モードの設定
	hsproj.set_projection_mode(PM::MIRROR);// ミラーモードON
	hsproj.set_projection_mode(PM::ILLUMINANCE_HIGH);     // 高照度モードに変更( 長く投影している場合はONにしちゃだめ )
	// 投影パラメタの設定
	hsproj.set_parameter_value(PP::FRAME_RATE, 100);
	hsproj.set_parameter_value(PP::BUFFER_MAX_FRAME, 0);
	

/*
	CameraUI cui;
	CameraUI cui2;

	int width = 512;
	int height = 512;
	int width2 = 10;
	int height2 = 20;
	float  fps = 50;
	float gain = 7.4;

#ifdef USE_IDPEXPRESS
	cui.connect(CameraUI::cameraType::IDPEXPRESS);
	cui.setParam(Camera::paramType::WIDTH, width);
	cui.setParam(Camera::paramType::HEIGHT, height);
	cui.setParam(Camera::paramType::FPS, fps);
	cui.setParam(Camera::paramType::GAIN, gain);
	cui.start();
	cui.getParam(Camera::paramType::WIDTH, width);
	cui.getParam(Camera::paramType::HEIGHT, height);
	std::cout << "WOW :" << width << " , " << height << std::endl;
	cv::Mat img(height, width, CV_8UC1, cv::Scalar::all(255));
	cv::Mat img2(height, width, CV_8UC1, cv::Scalar::all(255));
#endif

#ifdef USE_XIMEA
	cui2.connect(CameraUI::cameraType::XIMEA);
	cui2.setParam(Camera::paramType::WIDTH, 648);
	cui2.setParam(Camera::paramType::HEIGHT, 488);
	cui2.setParam(Camera::paramType::FPS, fps);
	cui2.setParam(Camera::paramType::GAIN, gain);
	cui2.start();
	cui2.getParam(Camera::paramType::WIDTH, width2);
	cui2.getParam(Camera::paramType::HEIGHT, height2);
	std::cout << "WOW :" << width2<< " , " << height2 << std::endl;
	cv::Mat img3(height2, width2, CV_8UC1, cv::Scalar::all(255));
#endif

	while (1) {
#ifdef USE_IDPEXPRESS
		//cui.captureFrameStereo((img.data), (img2.data));
		cui.captureFrame((img.data));
		cv::imshow("testL", img);
		//cv::imshow("testR", img2);
#endif
#ifdef USE_XIMEA
		cui2.captureFrame((img3.data));
		cv::imshow("testX", img3);
#endif
		if (cv::waitKey(1) == 'q')break;
	}
#ifdef USE_IDPEXPRESS
	cui.disconnect();
#endif
	return 1;
*/

	
	// 2台のカメラパラメータを設定
	// 1台目
	cv::Mat image1( cv::Size(CAMERA_WIDTH, CAMERA_HEIGHT), CV_8UC1, cv::Scalar( 0, 0, 0 ) );
	basler basler1;
	basler1.connect(0);
	// setParam関連は必ずスタート前に終わらせる 
	basler1.setParam( paramTypeBasler::CaptureType::MonocroGrab);
	basler1.setParam( paramType::HEIGHT, CAMERA_HEIGHT );
	basler1.setParam( paramTypeBasler::Param::ExposureTime,1950.0f );
	basler1.setParam( paramTypeBasler::AcquisitionMode::TriggerMode);
	// basler1.setParam( paramType::FPS, 500);
	basler1.setParam( paramTypeBasler::FastMode::SensorReadoutModeFast );
	basler1.setParam( paramTypeBasler::GrabStrategy::OneByOne);
	basler1.parameter_all_print();

	// 2台目
	cv::Mat image2( cv::Size(CAMERA_WIDTH, CAMERA_HEIGHT), CV_8UC1, cv::Scalar( 0, 0, 0 ) );
	basler basler2;
	basler2.connect(1);
	basler2.setParam( paramTypeBasler::CaptureType::MonocroGrab );
	basler2.setParam( paramType::WIDTH, CAMERA_WIDTH );
	basler2.setParam( paramTypeBasler::AcquisitionMode::TriggerMode );
	// basler2.setParam( paramType::FPS, 500);
	basler2.setParam( paramTypeBasler::FastMode::SensorReadoutModeFast );
	basler2.setParam( paramTypeBasler::Param::ExposureTime, 1950.0f );
	basler2.setParam( paramTypeBasler::GrabStrategy::OneByOne);
	basler2.parameter_all_print();
	
	


	// ユーザへの入力を促す（用途に応じて数字を入力）
	//std::cout << "csf法で計測するときは0を" << std::endl;
	//std::cout << "提案手法で計測（低周波）するときは1を" << std::endl;
	//std::cout << "提案手法で計測（高周波）するときは2を" << std::endl;
	std::cout << "テスト画像を投影するときは0を" << std::endl;
	//std::cout << "通常の正弦波パターンを投影するときは4を" << std::endl;
	std::cout << "チェッカーパターンを投影するときは1を" << std::endl;
	//std::cout << "Micro Phase Shiftingで計測するときはそれ以外の整数を" << std::endl;
	std::cout << "入力して下さい" << std::endl;
	int num;
	std::cin >> num;
	std::vector<cv::Mat> input;
	if (num == 0) {
		// テスト画像生成
		create_image_test(input);
	}
	else if (num == 1) {
		// チェッカーパターン生成
		create_image_check(input);
	}
	else {
		std::cout << "入力が不適切です" << std::endl;
		return -1;
	}



	//使用する変数を撮像前にまとめて定義
	std::vector<cv::Mat> tmp1;// カメラ1の撮像画像を格納
	std::vector<cv::Mat> tmp2;// カメラ2の撮像画像を格納
	bool flag = true;
	int frame1 = 0, frame2 = 0;

	//2カメラでの撮像開始
	std::thread thr1(
		[&]()
	{
		basler1.start();
		while (flag)
		{
			basler1.captureFrame(image1.data);
			tmp1.push_back( image1.clone() );
			//if (frame != 0) tmp1.at(frame1) = image1.clone();
			//std::cout << tmp1.size() << std::endl;
			frame1++;
		}
	}
	);

	
	std::thread thr2(
		[&]()
	{
		basler2.start();
		while (flag)
		{
			basler2.captureFrame(image2.data);
			tmp2.push_back( image2.clone() );
			//std::cout << tmp2.size() << std::endl;
			//if (frame != 0) tmp2.at(frame2) = image2.clone();
			frame2++;
		}
	}
	);

	//スレッドの立ち上がり待機
	std::this_thread::sleep_for(std::chrono::seconds(2));
	

	// プロジェクタでの投影開始
	hsproj.connect_projector();
	for (;; ) {
		for (int i = 0; i < input.size(); i++) {
			hsproj.send_image_8bit(input[i].data);

			//チェッカーパターンを投影するときは100fpsくらいにしましょう
			if ( num == 1 && frame1<=frame2 ) {
				if (tmp1[frame1-1].data != NULL || tmp2[frame1-1].data != NULL ) {
					cv::imshow("diff", abs(abs(tmp1[frame1-1]) - abs(tmp2[frame1-1])));
					cvWaitKey(1);
				}
			}
		}
		if ( _kbhit()== true )  break;
	}
	

	flag = false;
	thr1.join();
	thr2.join();
		

	
	basler1.stop();
	basler1.disconnect();
	basler2.stop();
	basler2.disconnect();
	
	
	if (num == 0) {
		char Filename[100];
		char _filename1[] = "data/cam1/";
		char _filename2[] = "data/cam2/";
		char fileext[] = "bmp";

		std::cout << "画像書き出し開始" << std::endl;
		int output_size = 500;
		int start_size = 1000;
		for (int i = 0; i < output_size; i++)
		{
			if (tmp1[i + start_size].data != NULL && tmp2[i + start_size].data != NULL) {
				sprintf_s(Filename, "%s%03d.%s", _filename1, i, fileext);
				cv::imwrite(Filename, tmp1[i + start_size]);
				sprintf_s(Filename, "%s%03d.%s", _filename2, i, fileext);
				cv::imwrite(Filename, tmp2[i + start_size]);
			}
			else
				std::cout << "empty image" << std::endl;
		}
	}

	else if (num == 1) {
		if (frame1 < 1) {
			return -1;
		}
		char Filename[100];
		char _filename1[] = "check/";
		char fileext[] = "bmp";

		std::cout << "画像書き出し開始" << std::endl;
		if (tmp1[frame1-1].data != NULL && tmp2[frame1-1].data != NULL) {
			sprintf_s(Filename, "%s%03d.%s", _filename1, frame1-1, fileext);
			cv::imwrite(Filename, abs(tmp1[frame1-1] - tmp2[frame1-1]));
		}
	}
	
	cv::destroyAllWindows();
	

	return 0;
}