/*!
	@file
	@author		Albert Semenov
	@date		05/2008
	@module
*/
#include "MyGUI_MenuBarFactory.h"
#include "MyGUI_MenuBar.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		MenuBarFactory::MenuBarFactory()
		{
			// ������������ ����
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);

			// ������������ ��� �������
			manager.registerDelegate("MenuBar_AddItem") = newDelegate(this, &MenuBarFactory::MenuBar_AddItem);
		}

		MenuBarFactory::~MenuBarFactory()
		{
			// ������� ����
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.unregisterFactory(this);

			// ������� ��� �������
			manager.unregisterDelegate("MenuBar_AddItem");
		}

		const Ogre::String& MenuBarFactory::getType()
		{
			return MenuBar::_getType();
		}

		WidgetPtr MenuBarFactory::createWidget(const Ogre::String& _skin, const IntCoord& _coord, Align _align, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String& _name)
		{
			return new MenuBar(_coord, _align, SkinManager::getInstance().getSkin(_skin), _parent, _creator, _name);
		}

		void MenuBarFactory::MenuBar_AddItem(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(MenuBarPtr, _widget, _key);
			static_cast<MenuBarPtr>(_widget)->addItem(_value);
		}

	} // namespace factory
} // namespace MyGUI
