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

#include <time.h>
#include <chrono>

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

void IssueTrigger (ScanVan::Cameras *cams) {

	std::chrono::system_clock::time_point PreviousStartTime { std::chrono::system_clock::now() };
	const std::chrono::microseconds intervalPeriodMicros { static_cast<int>(1 / (cams->getFps()) * 1000000) };

	std::chrono::system_clock::time_point nextStartTime { };

	while (cams->getExitStatus() == false) {
		// Setup timer
		nextStartTime = PreviousStartTime + intervalPeriodMicros;
		std::this_thread::sleep_until(nextStartTime);

		PreviousStartTime = std::chrono::system_clock::now();

		cams->IssueActionCommand();
	}
}

void GrabImages(ScanVan::Cameras *cams) {

	// For measuring the grabbing time
	std::chrono::high_resolution_clock::time_point t1{};
	std::chrono::high_resolution_clock::time_point t2{};
	std::chrono::high_resolution_clock::time_point t1_i{};
	std::chrono::high_resolution_clock::time_point t2_i{};

	long int counter { 0 };

	// Measure the starting of grabbing
	t1 = std::chrono::high_resolution_clock::now();

	while (cams->getExitStatus() == false) {

		t1_i = std::chrono::high_resolution_clock::now();

		// Grab images
		cams->GrabImages();

		t2_i = std::chrono::high_resolution_clock::now();

		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2_i - t1_i).count();
		cout << "fps: " << double(1000000) / duration << endl;

		std::cout << "DQueue: " << cams->getDisplayQueueSize() << std::endl;
		std::cout << "SQueue: " << cams->getStorageQueueSize() << std::endl;

		++counter;

	}

	// Measure the end of the grabbing
	t2 = std::chrono::high_resolution_clock::now();

	// Measure duration of grabbing
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
	cout << "fps: " << double(1000000) * counter / duration << endl;

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

    	//ScanVan::Cameras cams { config_path };
		ScanVan::Cameras cams {};
		//cams.setDataPath(data_path);

		std::thread thIssueActionCommand(IssueTrigger, &cams);
		std::thread thGrabImages(GrabImages, &cams);
		std::thread thDisplayImages(DisplayImages, &cams);
		std::thread thStoreImages(StoreImages, &cams);

		thIssueActionCommand.join();
		thGrabImages.join();
		thDisplayImages.join();
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

