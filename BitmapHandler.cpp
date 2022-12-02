/*!
 * @author Syed Asad Amin
 * @date Nov 02nd, 2019
 * @version 1.0.6
 *          - 1.0.0 - Added file reading feature along with data extraction
 *          - 1.0.1 - Corrected data skip problem during file read.
 *          - 1.0.2 - Added data structure support.
 *			- 1.0.3 - Added gray scale conversion support.
 *			- 1.0.4 - Added gray scale translation support.
 *			- 1.0.5 - Added gray scale scaling support.
 *			- 1.0.6 - Added gray scale rotation support.
 *
 * @desc This library is used to extract and manipulate the 'BMP' file data.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * A project for Image Processing For Intelligent System Course,
 * National University of Science and Technology (NUST), RWP.
 *
 * Course Instructor: Dr. Jawaid Iqbal
 */

#include "BitmapHandler.h"

BitmapHandler::BitmapHandler() {
	imageFound = false;
	memset(palette, 0, sizeof(palette));
	memset(&BMP_FH, 0, sizeof(BMP_FH));
	memset(&BMP_IH, 0, sizeof(BMP_IH));
}

BitmapHandler::~BitmapHandler() {

}

void BitmapHandler::getImageInfo(const uint8_t *fileName) {
	try {
		uint8_t rawData[HEADER_SIZE];
		readImage(fileName, 0, rawData, HEADER_SIZE);
		extractInfo(rawData);
	} catch(std::exception &e) {
		std::cout << e.what() << std::endl;
	}
}

bool BitmapHandler::convert2Gray(const uint8_t *srcFile, const uint8_t *dstFile) {
	bool result = false;
	try {
		//Reading the source image file for info.
		getImageInfo(srcFile);

		//Checking if the image is colored or not.
		if(getBitsPerPixel() < BIT_COLOR_IMAGE) { return 0; }

		//Calculating the row bytes with padding.
		uint32_t rowBytesColor = (((getBitsPerPixel() * getImageWidth()) + 31) / 32) * 4;
		uint32_t rowBytesGray = (((BIT_GRAY_IMAGE * getImageWidth()) + 31) / 32) * 4;

		//Calculating image size.
		uint32_t colorImageSize = rowBytesColor * getImageHeight();
		uint32_t grayImageSize = rowBytesGray * getImageHeight();

		//Creating heap memory according to calculated sizes.
		uint8_t *buffColorData = new uint8_t[colorImageSize];
		uint8_t *buffGrayData = new uint8_t[grayImageSize];

		//Resetting heap memory buffers.
		memset(buffColorData, 0, colorImageSize);
		memset(buffGrayData, 0, grayImageSize);

		//Reading colored image data.
		readImage(srcFile, HEADER_SIZE, buffColorData, colorImageSize);

		//Changing header data according to new gray scale image data.
		setFileSize(HEADER_SIZE + PALETTE_SIZE + grayImageSize);
		setReserved1(getReserved1());
		setReserved2(getReserved2());
		setImageOffset(getImageOffset() + PALETTE_SIZE);
		setInfoHeaderSize(getInfoHeaderSize());
		setImageWidth(getImageWidth());
		setImageHeight(getImageHeight());
		setColorPlane(getColorPlane());
		setBitsPerPixel(BIT_GRAY_IMAGE);
		setCompressionType(getCompressionType());
		setImageSize(grayImageSize);
		setHorPixPerMeter(getHorPixPerMeter());
		setVerPixPerMeter(getVerPixPerMeter());
		setColorUsed(lround(pow(2, BIT_GRAY_IMAGE)));
		setImpColorUsed(getImpColorUsed());

		//Creating header data.
		uint8_t rawData[HEADER_SIZE];
		rawData[0] = HEADER_B0;
		rawData[1] = HEADER_B1;
		memcpy(&rawData[FILE_INFO_ADD], &BMP_FH, sizeof(BMP_FH));
		memcpy(&rawData[IMAGE_INFO_ADD], &BMP_IH, sizeof(BMP_IH));

		//Writing header data.
		writeImage(dstFile, rawData, HEADER_SIZE);

		//Creating palette.
		createPalette();

		//Writing palette data.
		writeImage(dstFile, palette, PALETTE_SIZE);

		//Converting to gray scale.
		for(uint32_t i = 0; i < getImageHeight(); i++) {
			for(uint32_t j = 0; j < getImageWidth(); j++) {
				uint16_t value = 0;
				for(uint8_t k = 0; k < 3; k++) {
					value += buffColorData[(i * rowBytesColor) + (j * 3) + k];
				}
				value /= 3;
				buffGrayData[(i * rowBytesGray) + j] = value;
			}
		}
		
		//Writing gray image data.
		writeImage(dstFile, buffGrayData, grayImageSize);

		//Deleting heap memory
		delete[] buffGrayData;	//RULE: Always delete what you new.
		buffGrayData = 0;		//		Always free what you malloc.
		
		delete[] buffColorData;
		buffColorData = 0;

		result = true;
	} catch(std::exception &e) {
		std::cout << e.what() << std::endl;
	}

	return result;
}

