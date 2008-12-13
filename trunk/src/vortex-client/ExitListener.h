#ifndef _EXITLISTENER_H
#define _EXITLISTENER_H

#include <Ogre.h>
#include <OIS/OIS.h>

using namespace Ogre;

class ExitListener : public FrameListener
{
public:
    ExitListener(OIS::Keyboard *keyboard);
    bool frameStarted(const FrameEvent& evt);
private:
    OIS::Keyboard *mKeyboard;
};
#endif
