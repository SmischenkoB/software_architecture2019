#include "stdafx.h"

Controller::Controller() : UpdateList()
{
	object = nullptr;
}

Controller::~Controller()
{
	Remove();
}

void Controller::Remove()
{
	if (!object)
		return;

	Controller *c = object->pControllers;
	if (c == this) {
		object->pControllers = static_cast<Controller*>(pBrothers);
	}
	else {
		while (c) {
			if (c->pBrothers == this) {
				c->pBrothers = pBrothers;
				break;
			}
			c = static_cast<Controller*>(c->pBrothers);
		}
	}
	pParent = NULL;
}
