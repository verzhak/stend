
#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "all.hpp"

class CMatrix : public Mat
{
	static unsigned depth_to_unsigned(const int depth);

	void init(const unsigned height, const unsigned width, const unsigned ch_num, const int element_type);

	public:

		CMatrix();
		CMatrix(const unsigned height, const unsigned width, const unsigned ch_num, const int element_type);
		CMatrix(const char * fname); // TODO В QString
		CMatrix(const Mat & __mtx);
		CMatrix(const CMatrix & __mtx, const bool is_copy_data = true);
		~CMatrix();

		void save(const char * fname);
		void get_value(const unsigned row, const unsigned column, const unsigned channel, void * value);
		void set_value(const unsigned row, const unsigned column, const unsigned channel, const void * value);
		inline unsigned height() const { return rows; };
		inline unsigned width() const { return cols; };
		inline unsigned number_of_channels() const { return channels(); };
		unsigned element_type() const;
		inline void * pointer_to_data() const { return data; };
		void * pointer_to_row(const unsigned row);
		inline unsigned size() const { return total() * elemSize(); };
};

class CMatrices
{
	QList<CMatrix *> matrices;

	public:

		CMatrices();
		~CMatrices();

		void init();
		void clear();
		uint64_t memory();
};

#endif