bool BitmapHandler::rotateImage(const uint8_t *srcFile, const uint8_t *dstFile, double angle) {
	bool result = false;

	try {
		//Reading the source image file for info.
		getImageInfo(srcFile);

		//Checking if the image is gray or not.
		if(getBitsPerPixel() != BIT_GRAY_IMAGE) { return false; }

		//Calculating data sizes.
		while(angle >= 360.0) { angle -= 360.0; }									//Failsafe operations
		while(angle < 0.0) { angle += 360; }

		double cosA = cos(toRadians(angle));
		double sinA = sin(toRadians(angle));

		uint32_t gImageWidth = getImageWidth();
		uint32_t gimageHeight = getImageHeight();

		uint32_t rImageWidth = lround((gimageHeight * sinA) + (gImageWidth * cosA));
		uint32_t rImageHeight = lround((gimageHeight * cosA) + (gImageWidth * sinA));

		uint32_t rowGrayData = (((BIT_GRAY_IMAGE * gImageWidth) + 31) / 32) * 4;
		uint32_t rowRotData = (((BIT_GRAY_IMAGE * rImageWidth) + 31) / 32) * 4;

		uint32_t gImageSize = getImageSize();
		uint32_t rImageSize = rowRotData * rImageHeight;

		//Creating heap memory for gray image data.
		uint8_t *buffGrayData = new uint8_t[gImageSize];
		uint8_t *buffRotData = new uint8_t[rImageSize];

		//Resetting the created heap memory.
		memset(buffGrayData, 0, gImageSize);
		memset(buffRotData, 0, rImageSize);

		//Reading gray image data.
		readImage(srcFile, HEADER_SIZE + PALETTE_SIZE, buffGrayData, getImageSize());

		//Changing header data according to new rotated image.
		setFileSize(HEADER_SIZE + PALETTE_SIZE + rImageSize);
		setReserved1(0);
		setReserved2(0);
		setImageOffset(getImageOffset());
		setInfoHeaderSize(getInfoHeaderSize());
		setImageWidth(rImageWidth);
		setImageHeight(rImageHeight);
		setColorPlane(1);
		setBitsPerPixel(getBitsPerPixel());
		setCompressionType(0);
		setImageSize(rImageSize);
		setHorPixPerMeter(getHorPixPerMeter());
		setVerPixPerMeter(getVerPixPerMeter());
		setColorUsed(getColorUsed());
		setImpColorUsed(getImpColorUsed());

		uint8_t rawData[HEADER_SIZE];
		rawData[0] = HEADER_B0;
		rawData[1] = HEADER_B1;
		memcpy(&rawData[FILE_INFO_ADD], &BMP_FH, sizeof(BMP_FH));
		memcpy(&rawData[IMAGE_INFO_ADD], &BMP_IH, sizeof(BMP_IH));

		//Writing header data.
		writeImage(dstFile, rawData, HEADER_SIZE);

		//Creating palette.
		createPalette();

		//Writing palette data.
		writeImage(dstFile, palette, PALETTE_SIZE);

		//Rotating the image.
		//x' = x * cos(a) + y * sin(a)
		//y' = y * cos(a) - x * sin(a)
		for(uint32_t i = 0; i < gimageHeight; i++) {
			for(uint32_t j = 0; j < gImageWidth; j++) {
				uint32_t xo = lround(i * cosA + j * sinA);
				uint32_t yo = lround(j * cosA - i * sinA);
				buffRotData[(xo * rowRotData) + yo] = buffGrayData[(i * rowGrayData) + j];
			}
		}

		//Writing gray image data.
		writeImage(dstFile, buffRotData, rImageSize);

		//Deleting heap memory
		delete[] buffRotData;	//RULE: Always delete what you new.
		buffRotData = 0;		//		Always free what you malloc.

		delete[] buffGrayData;
		buffGrayData = 0;

		result = true;
	} catch(std::exception &e) {
		std::cout << e.what() << std::endl;
	}
	return result;
}

