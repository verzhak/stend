
function main(src)

	x1 = dilate (src, 3, 7)
	x2 = erode (src, 5, 9)
	x3 = morphologyEx (x2, 1,3,3)
	dst = morphologyEx (x3, 2,3,3)

	return dst

end

