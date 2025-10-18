#include "../frames.hpp"
#include <cstring>

frames::ChatRoomFrame::ChatChannel::ChatChannel(const uint32_t id, char name[20]) : id(id) {
    memcpy(this->name, name, sizeof(this->name));
}

uint32_t frames::ChatRoomFrame::ChatChannel::GetId() {
    return this->id;
}

const char* frames::ChatRoomFrame::ChatChannel::GetName() {
    return this->name;
}
