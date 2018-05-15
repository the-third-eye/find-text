#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <iostream>
#include <string>

void prepare_image(Pix **image);
void print_results(tesseract::TessBaseAPI *api);

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
	
	api->SetImage(image);
	api->Recognize(NULL);
	
	// print word statistics
	std::cout << '\n';	
	print_results(api);

	outText = api->GetUTF8Text();
	std::cout << "\nOCR output:\n" << outText;

	// clean up
	api->End();
	pixDestroy(&image);
	
	return 0;
}

/*	tesseract - improving quality
 *	func: prepare_image
 *	- param: Pix pointer-to-pointer
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
		status = pixOtsuAdaptiveThreshold(*image,
				2000, 2000, 0, 0, 0.f, NULL, image);
		// deskew image
		*image = pixFindSkewAndDeskew(*image, 0, NULL, NULL);
	}
}

/* func: print_results
 * - param: TessBaseAPI pointer
 * - desc: print confidence statistics
 * 	 for each word found by tesseract
 */
void print_results(tesseract::TessBaseAPI *api){
	
	if(!api) return;
	
	tesseract::ResultIterator *it = api->GetIterator();
	tesseract::PageIteratorLevel level = tesseract::RIL_WORD;
	std::string res;	
		
	if(it != NULL){
		do{
			const char *word = it->GetUTF8Text(level);
			float confidence = it->Confidence(level);
	
			std::cout << word << ", confidence: " << confidence << '\n';
			delete[] word;

		}while(it->Next(level));
	}
}