bool BitmapHandler::scaleImage(const uint8_t *srcFile, const uint8_t *dstFile, double X, double Y) {
	bool result = false;
	try {
		//Reading the source image file for info.
		getImageInfo(srcFile);

		//Checking if the image is gray or not.
		if(getBitsPerPixel() != BIT_GRAY_IMAGE) { return false; }

		//Calculating row data size.
		if(X == 0.0) { X = 1.0; }														//Failsafe operation
		if(Y == 0.0) { Y = 1.0; }

		uint32_t gImageWidth = getImageWidth();
		uint32_t sImageWidth = lround(getImageWidth() * X);

		uint32_t gimageHeight = getImageHeight();
		uint32_t sImageHeight = lround(getImageHeight() * Y);

		uint32_t rowGrayData = (((BIT_GRAY_IMAGE * gImageWidth) + 31) / 32) * 4;		//Original image rows data.
		uint32_t rowScaledData = (((BIT_GRAY_IMAGE * sImageWidth) + 31) / 32) * 4;		//Scaled image rows data.

		uint32_t gImageSize = getImageSize();
		uint32_t sImageSize = rowScaledData * sImageHeight;								//Scaled image size.

		//Creating heap memory for image data.
		uint8_t *buffGrayData = new uint8_t[gImageSize];
		uint8_t *buffScaleData = new uint8_t[sImageSize];

		//Resetting the created memory buffer.
		memset(buffGrayData, 0, gImageSize);
		memset(buffScaleData, 0, sImageSize);

		//Reading the image data into the buffer.
		readImage(srcFile, HEADER_SIZE + PALETTE_SIZE, buffGrayData, gImageSize);

		//Changing header data.
		setFileSize(HEADER_SIZE + PALETTE_SIZE + sImageSize);							//CHANGED
		setReserved1(0);
		setReserved2(0);
		setImageOffset(getImageOffset());
		setInfoHeaderSize(getInfoHeaderSize());
		setImageWidth(sImageWidth);														//CHANGED
		setImageHeight(sImageHeight);													//CHANGED
		setColorPlane(1);
		setBitsPerPixel(getBitsPerPixel());
		setCompressionType(0);
		setImageSize(sImageSize);														//CHANGED
		setHorPixPerMeter(getHorPixPerMeter());
		setVerPixPerMeter(getVerPixPerMeter());
		setColorUsed(getColorUsed());
		setImpColorUsed(getImpColorUsed());

		uint8_t rawData[HEADER_SIZE];
		rawData[0] = HEADER_B0;
		rawData[1] = HEADER_B1;
		memcpy(&rawData[FILE_INFO_ADD], &BMP_FH, sizeof(BMP_FH));
		memcpy(&rawData[IMAGE_INFO_ADD], &BMP_IH, sizeof(BMP_IH));

		//Writing header data.
		writeImage(dstFile, rawData, HEADER_SIZE);

		//Creating palette.
		createPalette();

		//Writing palette data.
		writeImage(dstFile, palette, PALETTE_SIZE);

		//Scaling image data.
		//x' = x * (width' / width)
		//y' = y * (height' / height)
		for(uint32_t i = 0; i < gimageHeight; i++) {
			for(uint32_t j = 0; j < gImageWidth; j++) {
				uint32_t xo = lround(i * sImageWidth / gImageWidth);
				uint32_t yo = lround(j * sImageHeight / gimageHeight);
				buffScaleData[(xo * rowScaledData) + yo] = buffGrayData[(i * rowGrayData) + j];
			}
		}

		//Writing scaled imaged data.
		writeImage(dstFile, buffScaleData, sImageSize);

		//Deleting heap memory.
		delete[] buffGrayData;
		buffGrayData = 0;

		delete[] buffScaleData;
		buffScaleData = 0;

		result = true;
	} catch(std::exception &e) {
		std::cout << e.what() << std::endl;
	}
	return result;
}

