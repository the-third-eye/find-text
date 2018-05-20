#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <iostream> 
#include <string>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <list>

#define ALPHANUM "abcdefghijklmnopqrstuvwxyz\
	ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"

static void prepare_image(Pix* &image);

static void print_help(){
	std::cout << "Usage: [options] <file>\n";
	std::cout << "Options:\n";
	std::cout << "--help\t Display this information\n";
}

int main(int argc, char *argv[]){

	Pix *image = NULL;
	FILE *fp = NULL;
		
	if(argc > 1){	
		for(int i = 1; i < argc; i++){
			if(strcmp(argv[i], "--help") == 0){
				print_help();
				return 0;
			}
		}

		for(int i = 1; i < argc; i++){
			fp = fopen(argv[i], "rb");
			if(fp != NULL){
				image = pixReadStream(fp, 0);
				if(image != NULL)
					break;
			}
		}

		if(image == NULL){
			std::cerr << "error: no valid file!\n";
			return EXIT_FAILURE;
		}
	}	
	else{
		std::cerr << "error: no input file!\n";
		return EXIT_FAILURE;
	}
	
	tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
	// initialize tesseract-ocr with english
	if(api->Init(NULL, "eng")){
		std::cerr << "Could not initialize tesseract.\n";
		return EXIT_FAILURE;
	}
	api->SetVariable("tessedit_char_whitelist", ALPHANUM);
	api->SetImage(image);
	Boxa *boxes = api->GetComponentImages(tesseract::RIL_WORD, true, NULL, NULL);
	std::cout << "components: " << boxes->n << "\n";
	
	std::list<std::string> words;
	for(int i = 0; i < boxes->n; ++i){
		BOX *box = boxaGetBox(boxes, i, L_CLONE);
		api->SetRectangle(box->x, box->y, box->w, box->h);
		char *ocrResult = api->GetUTF8Text();		
		words.push_back(ocrResult);	
		boxDestroy(&box);
		delete[] ocrResult;
	}		
	
	std::cout << "text found:\n";	
	std::list<std::string>::const_iterator it;
	for(it = words.begin(); it != words.end(); it++){
		std::cout << *it << '\n';
	}
	
	fclose(fp);
	api->End();
	pixDestroy(&image);
	boxaDestroy(&boxes);
	
	return 0;
}

