#ifndef __SAMPLE_FILE_IO_HELPER_H__
#define __SAMPLE_FILE_IO_HELPER_H__

#include "cocos2d.h"
#include "gesture\GeometricRecognizer.h"

/**
 * The Sample File IO Read/Write Helper
 */
class SampleFileIOHelper
{
public:

	/**
	Load multiple stroke gesture sample file from path.
	@param	name	assign a name to the sample to be loaded 
	@param	path	target sample file path
	@param	multiStrokeGesture	load sample as multiple stroke gesture
	@see DollarRecognizer::MultiStrokeGesture
	@return true    Load success
	@return false   Initialize failed, app terminate.
	*/
	static bool loadSample(std::string name, const char* path, DollarRecognizer::MultiStrokeGesture& multiStrokeGesture);

	/**
	Save multiple stroke gesture sample file to path
	@param	multiStrokeGesture	multiple stroke gesture structure
	@see DollarRecognizer::MultiStrokeGesture
	@param	path	target sample file path to save
	*/
	static void storeSample(const DollarRecognizer::MultiStrokeGesture& multiStrokeGesture, const char* path);
};

#endif	/* __SAMPLE_FILE_IO_HELPER_H__ */