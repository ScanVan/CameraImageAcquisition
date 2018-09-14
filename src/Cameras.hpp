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

#include "Images.hpp"
#include "PairImages.hpp"
#include "Queue.hpp"

#include <algorithm>

#include <atomic>

#include <sys/time.h>
#include <chrono>

namespace ScanVan {

class Cameras {
private:
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

	std::string path_map_0_1 = { "./config/map_0_1.xml" }; // path to the map1.xml of the calibration of the camera 0
	std::string path_map_0_2 = { "./config/map_0_2.xml" }; // path to the map2.xml of the calibration of the camera 0
	std::string path_map_1_1 = { "./config/map_1_1.xml" }; // path to the map1.xml of the calibration of the camera 1
	std::string path_map_1_2 = { "./config/map_1_2.xml" }; // path to the map2.xml of the calibration of the camera 1
	cv::Mat map_0_1;
	cv::Mat map_0_2;
	cv::Mat map_1_1;
	cv::Mat map_1_2;

	thread_safe_queue<PairImages> imgStorageQueue {}; // The queue where the pair of images are stored for storage.
	thread_safe_queue<PairImages> imgDisplayQueue {}; // The queue where the pair of images are stored for display.
	thread_safe_queue<std::string> triggerQueue {}; // The queue where the time stamps are stored and signals the grabbing procedure

	long int imgNum = 0; // Counts the number of images grabbed from the camera

	std::atomic<bool> exitProgram = false;

	void Init();

	double fps = 4.0; // Desired frame rate

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
	std::string StampTime();

	virtual ~Cameras();
};

} /* namespace ScanVan */

#endif /* CAMERAS_HPP_ */
