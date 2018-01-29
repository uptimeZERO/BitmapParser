#include "stdafx.h"
#include "BitmapParser.h"
#include <istream>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <sstream>
#include <string>
#include <string.h>
#include <vector>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <algorithm>

BitmapParser::BitmapParser(const std::string& filename)
{
	m_image = new BitmapImage();
	parseHeader(filename);
	parseDIB(filename);
	parsePixelArray(filename);
	const std::string filename2 = R"(C:\Users\pavilion.sahota\Desktop\output\output.bmp)";
	create(filename2);
}

BitmapParser::~BitmapParser()
{
	delete m_image;
}

std::vector<unsigned char> arrayOfByte(int paramInt, int allocation)
{
	std::vector<unsigned char> arrayOfByte(allocation);
	for (int i = 0;i < allocation;i++)
	{
		arrayOfByte[3 - i] = (paramInt >> (i * 8));
	}
	return arrayOfByte;
}


// NOTHING TO SEE HERE, PLEASE DISREGARD THIS FUNCTION AND MOVE ON
void justForFun() { while (true) { std::cout << "Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  " << '\r';std::this_thread::sleep_for(std::chrono::milliseconds(100));std::cout << "emes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  M" << '\r';std::this_thread::sleep_for(std::chrono::milliseconds(100));std::cout << "mes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Me" << '\r';std::this_thread::sleep_for(std::chrono::milliseconds(100));std::cout << "es  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Mem" << '\r';std::this_thread::sleep_for(std::chrono::milliseconds(100));std::cout << "s  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Meme" << '\r';std::this_thread::sleep_for(std::chrono::milliseconds(100));std::cout << "  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes" << '\r';std::this_thread::sleep_for(std::chrono::milliseconds(100));std::cout << " Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes  Memes " << '\r';std::this_thread::sleep_for(std::chrono::milliseconds(100)); } }


