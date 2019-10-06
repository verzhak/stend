#!/usr/bin/python

import sys, struct
from mtr import CMTR

src_fname = sys.argv[1]
dst_fname = sys.argv[2]
mtr_fname = sys.argv[3]

mtr = CMTR(mtr_fname)

fl = open(src_fname, mode = "rb")
raw_param = bytearray(48)
xml = \
[
"""<?xml version="1.0" encoding="utf-8"?>

<amv>
"""
]

while fl.readinto(raw_param):

    param = struct.unpack("dddddd", raw_param)

    x = param[0]
    y = param[1]
    h = param[2]
    course = param[3]
    roll = param[4]
    pitch = param[5]

    d_h = mtr.read_elem(x, y)

    xml.append(
"""
<frame>
    <x type="double">%f</x>
    <y type="double">%f</y>
    <h type="double">%f</h>
    <course type="double">%f</course>
    <roll type="double">%f</roll>
    <pitch type="double">%f</pitch>
    <aspect_x type="double">69.1</aspect_x>
    <aspect_y type="double">43.7</aspect_y>
    <original_x type="double">%f</original_x>
    <original_y type="double">%f</original_y>
    <original_h type="double">%f</original_h>
    <original_course type="double">%f</original_course>
    <original_roll type="double">%f</original_roll>
    <original_pitch type="double">%f</original_pitch>
    <coord_system type="unsigned">3</coord_system>
</frame>
"""
        % (
        x, y, h, - course, - roll, pitch,
        x, y, h, course, roll, pitch))

xml.append("\n</amv>")
fl.close()

fl = open(dst_fname, mode = "w")
fl.writelines(xml)
fl.close()

