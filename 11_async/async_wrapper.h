#include "async.h"

class AsyncWrapper {
    std::unique_ptr<void, decltype(&async::disconnect)> ptr;
    auto getPtr() { return reinterpret_cast<async::Handle*>(ptr.get()); }
public:
    AsyncWrapper(std::size_t bulk)
        : ptr {async::connect(bulk),&async::disconnect}
    {}

    void receive(const std::string& str) {
        async::receive(ptr.get(), str.c_str(), str.size());
    }

    void join() {
        getPtr()->parser.endOfFile();
        getPtr()->file->pool->join();
    }

    auto blocksQty() {
        return getPtr()->file->blocksQty;
    }

    auto cmdQty() {
        return getPtr()->file->cmdQty;
    }

    auto threadQty() {
        return getPtr()->file->cmdQty.size();
    }

};

