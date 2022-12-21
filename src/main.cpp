#include <iostream>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <Arr.h>
#include <Str.h>
#include <TypeAliases.h>
#include <File.h>
#include <Convert.h>

i32 main(i32 argc, const char** argv) {
    if (argc == 1) {
        std::cerr << "Please, specify path to bridges" << std::endl;
        exit(-1);
    }

    auto bridges_path = ml::Str(argv[1]);
    auto bridges_file = ml::File(bridges_path);
    if (!bridges_file.is_exist()) {
        std::cerr << "file: " << bridges_path << " is not exist" << std::endl;
        exit(-1);
    }

    auto bridges_lines = bridges_file.read().trim().split('\n');
    ml::Str ip;
    u16 port;
    ml::Str log; 
    ml::Str working; 

    auto working_file = ml::File("./working.txt");

    for (i32 i = 0; i < bridges_lines.len(); i++) {
        auto ip_port = bridges_lines[i].trim().split(' ')[1];
        auto splited_ip_port = ip_port.split(':');
        ip = splited_ip_port[0];
        port = ml::Convert::str_to_i32(splited_ip_port[1]);

        struct sockaddr_in address;  
        i16 sock = -1;        
        fd_set fdset;
        struct timeval tv;

        address.sin_family = AF_INET;
        address.sin_addr.s_addr = inet_addr(ip.to_c_str());
        address.sin_port = htons(port);            

        sock = socket(AF_INET, SOCK_STREAM, 0);
        fcntl(sock, F_SETFL, O_NONBLOCK);
        connect(sock, (struct sockaddr *)&address, sizeof(address));

        FD_ZERO(&fdset);
        FD_SET(sock, &fdset);
        tv.tv_sec = 4;             /* 4 second timeout */
        tv.tv_usec = 0;

        if (select(sock + 1, NULL, &fdset, NULL, &tv) == 1)
        {
            i32 so_error;
            socklen_t len = sizeof so_error;

            getsockopt(sock, SOL_SOCKET, SO_ERROR, &so_error, &len);

            if (so_error == 0) {
                std::cout << "[OK]\t\t" << ip << ":" << port << std::endl;
                working = working + bridges_lines[i].trim() + "\n";
            } else {
                std::cout << "[FAIL]\t\t" << ip << ":" << port << std::endl;
            }
        }

        close(sock);
    }

    working_file.write(working);

    return 0;
}