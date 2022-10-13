#include <cassert>
#include <cmath>
#include <iomanip>
#include <numeric>
#include <stdexcept>
#include <utility>

namespace Utils
{
    /**
 *
 */
    template<class T, std::size_t M, std::size_t N>
    Matrix<T, M, N>::Matrix(T value)
    {
        for (std::size_t row = 0; row < M; ++row)
        {
            for (std::size_t column = 0; column < N; ++column)
            {
                matrix.at(row).at(column) = value;
            }
        }
    }
    /**
 *
 */
    template<class T, std::size_t M, std::size_t N>
    Matrix<T, M, N>::Matrix(const std::initializer_list<T>& aList)
    {
        // Check the arguments
        assert(aList.size() == M * N);

        auto row_iter = aList.begin();
        for (std::size_t row = 0; row < M; ++row)
        {
            for (std::size_t column = 0; column < N; ++column, ++row_iter)
            {
                matrix.at(row).at(column) = *row_iter;
            }
        }
    }
    /**
 *
 */
    template<class T, std::size_t M, std::size_t N>
    Matrix<T, M, N>::Matrix(const std::initializer_list<std::initializer_list<T>>& aList)
    {
        // Check the arguments, the static assert assures that there is at least 1 M and 1 N!
        assert(aList.size() == M && (*aList.begin()).size() == N);

        auto row_iter = aList.begin();
        for (std::size_t row = 0; row < aList.size(); ++row, ++row_iter)
        {
            auto column_iter = (*row_iter).begin();
            for (std::size_t column = 0; column < (*row_iter).size(); ++column, ++column_iter)
            {
                matrix.at(row).at(column) = *column_iter;
            }
        }
    }
    /**
 *
 */
    template<class T, std::size_t M, std::size_t N>
    Matrix<T, M, N>::Matrix(const Matrix<T, M, N>& aMatrix) : matrix(aMatrix.matrix)
    {
    }
    /**
 *
 */
    template<class T, std::size_t M, std::size_t N>
    std::array<T, N>& Matrix<T, M, N>::at(std::size_t aRowIndex)
    {
        return matrix.at(aRowIndex);
    }
    /**
 *
 */
    template<class T, std::size_t M, std::size_t N>
    const std::array<T, N>& Matrix<T, M, N>::at(std::size_t aRowIndex) const
    {
        return matrix.at(aRowIndex);
    }
    /**
 *
 */
    template<class T, std::size_t M, std::size_t N>
    T& Matrix<T, M, N>::at(std::size_t aRowIndex, std::size_t aColumnIndex)
    {
        return matrix.at(aRowIndex).at(aColumnIndex);
    }
    /**
 *
 */
    template<class T, std::size_t M, std::size_t N>
    const T& Matrix<T, M, N>::at(std::size_t aRowIndex, std::size_t aColumnIndex) const
    {
        return matrix.at(aRowIndex).at(aColumnIndex);
    }
    /**
 *
 */
    template<class T, std::size_t M, std::size_t N>
    std::array<T, N>& Matrix<T, M, N>::operator[](std::size_t aRowIndex)
    {
        return matrix[aRowIndex];
    }
    /**
 *
 */
    template<class T, std::size_t M, std::size_t N>
    const std::array<T, N>& Matrix<T, M, N>::operator[](std::size_t aRowIndex) const
    {
        return matrix[aRowIndex];
    }
    /**
 *
 */
    template<class T, std::size_t M, std::size_t N>
    Matrix<T, M, N>& Matrix<T, M, N>::operator=(const Matrix<T, M, N>& rhs)
    {
        if (this != &rhs)
        {
            matrix = rhs.matrix;
        }
        return *this;
    }
    /**
 *
 */
    template<class T, std::size_t M, std::size_t N>
    bool Matrix<T, M, N>::operator==(const Matrix<T, M, N>& rhs) const
    {
        return matrix == rhs.matrix;
    }
    /**
 *
 */
    template<class T, std::size_t M, std::size_t N>
    template<class T2>
    Matrix<T, M, N>& Matrix<T, M, N>::operator*=(const T2& scalar)
    {
        static_assert(std::is_arithmetic<T2>::value,
                      "Value T2 must be arithmetic, see http://en.cppreference.com/w/cpp/types/is_arithmetic");

        for (std::size_t row = 0; row < M; ++row)
        {
            for (std::size_t column = 0; column < N; ++column)
            {
                matrix.at(row).at(column) *= scalar;
            }
        }
        return *this;
    }
    /**
 *
 */
    template<class T, std::size_t M, std::size_t N>
    template<class T2>
    Matrix<T, M, N> Matrix<T, M, N>::operator*(const T2& scalar) const
    {
        static_assert(std::is_arithmetic<T2>::value,
                      "Value T2 must be arithmetic, see http://en.cppreference.com/w/cpp/types/is_arithmetic");

        Matrix<T, M, N> result(*this);
        return result *= scalar;
    }
    /**
 *
 */
    template<class T, std::size_t M, std::size_t N>
    template<class T2>
    Matrix<T, M, N>& Matrix<T, M, N>::operator/=(const T2& aScalar)
    {
        static_assert(std::is_arithmetic<T2>::value,
                      "Value T2 must be arithmetic, see http://en.cppreference.com/w/cpp/types/is_arithmetic");

        for (std::size_t row = 0; row < M; ++row)
        {
            for (std::size_t column = 0; column < N; ++column)
            {
                matrix.at(row).at(column) /= aScalar;
            }
        }
        return *this;
    }
    /**
 *
 */
    template<class T, std::size_t M, std::size_t N>
    template<class T2>
    Matrix<T, M, N> Matrix<T, M, N>::operator/(const T2& aScalar) const
    {
        static_assert(std::is_arithmetic<T2>::value,
                      "Value T2 must be arithmetic, see http://en.cppreference.com/w/cpp/types/is_arithmetic");

        Matrix<T, M, N> result(*this);
        return result /= aScalar;
    }
    /**
 *
 */
    template<class T, std::size_t M, std::size_t N>
    Matrix<T, M, N>& Matrix<T, M, N>::operator+=(const Matrix<T, M, N>& rhs)
    {
        for (std::size_t row = 0; row < M; ++row)
        {
            for (std::size_t column = 0; column < N; ++column)
            {
                matrix[row][column] += rhs.at(row, column);
            }
        }
        return *this;
    }
    /**
 *
 */
    template<class T, std::size_t M, std::size_t N>
    Matrix<T, M, N> Matrix<T, M, N>::operator+(const Matrix<T, M, N>& rhs) const
    {
        Matrix<T, M, N> result(*this);
        return result += rhs;
    }
    /**
 *
 */
    template<class T, std::size_t M, std::size_t N>
    Matrix<T, M, N>& Matrix<T, M, N>::operator-=(const Matrix<T, M, N>& rhs)
    {
        for (std::size_t row = 0; row < M; ++row)
        {
            for (std::size_t column = 0; column < N; ++column)
            {
                matrix[row][column] -= rhs.at(row, column);
            }
        }
        return *this;
    }
    /**
 *
 */
    template<class T, std::size_t M, std::size_t N>
    Matrix<T, M, N> Matrix<T, M, N>::operator-(const Matrix<T, M, N>& rhs) const
    {
        Matrix<T, M, N> result(*this);
        return result -= rhs;
    }
    /**
 * (M, N) * (N, P) -> (M, P)
 */
    template<typename T, std::size_t M, std::size_t N>
    template<std::size_t columns>
    Matrix<T, M, columns> Matrix<T, M, N>::operator*(const Matrix<T, N, columns>& rhs) const
    {
        Matrix<T, M, columns> result;

        for (std::size_t i = 0; i < M; i++)
        {
            for (std::size_t j = 0; j < rhs.getColumns(); j++)
            {
                T sum = 0;
                for (std::size_t k = 0; k < N; k++)
                {
                    sum += matrix[i][k] * rhs.at(k, j);
                }
                result.at(i, j) = sum;
            }
        }

        return result;
    }
    /**
 *
 */
    template<class T, std::size_t M, std::size_t N>
    Matrix<T, N, M> Matrix<T, M, N>::transpose() const
    {
        Matrix<T, N, M> result;

        for (std::size_t i = 0; i < N; ++i)
        {
            for (std::size_t j = 0; j < M; ++j)
            {
                result.at(j, i) = at(i, j);
            }
        }

        return result;
    }
    /**
 *
 */
    template<class T, std::size_t M, std::size_t N>
    Matrix<T, M, N> Matrix<T, M, N>::identity() const
    {
        Matrix<T, N, M> result;
        for (std::size_t i = 0; i < N; ++i)
        {
            result[i][i] = 1;
        }
        return result;
    }
    /**
 *
 */
    template<class T, std::size_t M, std::size_t N>
    Matrix<T, M, N> Matrix<T, M, N>::gauss() const
    {
        Matrix<T, M, N> result = *this;

        std::size_t pivotRowCol = 0;
        while ((pivotRowCol < N) && (pivotRowCol < M))
        {
            std::size_t biggestRowIndex = pivotRowCol;
            T biggestValue = 0;
            for (std::size_t row = pivotRowCol; row < M; ++row)
            {
                const T val = std::abs(result.at(row).at(pivotRowCol));
                if (val > biggestValue)
                {
                    biggestValue = val;
                    biggestRowIndex = row;
                }
            }
            std::swap(result.at(pivotRowCol), result.at(biggestRowIndex));

            const T divisionValue = result.at(pivotRowCol).at(pivotRowCol);
            for (size_t col = 0; col < N; ++col)
            {
                result.at(pivotRowCol).at(col) /= divisionValue;
            }

            for (size_t row = pivotRowCol + 1; row < M; ++row)
            {
                const T multiplicityValue = result.at(row).at(pivotRowCol);

                for (size_t col = 0; col < N; ++col)
                {
                    result.at(row).at(col) -= multiplicityValue * result.at(pivotRowCol).at(col);
                }
            }
            ++pivotRowCol;
        }
        return result;
    }
    /**
 *
 */
    template<class T, std::size_t M, std::size_t N>
    Matrix<T, M, N> Matrix<T, M, N>::gaussJordan() const
    {
        Matrix<T, M, N> result = gauss();
        const std::size_t max = (M < N) ? M - 1 : N - 1;
        size_t pivotRowCol = max;

        do
        {
            const T divisionValue = result.at(pivotRowCol).at(pivotRowCol);
            for (std::size_t col = 0; col < N; ++col)
            {
                result.at(pivotRowCol).at(col) /= divisionValue;
            }
            for (std::size_t row = pivotRowCol - 1; row < N + 1; --row)
            {
                const T multiplicityValue = result.at(row).at(pivotRowCol);
                for (size_t colNr = 0; colNr < N; ++colNr)
                {
                    result.at(row).at(colNr) -= multiplicityValue * result.at(pivotRowCol).at(colNr);
                }
            }
            --pivotRowCol;
        } while (pivotRowCol < max);

        return result;
    }
    template<class T, std::size_t M, std::size_t N>
    Matrix<T, M, 1> Matrix<T, M, N>::solve() const
    {
        // TODO Implement this function
        Matrix<T, M, 1> result;
        return result;
    }

