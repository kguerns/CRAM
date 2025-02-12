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
    }

    // 2D Array Constructor
    ArrayWrapper(size_t nx, size_t ny) : nx(nx), ny(ny) {
        ndim = 2;
        data = new T[nx * ny];
    }

    // Destructor
    ~ArrayWrapper() {
        delete data;
    }

    // Write Access
    T& operator[](size_t index) {
        return data[index];
    }

    // Read Access
    const T& operator[](size_t index) const {
        return data[index];
    }

private:
    T* data;            // contains array data
    unsigned int ndim;  // dimensions of array (1, 2, or 3)
    size_t nx;          // x-dimension size
    size_t ny;          // y-dimension size
    size_t nz;          // z-dimension size

    // Convert flat index to (i, j, k) version
        // TODO: better to have the user access it with i, j, k or with single index?
    // Convery (i, j, k) index to flat index
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


    

}