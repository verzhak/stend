
function main(src)

	x1 = dft (src)
	--dst = idft (x1)
	dst = normalize (x1)

	return dst

end

