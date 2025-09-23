#pragma once

#include "sqlite3.h"
#include <arpa/inet.h>
#include <cstdint>
#include <fstream>
#include <netinet/in.h>
#include <vector>
#include <cJSON.h>

namespace objects {
    class HostedServer {
    public:
        HostedServer(uint32_t id);

        uint32_t GetServerId();
    private:
        uint32_t id;
    };

    class Database {
    public:
        typedef struct {
            uint32_t server_id;
        } HostedServerRow;

        Database();

        sqlite3* GetDatabaseConnection();

        void OpenDatabase();
        void InitializeDatabaseTables();

        std::vector<HostedServerRow>* SelectHostedServers();

        void InitializeHostedServersTable();

        void InsertHostedServer(const uint32_t server_id);
    private:
        sqlite3* database_connection;
    };

    class LocalStorageDataManager {
    public:
        struct LocalStorageSavedData {
            std::vector<HostedServer> hosted_servers;

            LocalStorageSavedData() {}
        };

        typedef LocalStorageSavedData LocalStorageSavedData;

        LocalStorageDataManager();

        void save_data();
        void load_data();
        void InsertHostedServer(const uint32_t server_id);

        Database* GetDatabase();

        LocalStorageSavedData& GetSavedData();
    private:
        LocalStorageSavedData saved_data;
        Database* database;
    };
}
