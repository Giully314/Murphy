/*
murphy/log/level.ixx

PURPOSE: Level of log.

CLASSES:
	Level: enum for enumerate the level of logging messages.
*/

export module murphy.log.logger:level;

import murphy.types;

namespace murphy::log
{
	export
	enum class Level : u8
	{
		Debug = 0,   //For debug.
		Info,		 //For information about the status of the program.
		Warning,	 //The program could have some errors that leads to bugs/exceptions.
		Error,		 //The program has serious errors that will interrupt the execution.
		Off,		 //Turn off the logger.
	}; 
} //namespace gargantua::log