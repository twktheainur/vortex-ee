/*!
	@file
	@author		Albert Semenov
	@date		05/2008
	@module
*/
#include "MyGUI_MenuBar.h"
#include "MyGUI_Button.h"
#include "MyGUI_PopupMenu.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_WidgetSkinInfo.h"
#include "MyGUI_Gui.h"
#include "MyGUI_CastWidget.h"

namespace MyGUI
{
	Ogre::String MenuBar::WidgetTypeName = "MenuBar";

	MenuBar::MenuBar(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name) :
		Widget(_coord, _align, _info, _parent, _creator, _name),
		mDistanceButton(0),
		mIndexSelect(ITEM_NONE)
	{
		for (VectorWidgetPtr::iterator iter=mWidgetChild.begin(); iter!=mWidgetChild.end(); ++iter) {
			if ((*iter)->_getInternalString() == "Client") {
				mWidgetClient = (*iter);
				//mWidgetClient->eventMouseWheel = newDelegate(this, &ItemBox::notifyMouseWheel);
				//mWidgetClient->eventMouseButtonPressed = newDelegate(this, &ItemBox::notifyMouseButtonPressed);
			}
		}

		// ������ ��������
		const MapString & param = _info->getParams();
		if (false == param.empty()) {
			MapString::const_iterator iter = param.find("DistanceButton");
			if (iter != param.end()) mDistanceButton = utility::parseInt(iter->second);

			iter = param.find("SkinButton");
			if (iter != param.end()) mButtonSkinName = iter->second;
		}

		if (mWidgetClient == null) mWidgetClient= this;
	}

	void MenuBar::insertItem(size_t _index, const Ogre::UTFString & _item)
	{
		MYGUI_ASSERT(_index < mVectorMenuItemInfo.size() || _index == ITEM_NONE, "index '" << _index << "' out of range");
		if (_index == ITEM_NONE) _index = mVectorMenuItemInfo.size();

		ButtonPtr button = mWidgetClient->createWidget<Button>(mButtonSkinName, IntCoord(), ALIGN_DEFAULT);
		button->eventMouseButtonPressed = newDelegate(this, &MenuBar::eventMouseButtonPressed);
		button->setCaption(_item);

		PopupMenuPtr menu = Gui::getInstance().createWidget<PopupMenu>("PopupMenu", IntCoord(), ALIGN_DEFAULT, "Popup");
		menu->eventPopupMenuClose = newDelegate(this, &MenuBar::notifyPopupMenuClose);
		menu->eventPopupMenuAccept = newDelegate(this, &MenuBar::notifyPopupMenuAccept);
		menu->_setOwner(this);

		mVectorMenuItemInfo.insert(mVectorMenuItemInfo.begin() + _index, MenuItemInfo(button, menu));
		update();
	}

	void MenuBar::setItem(size_t _index, const Ogre::UTFString & _item)
	{
		MYGUI_ASSERT(_index < mVectorMenuItemInfo.size(), "index '" << _index << "' out of range");
		mVectorMenuItemInfo[_index].button->setCaption(_item);

		update();
	}

	const Ogre::UTFString & MenuBar::getItemName(size_t _index)
	{
		MYGUI_ASSERT(_index < mVectorMenuItemInfo.size(), "index '" << _index << "' out of range");
		return mVectorMenuItemInfo[_index].button->getCaption();
	}

	PopupMenuPtr MenuBar::getItemMenu(size_t _index)
	{
		MYGUI_ASSERT(_index < mVectorMenuItemInfo.size(), "index '" << _index << "' out of range");
		return mVectorMenuItemInfo[_index].menu;
	}

	void MenuBar::deleteItem(size_t _index)
	{
		MYGUI_ASSERT(_index < mVectorMenuItemInfo.size(), "index '" << _index << "' out of range");

		WidgetManager::getInstance().destroyWidget(mVectorMenuItemInfo[_index].button);
		WidgetManager::getInstance().destroyWidget(mVectorMenuItemInfo[_index].menu);
		mVectorMenuItemInfo.erase(mVectorMenuItemInfo.begin() + _index);

		update();
	}

	void MenuBar::deleteAllItems()
	{
		for (VectorMenuItemInfo::iterator iter=mVectorMenuItemInfo.begin(); iter!=mVectorMenuItemInfo.end(); ++iter) {
			WidgetManager::getInstance().destroyWidget((*iter).button);
			WidgetManager::getInstance().destroyWidget((*iter).menu);
		}

		mVectorMenuItemInfo.clear();

		update();
	}

	void MenuBar::update()
	{
		int pos = 0;
		for (VectorMenuItemInfo::iterator iter=mVectorMenuItemInfo.begin(); iter!=mVectorMenuItemInfo.end(); ++iter) {
			int width = (*iter).button->getCoord().width - (*iter).button->getTextCoord().width + (*iter).button->getTextSize().width;
			(*iter).button->setPosition(pos, 0, width, mWidgetClient->getHeight());
			pos += width + mDistanceButton;
			(*iter).button->_setInternalData(iter - mVectorMenuItemInfo.begin());
		}
	}

	void MenuBar::eventMouseButtonPressed(MyGUI::WidgetPtr _sender, int _left, int _top, MouseButton _id)
	{
		if (_id == MB_Left) {
			size_t select = (size_t)_sender->_getInternalData();
			if (mIndexSelect == select) select = ITEM_NONE;
			setItemSelect(select);
		}
	}

	void MenuBar::setItemSelect(size_t _index)
	{
		if (mIndexSelect == _index) return;

		MYGUI_ASSERT(_index < mVectorMenuItemInfo.size() || _index == ITEM_NONE, "index '" << _index << "' out of range");

		if (mIndexSelect != ITEM_NONE) {
			mVectorMenuItemInfo[mIndexSelect].button->setButtonPressed(false);
			mVectorMenuItemInfo[mIndexSelect].menu->hidePopupMenu();
		}

		mIndexSelect = _index;

		if (mIndexSelect != ITEM_NONE) {
			mVectorMenuItemInfo[mIndexSelect].button->setButtonPressed(true);
			mVectorMenuItemInfo[mIndexSelect].menu->showPopupMenu(
				IntPoint(mVectorMenuItemInfo[mIndexSelect].button->getAbsoluteLeft(), getAbsoluteRect().bottom));
		}
	}

	void MenuBar::notifyPopupMenuClose(WidgetPtr _sender)
	{
		// ��� ������ �� �������� ����� ���� ���������� �� �����
		if (mIndexSelect != ITEM_NONE) {
			const IntPoint & point = InputManager::getInstance().getMousePosition();
			WidgetPtr widget = InputManager::getInstance().getWidgetFromPoint(point.left, point.top);
			if (mVectorMenuItemInfo[mIndexSelect].button == widget) return;
		}
		resetItemSelect();
	}

	void MenuBar::notifyPopupMenuAccept(WidgetPtr _sender, size_t _index)
	{
		resetItemSelect();
		eventPopupMenuAccept(this, castWidget<PopupMenu>(_sender), _index);
	}

} // namespace MyGUI
