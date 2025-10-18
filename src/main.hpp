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
    LOAD_SERVER_INFORMATION
} RequestType;

typedef struct {
    RequestType request_type;
} RequestInfo;

typedef struct {
    RequestStatus status;
} JoinServerResponse;

typedef struct {
    RequestStatus status;
    uint32_t server_chat_channels_size;
    objects::Database::ServerChatChannelRow server_chat_channels[];
} LoadServerInformationResponse;

class Application : public wxApp
{
public:
    virtual bool OnInit();

    Application();
    ~Application();

    objects::LocalStorageDataManager* GetLocalStorageDataManager();

    frames::HomeFrame* GetHomeFrame();

    void AddChatRoomFrame(frames::ChatRoomFrame* frame);
    std::vector<frames::ChatRoomFrame*>* GetChatRoomFrames();
    frames::ChatRoomFrame* GetChatRoomFrameById(const uint16_t server_id);
private:
    frames::HomeFrame* home_frame;
    objects::LocalStorageDataManager* local_storage_data_manager;
    std::vector<frames::ChatRoomFrame*> chat_room_frames;
};

wxDECLARE_APP(Application);
