#include <opencv2/core/utility.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <vector>

#define WHITE 255
#define BLACK 0

int main(int argc, char *argv[]){
	if(argc != 2){
		std::cerr << "Usage: <image file>\n";
		return -1;
	}

	cv::Mat input_img = cv::imread(argv[1]);
	cv::Mat src;
	cv::copyMakeBorder(input_img, src, 50, 50, 50, 50, cv::BORDER_CONSTANT);
	
	cv::Mat bgr[3]; // src 3 color channels
	cv::Mat blue, green, red;
	cv::Mat edges;

	cv::split(src, bgr); // split src into color channels
	cv::Canny(bgr[0], blue, 200, 250);
	cv::Canny(bgr[1], green, 200, 250);
	cv::Canny(bgr[2], red, 200, 250);

	edges = blue | green | red;

	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;		
	std::vector<int> final_rects;
	// find contours
	cv::findContours(edges.clone(), contours, hierarchy,
		   	cv::RETR_TREE, cv::CHAIN_APPROX_NONE);

	std::vector<cv::Rect> rects;
	cv::Mat dst(src.size(), CV_8UC3, cv::Scalar(WHITE, WHITE, WHITE));	
	cv::Vec3b color(BLACK, BLACK, BLACK);
	// draw contours and bounding boxes on src image
	for(auto contour: contours){
		for(const cv::Point& pt: contour){
			cv::Vec3b& pixel = src.at<cv::Vec3b>(pt.y, pt.x);
			pixel = color;
		}
		rects.push_back(cv::boundingRect(contour));
		cv::rectangle(src, rects.back(), color);
	}
	// draw pixels from bounding boxes on dst image
	for(const cv::Rect& rect: rects){
		for(int x = rect.x; x < rect.x + rect.width; ++x){
			for(int y = rect.y; y < rect.y + rect.height; ++y){
				cv::Vec3b pixel = src.at<cv::Vec3b>(y, x);
				dst.at<cv::Vec3b>(y, x) = pixel;
			}
		}	
	}	
	cv::imshow("src", src);
	cv::imshow("dst", dst);
	cv::imshow("contours", edges);
	cv::waitKey(0);	
	return 0;
}
