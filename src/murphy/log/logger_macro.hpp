#pragma once


#ifdef  MRP_LOGGER_ON
#define MRP_CORE_DEBUG(...)				::murphy::log::LoggerSystem::Instance().Debug(__VA_ARGS__)
#define MRP_CORE_INFO(...)				::murphy::log::LoggerSystem::Instance().Info(__VA_ARGS__)
#define MRP_CORE_WARNING(...)			::murphy::log::LoggerSystem::Instance().Warning(__VA_ARGS__)
#define MRP_CORE_ERROR(...)				::murphy::log::LoggerSystem::Instance().Error(__VA_ARGS__)
#define MRP_CORE_LOGGER_FILTER(x)		::murphy::log::LoggerSystem::Instance().SetLevel(x)

#else
#define MRP_CORE_DEBUG(...)		
#define MRP_CORE_INFO(...)		
#define MRP_CORE_WARNING(...)	
#define MRP_CORE_ERROR(...)		
#define MRP_CORE_LOGGER_FILTER(x)
#endif