#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>

class incorrect_matrix_size : public std::exception { // ¬ведены отрицательные размеры матрицы, когда таких размеров не предусмотрено
protected:
	const char* message;
public:
	incorrect_matrix_size() {
		this->message = "Incorrect matrix sizes";
	}
	const char* what() const noexcept override {
		return this->message;
	}
};

class different_size_matrices : public incorrect_matrix_size { // Ќа вход были отправлены матрицы с разными размерами, когда ожидались одинаковые
public:
	different_size_matrices() {
		this->message = "The sizes of the matrices that were sent to the method (function) are different";
	}
};

class incorrect_matrix_sizes_for_multiplication : public incorrect_matrix_size { // Ќа вход были отправлены матрицы с разными размерами дл€ умножени€, когда ожидались одинаковые
public:
	incorrect_matrix_sizes_for_multiplication() {
		this->message = "For matrix multiplication, the number of columns in the first matrix must be equal to the number of rows in the second matrix.";
	}
};

class Matrix {
private:
	int count_line;
	int count_column;
	int** matrix;

	int Random(int min, int max) {
		return min + rand() % (max - min);
	}

public:
	Matrix() {
		count_line = 1;
		count_column = 1;
		matrix = new int* [1];
		matrix[0] = new int[1];
		matrix[0][0] = 0;
	}
	Matrix(int value_line, int value_column) : count_line(value_line), count_column(value_column) {
		if (value_line < 0 || value_column < 0) throw incorrect_matrix_size();
		matrix = new int* [count_line];
		for (int i = 0; i < count_line; i++) {
			matrix[i] = new int[count_column];
		}
		for (int i = 0; i < count_line; i++) {
			for (int j = 0; j < count_column; j++) {
				matrix[i][j] = 0;
			}
		}
	}
	~Matrix() {
		for (int i = 0; i < count_line; i++) {
			delete[] matrix[i];
		}
		delete[] matrix;
		count_line = 0;
		count_column = 0;
	}

	Matrix(const Matrix& other) {
		this->count_line = other.count_line;
		this->count_column = other.count_column;
		this->matrix = new int* [this->count_line];
		for (int i = 0; i < other.count_line; i++) {
			this->matrix[i] = new int[this->count_column];
		}
		for (int i = 0; i < other.count_line; i++) {
			for (int j = 0; j < other.count_column; j++) {
				this->matrix[i][j] = other.matrix[i][j];
			}
		}
	}

	void Generation() {
		for (int i = 0; i < count_line; i++) {
			for (int j = 0; j < count_column; j++) {
				matrix[i][j] = Random(0, 20);
			}
		}
	}

	static Matrix transposition(Matrix& matrix) {
		Matrix result(matrix.count_column, matrix.count_line);
		for (int i = 0; i < result.count_line; i++) {
			for (int j = 0; j < result.count_column; j++) {
				result.matrix[i][j] = matrix.matrix[j][i];
			}
		}
		return result;
	}

	void operator=(const Matrix& other) {
		if (this->matrix != nullptr) {
			for (int i = 0; i < this->count_column; i++) {
				delete[] this->matrix[i];
			}
			delete[] this->matrix;
			this->count_line = 0;
			this->count_column = 0;
		}
		this->count_line = other.count_line;
		this->count_column = other.count_column;
		this->matrix = new int* [other.count_line];
		for (int i = 0; i < other.count_line; i++) {
			this->matrix[i] = new int[other.count_column];
		}
		for (int i = 0; i < other.count_line; i++) {
			for (int j = 0; j < other.count_column; j++) {
				this->matrix[i][j] = other.matrix[i][j];
			}
		}
	}
	friend Matrix operator-(Matrix& lhs, Matrix& rhs);
	friend Matrix operator+(Matrix& lhs, Matrix& rhs);
	friend Matrix operator*(Matrix& lhs, Matrix& rhs);
	friend std::ostream& operator<< (std::ostream& out, const Matrix& matrix);
};

Matrix operator-(Matrix& lhs, Matrix& rhs) {
	if ((lhs.count_line != rhs.count_line) || (lhs.count_column != rhs.count_column)) {
		throw different_size_matrices();
	}
	Matrix result(lhs.count_line, lhs.count_column);
	for (int i = 0; i < lhs.count_line; i++) {
		for (int j = 0; j < lhs.count_column; j++) {
			result.matrix[i][j] = lhs.matrix[i][j] - rhs.matrix[i][j];
		}
	}
	return result;
}

Matrix operator+(Matrix& lhs, Matrix& rhs) {
	if ((lhs.count_line != rhs.count_line) || (lhs.count_column != rhs.count_column)) {
		throw different_size_matrices();
	}
	Matrix result(lhs.count_line, lhs.count_column);
	for (int i = 0; i < lhs.count_line; i++) {
		for (int j = 0; j < lhs.count_column; j++) {
			result.matrix[i][j] = lhs.matrix[i][j] + rhs.matrix[i][j];
		}
	}
	return result;
}

Matrix operator*(Matrix& lhs, Matrix& rhs) {
	if (lhs.count_column == rhs.count_line) {
		Matrix result(lhs.count_line, rhs.count_column);
		for (int i = 0; i < result.count_line; i++) {
			for (int j = 0; j < result.count_column; j++) {
				for (int k = 0; k < lhs.count_column; k++) {
					result.matrix[i][j] += lhs.matrix[i][k] * rhs.matrix[k][j];
				}
			}
		}
		return result;
	}
	else throw incorrect_matrix_sizes_for_multiplication();
}

std::ostream& operator<< (std::ostream& out, const Matrix& matrix) {
	for (int i = 0; i < matrix.count_line; i++) {
		for (int j = 0; j < matrix.count_column; j++) {
			std::cout << std::setw(3) << matrix.matrix[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	return out;
}

int main() {

	try {
		/*srand(time(0));
		Matrix A(-1, 3);
		A.Generation();
		Matrix B(3, 3);
		B.Generation();
		Matrix C = A * B;
		std::cout << A << B << C << std::endl;*/

		Matrix A(2, 3);
		A.Generation();
		std::cout << A;

		Matrix tr = Matrix();
		tr = Matrix::transposition(A);
		std::cout << tr;

	}
	catch (const std::exception& ex) {
		std::cout << "Error: " << ex.what();
	}
	return 0;
}