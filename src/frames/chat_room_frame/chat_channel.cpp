#include "../frames.hpp"
#include <cstring>

using namespace frames;

ChatRoomFrame::ChatChannel::ChatChannel(const uint32_t id, char name[20]) : id(id) {
    memcpy(this->name, name, sizeof(this->name));
}

ChatRoomFrame::ChatChannel::~ChatChannel() {

}

uint32_t ChatRoomFrame::ChatChannel::GetId() {
    return this->id;
}

const char* ChatRoomFrame::ChatChannel::GetName() {
    return this->name;
}
