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

    size_t offsetX = 552;
    size_t offsetY = 0;

    size_t aoi_height = (1520 - 595);
	size_t aoi_width = (3131 - 958);

	size_t aoi_offsetX = 958;
	size_t aoi_offsetY = 595;

	int autoTargetVal = 100;

	bool autoExpTimeCont = true;
	bool autoGainCont = true;

	std::vector<size_t> sortedCameraIdx {};

	std::string config_path = {"./config/"}; // default location of the configuration files of the cameras
	bool loadParam = true; // when true, it will load the configuration files to the cameras

	std::string data_path = {"./data/"}; // default location where the images will be stored.

	thread_safe_queue<PairImages> imgQueue {}; // The queue where the pair of images are stored.

	long int imgNum = 0; // Counts the number of images grabbed from the camera

	bool exitProgram = false;

	void Init();

public:
	Cameras();
	Cameras(std::string path_to_config_files);
	size_t GetNumCam();
	void setConfigPath (const std::string path) {
		config_path = path;
	}
	void setDataPath (const std::string path) {
		data_path = path;
	}
	void setImgNum (long int n) { imgNum = n; };

	std::string getConfigPath () {
		return config_path;
	}
	std::string getDataPath () {
		return data_path;
	}

	bool imgQueueEmpty () {
		return imgQueue.empty();
	}

	long int getImgNum () { return imgNum; };

	bool getExitStatus () {
		return exitProgram;
	}

	void GrabImages();
	void StoreImages();
	void SaveParameters();
	void LoadParameters();
	virtual ~Cameras();
};

} /* namespace ScanVan */

#endif /* CAMERAS_HPP_ */