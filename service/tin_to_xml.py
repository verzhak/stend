#!/usr/bin/python2
#coding=utf-8

import sys, os, ConfigParser, cv2

# ~/trash/Полет/N3716\ -\ Совмещено/ 32660 42560 ~/disser/data/Полет/N3716_new.xml ~/disser/data/Полет/N3716_new.avi TV_Crop8 42360 tind

dname = sys.argv[1]
from_ind = int(sys.argv[2])
to_ind = int(sys.argv[3])
dst_fname = sys.argv[4]
dst_video_fname = sys.argv[5] # Разрешение mp4 для XVID'а
tv_dname = sys.argv[6] # TV_Crop8
ind_shift = int(sys.argv[7]) # 42360
tin_postfix = sys.argv[8] # tind

video = None
fourcc = cv2.cv.CV_FOURCC("I", "Y", "U", "V")
config = ConfigParser.ConfigParser()
xml = \
[
"""<?xml version="1.0" encoding="utf-8"?>

<amv>
"""
]

for ind in range(from_ind, to_ind + 1):

	print(ind, to_ind)

	bmp_fname= "%s/%s/%u.bmp" % (dname, tv_dname, ind + ind_shift)
	bmp = cv2.imread(bmp_fname, cv2.cv.CV_LOAD_IMAGE_COLOR)

	if video == None:
		
		video = cv2.VideoWriter(dst_video_fname, fourcc, 25., (bmp.shape[1], bmp.shape[0]), True)

		if not video.isOpened():

			raise Exception("Video not opened")

	video.write(bmp)

	config.read("%s/%u%s" % (dname, ind, tin_postfix))
	
	xml.append(
"""
<frame>
    <x type="double">%f</x>
    <y type="double">%f</y>
    <h type="double">%f</h>
    <course type="double">%f</course>
    <roll type="double">%f</roll>
    <pitch type="double">%f</pitch>
    <aspect_x type="double">44.9</aspect_x>
    <aspect_y type="double">26.6</aspect_y>
    <original_aspect_x type="double">%f</original_aspect_x>
    <original_aspect_y type="double">%f</original_aspect_y>
    <coord_system type="unsigned">2</coord_system>
</frame>
"""
	% 
	(
		float(config.get("LAT_1", "Sh").replace(",", ".")), # x
		float(config.get("LON_1", "Sh").replace(",", ".")), # y
		float(config.get("Height", "H").replace(",", ".")), # h
		float(config.get("Angles", "Cr").replace(",", ".")), # course
		float(config.get("Angles", "Kr").replace(",", ".")), # roll
		float(config.get("Angles", "Tan").replace(",", ".")), # pitch
		float(config.get("Aspect", "H_CTV").replace(",", ".")), # aspect_x
		float(config.get("Aspect", "V_CTV").replace(",", ".")) # aspect_y
	))

xml.append("\n</amv>")

fl = open(dst_fname, mode = "w")
fl.writelines(xml)
fl.close()

