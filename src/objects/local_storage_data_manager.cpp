#include "objects.hpp"
#include "../utils/file_system/file_system.hpp"
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <ios>
#include <string>
#include <filesystem>

objects::LocalStorageDataManager::LocalStorageDataManager(std::vector<LocalServer>& local_servers) : saved_data(LocalStorageSavedData(local_servers)) {
    std::string directory_location = utils::file_system::get_application_directory();

    if (!std::filesystem::exists(directory_location)) {
        std::filesystem::create_directory(directory_location);
    }

    directory_location += "/data.json";

    this->application_data_file = std::fstream(directory_location, std::ios::in | std::ios::out | std::ios::trunc);

    if (!this->application_data_file) {
        std::ofstream create(directory_location);
        create.close();

        this->application_data_file = std::fstream(directory_location, std::ios::in | std::ios::out | std::ios::trunc);
    }

    if (!this->application_data_file) {
        perror("Failed to open or create application data file!");
        exit(EXIT_FAILURE);
    }
}

void objects::LocalStorageDataManager::save_data() {

}

void objects::LocalStorageDataManager::load_data() {

}
