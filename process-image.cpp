#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <iostream>
#include <string>

void prepare_image(Pix **image);

int main(){
		
	char *outText;
	std::string file_name;

	tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
	// initialize tesseract-ocr with english
	if(api->Init(NULL, "eng")){
		std::cerr << "Could not initialize tesseract.\n";
		exit(1);
	}	

	std::cout << "enter file name:\n";
	std::cin >> file_name;
	
	// use leptonica library to open input image
	Pix *image = pixRead(file_name.c_str());
	if(!image){
		std::cerr << file_name << " not found!\n";
		exit(1);
	}
	// prep image for ocr
	prepare_image(&image);

	std::cout << "depth(in bits): " << image->d << '\n';
	api->SetImage(image);

	// get result from ocr
	outText = api->GetUTF8Text();
	std::cout << "OCR output:\n" << outText;
	
	// clean up
	api->End();
	delete[] outText;
	pixDestroy(&image);
	
	return 0;
}

/*	tesseract - improving quality
 *	- desc: converts an image to binary
 */
void prepare_image(Pix **image){
	if(!image) return;
	
	int status;
	if((*image)->d == 32){
		// convert image to grayscale
		*image = pixConvertRGBToGray(*image, 0.f, 0.f, 0.f);
	}

	if((*image)->d == 8){	
		
		// convert image to binary
<<<<<<< HEAD
		status = pixOtsuAdaptiveThreshold(*image,
				2000, 2000, 0, 0, 0.f, NULL, image);
		// deskew image
		*image = pixFindSkewAndDeskew(*image, 0, NULL, NULL);
	}
=======
		status = pixOtsuAdaptiveThreshold(image,
				2000, 2000, 0, 0, 0.f, NULL, &image);
	}

>>>>>>> e1a90353057ae1fa96968b06bd16fb0851f9ef40
}

