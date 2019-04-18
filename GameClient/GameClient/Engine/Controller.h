#pragma once

// -------------------------------------------------------------------------------------
// Class that is used to extend an Object's behavior, if deriving the Object is not a good option.
// Mostly useful if the extended behavior offered by this controller is common to multiple
//	object types or you already have an instance of an object created by the engine and
//	deriving it is not an option.
// Overide the Update method to change the functionality of the parent object
// -------------------------------------------------------------------------------------

class Object;

class Controller : public UpdateList
{
public:
	Controller();
	~Controller();

	virtual void SetObject(Object * _object) { object = _object; }
	virtual void Remove();

	virtual Type GetType() { return Type::CONTROLLER; }
	virtual bool IsTypeOf(Type type) { return (type == Type::CONTROLLER) || UpdateList::IsTypeOf(type); }

protected:
	Object* object;
};
