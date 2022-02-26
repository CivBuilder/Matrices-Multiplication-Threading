/// @author Chris
/// @date 2/27/2022
/// @brief this program multiplies two matrices using multi-threading.
/// @note I apologize for the weird formatting in this program, I used
/// the prettier extension in VSCode and it reformatted this 3 times.
/// next assignment will be better formatted.

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

using vec_int = std::vector<std::vector<int>>;

// global variable declaration
/// (made these global to reduce parameters in multiply_matrix)
vec_int product; // computed board
vec_int matrix1; // first matrix read in
vec_int matrix2; // second matrix read in

// // function prototypes
void multiply_matrices(size_t mtx1_row, size_t mtx2_col, size_t current);
void readin_matrix(vec_int &mtx, size_t &n, size_t &m, std::ifstream &in);
void output_matrix(vec_int mtx, size_t n, size_t m);

int main()
{
    std::vector<std::thread> threadPool; // vector of threads
    size_t maxThreads;                   // maxiumum amount of threads the hardware can use
    size_t n1 = 0;                       // rows of matrix one
    size_t m1 = 0;                       // cols of matrix one
    size_t n2 = 0;                       // rows of matrix two, m1 == n2 (assumed)
    size_t m2 = 0;                       // cols of matrix two
    std::ifstream infile;                // file to be read in
    std::string filename;                // file name to be inputted by user

    // prompt for file, and open it.
    std::cout << "Enter file name: ";
    std::cin >> filename;
    infile.open(filename);

    // read in and adjust matrices
    readin_matrix(matrix1, n1, m1, infile);
    readin_matrix(matrix2, n2, m2, infile);

    // resize output matrix. n = n1 and m = m2
    product.resize(n1);
    for (size_t i = 0; i < n1; ++i)
    {
        product[i].resize(m2);
    }

    // check hardware for # of threads available
    maxThreads = std::thread::hardware_concurrency();

    for (size_t row = 0; row < product.size(); ++row) // row == matrix1 row
    {
        for (size_t col = 0; col < product[row].size(); ++col)
        {
            for (size_t current = 0; current < matrix2.size(); ++current)
            {
                // spawn a thread
                threadPool.push_back(
                    std::thread(multiply_matrices, row, col, current));

                // join all of the threads before spawning anymore
                // threads if you have the max amount of threads
                // already running currently
                if (threadPool.size() % maxThreads == 0)
                {
                    for (int i = 0; i < threadPool.size(); i++)
                    {
                        threadPool[i].join();
                    }

                    // clear out the vector of all of the non living
                    // threads so we do not push active threads on
                    // top of the non live ones
                    threadPool.clear();
                }
            }
        }
    }

    output_matrix(product, n1, m2);

    return 0;
}

/// Computes the product of the first two matrices, storing the product
/// in the third matrix.
/// @param mtx1_row current row in matrix1
/// @param mtx2_col current col in matrix2
/// @param current current col in matrix1 and row in matrix2
void multiply_matrices(size_t mtx1_row, size_t mtx2_col, size_t current)
{
    // calculate current row*col and store into product matrix
    product[mtx1_row][mtx2_col] +=
        (matrix1[mtx1_row][current] * matrix2[current][mtx2_col]);
}

/// Reads in a matrix and its attributes, adjusts vector and inputs ints.
/// @param mtx matrix to read in.
/// @param n number of rows
/// @param m number of cols
/// @param in file to read from
void readin_matrix(vec_int &mtx, size_t &n, size_t &m, std::ifstream &in)
{
    int item; // temp int to hold objects from file being read in.

    in >> n >> m;

    // resize vector
    mtx.resize(n);
    for (size_t i = 0; i < n; ++i)
    {
        mtx[i].resize(m);
    }

    // read in matrix
    for (size_t i = 0; i < n; ++i)
    {
        for (size_t j = 0; j < m; ++j)
        {
            in >> item;
            mtx[i][j] = item;
        }
    }
}

/// Outputs a matrix.
/// @param mtx matrix to be outputted.
/// @param n number of rows
/// @param m number of cols
void output_matrix(vec_int mtx, size_t n, size_t m)
{
    for (size_t i = 0; i < n; ++i)
    {
        for (size_t j = 0; j < m; ++j)
        {
            std::cout << mtx[i][j] << " ";
        }
        std::cout << std::endl;
    }
}