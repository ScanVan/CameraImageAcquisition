//============================================================================
// Name        : PairImages.cpp
// Author      : Marcelo Kaihara
// Version     : 1.0
// Copyright   :
// Description : It encapsulates images from the two cameras into one entity.
//============================================================================

#include "PairImagesRaw.hpp"

namespace ScanVan {

PairImages::PairImages() {
	p_img0 = new ImagesRaw {};
	p_img1 = new ImagesRaw {};
}

PairImages::PairImages(const ImagesRaw &a, const ImagesRaw &b) {
	p_img0 = new ImagesRaw { a };
	p_img1 = new ImagesRaw { b };
}

PairImages::PairImages(const ImagesRaw &&a, const ImagesRaw &&b) {
	p_img0 = new ImagesRaw { std::move(a) };
	p_img1 = new ImagesRaw { std::move(b) };
}

PairImages::PairImages(const ImagesRaw &a) {
	p_img0 = new ImagesRaw { a };
	p_img1 = new ImagesRaw {};
}

PairImages::PairImages(const ImagesRaw &&a) {
	p_img0 = new ImagesRaw { std::move(a) };
	p_img1 = new ImagesRaw {};
}

PairImages::PairImages(const PairImages &a) {
	p_img0 = new ImagesRaw { *(a.p_img0) };
	p_img1 = new ImagesRaw { *(a.p_img1) };
}

PairImages::PairImages(PairImages &&a) {
	p_img0 = a.p_img0;
	p_img1 = a.p_img1;
	a.p_img0 = nullptr;
	a.p_img1 = nullptr;
}

void PairImages::showPair() {
	p_img0->show(p_img0->getSerialNumber());
	p_img1->show(p_img1->getSerialNumber());
}

void PairImages::showPairConcat() {
	if (p_img1->getImgBufferSize() != 0) {
		cv::Mat m0 = p_img0->convertToCvMat();
		cv::Mat m1 = p_img1->convertToCvMat();
		cv::Mat m;
		cv::hconcat(m0, m1, m);

		/// Display
		cv::namedWindow(p_img0->getSerialNumber() + "_" + p_img1->getSerialNumber(),cv::WINDOW_NORMAL);
		cv::imshow(p_img0->getSerialNumber() + "_" + p_img1->getSerialNumber(), m);
	} else {
		p_img0->show(p_img0->getSerialNumber());
	}
}

void PairImages::showUndistortPairConcat (const cv::Mat & map_0_1, const cv::Mat & map_0_2, const cv::Mat & map_1_1, const cv::Mat & map_1_2) {

	if (p_img1->getImgBufferSize() != 0) {
		cv::Mat m0 = p_img0->convertToCvMat();
		cv::Mat m1 = p_img1->convertToCvMat();

		cv::Mat undistorted_0;
		cv::Mat undistorted_1;

		// main remapping function that undistort the images
		cv::remap(m0, undistorted_0, map_0_1, map_0_2, cv::INTER_CUBIC, cv::BORDER_CONSTANT);
		cv::remap(m1, undistorted_1, map_1_1, map_1_2, cv::INTER_CUBIC, cv::BORDER_CONSTANT);

		cv::Mat m;
		cv::hconcat(undistorted_0, undistorted_1, m);

		/// Display
		cv::namedWindow("Equirectangular_" + p_img0->getSerialNumber() + "_" + p_img1->getSerialNumber(), cv::WINDOW_NORMAL);
		cv::imshow("Equirectangular_" + p_img0->getSerialNumber() + "_" + p_img1->getSerialNumber(), m);
	} else {
		cv::Mat m0 = p_img0->convertToCvMat();

		cv::Mat undistorted_0;

		// main remapping function that undistort the images
		cv::remap(m0, undistorted_0, map_0_1, map_0_2, cv::INTER_CUBIC, cv::BORDER_CONSTANT);

		/// Display
		cv::namedWindow("Equirectangular_" + p_img0->getSerialNumber(),cv::WINDOW_NORMAL);
		cv::imshow("Equirectangular_" + p_img0->getSerialNumber(), undistorted_0);
	}
}

void PairImages::savePair(std::string path) {
	p_img0->saveData(path);
	if (p_img1->getImgBufferSize() != 0) {
		p_img1->saveData(path);
	}
}

void PairImages::setImgNumber (const long int &n) {
	p_img0->setImgNumber(n);
	if (p_img1->getImgBufferSize() != 0) {
		p_img1->setImgNumber(n);
	}
}

PairImages & PairImages::operator=(const PairImages &a){
	if (this != &a) {
		delete p_img0;
		delete p_img1;
		p_img0 = new ImagesRaw { *(a.p_img0) };
		p_img1 = new ImagesRaw { *(a.p_img1) };
	}
	return *this;
}

PairImages & PairImages::operator=(PairImages &&a){
	if (this != &a) {
		delete p_img0;
		delete p_img1;
		p_img0 = a.p_img0;
		p_img1 = a.p_img1;
		a.p_img0 = nullptr;
		a.p_img1 = nullptr;
	}
	return *this;
}

PairImages::~PairImages() {
	delete p_img0;
	delete p_img1;
}

} /* namespace ScanVan */
