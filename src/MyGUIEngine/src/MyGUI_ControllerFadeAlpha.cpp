/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#include "MyGUI_Prerequest.h"
#include "MyGUI_ControllerFadeAlpha.h"
#include "MyGUI_Gui.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{

	ControllerFadeAlpha::ControllerFadeAlpha()
	{
	}

	ControllerFadeAlpha::ControllerFadeAlpha(float _alpha, float _coef, bool _enabled) :
		mAlpha(_alpha), mCoef(_coef), mEnabled(_enabled)
	{
	}

	const std::string & ControllerFadeAlpha::getType()
	{
		static std::string type("FadeAlphaController");
		return type;
	}

	void ControllerFadeAlpha::prepareItem(WidgetPtr _widget)
	{
		MYGUI_DEBUG_ASSERT(mCoef > 0, "coef must be > 0");
		// ���������� �������
		_widget->setEnabledSilent(mEnabled);

		if ((ALPHA_MIN != mAlpha) && (false == _widget->isShow())) {
			_widget->setAlpha(ALPHA_MIN);
			_widget->show();
		}

		// ���������� ��� �� �����
		if (false == mEnabled) InputManager::getInstance()._unlinkWidget(_widget);

		// �������� ���������������� ������� ��� ����������
		eventPreAction(_widget);
	}

	void ControllerFadeAlpha::replaseItem(WidgetPtr _widget, ControllerItem * _item)
	{
		ControllerFadeAlpha * item = static_cast<ControllerFadeAlpha*>(_item);
		mEnabled = item->getEnabled();
		mCoef = item->getCoef();
		mAlpha = item->getAlpha();
	}

	bool ControllerFadeAlpha::addTime(WidgetPtr _widget, float _time)
	{
		float alpha = _widget->getAlpha();

		// ��������� ����� �� � ���� ��� ����������
		if (mAlpha > alpha) {
			alpha += _time * mCoef;
			if (mAlpha > alpha) {
				_widget->setAlpha(alpha);
				return true;
			}
			else {
				_widget->setAlpha(mAlpha);
			}
		}
		else if (mAlpha < alpha) {
			alpha -= _time * mCoef;
			if (mAlpha < alpha) {
				_widget->setAlpha(alpha);
				return true;
			}
			else {
				_widget->setAlpha(mAlpha);
			}
		}

		// �������� ���������������� ������� ���� ���������
		eventPostAction(_widget);

		return false;
	}

} // namespace MyGUI
