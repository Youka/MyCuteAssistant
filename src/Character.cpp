/*
Project: MyCuteAssistant
File: Character.cpp

Copyright (c) 2014, Christoph "Youka" Spanknebel

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

	The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
	Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
	This notice may not be removed or altered from any source distribution.
*/

#include "Character.hpp"
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>

namespace MCA{
	QStringList Character::possibleNames(){
		return QDir(QCoreApplication::applicationDirPath() + "/chars").entryList(QDir::AllDirs | QDir::NoDot | QDir::NoDotDot, QDir::Name);
	}

	Character::Character(QString name){
		QString char_dir = QCoreApplication::applicationDirPath() + "/chars/" + name;
		this->idle.load(char_dir + "/idle.png", "PNG");
	}

	bool Character::allLoaded(){
		return !this->idle.isNull();
	}
}
