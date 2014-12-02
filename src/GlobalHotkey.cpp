/*
Project: MyCuteAssistant
File: GlobalHotkey.cpp

Copyright (c) 2014, Christoph "Youka" Spanknebel

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

	The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
	Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
	This notice may not be removed or altered from any source distribution.
*/

#include "GlobalHotkey.hpp"
#include <QtCore/QCryptographicHash>
#include <QtCore/QStringList>
#include <QtCore/QCoreApplication>

class NativeEventFilter : public QAbstractNativeEventFilter{
	private:
		std::function<void()> receiver;
	public:
		NativeEventFilter(std::function<void()> receiver) : receiver(receiver){}
		virtual bool isHotkey(void* message) = 0;
		bool nativeEventFilter(const QByteArray&, void* message, long*) override{
			if(this->isHotkey(message))
				this->receiver();
			return false;
		}
};

#ifdef _WIN32
#include <windows.h>

class WinEventFilter : public NativeEventFilter{
	private:
		UINT mods, vk;
	public:
		WinEventFilter(UINT mods, UINT vk, std::function<void()> receiver) : NativeEventFilter(receiver), mods(mods), vk(vk){}
		bool isHotkey(void* message) override{
			MSG* msg = reinterpret_cast<MSG*>(message);
			return msg->message == WM_HOTKEY && LOWORD(msg->lParam) == this->mods && HIWORD(msg->lParam) == this->vk;
		};
};

static QAbstractNativeEventFilter* registerHotkey(QStringList keys, int id, std::function<void()> receiver){
	UINT mods = 0, vk = 0;
	for(QString& key : keys){
		if(key == "ALT")
			mods |= MOD_ALT;
		else if(key == "CTRL")
			mods |= MOD_CONTROL;
		else if(key == "SHIFT")
			mods |= MOD_SHIFT;
		else if(key.length() == 1)
                        vk = key[0].toUpper().unicode();
	}
	return RegisterHotKey(NULL, id, mods, vk) ? new WinEventFilter(mods, vk, receiver) : nullptr;
}

static void unregisterHotkey(int id){
	UnregisterHotKey(NULL, id);
}

#else
#error "Non-windows body not implemented yet!"
#endif

GlobalHotkey::GlobalHotkey(const char* keys, std::function<void()> receiver) : id(QCryptographicHash::hash(keys, QCryptographicHash::Md5).toInt()),
										filter(registerHotkey(QString(keys).split('|', QString::SkipEmptyParts), this->id, receiver)){
	if(this->filter)
		QCoreApplication::instance()->installNativeEventFilter(this->filter);
}

GlobalHotkey::~GlobalHotkey(void){
	if(this->filter){
		QCoreApplication::instance()->removeNativeEventFilter(this->filter);
		delete this->filter;
		unregisterHotkey(this->id);
	}
}

bool GlobalHotkey::isOk() const{
	return this->filter;
}
