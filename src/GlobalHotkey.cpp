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
#include <QtCore/QCryptographicHash.h>
#include <QtCore/QStringList.h>
#include <QtCore/QCoreApplication.h>

#ifdef _WIN32
#include <windows.h>

class WinEventFilter : public QAbstractNativeEventFilter{
	private:
		UINT mods, vk;
		std::function<void()> receiver;
	public:
		WinEventFilter(UINT mods, UINT vk, std::function<void()> receiver) : mods(mods), vk(vk), receiver(receiver){}
		bool nativeEventFilter(const QByteArray&, void* message, long*) override{
			MSG* msg = reinterpret_cast<MSG*>(message);
			if(msg->message == WM_HOTKEY && (msg->lParam & static_cast<UINT>(0xffff)) == this->mods && (msg->lParam >> 16 & static_cast<UINT>(0xffff)) == this->vk)
				this->receiver();
			return false;
		};
};

GlobalHotkey::GlobalHotkey(const char* keys, std::function<void()> receiver) : id(QCryptographicHash::hash(keys, QCryptographicHash::Md5).toInt()){
	QStringList key_list = QString(keys).split('|', QString::SkipEmptyParts);
	UINT mods = 0, vk = 0;
	for(QString& key : key_list){
		if(key == "ALT")
			mods |= MOD_ALT;
		else if(key == "CTRL")
			mods |= MOD_CONTROL;
		else if(key == "SHIFT")
			mods |= MOD_SHIFT;
		else if(key.length() == 1)
                        vk = key[0].toUpper().unicode();
	}
	if((this->ok = RegisterHotKey(NULL, this->id, mods, vk))){
		this->filter = new WinEventFilter(mods, vk, receiver);
		QCoreApplication::instance()->installNativeEventFilter(this->filter);
	}
}

GlobalHotkey::~GlobalHotkey(void){
	if(this->ok){
		QCoreApplication::instance()->removeNativeEventFilter(this->filter);
		delete this->filter;
		UnregisterHotKey(NULL, this->id);
	}
}
#else
#error "Non-windows body not implemented yet!"
#endif

bool GlobalHotkey::isOk() const{
	return this->ok;
}
