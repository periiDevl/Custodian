#ifndef FILE_MONITOR_H
#define FILE_MONITOR_H

#include <filesystem>
#include <iostream>
#include <thread>
#include <chrono>
#include <map>
#include <string>
#include <optional>

namespace fs = std::filesystem;

class FileMonitor {
public:
    using FileMap = std::map<std::string, fs::file_time_type>;

    FileMonitor(const fs::path& folderPath) : folderPath_(folderPath) {}

    FileMap GetFileList() const {
        FileMap fileList;
        for (const auto& entry : fs::directory_iterator(folderPath_)) {
            if (entry.is_regular_file()) {
                const fs::path& filePath = entry.path();
                fileList[filePath.filename().string()] = fs::last_write_time(filePath);
            }
        }
        return fileList;
    }

    std::string DetectDirecGlobal(int DetectRate) {
        FileMap currentFileList = GetFileList();
        std::string result;

        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(DetectRate));

            FileMap newFileList = GetFileList();

            for (const auto& [fileName, newTime] : newFileList) {
                if (currentFileList.find(fileName) == currentFileList.end()) {
                    result = fileName;
                }
                else if (newTime > currentFileList[fileName]) {
                    result = fileName;
                }
            }

            for (const auto& [fileName, oldTime] : currentFileList) {
                if (newFileList.find(fileName) == newFileList.end()) {
                    result = fileName;
                }
            }


            currentFileList = newFileList;

            // Check if we found a result and return it.
            if (!result.empty()) {
                return result;
            }
        }
    }



    std::string DetectDirecChanged(int DetectRate) {
        FileMap currentFileList = GetFileList();

        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(DetectRate));

            FileMap newFileList = GetFileList();

            for (const auto& [fileName, newTime] : newFileList) {

                if (newTime > currentFileList[fileName]) {
                    return fileName;
                }
            }



            currentFileList = newFileList;
        }
    }


    std::string DetectDirecRemov_N_Add(int DetectRate) {
        FileMap currentFileList = GetFileList();

        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(DetectRate));

            FileMap newFileList = GetFileList();

            for (const auto& [fileName, newTime] : newFileList) {
                if (currentFileList.find(fileName) == currentFileList.end()) {
                    return fileName;
                }

            }

            for (const auto& [fileName, oldTime] : currentFileList) {
                if (newFileList.find(fileName) == newFileList.end()) {
                    return fileName;
                }
            }

            currentFileList = newFileList;
        }
    }


private:
    fs::path folderPath_;
};

#endif // FILE_MONITOR_H
