
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

// �`�F�b�J�[�p�^�[���𐶐�����֐��B
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
	const unsigned int cheker_size = 80;//80pixel�̃`�F�b�J�[�p�^�[���𐶐�
	auto _cheker_width = (_width + 2) * cheker_size; // �`�F�b�J�[�p�^�[���S�̂̉���(pixel)�A[+1]�̗��R�͊i�q�_�̐�+1�`�F�X�{�[�h�����邩��
	auto _cheker_height = (_height + 2) * cheker_size; // �`�F�b�J�[�p�^�[���S�̂̏c��(pixel)
	unsigned int _image_offset_width = (PROJ_WIDTH - _cheker_width) / 2; //�I�t�Z�b�g�̌v�Z
	unsigned int _image_offset_height = (PROJ_HEIGHT - _cheker_height) / 2;  //�I�t�Z�b�g�̌v�Z
	calib_image.create(PROJ_HEIGHT, PROJ_WIDTH, CV_8UC1);
	std::cout << "�I�t�Z�b�g��(x, y) = (" << _image_offset_width << ", " << _image_offset_height << ")" << std::endl;
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
						//�h��Ԃ��̈悪�͈͂𒴂��Ȃ����ǂ����̃`�F�b�N
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
							//�h��Ԃ��̈悪�͈͂𒴂��Ȃ����ǂ����̃`�F�b�N
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
// �`�F�b�J�[�p�^�[���𓊉e����֐�
void create_image_check(std::vector<cv::Mat> &result){
	cv::Mat image1(cv::Size(PROJ_WIDTH, PROJ_HEIGHT), CV_8UC3, cv::Scalar::all(255));
	Calibration_pattern(image1, 7, 5, true);
	result.push_back(image1);
};
// �e�X�g�摜�𓊉e����֐�
void create_image_test(std::vector<cv::Mat> &result) {
	int num_image = 256;
	for (int i = 0; i<num_image; i++) {
		cv::Mat img(cv::Size(PROJ_WIDTH, PROJ_HEIGHT), CV_8UC1, cv::Scalar::all(i));
		result.push_back(img);
	}
}


int main() {

	//�v���W�F�N�^���g��Ȃ��ꍇ�͈ȉ�9�s���R�����g�A�E�g
	
	// �v���W�F�N�^����̐ݒ�
	HighSpeedProjector hsproj;
	// ���e���[�h�̐ݒ�
	hsproj.set_projection_mode(PM::MIRROR);// �~���[���[�hON
	hsproj.set_projection_mode(PM::ILLUMINANCE_HIGH);     // ���Ɠx���[�h�ɕύX( �������e���Ă���ꍇ��ON�ɂ����Ⴞ�� )
	// ���e�p�����^�̐ݒ�
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

	
	// 2��̃J�����p�����[�^��ݒ�
	// 1���
	cv::Mat image1( cv::Size(CAMERA_WIDTH, CAMERA_HEIGHT), CV_8UC1, cv::Scalar( 0, 0, 0 ) );
	basler basler1;
	basler1.connect(0);
	// setParam�֘A�͕K���X�^�[�g�O�ɏI��点�� 
	basler1.setParam( paramTypeBasler::CaptureType::MonocroGrab);
	basler1.setParam( paramType::HEIGHT, CAMERA_HEIGHT );
	basler1.setParam( paramTypeBasler::Param::ExposureTime,1950.0f );
	basler1.setParam( paramTypeBasler::AcquisitionMode::TriggerMode);
	// basler1.setParam( paramType::FPS, 500);
	basler1.setParam( paramTypeBasler::FastMode::SensorReadoutModeFast );
	basler1.setParam( paramTypeBasler::GrabStrategy::OneByOne);
	basler1.parameter_all_print();

	// 2���
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
	
	


	// ���[�U�ւ̓��͂𑣂��i�p�r�ɉ����Đ�������́j
	//std::cout << "csf�@�Ōv������Ƃ���0��" << std::endl;
	//std::cout << "��Ď�@�Ōv���i����g�j����Ƃ���1��" << std::endl;
	//std::cout << "��Ď�@�Ōv���i�����g�j����Ƃ���2��" << std::endl;
	std::cout << "�e�X�g�摜�𓊉e����Ƃ���0��" << std::endl;
	//std::cout << "�ʏ�̐����g�p�^�[���𓊉e����Ƃ���4��" << std::endl;
	std::cout << "�`�F�b�J�[�p�^�[���𓊉e����Ƃ���1��" << std::endl;
	//std::cout << "Micro Phase Shifting�Ōv������Ƃ��͂���ȊO�̐�����" << std::endl;
	std::cout << "���͂��ĉ�����" << std::endl;
	int num;
	std::cin >> num;
	std::vector<cv::Mat> input;
	if (num == 0) {
		// �e�X�g�摜����
		create_image_test(input);
	}
	else if (num == 1) {
		// �`�F�b�J�[�p�^�[������
		create_image_check(input);
	}
	else {
		std::cout << "���͂��s�K�؂ł�" << std::endl;
		return -1;
	}



	//�g�p����ϐ����B���O�ɂ܂Ƃ߂Ē�`
	std::vector<cv::Mat> tmp1;// �J����1�̎B���摜���i�[
	std::vector<cv::Mat> tmp2;// �J����2�̎B���摜���i�[
	bool flag = true;
	int frame1 = 0, frame2 = 0;

	//2�J�����ł̎B���J�n
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

	//�X���b�h�̗����オ��ҋ@
	std::this_thread::sleep_for(std::chrono::seconds(2));
	

	// �v���W�F�N�^�ł̓��e�J�n
	hsproj.connect_projector();
	for (;; ) {
		for (int i = 0; i < input.size(); i++) {
			hsproj.send_image_8bit(input[i].data);

			//�`�F�b�J�[�p�^�[���𓊉e����Ƃ���100fps���炢�ɂ��܂��傤
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

		std::cout << "�摜�����o���J�n" << std::endl;
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

		std::cout << "�摜�����o���J�n" << std::endl;
		if (tmp1[frame1-1].data != NULL && tmp2[frame1-1].data != NULL) {
			sprintf_s(Filename, "%s%03d.%s", _filename1, frame1-1, fileext);
			cv::imwrite(Filename, abs(tmp1[frame1-1] - tmp2[frame1-1]));
		}
	}
	
	cv::destroyAllWindows();
	

	return 0;
}