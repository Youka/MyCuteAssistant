/*
Project: MyCuteAssistant
File: GlobalHotkey.hpp

Copyright (c) 2014, Christoph "Youka" Spanknebel

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

	The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
	Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
	This notice may not be removed or altered from any source distribution.
*/

#pragma once

#include <QtCore/QAbstractNativeEventFilter.h>
#include <functional>

class GlobalHotkey{
	private:
		int id;
		QAbstractNativeEventFilter* filter;
		bool ok;
	public:
		GlobalHotkey(const char* keys, std::function<void()> receiver);
		~GlobalHotkey(void);
		bool isOk() const;
};
