#include "objects.hpp"
#include <sqlite3.h>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

objects::Database::Database() {
    this->OpenDatabase();

    this->InitializeDatabaseTables();
    this->PrepareStatements();
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

sqlite3_stmt* objects::Database::GetStatement(const char* statement_name) {
    return this->GetStatements().at(statement_name);
};

void objects::Database::PrepareStatements() {
    const Statement statements[] = {
        (Statement){.statement_name = "insert_hosted_server_user", .query = "INSERT INTO hosted_server_users (ip_address, server_id) VALUES ($1, $2);"},
        (Statement){.statement_name = "insert_joined_server", .query = "INSERT INTO joined_servers (ip_address, server_id) VALUES ($1, $2);"},
        (Statement){.statement_name = "insert_hosted_server", .query = "INSERT INTO hosted_servers (id) VALUES ($1);"},
        (Statement){.statement_name = "select_hosted_servers", .query = "SELECT id FROM hosted_servers;"},
        (Statement){.statement_name = "select_joined_servers", .query = "SELECT ip_address, server_id FROM joined_servers;"},
    };

    for(const auto& statement : statements) {
        sqlite3_stmt* stmt;

        if(sqlite3_prepare_v2(this->GetDatabaseConnection(), statement.query, -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "Failed to prepare: " << sqlite3_errmsg(this->GetDatabaseConnection()) << "\n";
            sqlite3_close(this->GetDatabaseConnection());
            exit(EXIT_FAILURE);
        }

        this->GetStatements().insert({statement.statement_name, stmt});
    }
}

std::unordered_map<const char*, sqlite3_stmt*>& objects::Database::GetStatements() {
    return this->statements;
}

void objects::Database::InitializeDatabaseTables() {
    const char* queries[] = {
        "CREATE TABLE IF NOT EXISTS hosted_server_users (id INTEGER PRIMARY KEY AUTOINCREMENT, ip_address INTEGER UNIQUE NOT NULL, server_id INTEGER NOT NULL);",
        "CREATE TABLE IF NOT EXISTS hosted_servers (id INTEGER PRIMARY KEY NOT NULL);",
        "CREATE TABLE IF NOT EXISTS joined_servers (id INTEGER PRIMARY KEY AUTOINCREMENT, ip_address INTEGER UNIQUE NOT NULL, server_id INTEGER NOT NULL);"
    };

    for(const auto& query : queries) {
        int result = sqlite3_exec(this->GetDatabaseConnection(), query, 0, 0, nullptr);
        if (result) {
            std::cerr << "Failed to create table" << std::endl;
            exit(EXIT_FAILURE);
        };
    }
}

int objects::Database::InsertHostedServerUser(const uint16_t server_id, in_addr ip_address) {
    sqlite3_stmt* stmt = this->GetStatement("insert_hosted_server_user");

    sqlite3_bind_int(stmt, 1, ip_address.s_addr);
    sqlite3_bind_int(stmt, 2, server_id);

    int result = sqlite3_step(stmt);
    if (result != SQLITE_DONE) {
        std::cerr << "Failed to insert hosted_server" << std::endl;
        return -1;
    }

    sqlite3_reset(stmt);

    return 0;
}

int objects::Database::InsertJoinedServer(const uint16_t server_id, in_addr ip_address) {
    sqlite3_stmt* stmt = this->GetStatement("insert_joined_server");

    sqlite3_bind_int(stmt, 1, ip_address.s_addr);
    sqlite3_bind_int(stmt, 2, server_id);

    int result = sqlite3_step(stmt);
    if (result != SQLITE_DONE) {
        std::cerr << "Failed to insert insert_joined_server" << std::endl;
        return -1;
    }

    sqlite3_reset(stmt);

    return 0;
}

int objects::Database::InsertHostedServer(const uint16_t server_id) {
    sqlite3_stmt* stmt = this->GetStatement("insert_hosted_server");

    sqlite3_bind_int(stmt, 1, server_id);

    int result = sqlite3_step(stmt);
    if (result != SQLITE_DONE) {
        std::cerr << "Failed to insert insert_hosted_server" << std::endl;
        return -1;
    }

    sqlite3_reset(stmt);

    return 0;
}

std::vector<objects::Database::JoinedServerRow>* objects::Database::SelectJoinedServers() {
    sqlite3_stmt* stmt = this->GetStatement("select_joined_servers");

    std::vector<objects::Database::JoinedServerRow>* result = new std::vector<objects::Database::JoinedServerRow>();

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        in_addr_t server_ip_address = sqlite3_column_int(stmt, 0);
        uint16_t server_id = sqlite3_column_int(stmt, 1);

        result->push_back((objects::Database::JoinedServerRow){.server_id = server_id, .ip_address = server_ip_address});
    }

    sqlite3_reset(stmt);

    return result;
}

std::vector<objects::Database::HostedServerRow>* objects::Database::SelectHostedServers() {
    sqlite3_stmt* stmt = this->GetStatement("select_hosted_servers");

    std::vector<objects::Database::HostedServerRow>* result = new std::vector<objects::Database::HostedServerRow>();

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        uint16_t id = sqlite3_column_int(stmt, 0);

        result->push_back((objects::Database::HostedServerRow){.server_id = id});
    }

    sqlite3_reset(stmt);

    return result;
}
