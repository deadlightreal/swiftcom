#pragma once

#include <sqlite3.h>
#include <arpa/inet.h>
#include <cstdint>
#include <fstream>
#include <netinet/in.h>
#include <unordered_map>
#include <vector>
#include <swift_net.h>

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

    class JoinedServer {
    public:
        JoinedServer(uint16_t server_id, in_addr server_ip_address);

        uint16_t GetServerId();
        in_addr GetServerIpAddress();
    private:
        uint16_t server_id;
        in_addr server_ip_address;
    };

    class Database {
    public:
        typedef struct {
            uint16_t server_id;
        } HostedServerRow;

        typedef struct {
            uint16_t server_id;
            in_addr ip_address;
        } JoinedServerRow;

        typedef struct {
            uint16_t id;
            char name[20];
        } ServerChatChannelRow;

        typedef struct {
            const char* statement_name;
            const char* query;
        } Statement;

        Database();

        sqlite3* GetDatabaseConnection();

        void OpenDatabase();
        void InitializeDatabaseTables();

        void PrepareStatements();

        sqlite3_stmt* GetStatement(const char* statement_name);

        std::vector<HostedServerRow>* SelectHostedServers();
        std::vector<JoinedServerRow>* SelectJoinedServers();
        std::vector<ServerChatChannelRow>* SelectServerChatChannels(const uint16_t server_id);

        std::unordered_map<const char*, sqlite3_stmt*>& GetStatements();

        int InsertHostedServer(const uint16_t server_id);
        int InsertHostedServerUser(const uint16_t server_id, in_addr ip_address);
        int InsertJoinedServer(const uint16_t server_id, in_addr ip_address);
        int InsertServerChatChannel(const char* name, const uint16_t server_id);
    private:
        sqlite3* database_connection;
        std::unordered_map<const char*, sqlite3_stmt*> statements;
    };

    class LocalStorageDataManager {
    public:
        struct LocalStorageSavedData {
            std::vector<HostedServer> hosted_servers;
            std::vector<JoinedServer> joined_servers;

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
