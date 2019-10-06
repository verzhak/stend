
import struct

class CMTR:

	def __init__(self, fname):

		self.__fl = open(fname, "rb")
		self.__read_header()
		self.__data = self.__fl.read()

	def __read_header(self):

		raw = self.__fl.read(40)
		param = struct.unpack("IiiiifiiII", raw)

		self.file_id = param[0]
		self.x_min = param[1]
		self.y_min = param[2]
		self.x_max = param[3]
		self.y_max = param[4]
		self.size_el_mat = param[5]
		self.h_min = param[6]
		self.h_max = param[7]
		self.width = param[8]
		self.height = param[9]

	def print_header(self):

		print("file_id = ", self.file_id)
		print("x_min = ", self.x_min)
		print("y_min = ", self.y_min)
		print("x_max = ", self.x_max)
		print("y_max = ", self.y_max)
		print("size_el_mat = ", self.size_el_mat)
		print("h_min = ", self.h_min)
		print("h_max = ", self.h_max)
		print("width = ", self.width)
		print("height = ", self.height)

	def read_elem(self, x, y):

		y_step = (self.y_max - self.y_min) / (self.height - 1)
		x_step = (self.x_max - self.x_min) / (self.width - 1)

		y_raw = (y - self.y_min) / y_step
		x_raw = (x - self.x_min) / x_step

		y_raw_int = int(y_raw)
		x_raw_int = int(x_raw)

		d_y = y_raw - y_raw_int
		d_x = x_raw - x_raw_int

		def get_h(x, y):

			ind = (y * self.height + x) * 4
			bt = self.__data[ind : ind + 4]

			return struct.unpack("f", bt)[0]

		h = \
			(1 - d_y) * (1 - d_x) * get_h(x_raw_int, y_raw_int) + \
			d_y * (1 - d_x) * get_h(x_raw_int + 1, y_raw_int) + \
			(1 - d_y) * d_x * get_h(x_raw_int, y_raw_int + 1) + \
			d_y * d_x * get_h(x_raw_int + 1, y_raw_int + 1)

		return h

