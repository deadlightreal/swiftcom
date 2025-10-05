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
    std::vector<objects::Database::JoinedServerRow>* joined_servers = this->GetDatabase()->SelectJoinedServers();

    for (const auto& hosted_server : *hosted_servers) {
        this->GetSavedData().hosted_servers.push_back(objects::HostedServer(hosted_server.server_id));
    }

    for (const auto& joined_server : *joined_servers) {
        this->GetSavedData().joined_servers.push_back(objects::JoinedServer(joined_server.server_id, joined_server.ip_address));
    }
}

objects::Database* objects::LocalStorageDataManager::GetDatabase() {
    return this->database;
}
