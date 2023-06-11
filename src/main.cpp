#include "logger/logger.h"
#include <string>

int main(int argc, char *argv[])
{
    LOGGER::Logger *logger = LOGGER::Logger::GetInstance();
    for (uint64_t i = 0; i <= 1000000; i++)
    {
        if (i % 1000 == 0)
        {
            std::string message = "This is a test message number ";
            message += std::to_string(i);
            logger->log(message, LOGGER::LEVELS::INFO, LOGGER::FLAGS::DISPLAY | LOGGER::FLAGS::FORCE);
            logger->print();
        }
        else
        {
            std::string message = "This is a test message number ";
            message += std::to_string(i);
            logger->log(message, LOGGER::LEVELS::INFO, LOGGER::FLAGS::DISPLAY);
        }
    }
    return 0;
}
