#pragma once

#include <cstdint>
#include <vector>
#include <wx/wx.h>
#include "frames/frames.hpp"
#include "objects/objects.hpp"
#include <sqlite3.h>
#include "objects/objects.hpp"

// Request Types

typedef enum {
    SUCCESS,
    FAIL
} RequestStatus;

typedef enum {
    JOIN_SERVER,
    LOAD_SERVER_INFORMATION,
    LOAD_CHANNEL_DATA
} RequestType;

typedef struct {
    RequestType request_type;
} RequestInfo;

// Requests
typedef struct {
    uint32_t channel_id;
} LoadChannelDataRequest;

// Responses
typedef struct {
    RequestStatus status;
} JoinServerResponse;

typedef struct {
    RequestStatus status;
    uint32_t server_chat_channels_size;
    objects::Database::ServerChatChannelRow server_chat_channels[];
} LoadServerInformationResponse;

typedef struct {
    RequestStatus status;
    uint32_t channel_messages_len;
} LoadChannelDataResponse;

class Application : public wxApp
{
public:
    Application();
    ~Application();

    virtual bool OnInit();

    void AddChatRoomFrame(frames::ChatRoomFrame* frame);

    objects::LocalStorageDataManager* GetLocalStorageDataManager();
    frames::HomeFrame* GetHomeFrame();
    std::vector<frames::ChatRoomFrame*>* GetChatRoomFrames();
    frames::ChatRoomFrame* GetChatRoomFrameById(const uint16_t server_id);
private:
    frames::HomeFrame* home_frame;

    objects::LocalStorageDataManager* local_storage_data_manager;

    std::vector<frames::ChatRoomFrame*> chat_room_frames;
};

wxDECLARE_APP(Application);
