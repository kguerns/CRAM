

#include <libpressio_ext/cpp/libpressio.h>
#include <iostream>
using namespace std;

template <typename T>
class ArrayWrapper{
public:

    // Default Constructor
    ArrayWrapper() {}

    // 1D Array Constructor
    ArrayWrapper(size_t nx) : nx(nx) {
        ndim = 1;
        data = new T[nx];
        ny = 0;
        nz = 0;
    }

    // 2D Array Constructor
    ArrayWrapper(size_t nx, size_t ny) : nx(nx), ny(ny) {
        ndim = 2;
        data = new T[nx * ny];
        nz = 0;
    }

    // 3D Array Constructor
    ArrayWrapper(size_t nx, size_t ny, size_t nz) : nx(nx), ny(ny), nz(nz) {
        ndim = 3;
        data = new T[nx * ny * nz];
    }

    // Destructor
    ~ArrayWrapper() {
        delete data;
    }

    // Write Access for flat index
    T& operator[](size_t index) {
        return data[index];
    }

    // Read Access for flat index
    const T& operator[](size_t index) const {
        return data[index];
    }

    // Write Access for (i, j, k) index
    T& operator()(size_t x_idx, size_t y_idx = 0, size_t z_idx = 0) {
        return data[convert_to_flat(x_idx, y_idx, z_idx)];
    }

    // Read Access for (i, j, k) index
    const T& operator()(size_t x_idx, size_t y_idx = 0, size_t z_idx = 0) const {;
        return data[convert_to_flat(x_idx, y_idx, z_idx)];
    }

private:
    T* data;            // contains array data
    unsigned int ndim;  // dimensions of array (1, 2, or 3)
    size_t nx;          // x-dimension size
    size_t ny;          // y-dimension size
    size_t nz;          // z-dimension size

    // Convert (i, j, k) index to flat index
    size_t convert_to_flat(size_t x_idx, size_t y_idx, size_t z_idx) {
        return z_idx * nx * ny + y_idx * nx + x_idx;
    }

};

int main() {
    // Initialize 1D float array of size 10
    ArrayWrapper<float> test_array1(10);
    for (int i=0; i < 10; i++) {
        // Write to array
        test_array1[i] = static_cast<float>(i);
        // Read from array
        cout << test_array1[i] << "\n";
    }

    // Initialize 2D float array of size 3x4
    ArrayWrapper<float> test_array2(3, 4);
    for (int i=0; i < 3; i++) {
        for (int j=0; j < 4; j++) {
            // Write to array
            test_array2(i,j) = static_cast<float>(i+j);
            // Read from array
            cout << i << "," << j << ": " << test_array2(i,j) << "\n";
        }
    }

    // Initialize 3D float array of size 3x4x5
    ArrayWrapper<float> test_array3(3, 4, 5);
    for (int i=0; i < 3; i++) {
        for (int j=0; j < 4; j++) {
            for (int k=0; k < 5; k++) {
                // Write to array
                test_array3(i,j,k) = static_cast<float>(i+j+k);
                // Read from array
                cout << i << "," << j << "," << k << ": " << test_array3(i,j,k) << "\n";
            }
        }
    }
}