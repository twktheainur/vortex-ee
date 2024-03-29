/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_Gui.h"
#include "MyGUI_LayerManager.h"
#include "MyGUI_PointerManager.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_XmlDocument.h"

namespace MyGUI
{

	const std::string XML_TYPE("Pointer");

	INSTANCE_IMPLEMENT(PointerManager);

	void PointerManager::initialise()
	{
		MYGUI_ASSERT(false == mIsInitialise, INSTANCE_TYPE_NAME << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);

		WidgetManager::getInstance().registerUnlinker(this);
		Gui::getInstance().registerLoadXmlDelegate(XML_TYPE) = newDelegate(this, &PointerManager::_load);

		mMousePointer = null;
		mWidgetOwner = null;
		mShow = false;

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void PointerManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		// ������� ��� �������
		_destroyAllChildWidget();

		clear();

		WidgetManager::getInstance().unregisterUnlinker(this);
		Gui::getInstance().unregisterLoadXmlDelegate(XML_TYPE);

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully shutdown");
		mIsInitialise = false;
	}

	bool PointerManager::load(const std::string & _file, const std::string & _group)
	{
		return Gui::getInstance()._loadImplement(_file, _group, true, XML_TYPE, INSTANCE_TYPE_NAME);
	}

	void PointerManager::_load(xml::xmlNodePtr _node, const std::string & _file)
	{
		std::string layer, def, text;

		// ����� ����� � ��������, �������� ����
		xml::xmlNodeIterator pointer = _node->getNodeIterator();
		while (pointer.nextNode(XML_TYPE)) {

			// ������ ��������
			pointer->findAttribute("layer", layer);
			pointer->findAttribute("default", def);

			// ���������
			text = pointer->findAttribute("texture");

			IntSize textureSize = SkinManager::getTextureSize(text);

			// ����� ����� � ��������, �������� ����
			xml::xmlNodeIterator info = pointer->getNodeIterator();
			while (info.nextNode("Info")) {

				// �������� ����������
				FloatRect offset(0, 0, 1, 1);

				// ������ ��������
				std::string texture(info->findAttribute("texture"));
				std::string name(info->findAttribute("name"));
				std::string size(info->findAttribute("size"));
				IntPoint point = IntPoint::parse(info->findAttribute("point"));

				std::string offset_str(info->findAttribute("offset"));
				if (false == offset_str.empty()) {
					if (texture.empty()) offset = SkinManager::convertTextureCoord(FloatRect::parse(offset_str), textureSize);
					else offset = SkinManager::convertTextureCoord(FloatRect::parse(offset_str), SkinManager::getTextureSize(texture));
				}

				// ��������� ������
				if (mMapPointers.find(name) != mMapPointers.end()) {
					MYGUI_LOG(Warning, "pointer '" << name << "' exist, erase old data");
				}
				mMapPointers[name] = PointerInfo(offset, point, IntSize::parse(size), texture);

			};
		};

		// ���� ���� �����, �� �������������, ���� ��� �������, �� �������
		if (false == layer.empty()) {
			if (null == mMousePointer) {
				mMousePointer = _createWidget("Widget", "StaticImage", IntCoord(), ALIGN_DEFAULT, "", "");
			}
			LayerManager::getInstance().attachToLayerKeeper(layer, mMousePointer);
		}

		// ���� ���� ��������� ������ �� ������
		if (false == def.empty()) mDefaultPointer = def;
		if (false == text.empty()) mTexture = text;

		// ���� ���������� ���, �� ������� ������ �� ������
		if (mDefaultPointer.empty() && !mMapPointers.empty()) mDefaultPointer = mMapPointers.begin()->first;

		// ������ ��������� ���������
		setPointer(mDefaultPointer, null);
	}

	void PointerManager::clear()
	{
		/*if (null != mMousePointer) {
			WidgetManager::getInstance()._deleteWidget(mMousePointer);
			mMousePointer = null;
		}*/
		mWidgetOwner = null;
		mDefaultPointer.clear();
		mTexture.clear();
		mMapPointers.clear();
	}

