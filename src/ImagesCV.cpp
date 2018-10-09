#include "ImagesCV.hpp"

namespace ScanVan {

ImagesCV::ImagesCV(): Images() {
	p_openCvImage = new cv::Mat {};
}

ImagesCV::ImagesCV(ImagesRaw &img): Images{} {

	cv::Mat openCvImageRG8 = cv::Mat(height, width, CV_8UC1, img.getBufferP());

	p_openCvImage = new cv::Mat{};

	cv::cvtColor(openCvImageRG8, *p_openCvImage, cv::COLOR_BayerRG2RGB);

	height = img.getHeight();
	width = img.getWidth();
	cameraIdx = img.getCameraIdx();	// camera index
	captureTimeCPUStr = img.getCaptureCPUTime(); // capture time taken on the CPU in string format
	captureTimeCamStr = img.getCaptureCamTime(); // trigger time retrieved from the camera, number of ticks, string format
	exposureTime = img.getExposureTime();// exposure time
	gain = img.getGain();		// gain
	balanceR = img.getBalanceR();	// white balance R
	balanceG = img.getBalanceG();	// white balance G
	balanceB = img.getBalanceB();	// white balance B
	autoExpTime = img.getAutoExpTime();	// Auto Exposure Time
	autoGain = img.getAutoGain(); 		// Auto Gain
	numImages = img.getImgNumber();
	serialNum = img.getSerialNumber();
}

ImagesCV::ImagesCV(ImagesCV &img): Images{} {

	p_openCvImage = new cv::Mat{*(img.p_openCvImage)};

	//openCvImage = img.openCvImage.clone();

	height = img.getHeight();
	width = img.getWidth();
	cameraIdx = img.getCameraIdx();	// camera index
	captureTimeCPUStr = img.getCaptureCPUTime(); // capture time taken on the CPU in string format
	captureTimeCamStr = img.getCaptureCamTime(); // trigger time retrieved from the camera, number of ticks, string format
	exposureTime = img.getExposureTime();// exposure time
	gain = img.getGain();		// gain
	balanceR = img.getBalanceR();	// white balance R
	balanceG = img.getBalanceG();	// white balance G
	balanceB = img.getBalanceB();	// white balance B
	autoExpTime = img.getAutoExpTime();	// Auto Exposure Time
	autoGain = img.getAutoGain(); 		// Auto Gain
	numImages = img.getImgNumber();
	serialNum = img.getSerialNumber();
}

ImagesCV::ImagesCV(ImagesCV &&img): Images{} {

	p_openCvImage = img.p_openCvImage;
	img.p_openCvImage = nullptr;

	height = img.getHeight();
	width = img.getWidth();
	cameraIdx = img.getCameraIdx();	// camera index
	captureTimeCPUStr = img.getCaptureCPUTime(); // capture time taken on the CPU in string format
	captureTimeCamStr = img.getCaptureCamTime(); // trigger time retrieved from the camera, number of ticks, string format
	exposureTime = img.getExposureTime();// exposure time
	gain = img.getGain();		// gain
	balanceR = img.getBalanceR();	// white balance R
	balanceG = img.getBalanceG();	// white balance G
	balanceB = img.getBalanceB();	// white balance B
	autoExpTime = img.getAutoExpTime();	// Auto Exposure Time
	autoGain = img.getAutoGain(); 		// Auto Gain
	numImages = img.getImgNumber();
	serialNum = img.getSerialNumber();
}



void ImagesCV::show () const {
	/// Display
	cv::namedWindow("Image", cv::WINDOW_NORMAL);
	cv::imshow("Image", *p_openCvImage);
};

void ImagesCV::show(std::string name) const {
	/// Display
	cv::namedWindow(name, cv::WINDOW_NORMAL);
	cv::imshow(name, *p_openCvImage);
};

void ImagesCV::showConcat (std::string name, Images &img2) const {

	cv::Mat m;
	try {
		cv::hconcat(*p_openCvImage, *(dynamic_cast<ImagesCV &>(img2).p_openCvImage), m);
	} catch (...) {
		m = *p_openCvImage;
	}

	/// Display
	cv::namedWindow(name, cv::WINDOW_NORMAL);
	cv::imshow(name, m);
}

void ImagesCV::remap (const cv::Mat & map_1, const cv::Mat & map_2) {

	cv::Mat * undistorted = new cv::Mat{};
	cv::Mat * old_p_img = p_openCvImage;

	// main remapping function that undistort the images
	cv::remap(*p_openCvImage, *undistorted, map_1, map_2, cv::INTER_CUBIC, cv::BORDER_CONSTANT);

	p_openCvImage = undistorted;
	delete old_p_img;
}


ImagesCV::~ImagesCV() {
	delete p_openCvImage;
}

} /* namespace ScanVan */
