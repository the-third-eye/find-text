#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

int main(){

	char *outText;
	tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
	// initialize tesseract-ocr with english
	if(api->Init(NULL, "eng")){
		fprintf(stderr, "Could not initialize tesseract.\n");
		exit(1);
	}	
	// use leptonica library to open input image
	Pix *image = pixRead("phototest.jpg");
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
