#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <iostream> 
#include <string>
#include <cstdlib>
#include <cstring>
#include <cstdio>

static void prepare_image(Pix* &image);

static void print_help(){
	std::cout << "Usage: [options] <file>\n";
	std::cout << "Options:\n";
	std::cout << "--help\t Display this information\n";
	std::cout << "--print\t Print word statistics\n";
}

int main(int argc, char *argv[]){

	Pix *image = NULL;
	FILE *fp = NULL;
		
	if(argc > 1){
		// set program options	
		for(int i = 1; i < argc; i++){
			if(strcmp(argv[i], "--help") == 0){
				print_help();
				return 0;
			}
		}
		// use first valid image file found
		for(int i = 1; i < argc; i++){
			fp = fopen(argv[i], "rb");
			if(fp != NULL){
				image = pixReadStream(fp, 0);
				if(image != NULL)
					break;
			}
		}
		// no valid image file found
		if(image == NULL){
			std::cerr << "error: no valid file!\n";
			return EXIT_FAILURE;
		}

	}	
	else{
		std::cerr << "error: no input file!\n";
		return EXIT_FAILURE;
	}
	
	char *outText;
	tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
	// initialize tesseract-ocr with english
	if(api->Init(NULL, "eng")){
		std::cerr << "Could not initialize tesseract.\n";
		return EXIT_FAILURE;
	}	
	
	// prep image for ocr
	prepare_image(image);
	// segmentation mode set to sparse text
	api->SetPageSegMode(tesseract::PSM_SPARSE_TEXT);
	api->SetImage(image);	
	outText = api->GetUTF8Text();
	std::cout << "\nOCR output:\n" << outText;		
	
	// uncomment for output image from prepare	
<<<<<<< HEAD
	pixWriteImpliedFormat("output-prepare.png", image, 0, 0);
=======
	//pixWriteImpliedFormat("output-prepare.png", image, 0, 0);

>>>>>>> 17ccae0b50e7b9dbc240f3c1830abf2f08911004
	// clean up
	fclose(fp);
	api->End();
	delete[] outText;
	pixDestroy(&image);
	
	return 0;
}

/*	tesseract - improving quality
 *	func: prepare_image
 *	- param: Pix pointer-to-pointer
 *	- desc: prepares an image for the
 *			tesseract ocr
 */
static void prepare_image(Pix* &image){
	
	if(!image) return;
	Pix *gray = NULL, *close = NULL;
	if(image->d == 32){
		// convert image to grayscale
		image = pixConvertRGBToGray(image, 0.f, 0.f, 0.f);
	}

	if(image->d == 8 && image->colormap == NULL){
		// apply median filter
		gray = pixRankFilterGray(image, 2, 2, 0.5);
		if(gray != image){
			std::cerr << "gray not image\n";
		}
		// convert image to binary
		pixOtsuAdaptiveThreshold(gray, 4000, 4000, 
				0, 0, 0.f, NULL, &gray);
		// optionally close or open based on histogram
		 close = pixCloseBrick(NULL, gray, 2, 2);
		// deskew image
		image = pixFindSkewAndDeskew(close, 0, NULL, NULL);
	}
//	pixDestroy(&gray);
//	pixDestroy(&close);
}

