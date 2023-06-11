#include "logger/logger.h"
#include <string>

int main(int argc, char* argv[])
{
    LOGGER::Logger* logger = LOGGER::Logger::GetInstance();
    for (uint16_t i = 0; i < 10000; i++)
    {
        std::string message = "This is a test message number ";
        message += std::to_string(i);
        logger->log(message, LOGGER::Logger::c_defaultFlags);
    }
    return 0;
}
