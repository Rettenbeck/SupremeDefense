#pragma once

#include <Util/basic.hpp>


namespace SupDef {

    using ClientId = std::uint32_t;
    using ClientIds = std::vector<ClientId>;
    const ClientId CLIENT_ID_SERVER = -1;

    struct NetMessage {
        json j;
        bool ok = false;
        int type = 0;
        std::string error = "";
        ClientId clientId = CLIENT_ID_SERVER;

        static NetMessage NoMessage() {
            NetMessage result;
            result.ok = false;
            return result;
        }
    };

    using NetMessages = std::vector<NetMessage>;

    struct DiscoveryReply {
        bool ok = false;
        std::string ip = "";
        unsigned short port = 0;
        std::string server_name = "";
        std::string description = "";
    };

    using DiscoveryReplies = std::vector<DiscoveryReply>;

    // // ---------- Common types ----------
    struct Endpoint {
        std::string ip;      // dotted quad or hostname
        unsigned short port; // host order
        Endpoint(std::string ip_, unsigned short port_) {
            ip = ip_; port = port_;
        }
    };

    struct NetResult {
        bool ok;
        std::string error;   // empty if ok
        int sysError = 0;    // optional, OS error code for debugging
        
        static NetResult Ok() { return {true, "", 0}; }
        static NetResult Error(std::string msg) { return {false, msg, 0}; }
        static NetResult Error(std::string msg, int sysError_) { return {false, msg, sysError_}; }
    };

    enum class Role { None, Client, Server };

}
