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
	QStringList Character::possibleNames(void){
		return QDir(QCoreApplication::applicationDirPath() + "/chars").entryList(QDir::AllDirs | QDir::NoDot | QDir::NoDotDot, QDir::Name);
	}

	Character::Character(QString name){
		this->load(name);
	}

	bool Character::load(QString name){
		this->id = name;
		QString char_dir = QCoreApplication::applicationDirPath() + "/chars/" + this->id;
		this->idle.setFileName(char_dir + "/idle.png"); if(!this->idle.isValid()) this->idle.setFileName(char_dir + "/idle.gif");
		this->move.setFileName(char_dir + "/move.png"); if(!this->move.isValid()) this->move.setFileName(char_dir + "/move.gif");
		return this->errorString().isEmpty();
	}

	QString Character::name(void) const{
		return this->id;
	}

	Character::State Character::state(void) const{
		return this->m_state;
	}

	void Character::state(Character::State current){
		this->m_state = current;
	}

	QMovie& Character::currentImage(void){
		switch(this->m_state){
			case Character::State::IDLE: return this->idle;
			case Character::State::MOVE: return this->move;
		}
		return this->null;
	}

	QString Character::errorString(void){
		QStringList result;
		if(!this->idle.isValid())
			result << "Couldn't load idle image!";
		else if(!this->move.isValid())
			result << "Couldn't load move image!";
		return result.join(' ');
	}
}
