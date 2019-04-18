#pragma once

// -------------------------------------------------------------------------------------
// Base class from which updatable objects are derived
// It also provides a basic hierarchy implementation
// Update is also called down the hierarchy (towards the children)
// -------------------------------------------------------------------------------------

class UpdateList
{
public:
	enum Type
	{
		UPDATELIST,
		OBJECT,
		CONTROLLER,
		CAMERA,
		WORLDOBJECT,
		MENUITEM,
		PHYSICSOBJECT,
		EDITBOX,
		PLAYER,
		ENEMY,
		BULLET,
		EXPLOSION
	};

	UpdateList();
	virtual ~UpdateList();

	virtual UpdateList* AddChild(UpdateList *child);
	virtual void OnAddedAsChild() {}
	virtual void Remove();

	// Override this Update method to implement your own update functionality
	// Don't forget to call the base's class Update method when overriding
	virtual void Update(float dt);

	// Override this Render method to implement your own draw functionality
	// Don't forget to call the base's class Render method when overriding
	virtual void Render();

	// Override this UpdateGlobal method to copy or compute any global values used by the Render step
	// Don't forget to call the base's class UpdateGlobal method when overriding
	virtual void UpdateGlobal();

	virtual Type GetType() { return Type::UPDATELIST; }
	virtual bool IsTypeOf(Type type) { return type == Type::UPDATELIST; }

	UpdateList *pParent;
	UpdateList *pBrothers;
	UpdateList *pChildren;
};