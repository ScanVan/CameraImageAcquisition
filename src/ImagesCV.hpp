#ifndef SRC_IMAGESCV_HPP_
#define SRC_IMAGESCV_HPP_

#include "Images.hpp"
#include "ImagesRaw.hpp"

namespace ScanVan {

class ImagesCV: public Images {
private:
	cv::Mat openCvImage;
public:
	ImagesCV(): Images() {};
	ImagesCV(ImagesRaw &img);

	void show () const;
	void show (std::string name) const;
	virtual void showConcat (std::string name, Images &img2) const;

	size_t getImgBufferSize () const { return openCvImage.total() * openCvImage.elemSize();};

	virtual ~ImagesCV();
};

} /* namespace ScanVan */

#endif /* SRC_IMAGESCV_HPP_ */