/*
                  []=============================================================[]
     /M\         <|| /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\//\/\/\/\/\ ||>         /M\
   <|[o]|00===00<<|| ++++++++++++++++ - Recreate the bmp file - ++++++++++++++++ ||>>00===00|[o]|>
     \W/         <|| \/\/\/\/\/\/\/\/\/\/\/\/\\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ ||>         \W/
                  []=============================================================[]
*/
void BitmapParser::create(const std::string& filename2)
{
	std::cout << "|| File re-creation ||" << std::endl;

	//defining the binary output stream
	std::ofstream ofs(filename2, std::ios::binary);

	//retreive the ID and write it to the file
	std::string id = m_image->getID();
	char* id_bytes = _strdup(id.c_str());
	ofs.write(id_bytes, 2);

	//Retrieving both header and DIB header information and storing them
	int size = m_image->getSizeOfBitmap();
	int unused = 0;
	int offset = m_image->getOffset();
	int bytes = m_image->getBytes();
	int width = m_image->getWidth();
	int height = m_image->getHeight();
	int plane = m_image->getPlane();
	int bits = m_image->getBits();
	int pac = m_image->getPac();
	int raw = m_image->getRaw();
	int resW = m_image->getResW();
	int resH = m_image->getResH();
	int colours = m_image->getColours();
	int icolours = m_image->getiColours();
	int iBits = m_image->getBits();

	//writing the header to the file
	ofs.write((char*)&size, 4);
	ofs.write((char*)&unused, 4);
	ofs.write((char*)&offset, 4);

	//writing the DIB to the file
	ofs.write((char*)&bytes, 4);
	ofs.write((char*)&width, 4);
	ofs.write((char*)&height, 4);
	ofs.write((char*)&plane, 2);

	bool RLE4 = false;
	bool RLE8 = false;
	
	std::string yesno;
	if (int(bits) == 4)
	{
		std::cout << "Would you like to incorporate RLE4 compression Y/N? : ";
		std::cin >> yesno;
		if (yesno=="Y" || yesno == "y" || yesno == "yes" || yesno == "Yes" || yesno == "YES")
		{
			RLE4 = true;
			int(pac) = 2;
		}
		else
		{
			RLE4 = false;
		}
	}
	else if (int(bits) == 8)
	{
		std::cout << "Would you like to incorporate RLE8 compression Y/N? : ";
		std::cin >> yesno;
		if (yesno == "Y" || yesno == "y" || yesno == "yes" || yesno == "Yes" || yesno == "YES")
		{
			RLE8 = true;
			int(pac) = 1;
		}
		else
		{
			RLE8 = false;
		}
	}
	
	ofs.write((char*)&bits, 2);
	ofs.write((char*)&pac, 4);
	ofs.write((char*)&raw, 4);
	ofs.write((char*)&resW, 4);
	ofs.write((char*)&resH, 4);
	ofs.write((char*)&colours, 4);
	ofs.write((char*)&icolours, 4);

	//define the value of padding
	const int padding = 0;
	int pad = padding;
	int offset2 = m_image->getOffset();
	//retrieve the offset, if it exceeds 54 then fill the blanks with the excess DIB header
	if (offset2 > 54)
	{
		offset2 -= 54;
		std::vector<int> blanks = m_image->getBlanks();
		for (int i = 0; i < offset2;i++)
		{
			ofs.write((char*)&blanks.at(i), 1);
		}
	}

	//std::cout << "Pixels to process: " << b.size() << std::endl;

	//switch casing in order to process and recreate bitmap files of reach colour depth
	switch (iBits)
	{

		//32-bit colour depth bitmap recreation
	case 32:
	{
		//retrieving all vectors of BGRA values, all of which form a pixel
		std::vector<int> b = m_image->getB();
		std::vector<int> g = m_image->getG();
		std::vector<int> r = m_image->getR();
		std::vector<int> a = m_image->getAlpha();

		//loop this code until there are no more pixels to write to the file
		for (size_t i = 0; i < b.size(); i++)
		{
			//This makes it look prettier but slows down the processing
			//std::cout << "Completed pixel: " << i << "/" << b.size() << '\r';

			//defines the buffer which will temporarily hold pixel data until it is written to the file
			int blue = b.at(i);
			int green = g.at(i);
			int red = r.at(i);
			int alpha = a.at(i);

			//write the pixel data to the file
			ofs.write((char*)&blue, 1);
			ofs.write((char*)&green, 1);
			ofs.write((char*)&red, 1);
			ofs.write((char*)&alpha, 1);
		}
		//close the stream and save the changes
		ofs.close();
		//data display
		std::cout << "File re-creation completed!" << std::endl;
		std::cout << std::endl;
		break;
	}

	//24-bit colour depth bitmap recreation
	case 24:
	{
		//defining a counter
		int t = 1;
		//retrieving all vectors of BRG values all of which form a pixel
		std::vector<int> b = m_image->getB();
		std::vector<int> g = m_image->getG();
		std::vector<int> r = m_image->getR();

		//loop until there are no more pixels to write
		for (size_t i = 0; i < b.size(); i++)
		{
			//This makes it look prettier but slows down the processing
			//std::cout << "Completed pixel: " << i << "/" << b.size() << '\r';

			//defines the buffer which will temporarily hold the pixel data until it is written to the file
			int blue = b.at(i);
			int green = g.at(i);
			int red = r.at(i);

			

			//write the pixel data to the file
			ofs.write((char*)&blue, 1);
			ofs.write((char*)&green, 1);
			ofs.write((char*)&red, 1);

			//if it has reached the end of the row
			if (t == m_image->getWidth())
			{
				//if padding is required
				if (m_image->getPadding() == 1)
				{
					ofs.write((char*)&pad, 1);
				}
				else if (m_image->getPadding() == 2)
				{
					ofs.write((char*)&pad, 1);
					ofs.write((char*)&pad, 1);
				}
				else if (m_image->getPadding() == 3)
				{
					ofs.write((char*)&pad, 1);
					ofs.write((char*)&pad, 1);
					ofs.write((char*)&pad, 1);
				}
				t = 0;
			}
			t++;
		}
		//close the stream and save the changes
		ofs.close();
		//data display
		std::cout << "File re-creation completed!" << std::endl;
		std::cout << std::endl;
		break;
	}

	//16-bit colour depth bitmap recreation
	case 16:
	{
		//TODO: finish this section of code
		std::cout << "BitmapParser is currently unable to process bmp files with a 16bit colour depth" << std::endl;
		break;
	}

	//8-bit colour depth bitmap recreation
	case 8:
	{
		//counter
		int t = 1;
		//retriving the vector of pixels
		std::vector<int> pixel8 = m_image->getPixel8();

		//loop until there are no more pixels to write
		for (size_t i = 0; i < pixel8.size(); i++)
		{
			//This makes it look prettier but slows down the processing
			//std::cout << "Completed pixel: " << i << "/" << b.size() << '\r';

			//retrieve pixel data
			std::vector<int> pixel8 = m_image->getPixel8();
			int pixel8char = pixel8.at(i);

			//write the pixel to the file
			ofs.write((char*)&pixel8char, 1);

			//if it has reached the end of the row
			if (t == m_image->getWidth())
			{
				//if padding is required
				if (m_image->getPadding() == 1)
				{
					ofs.write((char*)&pad, 1);
				}
				else if (m_image->getPadding() == 2)
				{
					ofs.write((char*)&pad, 1);
					ofs.write((char*)&pad, 1);
				}
				else if (m_image->getPadding() == 3)
				{
					ofs.write((char*)&pad, 1);
					ofs.write((char*)&pad, 1);
					ofs.write((char*)&pad, 1);
				}
				t = 0;
			}
			t++;
		}
		//close the stream and save changes
		ofs.close();
		//data display
		std::cout << "File re-creation completed!" << std::endl;
		std::cout << std::endl;
		break;
	}

	//4-bit colour depth bitmap recreation
	case 4:
	{
		/*
		//identifying the amount of padding required (if any).
		int padRemainder = ((m_image->getWidth() / 2) % 4);
		int padding = (4 - padRemainder);
		if (padding == 4) padding = 0;

		//counter
		int t = 1;
		//retrieving the vector of pixels
		std::vector<int> pixel4 = m_image->getPixel4();
		//loop until there are no more pixels to write
		for (size_t i = 0; i < pixel4.size();)
		{
			//takes a 4-bit pixel out of the vector
			int pixel4char1 = pixel4.at(i++);
			//break out of loop if there are no more pixels to write
			if (i == pixel4.size()) break;
			//takes another 4-bit pixel out of the vector
			int pixel4char2 = pixel4.at(i++);
			//define a buffer
			int buffer = 0;
			//use the buffer to merge the 2 4-bit pixels into a 2 pixel byte
			buffer = buffer | pixel4char1;
			buffer = buffer << 4;

			buffer = buffer | pixel4char2;
			//write the 2 pixel byte to the file
			ofs.put(buffer);
			//if it has reached the end of the row
			if (t == m_image->getWidth())
			{
				//if padding is required
				if (padding == 1)
				{
					ofs.write((char*)&pad, 1);
				}
				else if (padding == 2)
				{
					ofs.write((char*)&pad, 1);
					ofs.write((char*)&pad, 1);
				}
				else if (padding == 3)
				{
					ofs.write((char*)&pad, 1);
					ofs.write((char*)&pad, 1);
					ofs.write((char*)&pad, 1);
				}
				t = 0;
			}
			t++;
			*/

		if (RLE4 == true)
		{
			int counter = 1;
			int counter2 = 1;
			int buffer = 0;
			std::vector<int> pixel4 = m_image->getPixel4();
			for (size_t i = 0; i < pixel4.size(); i++)
			{
				if (pixel4.at(i) == pixel4.at(i + counter))
				{
					while (pixel4.at(i) == pixel4.at(i + counter))
					{
						counter++;
					}
					
					ofs.write((char*)&counter, 1);
					buffer = pixel4.at(i);
					ofs.write((char*)&buffer, 1);
					i += counter;
					counter = 1;
				}
				else
				{
					if (pixel4.at(i+counter2) != pixel4.at(i + (counter2+1)))
					{
						while (pixel4.at(i + counter2) != pixel4.at(i + (counter2 + 1)))
						{
							counter2++;
						}
						buffer = 0;
						ofs.write((char*)&buffer, 1);
						ofs.write((char*)&counter2, 1);
						for (int b = 0; b < counter2; b++)
						{
							ofs.write((char*)&pixel4.at(i+b), 1);
						}
						i += counter2;
					}
					counter2 = 1;
					if (i == pixel4.size()) break;
				}
			}
		}
		else
		{
			std::vector<int> pixel4 = m_image->getPixel4();
			for (size_t i = 0; i < pixel4.size(); i++)
			{
				int buffer = pixel4.at(i);
				if (i == pixel4.size()) break;
				ofs.write((char*)&buffer, 1);
			}
		}
		/*
		std::vector<int> pixel4 = m_image->getPixel4();
		for (size_t i = 0; i < pixel4.size(); i++)
		{
			int buffer = pixel4.at(i);
			if (i == pixel4.size()) break;
			ofs.write((char*)&buffer, 1);
		}
		*/
		break;
	}

	default:
	{
		break;
	}
	}
	//close the stream dna save the changes
	ofs.close();

	//NOTHING TO SEE HERE, MOVE ALONG NOW
	justForFun();
}