bool BitmapHandler::translatedImage(const uint8_t *srcFile, const uint8_t *dstFile, const uint32_t X, const uint32_t Y) {
	bool result = false;
	try {
		//Reading the source image file for info.
		getImageInfo(srcFile);

		//Checking if the image is gray or not.
		if(getBitsPerPixel() != BIT_GRAY_IMAGE) { return false; }

		//Calculating row data size.
		uint32_t rowByteData = (((BIT_GRAY_IMAGE * getImageWidth()) + 31) / 32) * 4;

		//Creating heap memory for image data.
		uint8_t *buffGrayData = new uint8_t[getImageSize()];
		uint8_t *buffTransData = new uint8_t[getImageSize()];

		//Resetting the created memory buffer.
		memset(buffGrayData, 0, getImageSize());
		memset(buffTransData, 0, getImageSize());

		//Reading the image data into the buffer.
		readImage(srcFile, HEADER_SIZE + PALETTE_SIZE, buffGrayData, getImageSize());

		//Changing header data.
		setFileSize(getFileSize());
		setReserved1(0);
		setReserved2(0);
		setImageOffset(getImageOffset());
		setInfoHeaderSize(getInfoHeaderSize());
		setImageWidth(getImageWidth());
		setImageHeight(getImageHeight());
		setColorPlane(1);
		setBitsPerPixel(getBitsPerPixel());
		setCompressionType(0);
		setImageSize(getImageSize());
		setHorPixPerMeter(getHorPixPerMeter());
		setVerPixPerMeter(getVerPixPerMeter());
		setColorUsed(getColorUsed());
		setImpColorUsed(getImpColorUsed());

		uint8_t rawData[HEADER_SIZE];
		rawData[0] = HEADER_B0;
		rawData[1] = HEADER_B1;
		memcpy(&rawData[FILE_INFO_ADD], &BMP_FH, sizeof(BMP_FH));
		memcpy(&rawData[IMAGE_INFO_ADD], &BMP_IH, sizeof(BMP_IH));

		//Writing header data.
		writeImage(dstFile, rawData, HEADER_SIZE);

		//Creating palette.
		createPalette();

		//Writing palette data.
		writeImage(dstFile, palette, PALETTE_SIZE);

		//Translating the image data about X and Y axis.
		for(uint32_t i = 0; i < getImageHeight() - Y; i++) {
			for(uint32_t j = 0; j < getImageWidth() - X; j++) {
				buffTransData[((i + Y) * rowByteData) + (j + X)] = buffGrayData[(i * rowByteData) + j];
			}
		}
		
		//Writing translated image data.
		writeImage(dstFile, buffTransData, getImageSize());

		//Deleting heap memory.
		delete[] buffGrayData;
		buffGrayData = 0;

		delete[] buffTransData;
		buffTransData = 0;

		result = true;
	} catch(std::exception & e) {
		std::cout << e.what() << std::endl;
	}
	return result;
}

void BitmapHandler::readImage(const uint8_t *fileName, const uint32_t offset, uint8_t *buffer, const uint32_t size) {
	try {
		rimage.open((char *)fileName, std::ios::in | std::ios::binary);
		rimage.seekg(offset);
		rimage.read((char *)buffer, size);
		rimage.close();
	} catch(std::exception & e) {
		std::cout << e.what() << std::endl;
	}
}

void BitmapHandler::writeImage(const uint8_t *fileName, uint8_t *data, const uint32_t size) {
	try {
		wimage.open((char *)fileName, std::ios::out | std::ios::binary | std::ios::app);
		wimage.write((char *)data, size);
		wimage.flush();
		wimage.close();
	} catch(std::exception & e) {
		std::cout << e.what() << std::endl;
	}
}

void BitmapHandler::extractInfo(const uint8_t *data) {
	try {
		imageFound = false;
		if(data[0] == HEADER_B0 && data[1] == HEADER_B1) {
			memcpy(&BMP_FH, &data[FILE_INFO_ADD], sizeof(BMP_FH));
			memcpy(&BMP_IH, &data[IMAGE_INFO_ADD], sizeof(BMP_IH));
			imageFound = true;
		}
	} catch(std::exception &e) {
		std::cout << e.what() << std::endl;
	}
}

void BitmapHandler::createPalette(void) {	
	try {
		uint16_t k = 0;
		for(uint16_t i = 0; i < 256; i++) {
			for(uint8_t j = 0; j < 3; j++) {
				palette[k++] = (uint8_t)i;
			}
			palette[k++] = 0;
		}
	} catch(std::exception & e) {
		std::cout << e.what() << std::endl;
	}
}
