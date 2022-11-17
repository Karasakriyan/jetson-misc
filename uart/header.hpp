#pragma once

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>
#include <getopt.h>

using namespace std;

const char* const shortopts = "srf:p:h";
const option longopts[] = 
{
    {"send", no_argument, nullptr, 's'},
    {"receive", no_argument, nullptr, 'r'},
    {"file", required_argument, nullptr, 'f'},
    {"port", required_argument, nullptr, 'p'},
    {"help", no_argument, nullptr, 'h'},
    {nullptr, no_argument, nullptr, 0}
};

/// @brief Initialising port
/// @param port /dev/tty...
/// @return Port descriptor
int InitUART(string port)
{
    int uart = 0;
    try
    {
        uart = open(port.c_str(), O_RDWR);
    }
    catch(const exception& e)
    {
        cerr << "UART opening error: " << e.what() << '\n';
        return uart;
    }

    struct termios tty;

    tty.c_cflag &= ~PARENB;                                             // Set parity bit to none
    tty.c_cflag &= ~CSTOPB;                                             // Set stop bits to one
    tty.c_cflag &= ~CSIZE;                                              // Clear size bits
    tty.c_cflag |= CS8;                                                 // Set byte equal to 8 bits
    tty.c_cflag &= ~CRTSCTS;                                            // Disable hardware flow control
    tty.c_lflag &= ~ICANON;                                             // Disable canonical mode
    tty.c_lflag &= ~ISIG;                                               // Disable signal chars
    tty.c_iflag &= ~(IXON|IXOFF|IXANY);                                 // Disable software flow control
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);    // Make all data raw
    tty.c_oflag &= ~OPOST;                                              // Prevent special interpret of out bytes
    tty.c_oflag &= ~ONLCR;                                              // Prevent conversion of newline to carriage etc.
    cfsetspeed(&tty, B115200);

    if (tcsetattr(uart, TCSANOW, &tty) != 0)
        cout << "UART init error!" << endl;
    return uart;
}

/// @brief Send file to UART
/// @param path Path to file
/// @param port Descriptor of port
void SendFile(string path, int port)
{
    ifstream in;
    in.open(path, ios_base::in | ios_base::binary);

    char byte;
    while (in.read(&byte, sizeof(byte)))
        write(port, &byte, sizeof(byte));
}

/// @brief Write file to disk
/// @param path Byte-array file representation
/// @param port Descriptor of port
void ReceiveFile(string path, int port)
{
    ofstream out;
    out.open(path, ios_base::app | ios_base::binary);

    char byte;
    while(read(port, &byte, sizeof(byte)));
}