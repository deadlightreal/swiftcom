#pragma once

#include "sqlite3.h"
#include <arpa/inet.h>
#include <cstdint>
#include <fstream>
#include <netinet/in.h>
#include <vector>
#include <cJSON.h>
#include "../../external/include/swift_net.h"

namespace objects {
    typedef enum {
        STOPPED,
        RUNNING
    } HostedServerStatus;

    class HostedServer {
    public:
        HostedServer(uint16_t id);

        void StartServer();
        void StopServer();

        SwiftNetServer* GetServer();
        uint16_t GetServerId();
        HostedServerStatus GetServerStatus();
    private:
        uint16_t id;
        HostedServerStatus status = STOPPED;
        SwiftNetServer* server = nullptr;
    };

    class Database {
    public:
        typedef struct {
            uint16_t server_id;
        } HostedServerRow;

        Database();

        sqlite3* GetDatabaseConnection();

        void OpenDatabase();
        void InitializeDatabaseTables();

        std::vector<HostedServerRow>* SelectHostedServers();

        void InitializeHostedServersTable();

        void InsertHostedServer(const uint16_t server_id);
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
        void InsertHostedServer(const uint16_t server_id);
        HostedServer* GetServerById(const uint16_t server_id);

        Database* GetDatabase();

        LocalStorageSavedData& GetSavedData();
    private:
        LocalStorageSavedData saved_data;
        Database* database;
    };

    class ServerHostingManager {
    public:
                ServerHostingManager();
    private:
    };
}
