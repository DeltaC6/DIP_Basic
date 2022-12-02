/*!
 * @author Syed Asad Amin
 * @date Nov 02nd, 2019
 * 
 * A project for Image Processing For Intelligent System Course,
 * National University of Science and Technology (NUST), RWP.
 *
 * Course Instructor: Dr. Jawaid Iqbal
 */

#include <Windows.h>

#include "BitmapHandler.h"

using namespace std;

enum {
	BEGIN, READ, CONVERT, ROTATE, SCALE, TRANSLATE, EXIT
} States;

int currentState = BEGIN;
bool stateMachine = true;

uint8_t orgFileName[32] = "images/iamge1.bmp";
uint8_t grayFileName[32] = "images/image2.bmp";

void _handleMainMenu(void);
void _handleReadMenu(void);
void _handleConvertMenu(void);
void _handleRotateMenu(void);
void _handleScaleMenu(void);
void _handleTranslateMenu(void);
void stateTransition(void);
void printInfo(BitmapHandler *bmp);

int main(int *argc, char **argv) {
	SetConsoleTitle(TEXT("BMP Image App - by Syed Asad Amin"));

	while(stateMachine) {
		switch(currentState) {
			case BEGIN:
				_handleMainMenu();
				break;

			case READ:
				_handleReadMenu();
				break;

			case CONVERT:
				_handleConvertMenu();
				break;

			case ROTATE:
				_handleRotateMenu();
				break;

			case SCALE:
				_handleScaleMenu();
				break;

			case TRANSLATE:
				_handleTranslateMenu();
				break;

			case EXIT:
				stateMachine = false;
				break;

			default:
				currentState = BEGIN;
				break;
		}

		if(currentState != EXIT) { stateTransition(); }
	}
	
	cout << "Closing program..." << endl;
	return EXIT_SUCCESS;
}

void _handleMainMenu(void) {
	cout << "==========     MAIN MENU     ==========" << endl;
	cout << "=======================================" << endl;
	cout << "1. Read 24 bit image and print imformation. (Press I)" << endl;
	cout << "2. Convert 24 bit to 8 bit gray scale. (Press G)" << endl;
	cout << "3. Rotate image. (Press R)" << endl;
	cout << "4. Scale image. (Press S)" << endl;
	cout << "5. Translate Image. (Press T)" << endl;
	cout << "6. Exit. (Press X)" << endl;
	cout << "=======================================" << endl;
}

void _handleReadMenu(void) {
	cout << "----------     READ MENU     ----------" << endl;
	cout << "---------------------------------------" << endl;
	
	cout << "Input file name: ";
	cin >> orgFileName;

	BitmapHandler *bmp = new BitmapHandler();
	bmp->getImageInfo((uint8_t *)orgFileName);

	if(bmp->isImageFound()) {
		cout << "Given file was a valid 'BMP' file. File read successfull" << endl;
		cout << endl;
		printInfo(bmp);
	} else {
		cout << "Invalid file given. ";
		cout << "Unsecessfull read attecmpt..." << endl;
	}

	delete bmp;
	bmp = 0;

	cout << "---------------------------------------" << endl;
}

void _handleConvertMenu(void) {
	cout << "----------     CONVERT MENU     ----------" << endl;
	cout << "------------------------------------------" << endl;
	cout << "Enter name of output gray image: ";
	cin >> grayFileName;

	BitmapHandler *bmp = new BitmapHandler();
	bool stat = bmp->convert2Gray(orgFileName, grayFileName);

	if(stat) {
		cout << "Conversion to gray scale is successfull." << endl;
		cout << endl;
		printInfo(bmp);
	} else {
		cout << "Conversion to gray scale is unsuccessfull." << endl;
	}

	delete bmp;
	bmp = 0;

	cout << "------------------------------------------" << endl;
}

