#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <cstring>

// Линейный конгруэнтный генератор (LCG)
uint64_t lcg(uint64_t& state) {
    state = (state * 6364136223846793005ULL + 1);
    return state;
}

int main() {
    const char* filename = "big_file.tar.gz"; // укажи свой путь к файлу
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) {
        std::cerr << "Ошибка: не удалось открыть файл\n";
        return 1;
    }

    size_t file_size = file.tellg();
    file.seekg(0);
    std::vector<uint8_t> buffer(file_size);
    file.read(reinterpret_cast<char*>(buffer.data()), file_size);
    file.close();

    std::cout << "Файл загружен в память (" << file_size / (1024 * 1024) << " MB)\n";

    // Начальное смещение a
    size_t a = 1024 * 1024; // Например, 1MB

    // Кол-во итераций для каждого k
    const size_t iterations = 1e7;

    // Проверим разные значения k — от 16KB до 64MB
    // std::vector<size_t> ks = {
    //     16 * 1024,  // L1/2
    //     31 * 1024,  // --> L1
    //     32 * 1024,  //   L1
    //     33 * 1024,  // L1 <--

    //     (32 + 256) * 1024, // L2/2
    //     (31 + 512) * 1024, // --> L2
    //     (32 + 512) * 1024, //   L2
    //     (33 + 512) * 1024, // L2 <--

    //     (32 + 512) * 1024 + 8 * 1024 * 1024,  // L3/2
    //     (31 + 512) * 1024 + 16 * 1024 * 1024, // --> L3
    //     (32 + 512) * 1024 + 16 * 1024 * 1024, //   L3
    //     (33 + 512) * 1024 + 16 * 1024 * 1024, // L3 <--

    //     (32 + 512) * 1024 + 64 * 1024 * 1024  // > L3
    // };

    std::vector<size_t> ks = {
        16 * 1024,  // L1/2
        28 * 1024,  // --> L1
        30 * 1024,  // --> L1
        32 * 1024,  //   L1
        34 * 1024,  // L1 <--
        36 * 1024,  // L1 <--
        38 * 1024,  // L1 <--
    };

    for (size_t k : ks) {
        if (a + k >= buffer.size()) {
            std::cerr << "Пропуск k=" << k << " (выходит за границы файла)\n";
            continue;
        }

        volatile uint64_t sum = 0;
        uint64_t rng_state = 1;

        auto start = std::chrono::high_resolution_clock::now();

        for (size_t i = 0; i < iterations; ++i) {
            size_t offset = a + (lcg(rng_state) % k);
            uint64_t* ptr = reinterpret_cast<uint64_t*>(&buffer[offset]);
            sum += *ptr;
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto duration_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

        double avg_duration = static_cast<double>(duration_ns) / iterations;

        std::cout << "k = " << k / 1024 << " KB:\t" 
                  << avg_duration << " ns\t"
                  << "checksum = " << sum << "\n";
    }

    return 0;
}
