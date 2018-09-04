//============================================================================
// Name        : PairImages.cpp
// Author      : Marcelo Kaihara
// Version     : 1.0
// Copyright   :
// Description : It encapsulates images from the two cameras into one entity.
//============================================================================

#include "PairImages.hpp"

namespace ScanVan {

PairImages::PairImages() {
	p_img0 = new Images {};
	p_img1 = new Images {};
}

PairImages::PairImages(const Images &a, const Images &b) {
	p_img0 = new Images { a };
	p_img1 = new Images { b };
}

PairImages::PairImages(const Images &&a, const Images &&b) {
	p_img0 = new Images { std::move(a) };
	p_img1 = new Images { std::move(b) };
}

PairImages::PairImages(const PairImages &a) {
	p_img0 = new Images { *(a.p_img0) };
	p_img1 = new Images { *(a.p_img1) };
}

PairImages::PairImages(PairImages &&a) {
	p_img0 = a.p_img0;
	p_img1 = a.p_img1;
	a.p_img0 = nullptr;
	a.p_img1 = nullptr;
}

void PairImages::showPair() {
	p_img0->show("0");
	p_img1->show("1");
}

void PairImages::savePair(std::string path) {
	p_img0->saveData(path);
	p_img1->saveData(path);
}

void PairImages::setImgNumber (const long int &n) {
	p_img0->setImgNumber(n);
	p_img1->setImgNumber(n);
}

PairImages & PairImages::operator=(const PairImages &a){
	if (this != &a) {
		delete p_img0;
		delete p_img1;
		p_img0 = new Images { *(a.p_img0) };
		p_img1 = new Images { *(a.p_img1) };
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
