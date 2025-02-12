#include <string>
#include <iostream>
#include <vector>


class CRAM_Manager {
public:
    CRAM_Manager() {}

    // Constructor
    CRAM_Manager(std::string errBoundMode="PW_REL", 
                 std::string compressorType="sz3", 
                 double errBound=1e-5, 
                 std::string losslessCompressor="zstd") 
        : errBoundMode(errBoundMode), compressorType(compressorType),
        errBound(errBound), losslessCompressor(losslessCompressor) {}

    // Destructor
    ~CRAM_Manager() {}

    // Register Variable: registers array to be compressed, decompressed in memory map if it does not already exist and updates number of registrations
    void registerVar(std::string varName, std::vector<size_t> shape,
                     std::string dtype="float64", int numVectors = 1) {
        
    }
    
    // Remove
    void remove(std::string name, int index) {}
    
    // Compress
    void compress() {}
    
    // Decompress 
    void decompress() {}
    
private:
    std::string errBoundMode;       // Error Bound Mode (PW_REL)
    std::string compressorType;     // Compressor Type (SZ3)
    double errBound;                // Error Bound (1e-5)
    std::string losslessCompressor; // Lossless Compressor (zstd)
};

int main() {
    CRAM_Manager memory("PW_ABS", "sz3", 1e-5, "zstd");
    
}