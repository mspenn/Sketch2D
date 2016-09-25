#ifndef __RESOURCES_H__
#define __RESOURCES_H__

#define RES_IMAGE(img)				"images/"#img
#define RES_FONT(font)				"fonts/"#font
#define RES_GESTURE_THUMB(ges)		"gestures/mesh/"#ges"/"#ges".png"
//#define RES_GESTURE_MESH(ges, id)	"gestures/mesh/"#ges"/"#ges"."#id".ges"
#define RES_GESTURE_MESH(ges)	"gestures/mesh/"#ges"/"#ges".ges"
#define RES_GESTURE_MARKER(ges)	"gestures/marker/"#ges"/"#ges".ges"

#define RES_ITEM_DICT_L L"Footage"
#define RES_ITEM_DICT   "Footage"

#define GEOMETRY_SAMPLE_SEPERATOR "::"

#endif