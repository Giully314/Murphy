/*
* murphy/log/log.ixx
* 
* This module defines easy access to logging inside the project. Logging can be customized by creating a 
* custom stream type and pass it to the Logger.
*/
export module murphy.log;

export import murphy.log.logger;
export import murphy.log.console_stream;
export import murphy.log.mutex_console_stream;
export import murphy.log.logger_system;