/*
                  []==============================================================[]
     /M\         <|| /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\ ||>         /M\
   <|[o]|00===00<<|| ++++++++++++++++ - Parses the main header - ++++++++++++++++ ||>>00===00|[o]|>
     \W/         <|| \/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ ||>         \W/
                  []==============================================================[]
*/
void BitmapParser::parseHeader(const std::string& filename)
{
	//binary file stream
	std::ifstream ifs(filename, std::ios::binary);

	//define variables
	char * header_id = new char[2];
	unsigned int header_size;
	char * header_unused = new char[4];
	unsigned int header_offset;

	//reads the data into those variables
	ifs.read(header_id, 2);
	ifs.read((char*)&header_size, 4);
	ifs.read(header_unused, 4);
	ifs.read((char*)&header_offset, 4);

	//data display
	std::cout << "|| BMP Header ||" << std::endl;
	std::string id(header_id, 2);
	std::cout << "ID: " << id << std::endl;
	std::cout << "Size: " << header_size << "B";
	if (header_size / 1000 > 0) std::cout << " || " << header_size / 1000 << "KB";
	if (header_size / 1000000 > 0) std::cout << " || " << header_size / 1000000 << "MB";
	if (header_size / 1000000000 > 0) std::cout << " || " << header_size / 1000000000 << "GB";
	std::cout << std::endl;
	std::cout << "Offset: " << header_offset << std::endl;
	std::cout << std::endl;

	//stores the variables in the class "m_image"
	m_image->setID(id);
	m_image->setSizeOfBitmap(header_size);
	m_image->setOffset(header_offset);
}

