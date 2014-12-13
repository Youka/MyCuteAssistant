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
#include <QtCore/QSettings>

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
		QStringList error_list;
		QSettings properties(char_dir + "/properties.ini", QSettings::IniFormat, nullptr);
		if(properties.status())
			error_list << "Couldn't read properties!";
		else{
			this->idle.setFileName(char_dir + '/' + properties.value("Images/idle", "").toString());
			if(!this->idle.isValid())
				error_list << "Couldn't load idle image!";
			this->move.setFileName(char_dir + '/' + properties.value("Images/move", "").toString());
			if(!this->move.isValid())
				error_list << "Couldn't load move image!";
			if(!this->m_bubble.pixmap.load(char_dir + '/' + properties.value("Images/bubble", "").toString()))
				error_list << "Couldn't load bubble image!";
			this->m_bubble.x = properties.value("Geometries/bubble_x", this->m_bubble.x).toDouble(),
			this->m_bubble.y = properties.value("Geometries/bubble_y", this->m_bubble.y).toDouble(),
			this->m_bubble.edit_x = properties.value("Geometries/bubble_edit_x", this->m_bubble.edit_x).toDouble(),
			this->m_bubble.edit_y = properties.value("Geometries/bubble_edit_y", this->m_bubble.edit_y).toDouble(),
			this->m_bubble.edit_width = properties.value("Geometries/bubble_edit_width", this->m_bubble.edit_width).toDouble(),
			this->m_bubble.edit_height = properties.value("Geometries/bubble_edit_height", this->m_bubble.edit_height).toDouble();
		}
		return (this->error = error_list.join(' ')).isEmpty();
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

	Character::Bubble& Character::bubble(void){
		return this->m_bubble;
	}

	QString Character::errorString(void) const{
		return this->error;
	}
}
