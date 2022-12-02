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

#pragma once

#pragma comment(linker, "/STACK:10485760")
#pragma comment(linker, "/HEAP:10485760")

#include <conio.h>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>

#include <fstream>
#include <iostream>

#ifndef M_PI 
static const double M_PI = 3.1415926535897932384626433832795;
#endif

#ifndef BITMAP_HEADER_INFO
#define BITMAP_HEADER_INFO
static const uint8_t HEADER_SIZE		= 54;
static const uint16_t PALETTE_SIZE		= 1024;

static const uint8_t BIT_COLOR_IMAGE	= 24;
static const uint8_t BIT_GRAY_IMAGE		= 8;

static const uint8_t HEADER_B0			= 'B';		//Equivalent to 66 form ascii table
static const uint8_t HEADER_B1			= 'M';		//Equivalent to 77 from ascii table
static const uint8_t FILE_INFO_ADD		= 2;
static const uint8_t IMAGE_INFO_ADD		= 14;
#endif

class BitmapHandler {

	public:
		/*!
		 * @brief Constructor of the class initializing all the data variable(s).
		 */
		BitmapHandler();

		/*!
		 * @brief Destructor
		 */
		virtual ~BitmapHandler();

		/*!
		 * @brief Retrieves the image header info.
		 * @param [string] - Filename of the image.
		 * @return None
		 */
		void getImageInfo(const uint8_t *fileName);

		/*!
		 * @brief Retrieves the image header info.
		 * @param [string] - Source file that needs to be converted into gray.
		 * @param [string] - File name of the gray scale image to be saved.
		 * @return [boolean] - Set if conversion done successfully otherwise reset.
		 */
		bool convert2Gray(const uint8_t *srcFile, const uint8_t *dstFile);

		/*!
		 * @brief Rotates the image at given angles.
		 * @param [string] - Source file that needs to be rotated.
		 * @param [string] - File name to write the rotated image to.
		 * @param [double] - Rotation angle in degrees.
		 * @return [boolean] - Set if rotation is done successfully otherwise reset.
		 */
		bool rotateImage(const uint8_t *srcFile, const uint8_t *dstFile, double angle);

		/*!
		 * @brief Scales the images on x and y axis.
		 * @param [string] - Source file that needs to be scaled.
		 * @param [string] - File name to write the scaled image to.
		 * @param [double] - Value to scale along x axis.
		 * @param [double] - Value to scale along y axis.
		 * @return [boolean] - Set if scaled is done successfully otherwise reset.
		 */
		bool scaleImage(const uint8_t *srcFile, const uint8_t *dstFile, double X, double Y);

		/*!
		 * @brief Translate the image on x and y axis.
		 * @param [string] - Source file that needs to be translated.
		 * @param [string] - File name to write the translated image to.
		 * @param [int] - Value to translate along x axis.
		 * @param [int] - Value to translate along y axis.
		 * @return [boolean] - Set if translation is done successfully otherwise reset.
		 */
		bool translatedImage(const uint8_t *srcFile, const uint8_t *dstFile, const uint32_t X, const uint32_t Y);

		//GETTERS

		inline bool isImageFound(void) const { return imageFound; }

		inline uint32_t getFileSize(void) const { return BMP_FH.fileSize; }
		inline uint16_t getReserved1(void) const { return BMP_FH.reserved1; }
		inline uint16_t getReserved2(void) const { return BMP_FH.reserved2; }
		inline uint32_t getImageOffset(void) const { return BMP_FH.imageOffset; }
		inline uint32_t getInfoHeaderSize(void) const { return BMP_IH.infoHeaderSize; }
		inline uint32_t getImageWidth(void) const { return BMP_IH.imageWidth; }
		inline uint32_t getImageHeight(void) const { return BMP_IH.imageHeight; }
		inline uint16_t getColorPlane(void) const { return BMP_IH.colorPlane; }
		inline uint16_t getBitsPerPixel(void) const { return BMP_IH.bitsPerPixel; }
		inline uint32_t getCompressionType(void) const { return BMP_IH.compressionType; }
		inline uint32_t getImageSize(void) const { return BMP_IH.imageSize; }
		inline uint32_t getHorPixPerMeter(void) const { return BMP_IH.hPixPM; }
		inline uint32_t getVerPixPerMeter(void) const { return BMP_IH.vPixPM; }
		inline uint32_t getColorUsed(void) const { return BMP_IH.colorUsed; }
		inline uint32_t getImpColorUsed(void) const { return BMP_IH.impColorUsed; }