/*
                  []=============================================================[]
     /M\         <|| /\/\/\/\\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\ ||>         /M\
   <|[o]|00===00<<|| ++++++++++++++++ - Parses the DIB header - ++++++++++++++++ ||>>00===00|[o]|>
     \W/         <|| \/\/\/\/\\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ ||>         \W/
                  []=============================================================[]
*/
void BitmapParser::parseDIB(const std::string& filename)
{
	//binary file stream
	std::ifstream ifs(filename, std::ios::binary);

	//define variables
	char * skip_header = new char[14];
	unsigned int dib_bytes;
	unsigned int dib_pWidth;
	unsigned int dib_pHeight;
	unsigned short dib_plane;
	unsigned short dib_bits_per_pixel;
	unsigned int dib_bi_pixel_array_compression;
	unsigned int dib_raw;
	unsigned int dib_horizontal_resolution;
	unsigned int dib_verticle_resolution;
	unsigned int dib_colours;
	unsigned int dib_important_colours;

	//skips the reading straight to the offset
	ifs.read(skip_header, 14);
	delete skip_header;

	//reads the data into those variables
	ifs.read((char*)&dib_bytes, 4);
	ifs.read((char*)&dib_pWidth, 4);
	ifs.read((char*)&dib_pHeight, 4);
	ifs.read((char*)&dib_plane, 2);
	ifs.read((char*)&dib_bits_per_pixel, 2);
	ifs.read((char*)&dib_bi_pixel_array_compression, 4);
	ifs.read((char*)&dib_raw, 4);
	ifs.read((char*)&dib_horizontal_resolution, 4);
	ifs.read((char*)&dib_verticle_resolution, 4);
	ifs.read((char*)&dib_colours, 4);
	ifs.read((char*)&dib_important_colours, 4);

	//any extra data stored in the DIB will be stored for recreation later on
	int offset = m_image->getOffset();
	if (offset > 54)
	{
		offset -= 54;
		int buffer;
		for (int i = 0; i < offset; i++)
		{
			ifs.read((char*)&buffer, 1);
			m_image->addBlanks(buffer);
			buffer = 0;
		}
	}

	//data display
	std::cout << "|| DIB Header ||" << std::endl;
	std::cout << "Bytes: " << dib_bytes << std::endl;
	std::cout << "Width: " << dib_pWidth << std::endl;
	std::cout << "Height: " << dib_pHeight << std::endl;
	std::cout << "No. of colour plane/s: " << dib_plane << std::endl;
	std::cout << "Bits per pixel: " << dib_bits_per_pixel << std::endl;
	std::cout << "Pixel array compression: " << dib_bi_pixel_array_compression << std::endl;
	std::cout << "Raw bitmap data: " << dib_raw << " bytes" << std::endl;
	std::cout << "Horizontal  print resolution: " << dib_horizontal_resolution << " pixels" << std::endl;
	std::cout << "Verticle print resolution: " << dib_verticle_resolution << " pixels" << std::endl;
	std::cout << "Colours in the palette: " << dib_colours << std::endl;
	std::cout << "Important colours (0 means all): " << dib_important_colours << std::endl;
	std::cout << std::endl;

	//stores the variables in the class "m_image"
	m_image->setBytes(dib_bytes);
	m_image->setWidth(dib_pWidth);
	m_image->setHeight(dib_pHeight);
	m_image->setPlane(dib_plane);
	m_image->setBits(dib_bits_per_pixel);
	m_image->setPac(dib_bi_pixel_array_compression);
	m_image->setRaw(dib_raw);
	m_image->setResH(dib_horizontal_resolution);
	m_image->setResW(dib_verticle_resolution);
	m_image->setColours(dib_colours);
	m_image->setiColours(dib_important_colours);
}

