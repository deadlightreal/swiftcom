#include "objects.hpp"
#include "../utils/file_system/file_system.hpp"
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <ios>
#include <sstream>
#include <streambuf>
#include <string>
#include <filesystem>
#include <cJSON.h>
#include <sqlite3.h>

std::string directory_location;

static std::fstream open_file(const bool trunc) {
    std::cout << "openning file" << std::endl;

    std::fstream file = trunc == true ? std::fstream(directory_location, std::ios::in | std::ios::out | std::ios::trunc) : std::fstream(directory_location, std::ios::in | std::ios::out);

    if (!file) {
        perror("Failed to open save file!");
        exit(EXIT_FAILURE);
    }

    return file;
}

objects::LocalStorageDataManager::LocalStorageDataManager(std::vector<LocalServer>& local_servers) : saved_data(LocalStorageSavedData(local_servers)) {
    directory_location = utils::file_system::get_application_directory();

    if (!std::filesystem::exists(directory_location)) {
        std::filesystem::create_directory(directory_location);
    }

    directory_location += "/data.json";

    std::fstream application_data_file = std::fstream(directory_location, std::ios::in | std::ios::out);

    if (!application_data_file) {
        std::ofstream create(directory_location);
        create.close();

        application_data_file = std::fstream(directory_location, std::ios::in | std::ios::out | std::ios::trunc);
    }

    if (!application_data_file) {
        perror("Failed to open or create application data file!");
        exit(EXIT_FAILURE);
    }

    application_data_file.close();

    this->load_data();
}

objects::LocalStorageDataManager::LocalStorageSavedData& objects::LocalStorageDataManager::GetSavedData() {
    return this->saved_data;
}

void objects::LocalStorageDataManager::save_data() {
    std::fstream save_data_file = open_file(true);

    const objects::LocalStorageDataManager::LocalStorageSavedData saved_data = this->GetSavedData();

    cJSON* root = cJSON_CreateObject();

    cJSON* local_servers_array = cJSON_AddArrayToObject(root, "local_servers");
    
    for (auto &server : saved_data.local_servers) {
        cJSON* server_object = cJSON_CreateObject();

        cJSON_AddNumberToObject(server_object, "ip_address", server.GetIpAddress().s_addr);
        cJSON_AddNumberToObject(server_object, "id", server.GetServerId());

        cJSON_AddItemToArray(local_servers_array, server_object);
    }

    char* cjson_print = cJSON_Print(root);

    save_data_file << cjson_print;

    save_data_file.close();

    free(cjson_print);

    cJSON_Delete(root);
}

void objects::LocalStorageDataManager::load_data() {
    std::fstream save_data_file = open_file(false);

    std::ostringstream file_buffer;
    
    file_buffer << save_data_file.rdbuf();

    const std::string json_content = file_buffer.str();

    std::cout << "json content: " << json_content << std::endl;

    cJSON* root = cJSON_Parse(json_content.c_str());

    cJSON* local_servers_array = cJSON_GetObjectItem(root, "local_servers");

    for (uint32_t i = 0; i < cJSON_GetArraySize(local_servers_array); i++) {
        std::cout << "Parsing server" << std::endl;

        cJSON* server = cJSON_GetArrayItem(local_servers_array, i);

        const in_addr ip_address = {.s_addr = (in_addr_t)cJSON_GetObjectItem(server, "ip_address")->valueint};
        const uint32_t id = cJSON_GetObjectItem(server, "id")->valueint;

        this->GetSavedData().local_servers.push_back(LocalServer(ip_address, id));

        std::cout << "Server ip: " << ip_address.s_addr << " id: " << id << std::endl;
    } 
}
