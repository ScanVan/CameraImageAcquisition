/*
 * EquiToPinhole.h
 *
 *  Created on: Mar 1, 2019
 *      Author: scanvan
 */

#ifndef SRC_EQUITOPINHOLE_HPP_
#define SRC_EQUITOPINHOLE_HPP_

#include <opencv4/opencv2/core/core.hpp>
extern "C" {
void equiToPinhole(cv::Mat &image_input, cv::Mat &image_output,float angle_ouvert, float azim, float elev);
}


#endif /* SRC_EQUITOPINHOLE_HPP_ */