/*
                  []====================================================================================[]
     /M\         <|| /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\ ||>         /M\
   <|[o]|00===00<<|| ++++++++++++++++ - Parses the pixel array for each colour depth - ++++++++++++++++ ||>>00===00|[o]|>
     \W/         <|| \/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ ||>         \W/
                  []====================================================================================[]
*/
void BitmapParser::parsePixelArray(const std::string& filename)
{
	//binary file stream
	std::ifstream ifs(filename, std::ios::binary);

	//skips the reading straight to the offset
	int skip = m_image->getOffset();
	char * skip_header = new char[skip];
	ifs.read(skip_header, skip);
	delete skip_header, skip;

	//defining variables to identify what kind of bmp file needs to be processed
	int pad = 0;
	int t = 0;
	int bit = m_image->getBits();

	//data display
	std::cout << "|| Pixel Array ||" << std::endl;
	std::cout << "Processing..." << std::endl;

	//processes the pixel array according to the structure defined by the colour depth
	switch (bit)
	{

	//32 bits per pixel, each pixel takes up 4 bytes and has 4,294,967,296 distinct colours
	case 32:
	{
		//defining variables to be used as temporary buffers
		int blueBuffer;
		int greenBuffer;
		int redBuffer;
		int alphaBuffer;

		//loop through the pixel array till the end of file
		while (!ifs.eof())
		{
			//read the data into the buffers
			ifs.read((char*)&blueBuffer, 1);
			ifs.read((char*)&greenBuffer, 1);
			ifs.read((char*)&redBuffer, 1);
			ifs.read((char*)&alphaBuffer, 1);

			//break out of look if its the end of the file
			if (ifs.eof()) break;

			//store the pixel data from the buffers into the class variables
			m_image->add2b(blueBuffer);
			m_image->add2g(greenBuffer);
			m_image->add2r(redBuffer);
			m_image->addAlpha(alphaBuffer);

			//resets the values in the buffers
			blueBuffer = 0;
			greenBuffer = 0;
			redBuffer = 0;
			alphaBuffer = 0;
		}

		//data display
		std::cout << "Processing completed!" << std::endl;
		std::cout << std::endl;

		//defines the amount of padding needed
		m_image->setPadding(0);
		break;
	}

	//24 bits per pixel, each pixel takes up 3 bytes and has 16,777,216 distinct colours
	case 24:
	{
		//determining how much padding is required for a 4 byte allignment
		int padRemainder = ((m_image->getWidth() * 3) % 4);
		int padding = 4 - padRemainder;
		if (padRemainder == 0) padding = 0;

		//defining variables to be used as temporary buffers
		int blueBuffer;
		int greenBuffer;
		int redBuffer;

		//loop through the pixel array till the end of file
		while (!ifs.eof())
		{
			//if padding is required
			if (padding > 0)
			{
				//if its the end of the row
				if (t == m_image->getWidth())
				{
					//loop until the padding has been skipped
					for (int i = 0; i < padding;i++)
					{
						ifs.read((char*)&pad, 1);
						pad = 0;
					}
					t = 0;
				}
			}

			//read the data into the buffers
			ifs.read((char*)&blueBuffer, 1);
			ifs.read((char*)&greenBuffer, 1);
			ifs.read((char*)&redBuffer, 1);

			//if end of file, break out of while loop
			if (ifs.eof()) break;

			//stores the pixel data from the buffers into the class variables
			m_image->add2b(blueBuffer);
			m_image->add2g(greenBuffer);
			m_image->add2r(redBuffer);

			//resets the values in the buffers
			blueBuffer = 0;
			greenBuffer = 0;
			redBuffer = 0;
			t++;
		}

		//data display
		std::cout << "Processing completed!" << std::endl;
		std::cout << std::endl;

		//define the amount of padding needed for file recreation
		m_image->setPadding(padding);
		break;
	}

	//16 bits per pixel, each pixel takes up 2 bytes and has 65,536 distinct colours
	case 16:
	{
		//defining variables to be used as temporary buffers
		int firstIndex;
		int secondIndex;
		int pixelBuffer;

		//loop through the pixel array until the end of the file
		while (!ifs.eof())
		{
			//read the pixel data into the buffer
			ifs.read((char*)&pixelBuffer, 1);
			//if end of file, break out of the loop
			if (ifs.eof()) break;
			//extracts the last 4 bits of the byte using a bitmask
			firstIndex = pixelBuffer & 0xF0;
			firstIndex = firstIndex >> 4;
			//extracts the first 4 bits of the byte using a bitmask
			secondIndex = pixelBuffer & 0x0F;
			secondIndex = secondIndex >> 0;
			//stores the data from the buffers into the class variables
			m_image->add2b(firstIndex);
			m_image->add2g(secondIndex);
			//resets the value in the buffer
			pixelBuffer = 0;

			//read the pixel data into the buffer
			ifs.read((char*)&pixelBuffer, 1);
			//if end of file, break out of the loop
			if (ifs.eof()) break;
			//extracts the last 4 bits of the byte using a bitmask
			firstIndex = pixelBuffer & 0xF0;
			firstIndex = firstIndex >> 4;
			//extracts the first 4 bits of the byte using a bitmask
			secondIndex = pixelBuffer & 0x0F;
			secondIndex = secondIndex >> 0;
			//stores the data from the buffers into the class variables
			m_image->add2r(firstIndex);
			m_image->addAlpha(secondIndex);
			//resets the value in the buffer
			pixelBuffer = 0;
		}
		//data display
		std::cout << "Processing completed!" << std::endl;
		std::cout << std::endl;
		//define the amount of padding needed
		m_image->setPadding(0);
		break;
	}

	//8 bits per pixel, each pixel takes up 1 byte and has 256 distinct colours
	case 8:
	{
		//determining the amount of padding required for a 4 byte allignment
		int padRemainder = (m_image->getWidth() % 4);
		int padding = (4 - padRemainder);
		if (padding == 4) padding = 0;

		//defining a temporary buffer to store the pixel data
		int pixelBuffer;

		//loop through the pixel array until the end of the file
		while (!ifs.eof())
		{
			//if padding is required
			if (padding > 0)
			{
				//if it has reached the end of row
				if (t == m_image->getWidth())
				{
					//loop until the padding has been skipped
					for (int i = 0; i < padding;i++)
					{
						ifs.read((char*)&pad, 1);
						pad = 0;
					}
					t = 0;
				}
			}
			//read the pixel data into the buffer
			ifs.read((char*)&pixelBuffer, 1);
			//if its the end of the file, break out of the loop
			if (ifs.eof()) break;
			//store the data from the buffers into the class variables
			m_image->addPixel8(pixelBuffer);
			//resets the value in the buffer
			pixelBuffer = 0;
			t++;
		}
		//data display
		std::cout << "Processing completed!" << std::endl;
		std::cout << std::endl;
		//define the amount of padding needed
		m_image->setPadding(padding);
		break;
	}

	//4 bits per pixel, 1 byte stores 2 pixels in this case, 16 distinct colours
	case 4:
	{
		/*
		//determining how much padding there is that needs to be skipped in order to read the pixel array correctly
		int padRemainder = ((m_image->getWidth() / 2) % 4);
		int padding = (4 - padRemainder);
		if (padding == 4) padding = 0;

		//defining a temporary buffer to store the pixel data
		int pixelBuffer;
		int firstIndex;
		int secondIndex;

		//loop through the pixel array until the end of the file
		while (!ifs.eof())
		{
			//if padding is required
			if (padding > 0)
			{
				//if it has reached the end of the row
				if (t == m_image->getWidth())
				{
					//loop until the padding has been skipped
					for (int i = 0; i < padding;i++)
					{
						ifs.read((char*)&pad, 1);
						pad = 0;
					}
					t = 0;
				}
			}
			//read the pixel data into the buffer, or in this case, 2 pixels worth of data
			ifs.read((char*)&pixelBuffer, 1);
			//if it has reached the end of the file, break out of the loop
			if (ifs.eof()) break;
			//extracts the last 4 bits from the byte
			firstIndex = pixelBuffer & 0xF0;
			firstIndex = firstIndex >> 4;
			//extracts the first 4 bits from the byte
			secondIndex = pixelBuffer & 0x0F;
			secondIndex = secondIndex >> 0;
			//stores both pixel data into the class variables
			m_image->addPixel4(firstIndex);
			m_image->addPixel4(secondIndex);
			t++;
		}
		*/
		int buffer = 0;
		while (!ifs.eof())
		{
			ifs.read((char*)&buffer, 1);
			if (ifs.eof()) break;
			m_image->addPixel4(buffer);
			buffer = 0;
		}
		//data display
		std::cout << "Processing completed!" << std::endl;
		std::cout << std::endl;
		//define the amount of padding needed
		//m_image->setPadding(padding);
		break;
	}

	default:
		std::cout << "BitmapParser is unable to determine the file's properties, please try another file." << std::endl;
		break;
	}
}
