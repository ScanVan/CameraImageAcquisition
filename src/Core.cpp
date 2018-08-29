//============================================================================
// Name        : Core.cpp
// Author      : Marcelo Kaihara
// Version     :
// Copyright   : 
// Description : Grabs images from the camera and writes the data into file.
//============================================================================

#include <time.h>   // for time
#include <stdlib.h> // for rand & srand

#include <iostream>
#include <unistd.h>
#include <chrono>
#include "Images.hpp"
#include <thread>

// Include files to use OpenCV API
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

// Include files to use the PYLON API.
#include <pylon/PylonIncludes.h>
#include <pylon/gige/PylonGigEIncludes.h>
#include <pylon/gige/ActionTriggerConfiguration.h>
#include "Cameras.hpp"

// Settings to use Basler GigE cameras.
using namespace Basler_GigECameraParams;

// Namespace for using pylon objects.
using namespace Pylon;

// Namespace for using cout.
using namespace std;

std::string GetCurrentWorkingDir( void ) {
// gets the current working directory
	std::array<char, FILENAME_MAX> buff { { } };
	if (getcwd(buff.data(), FILENAME_MAX) == nullptr) {
		throw(std::runtime_error("The directory could not be determined."));
	}
	std::string current_working_dir(buff.data());
	return current_working_dir;
}

/*int main() {

	std::string curr_path = GetCurrentWorkingDir();
	std::string img_path = curr_path + "/" + "img_0_0.raw";
	std::string img_path_out = curr_path + "/" + "img_0_1";
	Images img1 {img_path};
	img1.setCameraIdx(1);
	auto tnow = std::chrono::system_clock::now();
	time_t captureTime = std::chrono::system_clock::to_time_t(tnow);
	img1.setCaptureTime(captureTime);
	img1.setExposureTime(1.2);
	img1.setAutoExpTime(1);
	img1.setAutoGain(1);
	img1.setBalanceR(4.4);
	img1.setBalanceG(3.3);
	img1.setBalanceB(2.2);
	img1.setAutoGain(5);
	img1.setGain(5);

	img1.saveData(img_path_out);
	//img1.show("Img1");
	//cv::waitKey(0);
	Images img2 {img1};
	img2.saveData(img_path_out);
	Images img3 {};
	img3.loadData(img_path_out);

	return 0;
} */

void GrabImages(ScanVan::Cameras *cams) {

	while (cams->getExitStatus() == false) {
		cams->GrabImages();
	}

}

void StoreImages(ScanVan::Cameras *cams) {

	while (cams->getExitStatus() == false) {
		cams->StoreImages();
	}
	while (cams->imgQueueEmpty() == false) {
		cams->StoreImages();
	}

}

int main(int argc, char* argv[])
{

    int exitCode { 0 };

    PylonAutoInitTerm autoinitTerm{};

    std::string curr_path = GetCurrentWorkingDir();
    std::string config_path = curr_path + "/" + "config/";
    std::string data_path = "/media/scanvan/Windows/Users/marcelo.kaihara.HEVS/Documents/img/";

    try {

    	//ScanVan::Cameras cams { config_path };
		ScanVan::Cameras cams {};
		cams.setDataPath(data_path);

		std::thread thGrabImages(GrabImages, &cams);
		std::thread thStoreImages(StoreImages, &cams);

		thGrabImages.join();
		thStoreImages.join();
		//cams.SaveParameters();

	} catch (const GenericException &e) {
		// Error handling
		cerr << "An exception occurred." << endl << e.GetDescription() << endl;
		exitCode = 1;
	} catch (const std::exception &e) {
		cerr << "An exception ocurred." << endl << e.what() << endl;
		exitCode = 1;

	}

    return exitCode;
}

