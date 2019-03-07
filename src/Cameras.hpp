//============================================================================
// Name        : Cameras.hpp
// Author      : Marcelo Kaihara
// Version     : 1.0
// Copyright   :
// Description : It initializes the camera, sets the configuration of the cameras
//				 from files. It gives the main interface to operate with the cameras
//				 GrabImages, DisplayImages and StoreImages are the main member functions
//				 that are executed in parallel.
//============================================================================

#ifndef CAMERAS_HPP_
#define CAMERAS_HPP_

// Include files to use the PYLON API.
#include <pylon/PylonIncludes.h>
#include <pylon/gige/PylonGigEIncludes.h>
#include <pylon/gige/ActionTriggerConfiguration.h>

// Include files to use OpenCV API
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "Queue.hpp"

#include <algorithm>

#include <atomic>

#include <sys/time.h>
#include <chrono>
#include "ImagesRaw.hpp"
#include "PairImages.hpp"

namespace ScanVan {

class RotCalibContext{
public:
	cv::Point pos[2];
	int clickCounter = 0;

	void draw(cv::Mat &g){
		cv::circle(g, pos[0],10,cv::Scalar(0,255,0),2);
		cv::circle(g, pos[1],10,cv::Scalar(0,255,0),2);

		cv::line(g, pos[0]-2*(pos[1]-pos[0]), pos[1]-2*(pos[0]-pos[1]), cv::Scalar(255,0,0), 1);
	}
};

class Cameras {
private:
	// for measuring the time for each part
	long int number_disp { 0 };
	long int number_conversions_raw2cv { 0 };
	long int number_conversions_cv2equi { 0 };

	long int number_grab { 0 };
	long int number_grab_int { 0 };

	long int number_sto { 0 };
	long int number_sto_raw { 0 };
	long int number_sto_cv { 0 };
	long int number_sto_equi { 0 };

	// for measuring the time for each part
	double total_duration_disp { 0 };
	std::chrono::duration<double> total_duration_raw2cv { 0 };
	std::chrono::duration<double> total_duration_cv2equi { 0 };

	double total_duration_grab { 0 };
	std::chrono::duration<double> total_duration_grab_int { 0 };

	double total_duration_sto { 0 };
	std::chrono::duration<double> total_duration_sto_raw { 0 };
	std::chrono::duration<double> total_duration_sto_cv { 0 };
	std::chrono::duration<double> total_duration_sto_equi { 0 };

	Pylon::IGigETransportLayer *pTL{};
	// Limits the amount of cameras used for grabbing.
	// It is important to manage the available bandwidth when grabbing with multiple
	// cameras. This applies, for instance, if two GigE cameras are connected to the
	// same network adapter via a switch. To manage the bandwidth, the GevSCPD
	// interpacket delay parameter and the GevSCFTD transmission delay parameter can
	// be set for each GigE camera device. The "Controlling Packet Transmission Timing
	// with the Interpacket and Frame Transmission Delays on Basler GigE Vision Cameras"
	// Application Note (AW000649xx000) provides more information about this topic.
	uint32_t c_maxCamerasToUse = 2;
	Pylon::CBaslerGigEInstantCameraArray cameras{};
	uint32_t DeviceKey = 0;
	// For this sample we configure all cameras to be in the same group.
    uint32_t GroupKey = 0x112233;
    Pylon::String_t subnet {};

    double exposureTime = 13057;// exposure time
    int64_t gain = 23;		// gain

    size_t height = 3008;
    size_t width = 3008;

    // offsets of the camera 0
    size_t offsetX_0 = 552 - 82;
    size_t offsetY_0 = 0;

    // offsets of the camera 1
    size_t offsetX_1 = 552 - 8;
    size_t offsetY_1 = 0;

    size_t aoi_height = (1520 - 595);
	size_t aoi_width = (3131 - 958);

	size_t aoi_offsetX_0 = 958 - 82;
	size_t aoi_offsetY_0 = 595;

	size_t aoi_offsetX_1 = 958 - 8;
	size_t aoi_offsetY_1 = 595;


	int autoTargetVal = 100;

	bool autoExpTimeCont = true;
	bool autoGainCont = true;

	std::vector<size_t> sortedCameraIdx {};

	std::string config_path = {"./config/"}; // default location of the configuration files of the cameras
	bool loadParam = true; // when true, it will load the configuration files to the cameras

	std::string data_path = {"./data/"}; // default location where the images will be stored.

