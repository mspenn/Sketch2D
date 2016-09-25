#include "SampleFileIOHelper.h"
#include <fstream>

USING_NS_CC;
using namespace DollarRecognizer;

#define MAKE_SAMPLE_SCALE
#define SCALE 4

const int LINE_END_MARK = -1;

bool SampleFileIOHelper::loadSample(std::string name, const char* path, MultiStrokeGesture& multiStrokeGesture)
{
	int x, y;
	ifstream fin(path, ios::binary);
	bool isLoadSuccessful = false;
	if (fin.is_open())
	{
		multiStrokeGesture.clear();
		Path2D line;
		while (!fin.eof())
		{
			fin.read((char*)&x, sizeof(int));
			fin.read((char*)&y, sizeof(int));
			if (LINE_END_MARK == x&&LINE_END_MARK == y&&!line.empty())
			{
				multiStrokeGesture.push_back(line);
				line.clear();
			}
			else
			{
				line.push_back(Point2D(x, y));
			}
		}
		isLoadSuccessful = true;
	}
	else
	{
		log("LOAD_TEMPLATE >> READ_FILE: %s Invalid.", path);
	}
	fin.close();
	return isLoadSuccessful;
}

void SampleFileIOHelper::storeSample(const MultiStrokeGesture& multiStrokeGesture, const char* path)
{
	ofstream fout(path, ios::binary);
	if (fout.is_open())
	{
		for (auto l = multiStrokeGesture.begin(); l != multiStrokeGesture.end(); l++)
		{
			for (auto p = l->begin(); p != l->end(); p++)
			{

#ifdef MAKE_SAMPLE_SCALE
				int x = (p->x) / SCALE, y = (p->y) / SCALE;
#else
				int x = p->x, y = p->y;
#endif
				fout.write((char*)&x, sizeof(int));
				fout.write((char*)&y, sizeof(int));
			}
			fout.write((char*)&LINE_END_MARK, sizeof(int));
			fout.write((char*)&LINE_END_MARK, sizeof(int));
		}
	}
	else
	{
		log("STORE_TEMPLATE >> WRITE_FILE: %s Invalid.", path);
	}
	fout.close();
}