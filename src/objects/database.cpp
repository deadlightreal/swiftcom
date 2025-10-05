#include "objects.hpp"
#include <sqlite3.h>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

objects::Database::Database() {
    this->OpenDatabase();

    this->InitializeDatabaseTables();
}

sqlite3* objects::Database::GetDatabaseConnection() {
    return this->database_connection;
}

void objects::Database::OpenDatabase() {
    sqlite3* database_ptr;

    int result = sqlite3_open("swift_com", &database_ptr);
    if (result != SQLITE_OK) {
        std::cerr << "Failed to open database: " << sqlite3_errmsg(database_ptr) << std::endl;
        exit(EXIT_FAILURE);
    }

    this->database_connection = database_ptr;
}

void objects::Database::InitializeDatabaseTables() {
    this->InitializeHostedServersTable();
    this->InitializeHostedServerUsersTable();
}

void objects::Database::InitializeHostedServerUsersTable() {
    const char* query = "CREATE TABLE IF NOT EXISTS hosted_server_users (id INTEGER PRIMARY KEY AUTOINCREMENT, ip_address INTEGER UNIQUE NOT NULL, server_id INTEGER NOT NULL);";

    int result = sqlite3_exec(this->GetDatabaseConnection(), query, 0, 0, nullptr);
    if (result) {
        std::cerr << "Failed to create table hosted_server_users" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void objects::Database::InitializeHostedServersTable() {
    const char* query = "CREATE TABLE IF NOT EXISTS hosted_servers (id INTEGER PRIMARY KEY NOT NULL);";

    int result = sqlite3_exec(this->GetDatabaseConnection(), query, 0, 0, nullptr);
    if (result) {
        std::cerr << "Failed to create table hosted_servers" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void objects::Database::InitializeJoinedServers() {
    const char* query = "CREATE TABLE IF NOT EXISTS joined_servers (id INTEGER PRIMARY KEY AUTOINCREMENT, ip_address INTEGER UNIQUE NOT NULL, server_id INTEGER NOT NULL)";

    int result = sqlite3_exec(this->GetDatabaseConnection(), query, 0, 0, nullptr);
    if (result) {
        std::cerr << "Failed to create table hosted_servers" << std::endl;
        exit(EXIT_FAILURE);
    }
}

int objects::Database::InsertHostedServerUser(const uint16_t server_id, in_addr ip_address) {
    std::string query = "INSERT INTO hosted_server_users (ip_address, server_id) VALUES (" + std::to_string(server_id) + ", " + std::to_string(ip_address.s_addr) + ");";

    int result = sqlite3_exec(this->GetDatabaseConnection(), query.c_str(), 0, 0, nullptr);
    if (result) {
        std::cerr << "Failed to insert hosted_server" << std::endl;
        return -1;
    }

    return 0;
}

int objects::Database::InsertJoinedServer(const uint16_t server_id, in_addr ip_address) {
    std::string query = "INSERT INTO joined_servers (ip_address, server_id) VALUES (" + std::to_string(server_id) + ", " + std::to_string(ip_address.s_addr) + ");";

    int result = sqlite3_exec(this->GetDatabaseConnection(), query.c_str(), 0, 0, nullptr);
    if (result) {
        std::cerr << "Failed to insert hosted_server" << std::endl;
        return -1;
    }

    return 0;
}

int objects::Database::InsertHostedServer(const uint16_t server_id) {
    std::string query = "INSERT INTO hosted_servers (id) VALUES (" + std::to_string(server_id) + ");";

    int result = sqlite3_exec(this->GetDatabaseConnection(), query.c_str(), 0, 0, nullptr);
    if (result) {
        std::cerr << "Failed to insert hosted_server" << std::endl;
        return -1;
    }

    return 0;
}

std::vector<objects::Database::JoinedServerRow>* objects::Database::SelectJoinedServers() {

}

std::vector<objects::Database::HostedServerRow>* objects::Database::SelectHostedServers() {
    sqlite3_stmt* stmt;

    const char* sql = "SELECT id FROM hosted_servers;";

    if (sqlite3_prepare_v2(this->GetDatabaseConnection(), sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare: " << sqlite3_errmsg(this->GetDatabaseConnection()) << "\n";
        sqlite3_close(this->GetDatabaseConnection());
        exit(EXIT_FAILURE);
    }

    std::vector<objects::Database::HostedServerRow>* result = new std::vector<objects::Database::HostedServerRow>();

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        uint16_t id = sqlite3_column_int(stmt, 0);

        result->push_back((objects::Database::HostedServerRow){.server_id = id});
    }

    sqlite3_finalize(stmt);

    return result;
}
