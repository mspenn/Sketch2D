#include "SampleFileIOHelper.h"
#include <fstream>

USING_NS_CC;
using namespace DollarRecognizer;

// Scale sample when store to file
#define MAKE_SAMPLE_SCALE
// Scale factor
#define SCALE 4

/** 
 * Sample write in binary format,
 * use Line End Mark to seperate strokes in MultiStrokeGesture
 */
const int LINE_END_MARK = -1;

bool SampleFileIOHelper::loadSample(std::string name, const char* path, MultiStrokeGesture& multiStrokeGesture)
{
	int x, y;
	// read file in binary
	ifstream fin(path, ios::binary);

	bool isLoadSuccessful = false;

	// if file is open successfully
	if (fin.is_open())
	{
		multiStrokeGesture.clear();
		Path2D line;
		while (!fin.eof())
		{
			// read <x, y> from binary file
			fin.read((char*)&x, sizeof(int));
			fin.read((char*)&y, sizeof(int));

			// seperate stroke when see a Line End Mark
			if (LINE_END_MARK == x&&LINE_END_MARK == y&&!line.empty())
			{
				multiStrokeGesture.push_back(line);
				line.clear();
			}
			else
			{
				// if not see a Line End Mark, save point <x, y> as Point2D,
				// save point in a consecutive line
				line.push_back(Point2D(x, y));
			}
		}
		// mark isLoadSuccessful as true when read all the points in file
		isLoadSuccessful = true;
	}
	else
	{
		log("LOAD_TEMPLATE >> READ_FILE: %s Invalid.", path);
	}

	// close file stream
	fin.close();

	return isLoadSuccessful;
}

void SampleFileIOHelper::storeSample(const MultiStrokeGesture& multiStrokeGesture, const char* path)
{
	// save file as binary
	ofstream fout(path, ios::binary);

	// if file is open successfully
	if (fout.is_open())
	{
		// iterate lines in multiple stroke gesture 
		for (auto l = multiStrokeGesture.begin(); l != multiStrokeGesture.end(); l++)
		{
			// iterate points in a consecutive line
			for (auto p = l->begin(); p != l->end(); p++)
			{
				// if SCALE points, scale it by over the Scale Factor
#ifdef MAKE_SAMPLE_SCALE
				int x = (p->x) / SCALE, y = (p->y) / SCALE;
#else
				int x = p->x, y = p->y;
#endif
				fout.write((char*)&x, sizeof(int));
				fout.write((char*)&y, sizeof(int));
			}
			// write Line End Mark to file
			// write twice so we can read it as <x, y>
			// and check x!=LINE_END_MARK && x!=LINE_END_MARK
			fout.write((char*)&LINE_END_MARK, sizeof(int));
			fout.write((char*)&LINE_END_MARK, sizeof(int));
		}
	}
	else
	{
		log("STORE_TEMPLATE >> WRITE_FILE: %s Invalid.", path);
	}

	// close file stream
	fout.close();
}