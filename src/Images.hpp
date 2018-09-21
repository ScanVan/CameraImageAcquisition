//============================================================================
// Name        : Images.hpp
// Author      : Marcelo Kaihara
// Version     : 1.0
// Copyright   :
// Description : It has the routines for manimulating the underlying images.
// 				 It has functions to write and load images together with camera configuration
//				 It has functions that displays the images on screen.
//============================================================================

#ifndef IMAGES_HPP_
#define IMAGES_HPP_

#include <vector>
#include <string>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include <sstream>


// Include files to use OpenCV API
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

namespace ScanVan {

class Images {
private:
	std::vector<uint8_t> * p_img;

protected:
	size_t height = 3008;
	size_t width = 3008;
	size_t cameraIdx = 0;	// camera index
	std::string captureTimeStr = {}; // capture time in string format
	double exposureTime = 0;// exposure time
	int64_t gain = 0;		// gain
	double balanceR = 0;	// white balance R
	double balanceG = 0;	// white balance G
	double balanceB = 0;	// white balance B
	int autoExpTime = 0;	// Auto Exposure Time
	int autoGain = 0; 		// Auto Gain
	long int numImages = 0;
	std::string serialNum{ };

public:
	Images();
	Images(char * p);
	Images(size_t h, size_t w);
	Images(size_t h, size_t w, char * p);
	Images(std::string path);
	Images(const Images &img);
	Images(Images &&img);

	void setHeight(size_t h) {height = h;};
	void setWidth(size_t w) {width = w;};
	void setCameraIdx (size_t idx) { cameraIdx = idx; };
	void setCaptureTime (std::string ct) { captureTimeStr = ct; };
	void setExposureTime (double et) { exposureTime = et; };
	void setGain (int64_t g) { gain = g; };
	void setBalanceR (double r) { balanceR = r; };
	void setBalanceG (double g) { balanceG = g; };
	void setBalanceB (double b) { balanceB = b; };
	void setAutoExpTime (int b) { autoExpTime = b; };
	void setAutoGain (int b) { autoGain = b; };
	void setSerialNumber (const std::string &sn) { serialNum = sn; };
	void setImgNumber (long int n) { numImages = n; };

	size_t getHeight() const { return height;};
	size_t getWidth() const { return width;};
	size_t getCameraIdx() const { return cameraIdx; };
	std::string getCaptureTime() const { return captureTimeStr; };
	double getExposureTime() const { return exposureTime; };
	int64_t getGain() const { return gain; };
	double getBalanceR () const { return balanceR; };
	double getBalanceG () const { return balanceG; };
	double getBalanceB () const { return balanceB; };
	std::string getSerialNumber() const { return serialNum; };
	int getAutoExpTime() const { return autoExpTime; };
	int getAutoGain() const { return autoGain; };
	long int getImgNumber () const { return numImages; };
	size_t getImgBufferSize () const { return p_img->size(); };

public:
	void getBuffer (char *p) const;
	void copyBuffer (char *p);

	void loadImage (std::string path);
	void saveImage (std::string path);
	void loadData (std::string path);
	void saveData (std::string path);
	void show () const;
	void show (std::string name) const;

	cv::Mat convertToCvMat ();

protected:
	std::string convertTimeToString (time_t t);
	time_t convertStringToTime (std::string str);

public:
	Images & operator=(const Images &a);
	Images & operator=(Images &&a);

	virtual ~Images();

	friend std::ostream & operator <<(std::ostream & out, const Images &a) {
		out << "[";
		for (int i=0; i < 10; ++i) {
			out << static_cast<int>((*a.p_img)[i]) << " ";
		}
		out << "...]" << std::endl;
		out << "height: " << a.height << std::endl;
		out << "width: " << a.width << std::endl;
		out << "cameraIdx: " << a.cameraIdx << std::endl;
		out << "captureTime: " << a.captureTimeStr << std::endl;
		out << "exposureTime: " << a.exposureTime << std::endl;
		out << "gain: " << a.gain << std::endl;
		out << "balanceR: " << a.balanceR << std::endl;
		out << "balanceG: " << a.balanceG << std::endl;
		out << "balanceB: " << a.balanceB << std::endl;
		out << "autoExpTime: " << a.autoExpTime << std::endl;
		out << "autoGain: " << a.autoGain << std::endl;

		return out;
	}
};

} /* namespace ScanVan */

#endif /* IMAGES_HPP_ */
