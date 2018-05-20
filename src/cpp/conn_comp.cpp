#include <opencv2/core/utility.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

static void help()
{
    cout << "\n This program demonstrates connected components\n"
             "Usage: \n"
             "  ./connected_components <image(../data/stuff.jpg as default)>\n"
             "The image is converted to grayscale and displayed.\n"
             "Press any key to cycle through the connected components.\n"
	     "All the connected components are displayed as the final image\n";
}

const char* keys =
{
    "{help h||}{@image|../data/stuff.jpg|image for converting to a grayscale}"
};

int main( int argc, const char** argv )
{
    CommandLineParser parser(argc, argv, keys);
    if (parser.has("help"))
    {
        help();
        return 0;
    }
    string inputImage = parser.get<string>(0);
	Mat img = imread(inputImage.c_str(), 0);

    if(img.empty())
    {
        cout << "Could not read input image file: " << inputImage << endl;
        return -1;
    }

    namedWindow( "Image", 1 );
    imshow( "Image", img );
    namedWindow( "Connected Components", 1 );
 
    const int threshval = 100;
    // change to display smaller
    // or larger components
    const int pixel_thresh = 40;

    Mat bw = threshval < 128 ? (img < threshval) : (img > threshval);
    Mat labelImage(img.size(), CV_32S);
    int nLabels = connectedComponents(bw, labelImage, 8);
    std::vector<Vec3b> colors(nLabels);
    colors[0] = Vec3b(0, 0, 0);// background
    
    for(int label = 1; label < nLabels; ++label){
        colors[label] = Vec3b( (rand()&255), (rand()&255), (rand()&255) );
    }

    Mat dst(img.size(), CV_8UC3);
    for(int r = 0; r < dst.rows; ++r){
        for(int c = 0; c < dst.cols; ++c){
            int label = labelImage.at<int>(r, c);
            Vec3b &pixel = dst.at<Vec3b>(r, c);
            pixel = colors[label];
         }
     }
	
     Mat comp(img.size(), CV_8UC3);
     for(int lab = 0; lab < nLabels; ++lab){
     	int comp_pixels = 0;
	for(int r = 0; r < dst.rows; ++r){
		for(int c = 0; c < dst.cols; ++c){
			int label = labelImage.at<int>(r, c);
			Vec3b &pixel = comp.at<Vec3b>(r, c);
			if(label == lab){
				pixel = colors[label];
				comp_pixels++;
			}
			else{
				pixel = colors[0];
			}
		}
	}
	if(comp_pixels >= pixel_thresh){
		imshow("Connected Components", comp);
		waitKey(0);
	}
    }

    imshow( "Connected Components", dst );
    waitKey(0);
    return 0;
}