	std::string path_cal = {"./config/" }; // path to calibration directory
	// From the path_cal we expect the following sub-directory structure
	// path_cal
	//    |- calibration_40008603
	//               |- map1.xml
	//               |- map2.xml
	//    |- calibration_40009302
	//               |- map1.xml
	//               |- map2.xml

	cv::Mat map_0_1f;
	cv::Mat map_0_2f;
	cv::Mat map_1_1f;
	cv::Mat map_1_2f;

	cv::Mat map_0_1s;
	cv::Mat map_0_2s;
	cv::Mat map_1_1s;
	cv::Mat map_1_2s;

	thread_safe_queue<PairImages> imgStorageQueue {}; // The queue where the pair of images are stored for storage.
	thread_safe_queue<PairImages> imgDisplayQueue {}; // The queue where the pair of images are stored for display.
	thread_safe_queue<std::string> triggerQueue {}; // The queue where the time stamps are stored and signals the grabbing procedure

	long int imgNum { 0 }; // Counts the number of images grabbed from the camera

	std::atomic<bool> exitProgram { false } ;

	void Init();

	double fps = 4.0; // Desired frame rate
	bool startSaving { false }; // Flag used to start saving the images into the disk

	bool useExternalTrigger { false }; // If true it configures the program to use the external trigger in line 1

	bool useChunkFeatures { true }; // If true it uses the camera's clock to get the timestamp

	// White balance settings from cameras
	// They are read at initialization
	double balanceR_0 {};
	double balanceG_0 {};
	double balanceB_0 {};

	double balanceR_1 { };
	double balanceG_1 { };
	double balanceB_1 { };

	//Rotation calibration stuff
	float rotCalibAlpha = 0;
	bool pineholeDisplayEnable = false;
	RotCalibContext rotCalibContexts[2];


public:

	Cameras();
	Cameras(std::string path_to_config_files);
	size_t GetNumCam() const;
	void setConfigPath (const std::string path) {
		config_path = path;
	}
	void setDataPath (const std::string path) {
		data_path = path;
	}
	void setImgNum (long int n) { imgNum = n; };

	void setUseExternalTrigger (bool val) { useExternalTrigger = val; };

	bool getUseExternalTrigger () const { return useExternalTrigger; };

	inline double getFps() const { return fps; }

	std::string getConfigPath () const {
		return config_path;
	}
	std::string getDataPath () const {
		return data_path;
	}

	size_t getStorageQueueSize () {
		return imgStorageQueue.size();
	}

	size_t getDisplayQueueSize() {
		return imgDisplayQueue.size();
	}

	bool imgStorageQueueEmpty () {
		return imgStorageQueue.empty();
	}

	bool imgDisplayQueueEmpty() {
		return imgDisplayQueue.empty();
	}

	long int getImgNum () const { return imgNum; };

	bool getExitStatus () const {
		return exitProgram;
	}

	void IssueActionCommand();
	void GrabImages();
	void StoreImages();
	void DisplayImages();
	void SaveParameters();
	void LoadParameters();
	void LoadCameraConfig();
	void LoadMap();
	void DemoLoadImages();
	std::string StampTime();

	void inc_disp_counter() {
		number_disp++;
	}
	void inc_sto_counter() {
		number_sto++;
	}
	void inc_grab_counter() {
		number_grab++;
	}

	void set_disp_duration(const double &d) {
		total_duration_disp = d;
	}
	void set_sto_duration(const double &d) {
		total_duration_sto = d;
	}
	void set_grab_duration(const double &d) {
		total_duration_grab = d;
	}

	double get_avg_disp() {
		return total_duration_disp / number_disp * 1000.0;
	}

	double get_avg_sto() {
		return total_duration_sto / number_sto * 1000.0;
	}

	double get_avg_grab() {
		return total_duration_grab / number_grab * 1000.0;
	}

	double get_avg_grab_int() {
			return total_duration_grab_int.count() / number_grab_int * 1000.0;
	}

	double get_avg_raw2cv() {
		return total_duration_raw2cv.count() / number_conversions_raw2cv * 1000.0;
	}
	double get_avg_cv2equi() {
		return total_duration_cv2equi.count() / number_conversions_cv2equi * 1000.0;
	}
	double get_avg_sto_raw() {
		return total_duration_sto_raw.count() / number_sto_raw * 1000.0;
	}
	double get_avg_sto_cv() {
		return total_duration_sto_cv.count() / number_sto_cv * 1000.0;
	}
	double get_avg_sto_equi() {
		return total_duration_sto_equi.count() / number_sto_equi * 1000.0;
	}


	virtual ~Cameras();
};

} /* namespace ScanVan */

#endif /* CAMERAS_HPP_ */
