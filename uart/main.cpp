#include "header.hpp"

#define SEND_PORT "/dev/ttyS0"
#define RECEIVE_PORT "/dev/tty1"

bool mode = 0;
string file = "a.jpg";
string port_path = "/dev/tty";


void PrintHelp();

int main(int argc, char *argv[])
{
    while (true)
    {
        const auto option = getopt_long(argc, argv, shortopts, longopts, nullptr);

        if (option == -1)
            break;

        switch (option)
        {
        case 's':
            break;

        case 'r':
            mode = 1;
            break;

        case 'f':
            file = optarg;
            break;

        case 'p':
            port_path = optarg;
            break;
        
        case 'h':
        case '?':
        default:
            PrintHelp();
        }        
    }

    int port = InitUART(port_path);
    
    if (mode)
        ReceiveFile(file, port);
    else
        SendFile(file, port);
    
    return 0;
}

void PrintHelp()
{
    cout << "Usage: ./uart -s(--send)/-r(--receive) -f(--file) path-to-file -p(--port) /dev/ttyX" << endl;
    exit(1);
}