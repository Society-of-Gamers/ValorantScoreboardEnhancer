#include "logger.h"

// variables definitions
LOGGER::Logger *LOGGER::Logger::c_Instance = nullptr;

// method implementations
LOGGER::Logger::Logger()
{
	activeMode = LOGGER::Logger::c_defaultFlags | LOGGER::Logger::c_defaultLevel;
	m_isRunning = true;
};

LOGGER::Logger::~Logger()
{
	m_isRunning = false;
	delete c_Instance;
	c_Instance = nullptr;
	// Deallocate m_thread
};
void LOGGER::Logger::print()
{
	writeBuffer(&m_messageBuffer, activeMode, m_messageBuffer.size - m_messageBuffer.lastPrinted);
	m_messageBuffer.lastPrinted = m_messageBuffer.size;
}
// void *LOGGER::Logger::m_printCycleInit(void *arg)
// {
// 	c_Instance->m_printCycle();
// 	return nullptr;
// }
bool LOGGER::Logger::log(const std::string message, const LOGGER::LEVELS &levelFlag, const uint8_t &flags)
{
	uint8_t level = static_cast<uint8_t>(levelFlag) & 0xF;
	Message msg = {message, static_cast<uint8_t>(level | flags), std::chrono::system_clock::now()};
	m_messageBuffer.addMessage(msg);
	m_messageBuffer.size++;
	return false;
}
void LOGGER::Logger::writeBuffer(MessageBuffer *buffer, const uint8_t &activeMode, const uint64_t &length)
{
	try
	{
		for (size_t traversal = buffer->lastPrinted; traversal < buffer->size; traversal++)
		{
			try
			{
				Message message = buffer->messages[traversal];
				bool shouldPrint = false;

				bool levelState = (message.flags & 0xF) - (activeMode & 0xF) >= 0;
				static constexpr uint8_t forcedFlag = static_cast<uint8_t>(LOGGER::FLAGS::FORCE);
				bool isForced = message.flags & forcedFlag;

				shouldPrint = levelState || isForced;

				if (shouldPrint)
				{
					uint8_t messageLevel = message.flags & 0xF;
					std::string messageLevelString = LOGGER::Logger::getLevelName(messageLevel);
					std::cout << "[" << messageLevelString << "]: " << message.message << '\n';
				}
			}
			catch (const std::exception &e)
			{
				std::printf("Error: %s\n", e.what());
			}
			buffer->lastPrinted = traversal;
		}
	}
	catch (const std::exception &e)
	{
		std::printf("Error: %s\n", e.what());
	}
}

// static method implementations
LOGGER::Logger *LOGGER::Logger::GetInstance()
{
	if (c_Instance == nullptr)
		c_Instance = new Logger();
	return c_Instance;
}
std::string LOGGER::Logger::getLevelName(const uint8_t &level)
{
	switch (level)
	{
	case static_cast<uint8_t>(LOGGER::LEVELS::INFO):
		return LOGGER_LEVEL_NAME_INFO;
	case static_cast<uint8_t>(LOGGER::LEVELS::WARNING):
		return LOGGER_LEVEL_NAME_WARNING;
	case static_cast<uint8_t>(LOGGER::LEVELS::ERROR):
		return LOGGER_LEVEL_NAME_ERROR;
	case static_cast<uint8_t>(LOGGER::LEVELS::FATAL):
		return LOGGER_LEVEL_NAME_FATAL;
	default:
		return "UNKNOWN LEVEL";
	}
}

// structures implementations
std::chrono::time_point<std::chrono::system_clock> LOGGER::MessageBuffer::addMessage(Message message)
{
	messages.push_back(message);
	return std::chrono::system_clock::now();
}