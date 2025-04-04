#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <thread>
#include <dirent.h>
#include <cstring>


#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#pragma message "Macro value: " TOSTRING(__riscv_v_intrinsic)

// Determine the current XLEN if available
#ifdef __riscv_xlen
constexpr int RISCV_XLEN = __riscv_xlen;
#else
constexpr int RISCV_XLEN = 32;
#endif

// Structure to hold cache information.
struct CacheInfo {
    std::string level;
    std::string type;
    std::string size;
};

// Reads cache info from sysfs by scanning the /sys/devices/system/cpu/cpu0/cache directory.
std::vector<CacheInfo> getCacheInfo() {
    std::vector<CacheInfo> caches;
    const char* cacheDir = "/sys/devices/system/cpu/cpu0/cache";
    DIR* dir = opendir(cacheDir);
    if (dir == nullptr) {
        std::cerr << "Could not open cache directory: " << cacheDir << "\n";
        return caches;
    }
    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (strncmp(entry->d_name, "index", 5) == 0) {
            std::string basePath = std::string(cacheDir) + "/" + entry->d_name;
            std::ifstream levelFile(basePath + "/level");
            std::ifstream typeFile(basePath + "/type");
            std::ifstream sizeFile(basePath + "/size");
            if (levelFile && typeFile && sizeFile) {
                CacheInfo info;
                std::getline(levelFile, info.level);
                std::getline(typeFile, info.type);
                std::getline(sizeFile, info.size);
                caches.push_back(info);
            }
        }
    }
    closedir(dir);
    return caches;
}

// Prints cache information.
void printCacheInfo() {
    std::cout << "=== Cache Information ===\n";
    auto caches = getCacheInfo();
    if (caches.empty()) {
        std::cout << "No cache information found.\n";
    } else {
        for (const auto& cache : caches) {
            std::cout << "L" << cache.level << " " << cache.type 
                      << " Cache: " << cache.size << "\n";
        }
    }
    std::cout << std::endl;
}

// Prints the number of hardware cores using the C++ standard library.
void printCpuCoreInfo() {
    unsigned int cores = std::thread::hardware_concurrency();
    std::cout << "Number of Hardware Cores: " << cores << "\n\n";
}

// Extracts and prints the ISA information from /proc/cpuinfo.
// Many RISC-V kernels include an "isa" line that details the base ISA and extensions.
void printIsaFromCpuinfo() {
    std::cout << "=== ISA Information from /proc/cpuinfo ===\n";
    std::ifstream cpuinfoFile("/proc/cpuinfo");
    if (!cpuinfoFile) {
        std::cerr << "Could not open /proc/cpuinfo\n";
        return;
    }
    std::string line;
    bool found = false;
    while (std::getline(cpuinfoFile, line)) {
        // Look for a line that contains "isa"
        if (line.find("isa") != std::string::npos) {
            std::cout << line << "\n";
            found = true;
        }
    }
    if (!found) {
        std::cout << "ISA information not found in /proc/cpuinfo.\n";
    }
    std::cout << std::endl;
}

// Optionally, print the entire /proc/cpuinfo for additional details.
void printFullCpuinfo() {
    std::cout << "=== Full /proc/cpuinfo ===\n";
    std::ifstream cpuinfoFile("/proc/cpuinfo");
    if (!cpuinfoFile) {
        std::cerr << "Could not open /proc/cpuinfo\n";
        return;
    }
    std::string line;
    while (std::getline(cpuinfoFile, line)) {
        std::cout << line << "\n";
    }
    std::cout << std::endl;
}

int printHardware() {
    std::cout << "RISC-V Hardware Information\n";
    std::cout << "===========================\n\n";

    // Print compile-time detected XLEN.
    std::cout << "Compile-time detected XLEN: " << RISCV_XLEN << "\n\n";

    printCpuCoreInfo();
    printCacheInfo();
    printIsaFromCpuinfo();

    // Optionally, dump the entire /proc/cpuinfo.
    printFullCpuinfo();

    return 0;
}
