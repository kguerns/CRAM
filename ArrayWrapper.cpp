#include <iostream>
#include <vector>
#include <stdexcept>
#include <cstring>
#include "libpressio.h"
#include "libpressio_ext/cpp/libpressio.h"
using namespace std;

template <typename T>
class ArrayWrapper{
public:
    // Notes:
    // for now, just give it an array of all the libpressio options (vector of std::pair)
    // error checking valid options
    // memcpy convenience entire
    // compress data then remove the data variable data (unless need for debugging) - use lipressio data type for representing compressed data

    // Default Constructor
    ArrayWrapper() {}

    // 1D Array Constructor
    ArrayWrapper(size_t nx, const char* compressor_choice) : nx(nx), ny(0), nz(0), ndim(1), dimensions{nx} {
        data = new T[nx];
        init_libpressio(compressor_choice);
    }

    // 2D Array Constructor
    ArrayWrapper(size_t nx, size_t ny, const char* compressor_choice) : nx(nx), ny(ny), nz(0), ndim(2), dimensions{nx, ny} {
        data = new T[nx * ny];
        init_libpressio(compressor_choice);
    }

    // 3D Array Constructor
    ArrayWrapper(size_t nx, size_t ny, size_t nz, const char* compressor_choice) : nx(nx), ny(ny), nz(nz), ndim(3), dimensions{nx, ny, nz} {
        data = new T[nx * ny * nz];
        init_libpressio(compressor_choice);
    }

    // Destructor
    ~ArrayWrapper() {
        if (data) delete[] data;
        if (compressed_data) pressio_data_free(compressed_data);
        if (decompressed_data) pressio_data_free(decompressed_data);
        if (options) pressio_options_free(options);
        if (compressor) pressio_compressor_release(compressor);
        if (library) pressio_release(library);
    }

    // 1D store (x)
    void store(size_t x, T value) {
        if (ndim != 1) throw std::invalid_argument("This store function is for 1D arrays");
        if (x >= nx) throw std::out_of_range("Index out of range");
        decompress();
        data[x] = value;
        compress();
    }

    // 2D store (x, y)
    void store(size_t x, size_t y, T value) {
        if (ndim != 2) throw std::invalid_argument("This store function is for 2D arrays");
        if (x >= nx || y >= ny) throw std::out_of_range("Index out of range");
        decompress();
        size_t flat = convert_to_flat(x, y, 0);
        data[flat] = value;
        compress();
    }

    // 3D store (x, y, z)
    void store(size_t x, size_t y, size_t z, T value) {
        if (ndim != 3) throw std::invalid_argument("This store function is for 3D arrays");
        if (x >= nx || y >= ny || z >= nz) throw std::out_of_range("Index out of range");
        decompress();
        size_t flat = convert_to_flat(x, y, z);
        data[flat] = value;
        compress();
    }

    // 1D Load (x)
    T load(size_t x) {
        if (ndim != 1) throw std::invalid_argument("This store function is for 1D arrays");
        if (x >= nx) throw std::out_of_range("Index out of range");
        decompress();
        T value = data[x];
        compress();
        return value;
    }

    // 2D Load (x, y)
    T load(size_t x, size_t y) {
        if (ndim != 2) throw std::invalid_argument("This store function is for 2D arrays");
        if (x >= nx || y >= ny) throw std::out_of_range("Index out of range");
        decompress();
        size_t flat = convert_to_flat(x, y, 0);
        T value = data[flat];
        compress();
        return value;
    }

    // 3D Load (x, y, z)
    T load(size_t x, size_t y, size_t z) {
        if (ndim != 3) throw std::invalid_argument("This store function is for 3D arrays");
        if (x >= nx || y >= ny || z >= nz) throw std::out_of_range("Index out of range");
        decompress();
        size_t flat = convert_to_flat(x, y, z);
        T value = data[flat];
        compress();
        return value;
    }

private:
    T* data = nullptr;  // array data for storing
    size_t ndim;        // dimensions of array (1, 2, or 3)
    size_t nx;          // x-dimension size
    size_t ny;          // y-dimension size
    size_t nz;          // z-dimension size
    std::vector<size_t> dimensions;
    bool is_compressed = false;

    // LibPressio variables
    struct pressio* library;
    struct pressio_compressor* compressor;
    struct pressio_options* options;
    struct pressio_data* data_to_compress;
    struct pressio_data* compressed_data;
    struct pressio_data* decompressed_data;

    // Convert (x, y, z) index to flat index
    size_t convert_to_flat(size_t x, size_t y, size_t z) {
        return z * nx * ny + y * nx + x;
    }

    // Initialize LibPressio
    void init_libpressio(const char* compressor_choice) {
        // Error Checking:
        // is compressor_choice one of the valid options?
        // can i have more than one libpressio instance, like one for each array?
        
        library = pressio_instance();

        // All compressor options
        //for (auto c: compressor_plugins()) {
        //    std::cout << c.first << std::endl;
        //}

        compressor = pressio_get_compressor(library, compressor_choice);
        options = pressio_compressor_get_options(compressor);
        pressio_options_set_double(options, "pressio:abs", 1e-5);

    }

    // Compress
    void compress() {
        if (!is_compressed) {
            //cout << "Compressing...\n";

            data_to_compress = pressio_data_new_move(pressio_float_dtype, data, ndim, dimensions.data(), pressio_data_libc_free_fn, NULL);
            compressed_data = pressio_data_new_empty(pressio_byte_dtype, 0, NULL);

            if (pressio_compressor_compress(compressor, data_to_compress, compressed_data)) {
                printf("%s\n", pressio_compressor_error_msg(compressor));
                exit(pressio_compressor_error_code(compressor));
            }

            //delete[] data;
            //data = nullptr;
            is_compressed = true;
        }
    }

    // Decompress
    void decompress() {
        if (is_compressed) {
            //cout << "Decompressing...\n";

            decompressed_data = pressio_data_new_empty(pressio_float_dtype, ndim, dimensions.data());
            
            if (pressio_compressor_decompress(compressor, compressed_data, decompressed_data)) {
                printf("%s\n", pressio_compressor_error_msg(compressor));
                exit(pressio_compressor_error_code(compressor));
            }
            /*
            void* decompressed_ptr = pressio_data_ptr(decompressed_data, nullptr);
            cout <<"passed\n";
            if ( ndim = 1) {
                std::memcpy(data, decompressed_ptr, nx * sizeof(T));
            } else if(ndim = 2) {
                std::memcpy(data, decompressed_ptr, nx * ny * sizeof(T));
            } else if (ndim = 3) {
                std::memcpy(data, decompressed_ptr, nx * ny * nz * sizeof(T));
            }
            */

            is_compressed = false;
        }
    }

    // Pressio data type helper
    pressio_dtype dtype_from_type() {
        if constexpr (std::is_same<T, float>::value) return pressio_float_dtype;
        if constexpr (std::is_same<T, double>::value) return pressio_double_dtype;
        if constexpr (std::is_same<T, int32_t>::value) return pressio_int32_dtype;
        if constexpr (std::is_same<T, uint8_t>::value) return pressio_uint8_dtype;
        throw std::runtime_error("Unsupported type for pressio_data");
    }

};

int main() {
    // Initialize data
    int n = 32;
    ArrayWrapper<float> test_array2(n, n, "sz3");
    for (int i=0; i < n; i++) {
        for (int j=0; j < n; j++) {
            test_array2.store(i, j, i*n+j);
        }
    }

    cout << test_array2.load(1,1);

}