		//SETTERS

		inline void setFileSize(const uint32_t size) { BMP_FH.fileSize = size; }
		inline void setReserved1(const uint16_t res1) { BMP_FH.reserved1 = res1; }
		inline void setReserved2(const uint16_t res2) { BMP_FH.reserved2 = res2; }
		inline void setImageOffset(const uint32_t offset) { BMP_FH.imageOffset = offset; }
		inline void setInfoHeaderSize(const uint32_t size) { BMP_IH.infoHeaderSize = size; }
		inline void setImageWidth(const uint32_t width) { BMP_IH.imageWidth = width; }
		inline void setImageHeight(const uint32_t height) { BMP_IH.imageHeight = height; }
		inline void setColorPlane(const uint16_t plane) { BMP_IH.colorPlane = plane; }
		inline void setBitsPerPixel(const uint16_t bpp) { BMP_IH.bitsPerPixel = bpp; }
		inline void setCompressionType(const uint32_t comp) { BMP_IH.compressionType = comp; }
		inline void setImageSize(const uint32_t size) { BMP_IH.imageSize = size; }
		inline void setHorPixPerMeter(const uint32_t hpm) { BMP_IH.hPixPM = hpm; }
		inline void setVerPixPerMeter(const uint32_t vpm) { BMP_IH.vPixPM = vpm; }
		inline void setColorUsed(const uint32_t color) { BMP_IH.colorUsed = color; }
		inline void setImpColorUsed(const uint32_t impcol) { BMP_IH.impColorUsed = impcol; }

	protected:
		/*!
		 * @brief Reads the given image file into the data buffer.
		 * @param [string] - Source file that needs to be read.
		 * @param [int] - Offset cursor location if needed.
		 * @param [string] - Raw data buffer.x
		 * @param [int] - Size of the data to be read.
		 * @return None
		 */
		void readImage(const uint8_t *fileName, const uint32_t offset, uint8_t *buffer, const uint32_t size);

		/*!
		 * @brief Reads the given image file into the data buffer.
		 * @param [string] - Source file that needs to be read.
		 * @param [int] - Offset cursor location if needed.
		 * @param [string] - Raw data buffer.
		 * @param [int] - Size of the data to be read.
		 * @return None
		 */
		void writeImage(const uint8_t *fileName, uint8_t *data, const uint32_t size);

		/*!
		 * @brief Extracts header info to data structure.
		 * @param [string] - Raw data for information extraction.
		 * @return None
		 */
		void extractInfo(const uint8_t *data);

		/*!
		 * @brief Create color palette for gray scale images.
		 * @param None
		 * @return None
		 */
		void createPalette(void);

		/*!
		 * @brief Converts the given angle in radian into degrees
		 * @param [double] - Angle in radians
		 * @return [double] - Angle in degrees
		 */
		inline double toDegrees(const double rad) const { return rad * 180.0 / M_PI; }

		/*!
		 * @brief Converts the given angle in degrees into radians
		 * @param [double] - Angle in degrees
		 * @return [double] - Angle in radians
		 */
		inline double toRadians(const double deg) const { return deg * M_PI / 180.0; }

	private:
		std::ifstream rimage;
		std::ofstream wimage;

		bool imageFound;
		uint8_t palette[PALETTE_SIZE];

		/*! Struct for BMP File Header */
		struct {
			uint32_t fileSize;              /*! BMP file size */
			uint16_t reserved1;             /*! Reserved Bytes */
			uint16_t reserved2;             /*! Reserved Bytes */
			uint32_t imageOffset;           /*! Image data offset */
		} BMP_FH;

		/*! Struct for BMP Image Header */
		struct {
			uint32_t infoHeaderSize;        /*! Info header size */
			uint32_t imageWidth;            /*! Image width */
			uint32_t imageHeight;           /*! Image height */
			uint16_t colorPlane;            /*! Color plane: 1/0*/
			uint16_t bitsPerPixel;          /*! Bits per pixel: 8/16/24 */
			uint32_t compressionType;       /*! Compression type: 0 None */
			uint32_t imageSize;             /*! Image size */
			uint32_t hPixPM;                /*! Horizontal resolution */
			uint32_t vPixPM;                /*! Vertical resolution */
			uint32_t colorUsed;             /*! Color used*/
		 	uint32_t impColorUsed;          /*! Important color used */
		} BMP_IH;
};
