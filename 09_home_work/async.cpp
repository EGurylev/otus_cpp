#include "async.h"
#include "async_bulk.h"

namespace async {

handle_t connect(std::size_t bulk) {
    auto& async_bulk {AsyncBulk::getInstance()};
    return async_bulk.create_connection(bulk);
}

void receive(handle_t handle, const char *data, std::size_t size) {
    auto& async_bulk {AsyncBulk::getInstance()};
    async_bulk.update(static_cast<Connection*>(handle), data, size);
}

void disconnect(handle_t handle) {
    auto& async_bulk {AsyncBulk::getInstance()};
    async_bulk.remove_connection(static_cast<Connection*>(handle));
}

}