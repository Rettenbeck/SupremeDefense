#pragma once

#include <Util/basic.hpp>


namespace SupDef {

    const int NETWORK_MAX_WAIT = 2;
    
    const std::string NETWORK_MESSAGE_TYPE                  = "type"                                ;
    const std::string NETWORK_VERSION                       = "version"                             ;
    const std::string NETWORK_VERSION_CURRENT               = "SD_1.0.0"                            ;

    const int NETWORK_TYPE_DISCOVERY_REQUEST                = 10                                    ;
    const int NETWORK_TYPE_DISCOVERY_REPLY                  = 11                                    ;
    const int NETWORK_TYPE_JOIN_ACCEPTED                    = 20                                    ;
    const int NETWORK_TYPE_JOIN_REJECTED                    = 21                                    ;
    const int NETWORK_TYPE_DISCONNECT_FROM_CLIENT           = 30                                    ;
    const int NETWORK_TYPE_CLIENT_KICK                      = 31                                    ;

    const std::string NETWORK_IP_ADDRESS                    = "ip"                                  ;
    const std::string NETWORK_CONNECTION_PORT               = "connection_port"                     ;
    const std::string NETWORK_SERVERNAME                    = "servername"                          ;
    const std::string NETWORK_ASSIGNED_CLIENT_ID            = "client_id"                           ;
    const std::string NETWORK_REASON                        = "reason"                              ;
    const std::string NETWORK_REASON_SERVER_CLOSE           = "server_close"                        ;

}
