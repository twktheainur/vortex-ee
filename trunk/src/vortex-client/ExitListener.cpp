#include "ExitListener.h"

#ifdef _EXITLISTENER_H
#include <Ogre.h>
#include <OIS/OIS.h>

using namespace Ogre;

ExitListener::ExitListener(OIS::Keyboard *keyboard)
    : mKeyboard(keyboard)
{
}
bool ExitListener::frameStarted(const FrameEvent& evt)
{
  mKeyboard->capture();
  return !mKeyboard->isKeyDown(OIS::KC_ESCAPE);
}

#endif


