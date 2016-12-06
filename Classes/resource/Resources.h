#ifndef __RESOURCES_H__
#define __RESOURCES_H__

#define RES_IMAGE(img)				"images/"#img
#define RES_FONT(font)				"fonts/"#font
#define RES_GESTURE_THUMB(ges)		"gestures/mesh/"#ges"/"#ges".png"

#define RES_GESTURE_MESH(ges)		"gestures/mesh/"#ges"/"#ges".ges"
#define RES_GESTURE_MARKER(ges)		"gestures/marker/"#ges"/"#ges".ges"

#define RES_ITEM_DICT_L				L"Footage"
#define RES_ITEM_DICT				"Footage"

#define GEOMETRY_SAMPLE_SEPERATOR	"::"

#define PAT_GESTURE_FILTER			"gestures/%s/*.ges"
#define PAT_GESTURE_PATH			"gestures/%s/%s"

#define DEFAULT_FONT				RES_FONT(Marker Felt.ttf)
#endif