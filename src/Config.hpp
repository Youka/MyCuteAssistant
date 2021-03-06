/*
Project: MyCuteAssistant
File: Config.hpp

Copyright (c) 2014, Christoph "Youka" Spanknebel

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

	The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
	Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
	This notice may not be removed or altered from any source distribution.
*/

#pragma once

#include <QtCore/QSettings>
#include <QtCore/QPoint>

namespace MCA{
	class Config : protected QSettings{
		public:
			// Object managment
			static Config* instance(void);
			Config(QString filename);
			~Config(void);
			// Settings
			bool alwaysOnTop(void);
			void alwaysOnTop(bool on);
			QString character(void);
			void character(QString name);
			unsigned char opacity(void);
			void opacity(unsigned char strength);
			unsigned short size(void);
			void size(unsigned short pct);
			QString hotkey(void);
			void hotkey(QString keys);
			QPoint position(void);
			void position(QPoint pos);
	};
}
