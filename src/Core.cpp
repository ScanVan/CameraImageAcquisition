//============================================================================
// Name        : Core.cpp
// Author      : Marcelo Kaihara
// Version     : 1.0
// Copyright   : 
// Description : Main source code. It grabs images from the two cameras and
//				 displays them on the screen and stores them in the disk
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

void GrabImages(ScanVan::Cameras *cams) {

	while (cams->getExitStatus() == false) {
		cams->GrabImages();
	}

}

void StoreImages(ScanVan::Cameras *cams) {

	while (cams->getExitStatus() == false) {
		cams->StoreImages();
	}
	while (cams->imgStorageQueueEmpty() == false) {
		cams->StoreImages();
	}

}

void DisplayImages(ScanVan::Cameras *cams) {

	while (cams->getExitStatus() == false) {
		cams->DisplayImages();
	}
	while (cams->imgDisplayQueueEmpty() == false) {
		cams->DisplayImages();
	}

}

int main(int argc, char* argv[])
{

    int exitCode { 0 };

    PylonAutoInitTerm autoinitTerm{};

    std::string curr_path = GetCurrentWorkingDir();
    std::string config_path = curr_path + "/" + "config/";
    //std::string data_path = "/media/Windows/Users/marcelo.kaihara.HEVS/Documents/img/";

    try {

    	ScanVan::Cameras cams { config_path };
		//ScanVan::Cameras cams {};
		//cams.setDataPath(data_path);

		std::thread thGrabImages(GrabImages, &cams);
		std::thread thStoreImages(StoreImages, &cams);
		std::thread thDisplayImages(DisplayImages, &cams);

		thGrabImages.join();
		thStoreImages.join();
		thDisplayImages.join();

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

