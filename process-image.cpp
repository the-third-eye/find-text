#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <iostream>
#include <string>

void prepare_image(const std::string& file_name);

int main(){

	char *outText;
	tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
	// initialize tesseract-ocr with english
	if(api->Init(NULL, "eng")){
		fprintf(stderr, "Could not initialize tesseract.\n");
		exit(1);
	}	

	prepare_image("phototest5.jpg");
	// use leptonica library to open input image
	Pix *image = pixRead("phototest5.jpg");
	api->SetImage(image);
	// get result from ocr
	outText = api->GetUTF8Text();
	printf("OCR output:\n %s", outText);
	// clean up
	api->End();
	delete[] outText;
	pixDestroy(&image);
	
	return 0;
}

void prepare_image(const std::string& file_name){
	
	Pix *image = pixRead(file_name.c_str());
	if(!image){
		std::cerr << "error - file not found!\n";
		return;
	}
	// grey scale convertion
	image = pixConvertRGBToGray(image, 0.f, 0.f, 0.f);	
	// perform otsu
	int status = pixOtsuAdaptiveThreshold(image, 
				2000, 2000, 0, 0, 0.f, NULL, &image);
	// write the image to file
	status = pixWriteImpliedFormat(file_name.c_str(),
				       		image, 0, 0);
	pixDestroy(&image);
}

