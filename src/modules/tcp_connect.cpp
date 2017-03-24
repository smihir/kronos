/*
 * Logs the time required to establish tcp connection to google.com
 */
#include <cstring>
#include <cerrno>
#include <module.h>
#include <netdb.h>
#include <netinet/in.h>
#include <spdlog/spdlog.h>
#include <sys/socket.h>
#include <sys/types.h>

using namespace kronos::module;

auto tcplogger = spdlog::stdout_color_mt("tcpconnect");

class TCPConnectTask : public PersistModule {
    public:
        TCPConnectTask() : PersistModule("tcpconnect") {};
        float run() {
            int fd, err;  
            struct addrinfo hint, *servinfo, *p;
            unsigned int ms;

            memset(&hint, 0, sizeof hint);
            hint.ai_family = AF_INET;
            hint.ai_socktype = SOCK_STREAM;

            if ((err = getaddrinfo(std::string("google.com").c_str(), "80", &hint, &servinfo)) != 0) {
                tcplogger->error("getaddrinfo: {}", std::string(gai_strerror(err)));
                return -1;
            }

            // loop through all the results and connect to the first we can
            for(p = servinfo; p != NULL; p = p->ai_next) {
                if ((fd = socket(p->ai_family, p->ai_socktype,
                        p->ai_protocol)) == -1) {
                    tcplogger->error("socket error: {}", strerror(errno));
                    continue;
                }

                auto begin = std::chrono::high_resolution_clock::now();
                if (connect(fd, p->ai_addr, p->ai_addrlen) == -1) {
                    close(fd);
                    tcplogger->error("connect error error: {}", strerror(errno));
                    continue;
                }
                auto end = std::chrono::high_resolution_clock::now();
                ms = std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count();
                close(fd);
                break;
            }

            freeaddrinfo(servinfo);
            if (p == NULL) {
                tcplogger->error("failed to get any hints");
                return -1;
            }
            return ms;
        }
        
        ~TCPConnectTask(){}
};

REGISTER(TCPConnectTask,5);
