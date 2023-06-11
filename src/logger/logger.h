#pragma once
#include <array>
#include <chrono>
#include <ctime>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <iomanip>
#include <iostream>
#include <exception>
#include <ctime>

#define LOGGER_LEVEL_NAME_INFO      "INFO"
#define LOGGER_LEVEL_NAME_WARNING   "WARNING"
#define LOGGER_LEVEL_NAME_ERROR     "ERROR"
#define LOGGER_LEVEL_NAME_FATAL     "FATAL"

namespace LOGGER
{
    struct Message
    {
        std::string message;
        uint8_t flags;
        std::chrono::time_point<std::chrono::system_clock> timestamp;
    };

    struct MessageBuffer
    {
        std::vector<Message> messages;
        std::chrono::time_point<std::chrono::system_clock> addMessage(const Message &message);
        uint64_t lastPrinted = 0;
        uint64_t size = 0;
    };

    enum LEVELS
    {
        INFO = 0b1 << 0,    // 1 is the info level, just information to be included when necessary
        WARNING = 0b1 << 1, // 2 is the second lowest level, just warnings to be included when necessary
        ERROR = 0b1 << 2,   // 4 is the second highest level, just errors that can cause failures
        FATAL = 0b1 << 3    // 8 is the highest level, just for fatal errors, information about failures
    };

    enum FLAGS
    {
        WRITE = 0b1 << 4,   // 8 is the write flag, writing the log to a file (optional)
        SEND = 0b1 << 5,    // 32 is the send flag, sending the log to a server (optional)
        DISPLAY = 0b1 << 6, // 64 is the display flag, displaying the log in the console (default)
        FORCE = 0b1 << 7    // 128 is the force flag, forcing the log to be written, sent and/or displayed (optional)
    };

    class Logger
    {
    private:
        uint8_t activeMode;
        MessageBuffer m_messageBuffer;

        pthread_t *m_thread;
        static Logger *c_Instance;
        Logger();
        ~Logger();
        uint64_t writeBuffer(const MessageBuffer &buffer, const uint8_t &activeMode);
        static std::string getLevelName(const uint8_t &level);
        static void *m_printCycleInit(void *arg);
        void *m_printCycle();

    protected:
        volatile bool m_isRunning = true;
    public:
        const static uint8_t c_defaultFlags = static_cast<uint8_t>(LOGGER::LEVELS::FATAL) | static_cast<uint8_t>(LOGGER::FLAGS::DISPLAY);
        static Logger *GetInstance();
        bool log(const std::string message, const uint8_t flags = c_defaultFlags);
    };
}