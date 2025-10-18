#include <cstdlib>
#include <ctime>
#include <iostream>
#include <wx/wx.h>
#include "frames/frames.hpp"
#include "objects/objects.hpp"
#include "swift_net.h"
#include "main.hpp"

void Application::AddChatRoomFrame(frames::ChatRoomFrame* frame) {
    this->chat_room_frames.push_back(frame);
}

std::vector<frames::ChatRoomFrame*>* Application::GetChatRoomFrames() {
    return &this->chat_room_frames;
}

frames::ChatRoomFrame* Application::GetChatRoomFrameById(const uint16_t server_id) {
    for (auto server : *this->GetChatRoomFrames()) {
        std::cout << server->GetServerId() << " : " << server_id << std::endl;

        if (server->GetServerId() == server_id) {
            return server;
        }
    }

    return nullptr;
}

Application::Application() {
    this->home_frame = new frames::HomeFrame();
}

Application::~Application() {
    const objects::LocalStorageDataManager* local_storage_data_manager = this->GetLocalStorageDataManager();
    
    if (local_storage_data_manager != nullptr) {
        delete local_storage_data_manager;
    }

    swiftnet_cleanup();
}

frames::HomeFrame* Application::GetHomeFrame() {
    return this->home_frame;
}

objects::LocalStorageDataManager* Application::GetLocalStorageDataManager() {
    return this->local_storage_data_manager;
}

bool Application::OnInit() {
    swiftnet_initialize();

    swiftnet_add_debug_flags(DEBUG_INITIALIZATION | DEBUG_LOST_PACKETS | DEBUG_PACKETS_RECEIVING | DEBUG_PACKETS_SENDING);

    srand(time(0));

    frames::HomeFrame* home_frame = this->GetHomeFrame();

    this->local_storage_data_manager = new objects::LocalStorageDataManager();

    home_frame->GetHostingPanel()->DrawServers();
    home_frame->GetServersPanel()->DrawServers();

    home_frame->Show(true);
    return true;
}

wxIMPLEMENT_APP(Application);
