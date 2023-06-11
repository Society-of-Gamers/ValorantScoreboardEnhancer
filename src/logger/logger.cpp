#include "logger.h"

// variables definitions
LOGGER::Logger *LOGGER::Logger::c_Instance = nullptr;

// method implementations
LOGGER::Logger::Logger()
{
	activeMode = LOGGER::Logger::c_defaultFlags;
	m_isRunning = true;
	m_thread = new pthread_t(); // Initialize m_thread
	if (pthread_create(m_thread, nullptr, &m_printCycleInit, nullptr) != 0)
	{
		throw std::runtime_error("Failed to create thread");
	}
};

LOGGER::Logger::~Logger()
{
	m_isRunning = false;
	pthread_join(*m_thread, nullptr);
	delete c_Instance;
	c_Instance = nullptr;
	// Deallocate m_thread
	delete m_thread;
	m_thread = nullptr;
};
void *LOGGER::Logger::m_printCycle()
{
	while (m_isRunning)
	{
		writeBuffer(m_messageBuffer, activeMode);
	};
	return nullptr;
}
void *LOGGER::Logger::m_printCycleInit(void *arg)
{
	c_Instance->m_messageBuffer.lastPrinted = c_Instance->writeBuffer(c_Instance->m_messageBuffer, c_Instance->activeMode);
	return nullptr;
}
bool LOGGER::Logger::log(const std::string message, const uint8_t flags)
{
	Message msg = {message, flags, std::chrono::system_clock::now()};
	m_messageBuffer.addMessage(msg);
	m_messageBuffer.size++;
	return false;
}
uint64_t LOGGER::Logger::writeBuffer(const MessageBuffer &buffer, const uint8_t &activeMode)
{
	uint64_t len = buffer.size;
	// Check if lastPrinted is greater than or equal to len
	if (buffer.lastPrinted >= len)
	{
		// This means all messages in buffer have been printed, so return zero
		return buffer.lastPrinted;
	}
	// Iterate over each message in the span using a range-based for loop
	const auto messageBufferBegin = std::begin(buffer.messages);
	const auto messageBufferEnd = messageBufferBegin + len;

	for (auto messagePtr = messageBufferBegin; messagePtr != messageBufferEnd - 1; ++messagePtr)
	{
		try
		{
			const Message &message = *messagePtr;
			bool shouldPrint = false;

			uint8_t flagCompare = (message.flags & activeMode);
			static constexpr uint8_t forcedFlag = static_cast<uint8_t>(LOGGER::FLAGS::FORCE);
			bool isForced = message.flags & forcedFlag;

			shouldPrint = flagCompare > 0 || isForced;

			if (shouldPrint)
			{
				uint8_t messageLevel = message.flags & 0xF;
				std::printf("[%s] %s\n", LOGGER::Logger::getLevelName(messageLevel).c_str(), message.message.c_str());
			}
		}
		catch (const std::exception &e)
		{
			std::printf("Error: %s\n", e.what());
		}
	}
	return len;
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
		return std::string(LOGGER_LEVEL_NAME_INFO);
	case static_cast<uint8_t>(LOGGER::LEVELS::WARNING):
		return std::string(LOGGER_LEVEL_NAME_WARNING);
	case static_cast<uint8_t>(LOGGER::LEVELS::ERROR):
		return std::string(LOGGER_LEVEL_NAME_ERROR);
	case static_cast<uint8_t>(LOGGER::LEVELS::FATAL):
		return std::string(LOGGER_LEVEL_NAME_FATAL);
	default:
		return nullptr;
	}
}

// structures implementations
std::chrono::time_point<std::chrono::system_clock> LOGGER::MessageBuffer::addMessage(const Message &message)
{
	messages.push_back(message);
	return std::chrono::system_clock::now();
}