// ©2018 Ubisoft Bucharest
// This file is property of Ubisoft Bucharest, you are not allowed to distribute it.

#pragma once

// -------------------------------------------------------------------------------------
// Base class from which any serializable object should inherit.
// -------------------------------------------------------------------------------------

class ByteStream;
class JSONStream;

#define Stream JSONStream

class Serializable
{
public:
	virtual int Serialize(Stream & stream) { return 0; }
	virtual int Deserialize(Stream & stream) { return 0; }

	virtual unsigned int GetClassAsID() {  return 0; }

	virtual const char * GetClassAsString() { return ""; }

	static Serializable * FactoryCreateInstance(const std::string name)
	{
		return nullptr;
	}

	static Serializable * FactoryCreateInstance(const std::size_t id)
	{
		return nullptr;
	}
};
