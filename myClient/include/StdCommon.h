#ifndef STDCOMMON
#define STDCOMMON

#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif

#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

#include <chrono>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <thread>
#include <mutex>


#endif // STDCOMMON