void _handleRotateMenu(void) {
	uint8_t fileName[32];
	double rotAngle = 0;

	cout << "----------     ROTATE MENU     ----------" << endl;
	cout << "-----------------------------------------" << endl;

	cout << "Enter the output file name of rotated image: ";
	cin >> fileName;

	cout << "Enter the rotation angle in degree(s): ";
	cin >> rotAngle;

	BitmapHandler *bmp = new BitmapHandler();
	bool stat = bmp->rotateImage(grayFileName, fileName, rotAngle);

	if(stat) {
		cout << "Rotation of gray scale image is successfull." << endl;
		cout << endl;
		printInfo(bmp);
	} else {
		cout << "Rotation of gray scale image is unsuccessfull." << endl;
	}

	delete bmp;
	bmp = 0;

	cout << "-----------------------------------------" << endl;
}

void _handleScaleMenu(void) {
	uint8_t fileName[32];
	double x = 0.0;
	double y = 0.0;

	cout << "----------     SCALE MENU     ----------" << endl;
	cout << "----------------------------------------" << endl;
	
	cout << "Enter the scaled image name: ";
	cin >> fileName;
	cout << "Enter the scaling factor of x axis: ";
	cin >> x;
	cout << "Enter the scaling factor of y axis: ";
	cin >> y;

	BitmapHandler *bmp = new BitmapHandler();
	bool stat = bmp->scaleImage(grayFileName, fileName, x, y);

	if(stat) {
		cout << "Scaling of gray scale image is successfull." << endl;
		cout << endl;
		printInfo(bmp);
	} else {
		cout << "Scaling of gray scale image is unsuccessfull." << endl;
	}

	delete bmp;
	bmp = 0;

	cout << "----------------------------------------" << endl;
}

void _handleTranslateMenu(void) {
	uint8_t fileName[32];
	uint32_t x = 0;
	uint32_t y = 0;

	cout << "----------     TRANSLATE MENU     ----------" << endl;
	cout << "--------------------------------------------" << endl;

	cout << "Enter the file name of translated image: ";
	cin >> fileName;

	cout << "Enter the new X axis: ";
	cin >> x;

	cout << "Enter the new Y axis: ";
	cin >> y;

	BitmapHandler *bmp = new BitmapHandler();
	bool stat = bmp->translatedImage(grayFileName, fileName, x, y);

	if(stat) {
		cout << "Translaitons of gray scale image is successfull." << endl;
		cout << endl;
		printInfo(bmp);
	} else {
		cout << "Translation of gray scale image is unsuccessfull." << endl;
	}

	delete bmp;
	bmp = 0;

	cout << "--------------------------------------------" << endl;
}

void stateTransition(void) {
	//Mealy state machine transition.
	char c = 0;
	cout << "Select from the menu or press [ENTER] to return or [ESC] to exit..." << endl;
	c = _getch();
	c = toupper(c);
	if(c == 'I') currentState = READ;
	else if(c == 'G') currentState = CONVERT;
	else if(c == 'R') currentState = ROTATE;
	else if(c == 'S') currentState = SCALE;
	else if(c == 'T') currentState = TRANSLATE;
	else if(c == 'X') currentState = EXIT;
	else if(c == '\x1b') currentState = EXIT;	//ESC
	else if(c == '\r') currentState = BEGIN;	//ENTER
	else currentState = BEGIN;					//ANY OTHER KEY

	system("CLS");
}

void printInfo(BitmapHandler *bmp) {
	cout << "File size: " << bmp->getFileSize() << endl;
	cout << "Reserved1: " << bmp->getReserved1() << endl;
	cout << "Reserved2: " << bmp->getReserved2() << endl;
	cout << "Image offset: " << bmp->getImageOffset() << endl;
	cout << "Info header size: " << bmp->getInfoHeaderSize() << endl;
	cout << "Image width: " << bmp->getImageWidth() << endl;
	cout << "Image height: " << bmp->getImageHeight() << endl;
	cout << "Color plane: " << bmp->getColorPlane() << endl;
	cout << "Bits per pixel: " << bmp->getBitsPerPixel() << endl;
	cout << "Compression Type: " << bmp->getCompressionType() << endl;
	cout << "Image size: " << bmp->getImageSize() << endl;
	cout << "Hor Pix/M: " << bmp->getHorPixPerMeter() << endl;
	cout << "Ver Pix/M: " << bmp->getVerPixPerMeter() << endl;
	cout << "Color used: " << bmp->getColorUsed() << endl;
	cout << "Imp color used: " << bmp->getImpColorUsed() << endl;
}
