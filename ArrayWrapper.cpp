#include <iostream>
#include <vector>
#include <stdexcept>
#include <cstring>
#include "libpressio.h"
using namespace std;

template <typename T>
class ArrayWrapper{
public:

    // Default Constructor
    ArrayWrapper() {}

    // in constructor, store compressor name (line 36 & 42)
    // for now, just give it an array of all the libpressio options (vector of std::pair)
    // error cehcking valid options
    // memcpy convenience entire
    // compress data then remove the data variable data (unless need for debugging) - use lipressio data type for representing compressed data

    // 1D Array Constructor
    ArrayWrapper(size_t nx, const char* compressor_choice) : nx(nx), ny(0), nz(0), ndim(1) {
        data = new T[nx];   // Allocate array
        init_libpressio(compressor_choice);  // Initialize Libpressio
    }

    // Destructor
    ~ArrayWrapper() {
        if (data) delete[] data;
    }

    // Store value at flat index (write)
    void store(size_t idx, T value) {
        decompress_if_needed();
        if (idx >= nx) throw std::out_of_range("Index out of range");
        data[idx] = value;
        // compress with new data
    }

    // Load value at flat index (read)
    T load(size_t idx) {
        decompress_if_needed();
        if (idx >= nx) throw std::out_of_range("Index out of range");
        return data[idx];
    }

    // Compress array data
    void compress() {
        if (!data) throw std::runtime_error("No data to compress");
        cout << "Compressing...\n";

        size_t dimensions[] = {nx};

        cout << data[0] << "\n";

        // Convert input data to a pressio_data object
        // nonowning function so libpressio doesnt delete "data"
        input_data = pressio_data_new_move(
            dtype_from_type(),
            data,
            ndim,
            dimensions,
            pressio_data_libc_free_fn,
            NULL
        );

        // Create an output dataset pointer
        compressed_data = pressio_data_new_empty(
            pressio_byte_dtype, 
            0, 
            NULL
        );

        // Compress data
        if (pressio_compressor_compress(compressor, input_data, compressed_data)) {
            printf("%s\n", pressio_compressor_error_msg(compressor));
            exit(pressio_compressor_error_code(compressor));
        }

        delete[] data;
        data = nullptr;
        is_compressed = true;
    }

private:
    T* data = nullptr;  // array data for initial storing
    size_t ndim;        // dimensions of array (1, 2, or 3)
    size_t nx;          // x-dimension size
    size_t ny;          // y-dimension size
    size_t nz;          // z-dimension size
    bool is_compressed = false;

    // Libpressio variables
    pressio* library;
    pressio_compressor* compressor;
    pressio_options* options;
    pressio_data* input_data;
    pressio_data* compressed_data;
    pressio_data* decompressed_data;

    // Convert (i, j, k) index to flat index
    size_t convert_to_flat(size_t x_idx, size_t y_idx, size_t z_idx) {
        return z_idx * nx * ny + y_idx * nx + x_idx;
    }

    // Initialize LibPressio
    void init_libpressio(const char* compressor_choice) {
        cout << "Initializing LibPressio...\n";
        // Error checking:
        // Is compressor_choice one of the valid options?
        // Can I have more than one libressio instance, like one for each initialized array?
        //compressor_choice = "sz";   // remove eventually
        
        library = pressio_instance();
        compressor = pressio_get_compressor(library, compressor_choice);
        if (!compressor) {
            throw std::runtime_error("Invalid compressor");
        }

        // Set compressor options
        options = pressio_compressor_get_options(compressor);
        pressio_options_set_integer(options, "sz:error_bound_mode", 1);
        pressio_options_set_double(options, "sz:abs_err_bound", 1e-1);

        // Check valid options
        if (pressio_compressor_check_options(compressor, options)) {
            printf("%s\n", pressio_compressor_error_msg(compressor));
            exit(pressio_compressor_error_code(compressor));
        }
        if (pressio_compressor_set_options(compressor, options)) {
            printf("%s\n", pressio_compressor_error_msg(compressor));
            exit(pressio_compressor_error_code(compressor));
        }
    }

    // Decompress array data
    void decompress_if_needed() {
        if (is_compressed) {
            cout << "Decompressing...\n";
            size_t dimensions[] = {nx};

            decompressed_data = pressio_data_new_empty(dtype_from_type(), ndim, dimensions);

            if (pressio_compressor_decompress(compressor, compressed_data, decompressed_data)) {
                printf("%s\n", pressio_compressor_error_msg(compressor));
                exit(pressio_compressor_error_code(compressor));
            }     

            // Copy decompressed data back to user-accessible array
            void* decompressed_ptr = pressio_data_ptr(decompressed_data, nullptr);
            std::memcpy(data, decompressed_ptr, nx * sizeof(T));

            is_compressed = false;
        }
    }

    pressio_dtype dtype_from_type() {
        if constexpr (std::is_same<T, float>::value) return pressio_float_dtype;
        if constexpr (std::is_same<T, double>::value) return pressio_double_dtype;
        if constexpr (std::is_same<T, int32_t>::value) return pressio_int32_dtype;
        if constexpr (std::is_same<T, uint8_t>::value) return pressio_uint8_dtype;
        throw std::runtime_error("Unsupported type for pressio_data");
    }

};

int main() {
    // Initialize 1D float array of size 10, use SZ compressor
    ArrayWrapper<float> test_array1(10, "sz");
    /*for (int i=0; i < 1; i++) {
        test_array1.store(i, i);        // Store (write)
    }*/
    test_array1.store(0, 27.1);
    test_array1.compress();
    test_array1.load(0);

    /*for (int i=0; i < 1; i++) {
        cout << test_array1.load(i) << "\n";    // Load (read)
    }*/
}