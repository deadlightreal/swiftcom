#pragma once

#include <arpa/inet.h>
#include <cstdint>
#include <fstream>
#include <netinet/in.h>
#include <vector>
#include <cJSON.h>

namespace objects {
    class LocalServer {
    public:
        typedef struct {
            in_addr ip_address;
            uint32_t id;
        } LocalServerParsedData;

        LocalServerParsedData parse();
        LocalServer(in_addr ip_address, uint32_t id);

        in_addr GetIpAddress();
        uint32_t GetServerId();
    private:
        in_addr ip_address;
        uint32_t id;
    };

    class LocalStorageDataManager {
    public:
        struct LocalStorageSavedData {
            std::vector<LocalServer>& local_servers;

            LocalStorageSavedData(std::vector<LocalServer>& local_servers) : local_servers(local_servers) {}
        };

        typedef LocalStorageSavedData LocalStorageSavedData;

        LocalStorageDataManager(std::vector<LocalServer>& local_servers);

        void save_data();
        void load_data();

        LocalStorageSavedData& GetSavedData();
    private:
        LocalStorageSavedData saved_data;
    };
}
