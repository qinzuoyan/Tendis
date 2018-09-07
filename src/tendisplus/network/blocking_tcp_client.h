#ifndef SRC_TENDISPLUS_NETWORK_BLOCKING_TCP_CLIENT_H_
#define SRC_TENDISPLUS_NETWORK_BLOCKING_TCP_CLIENT_H_

#include <string>
#include <chrono>
#include <memory>
#include "asio.hpp"
#include "tendisplus/utils/status.h"

namespace tendisplus {
class BlockingTcpClient: public std::enable_shared_from_this<BlockingTcpClient> {
 public:
    BlockingTcpClient(std::shared_ptr<asio::io_context> ctx, size_t maxBufSize);
    BlockingTcpClient(std::shared_ptr<asio::io_context> ctx,
        asio::ip::tcp::socket, size_t maxBufSize);
    Status connect(const std::string& host, uint16_t port,
        std::chrono::seconds timeout);
    Expected<std::string> readLine(std::chrono::seconds timeout);
    Expected<std::string> read(size_t bufSize, std::chrono::seconds timeout);
    Status writeLine(const std::string& line, std::chrono::seconds timeout);
    Status writeData(const std::string& data, std::chrono::seconds timeout);

    std::string getRemoteRepr() const {
        if (_socket.is_open()) {
            return _socket.remote_endpoint().address().to_string();
        }
        return "closed conn";
    }

    std::string getLocalRepr() const {
        if (_socket.is_open()) {
            return _socket.local_endpoint().address().to_string();
        }
        return "closed conn";
    }

    size_t getReadBufSize() const { return _inputBuf.size(); }

    asio::ip::tcp::socket borrowConn();

 private:
    void closeSocket();
    std::mutex _mutex;
    std::condition_variable _cv;
    bool _inited;
    bool _notified;
    asio::error_code _ec;
    std::shared_ptr<asio::io_context> _ctx;
    asio::ip::tcp::socket _socket;
    asio::streambuf _inputBuf;
};

}  // namespace tendisplus

#endif  // SRC_TENDISPLUS_NETWORK_BLOCKING_TCP_CLIENT_H_