    /**
 *
 */
    template<class T, std::size_t M, std::size_t N>
    Matrix<T, M, N> Matrix<T, M, N>::inverse() const
    {
        Matrix<T, N, M * 2> placeholder;

        for (std::size_t i = 0; i < N; ++i)
        {
            for (std::size_t j = 0; j < M; ++j)
            {
                placeholder[j][i + M] = identity()[j][i];
                placeholder[j][i] = matrix[j][i];
            }
        }

        placeholder = placeholder.gaussJordan();
        Matrix<T, N, M> result;

        for (std::size_t i = 0; i < N; ++i)
        {
            for (std::size_t j = 0; j < M; ++j)
            {
                result[j][i] = placeholder[j][i + M];
            }
        }
        return result;
    }
    /**
 *
 */
    template<class T, std::size_t M, std::size_t N>
    std::string Matrix<T, M, N>::to_string() const
    {
        std::string result = "Matrix<" + std::to_string(N) + "," + std::to_string(M) + ">\n{\n";
        for (std::size_t i = 0; i < M; ++i)
        {
            for (std::size_t j = 0; j < N; ++j)
            {
                result += std::to_string(matrix[i][j]) + ",";
            }
            result += "\n";
        }
        result += "}";
        return result;
    }
    /**
 *
 */
    template<typename T, const std::size_t N>
    bool equals(const Matrix<T, 1, N>& lhs, const Matrix<T, 1, N>& rhs,
                const T aPrecision /*= std::numeric_limits<T>::epsilon()*/, const unsigned long aFactor /*= 1*/)
    {
        // TODO Implement this function
        return true;
    }
    /**
 *
 */
    template<typename T, const std::size_t M>
    bool equals(const Matrix<T, M, 1>& lhs, const Matrix<T, M, 1>& rhs,
                const T aPrecision /*= std::numeric_limits<T>::epsilon()*/, const unsigned long aFactor /*= 1*/)
    {
        // TODO Implement this function
        return true;
    }
    /**
 *
 */
    template<typename T, const std::size_t M, const std::size_t N>
    bool equals(const Matrix<T, M, N>& lhs, const Matrix<T, M, N>& rhs,
                const T aPrecision /*= std::numeric_limits<T>::epsilon()*/, const unsigned long aFactor /*= 1*/)
    {
        // TODO Implement this function
        return true;
    }

}// namespace Utils