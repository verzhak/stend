
function main(src)

	x1 = dilate (src, 3, 7)
	x2 = erode (x1, 5, 9)
	x3 = morphologyEx (x2, 1,3,3)
	dst = Canny (x3, 10, 100, 3)

	return dst

end