	void PointerManager::show()
	{
		if (null != mMousePointer) mMousePointer->show();
		mShow = true;
	}

	void PointerManager::hide()
	{
		if (null != mMousePointer) mMousePointer->hide();
		mShow = false;
	}

	void PointerManager::setPosition(const IntPoint& _pos)
	{
		if (null != mMousePointer) mMousePointer->setPosition(_pos - mPoint);
	}

	void PointerManager::setPointer(const std::string & _name, WidgetPtr _owner)
	{
		if (null == mMousePointer) return;

		MapPointerInfo::iterator iter = mMapPointers.find(_name);
		if (iter == mMapPointers.end()) return;
		const FloatRect & rect = iter->second.offset;

		// ���� ������ ����� ���� ��������
		if (false == iter->second.texture.empty()) {
			if (mMousePointer->_getTextureName() != iter->second.texture) mMousePointer->_setTextureName(iter->second.texture);
		}
		else if (false == mTexture.empty()) {
			if (mMousePointer->_getTextureName() != mTexture) mMousePointer->_setTextureName(mTexture);
		}

		// �������� � ������ ������ � ������� ��������
		if (null != mMousePointer) {
			mMousePointer->setPosition(mMousePointer->getLeft()+mPoint.left-iter->second.point.left, mMousePointer->getTop()+mPoint.top-iter->second.point.top, iter->second.size.width, iter->second.size.height);
			mMousePointer->_setUVSet(rect);
		}

		// � ��������� ����� ��������
		mPoint = iter->second.point;
		mWidgetOwner = _owner;
	}

	void PointerManager::_unlinkWidget(WidgetPtr _widget)
	{
		if (_widget == mWidgetOwner) setPointer(mDefaultPointer, null);
		else if (_widget == mMousePointer) mMousePointer = null;
	}

	// ������� ������
	WidgetPtr PointerManager::_createWidget(const std::string & _type, const std::string & _skin, const IntCoord& _coord, Align _align, const std::string & _layer, const std::string & _name)
	{
		WidgetPtr widget = WidgetManager::getInstance().createWidget(_type, _skin, _coord, _align, null, this, _name);
		mWidgetChild.push_back(widget);
		// ������������ ������ � ������
		if (false == _layer.empty()) LayerManager::getInstance().attachToLayerKeeper(_layer, widget);
		return widget;
	}

	// ������� ����������
	void PointerManager::_destroyChildWidget(WidgetPtr _widget)
	{
		MYGUI_ASSERT(null != _widget, "invalid widget pointer");

		VectorWidgetPtr::iterator iter = std::find(mWidgetChild.begin(), mWidgetChild.end(), _widget);
		if (iter != mWidgetChild.end()) {

			// ��������� ���������
			MyGUI::WidgetPtr widget = *iter;

			// ������� �� ������
			*iter = mWidgetChild.back();
			mWidgetChild.pop_back();

			// ���������� �� ����
			WidgetManager::getInstance().unlinkFromUnlinkers(_widget);

			// ���������������� ��������
			_deleteWidget(widget);
		}
		else MYGUI_EXCEPT("Widget '" << _widget->getName() << "' not found");
	}

	// ������� ���� �����
	void PointerManager::_destroyAllChildWidget()
	{
		WidgetManager & manager = WidgetManager::getInstance();
		while (false == mWidgetChild.empty()) {

			// ����� ���� ���������, ����� ��������� �������� ������� ���
			WidgetPtr widget = mWidgetChild.back();
			mWidgetChild.pop_back();

			// ���������� �� ����
			manager.unlinkFromUnlinkers(widget);

			// � ���� ������, ��� ��� ��� ������ � ������ ���
			_deleteWidget(widget);
		}
	}

} // namespace MyGUI	
