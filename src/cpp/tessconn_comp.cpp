#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <iostream> 
#include <string>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <unordered_set>

#define ALPHANUM "abcdefghijklmnopqrstuvwxyz\
	ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"

static void prepare_image(Pix* &image);

inline bool is_num(const std::string &str){
	for(int i = 0; i < str.size(); ++i){
		if(!isdigit(str[i]))
			return false;
	}

	return true;
}


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
	std::cout << "processing components...\n";

	std::unordered_set<std::string> words;
	for(int i = 0; i < boxes->n; ++i){
		BOX *box = boxaGetBox(boxes, i, L_CLONE);
		api->SetRectangle(box->x, box->y, box->w, box->h);
		std::string ocrResult(api->GetUTF8Text());	
		// split ocr result into white space delimited words	
		std::stringstream ss(ocrResult);
		std::string word;
		while(ss >> word)
			words.insert(word);

		boxDestroy(&box);
	}		
		
	std::cout << "words found:\n";	
	for(const std::string &word: words){
		// print only words in the dictionary
		// or numbers
		if(word.size() > 1){
			if(api->IsValidWord(word.c_str()) || is_num(word))
				std::cout << word << '\n';
		}
	}
	
	// draw word bounding boxes on image	
	Pix *image_bb = pixDrawBoxa(image, boxes, 5, 0xff0000);
	// write image to output file	
	pixWriteImpliedFormat("output.png", image_bb, 0, 0);
	
	// clean up
	fclose(fp);
	api->End();
	pixDestroy(&image);
	pixDestroy(&image_bb);
	boxaDestroy(&boxes);
	
	return 0;
}

