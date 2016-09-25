#ifndef __SAMPLE_FILE_IO_HELPER_H__
#define __SAMPLE_FILE_IO_HELPER_H__

#include "cocos2d.h"
#include "gesture\GeometricRecognizer.h"

class SampleFileIOHelper
{
public:
	static bool loadSample(std::string name, const char* path, DollarRecognizer::MultiStrokeGesture& multiStrokeGesture);
	static void storeSample(const DollarRecognizer::MultiStrokeGesture& multiStrokeGesture, const char* path);
};

#endif