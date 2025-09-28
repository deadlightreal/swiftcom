#include "objects.hpp"
#include "../utils/file_system/file_system.hpp"
#include <cstdint>
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
#include <vector>

objects::HostedServer* objects::LocalStorageDataManager::GetServerById(const uint16_t server_id) {
    for (auto &server : this->GetSavedData().hosted_servers) {
        if (server.GetServerId() == server_id) {
            return &server;
        }
    }

    return nullptr;
}

objects::LocalStorageDataManager::LocalStorageDataManager() : saved_data(LocalStorageSavedData()) {
    this->database = new Database();

    this->load_data();
}

objects::LocalStorageDataManager::LocalStorageSavedData& objects::LocalStorageDataManager::GetSavedData() {
    return this->saved_data;
}

void objects::LocalStorageDataManager::InsertHostedServer(const uint16_t server_id) {
    this->GetSavedData().hosted_servers.push_back(objects::HostedServer(server_id));

    this->GetDatabase()->InsertHostedServer(server_id);
};

void objects::LocalStorageDataManager::load_data() {
    std::vector<objects::Database::HostedServerRow>* hosted_servers = this->GetDatabase()->SelectHostedServers();

    for (uint32_t i = 0; i < hosted_servers->size(); i++) {
        this->GetSavedData().hosted_servers.push_back(objects::HostedServer(hosted_servers->at(i).server_id));
    }
}

objects::Database* objects::LocalStorageDataManager::GetDatabase() {
    return this->database;
}
