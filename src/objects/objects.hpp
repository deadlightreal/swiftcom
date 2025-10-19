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

    typedef struct {
        SwiftNetClientAddrData addr_data;
        uint32_t user_id;
    } ConnectedUser;

    class HostedServer {
    public:
        HostedServer(uint16_t id);
        ~HostedServer();

        void StartServer();
        void StopServer();

        void AddConnectedUser(const ConnectedUser connected_user);

        std::vector<ConnectedUser>* GetConnectedUsers();
        SwiftNetServer* GetServer();
        uint16_t GetServerId();
        HostedServerStatus GetServerStatus();
    private:
        uint16_t id;

        HostedServerStatus status = STOPPED;

        SwiftNetServer* server = nullptr;

        std::vector<ConnectedUser> connected_users;
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
            uint32_t id;
            const char* message;
            uint32_t message_length;
            uint32_t sender_id;
        } ChannelMessageRow;

        typedef struct {
            uint16_t id;
            char name[20];
        } ServerChatChannelRow;

        typedef struct {
            const char* statement_name;
            const char* query;
        } Statement;

        Database();
        ~Database();

        void OpenDatabase();
        void InitializeDatabaseTables();
        void PrepareStatements();

        std::vector<HostedServerRow>* SelectHostedServers();
        std::vector<JoinedServerRow>* SelectJoinedServers();
        std::vector<ServerChatChannelRow>* SelectServerChatChannels(const uint16_t server_id);
        std::vector<ChannelMessageRow>* SelectChannelMessages(const uint32_t channel_id);

        uint32_t SelectUserId(const in_addr_t ip_address, const uint32_t server_id);

        int InsertHostedServer(const uint16_t server_id);
        int InsertHostedServerUser(const uint16_t server_id, in_addr ip_address);
        int InsertJoinedServer(const uint16_t server_id, in_addr ip_address);
        int InsertServerChatChannel(const char* name, const uint16_t server_id);
        int InsertChannelMessage(const char* message, const uint32_t channel_id);

        sqlite3_stmt* GetStatement(const char* statement_name);

        std::unordered_map<const char*, sqlite3_stmt*>& GetStatements();
        sqlite3* GetDatabaseConnection();
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
        ~LocalStorageDataManager();

        void SaveData();
        void LoadData();

        void InsertHostedServer(const uint16_t server_id);
        HostedServer* GetServerById(const uint16_t server_id);

        Database* GetDatabase();

        LocalStorageSavedData& GetSavedData();
    private:
        LocalStorageSavedData saved_data;

        Database* database;
    };
}
