#include <opencv2/core/utility.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <vector>
#include <string>

static void help()
{
	std::cout << "\n This program demonstrates contours\n"
             "Usage: \n"
             "  ./connected_components <image(../../photos/signs.jpg as default)>\n";
}

const char* keys =
{
    "{help h||}{@image|../../photos/signs.jpg|image for creating contours}"
};

int main(int argc, char *argv[]){

	cv::CommandLineParser parser(argc, argv, keys);
	if(parser.has("help")){
		help();
		return 0;
	}
	
	std::string inputImage = parser.get<std::string>(0);
	cv::Mat img = cv::imread(inputImage);
	
	if(img.empty()){
		std::cerr << "Could not read input image: " << inputImage << '\n';
		return -1;
	}
	
	cv::Mat gray_img;
	cv::Mat threshold_img;
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	
	// convert image to grayscale
	cv::cvtColor(img, gray_img, CV_BGR2GRAY);
	// smooth image
	cv::blur(gray_img, gray_img, cv::Size(2,2));
	// convert image to binary
	cv::adaptiveThreshold(gray_img, threshold_img, 100, 
			cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 3, 2);
	// find contours and hierarchy of contours
	cv::findContours(threshold_img, contours, hierarchy, cv::RETR_TREE, 
					cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
   	std::cout << "contours: " << contours.size() << '\n';	
	// contour information
	std::vector<std::vector<cv::Point> > polygons(contours.size());
	std::vector<cv::Rect> boundingRects(contours.size());
	// approximate polygons for contours	
	for(int i = 0; i < contours.size(); ++i){
		cv::approxPolyDP(cv::Mat(contours[i]), polygons[i], 3, true);
		boundingRects[i] = cv::boundingRect(cv::Mat(polygons[i]));	
	}
	cv::Mat draw = cv::Mat::zeros(threshold_img.size(), CV_8UC3);
	// contour colors
	cv::Scalar parent_color(rand()&255, rand()&255, rand()&255);
	cv::Scalar child_color(rand()&255, rand()&255, rand()&255);
	cv::Scalar color;
	// draw polygons
	for(int i = 0; i < contours.size(); ++i){
		int child = hierarchy[i][2];
		int parent = hierarchy[i][3];
		if(child == -1)
			color = child_color;
		else if(parent == -1)
			color = parent_color;
			
		cv::drawContours(draw, contours, i, color);
		cv::rectangle(draw, boundingRects[i], color);	
	}	

	cv::imshow("Contours", draw);	
	cv::waitKey(0);
	return 0;
}
