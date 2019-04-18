// ©2018 Ubisoft Bucharest
// This file is property of Ubisoft Bucharest, you are not allowed to distribute it.

#pragma once

// -------------------------------------------------------------------------------------
// Class that defines a BaseStream of data interface.
// Has 2 basic implementations:
//	- a ByteStream that streams everything in a binary format (compact format)
//	- a JSONStream that streams everything in a JSON format (readable / editable format)
// It knows how to serialize most basic types, as well as any serializable object.
//
// Note: Use "stream << object;" and "stream >> object;" to serialize and deserialize
// Note: For compatibility with both binary and JSON, you can use "stream << VALUE_AND_NAME(object);", that
//	would provide both the name and the value of the object to the stream serialization
// Note: When deserializing "stream >> VALUE_AND_NAME(object);", the stream will be cross-checking names to
//	assure that you indeed deserialize what you serialized
// -------------------------------------------------------------------------------------

#define DEFAULT_BYTE_STREAM_SIZE 4096
#define VALUE_AND_NAME(T) Variable(T, #T)

template <class T> class Variable
{
public:
	Variable(T & _value, const char * _name)
	{
		value = &_value;
		name = _name;
	}

	const char* name;
	T * value;
};

class BaseStream
{
public:
	BaseStream() {}
	BaseStream(int defaultCapacity) {}

	virtual ~BaseStream() { }

	virtual void Duplicate(BaseStream & stream) = 0;
	virtual void Duplicate(char * data, int length) = 0;
	virtual void Copy(BaseStream & stream) = 0;
	virtual void Copy(char * data, int length) = 0;
	virtual void Swap(BaseStream & stream) = 0;

	virtual void ResizeBuffer(int size) = 0;
	virtual char* GetBuffer(bool packed = false) = 0;
	virtual int GetSize() = 0;
	virtual int GetCapacity() = 0;
	virtual void Reset() = 0;
	virtual bool IsAnythingToDeserialize() = 0;

	virtual int Serialize(Serializable * value, const char* name = "", bool writeTypeName = false) = 0;
	virtual int Serialize(char value, const char* name = "", bool writeTypeName = false) = 0;
	virtual int Serialize(unsigned char value, const char* name = "", bool writeTypeName = false) = 0;
	virtual int Serialize(short value, const char* name = "", bool writeTypeName = false) = 0;
	virtual int Serialize(unsigned short value, const char* name = "", bool writeTypeName = false) = 0;
	virtual int Serialize(int value, const char* name = "", bool writeTypeName = false) = 0;
	virtual int Serialize(unsigned int value, const char* name = "", bool writeTypeName = false) = 0;
	virtual int Serialize(long value, const char* name = "", bool writeTypeName = false) = 0;
	virtual int Serialize(unsigned long value, const char* name = "", bool writeTypeName = false) = 0;
	virtual int Serialize(long long value, const char* name = "", bool writeTypeName = false) = 0;
	virtual int Serialize(unsigned long long value, const char* name = "", bool writeTypeName = false) = 0;
	virtual int Serialize(float value, const char* name = "", bool writeTypeName = false) = 0;
	virtual int Serialize(double value, const char* name = "", bool writeTypeName = false) = 0;
	virtual int Serialize(bool value, const char* name = "", bool writeTypeName = false) = 0;
	virtual int Serialize(const char* value, const char* name = "", bool writeTypeName = false) = 0;
	virtual int Serialize(const char* value, int len, const char* name = "", bool writeTypeName = false) = 0;
	virtual int Serialize(const std::string value, const char* name = "", bool writeTypeName = false) = 0;
	virtual int Serialize(const Vector2D value, const char* name = "", bool writeTypeName = false) = 0;

	template<typename T> int operator<<(const Variable<T> & value) { return Serialize(*value.value, value.name); }

	int operator<<(Serializable * value) { return Serialize(value); }
	int operator<<(char value) { return Serialize(value); }
	int operator<<(unsigned char value) { return Serialize(value); }
	int operator<<(short value) { return Serialize(value); }
	int operator<<(unsigned short value) { return Serialize(value); }
	int operator<<(int value) { return Serialize(value); }
	int operator<<(unsigned int value) { return Serialize(value); }
	int operator<<(long value) { return Serialize(value); }
	int operator<<(unsigned long value) { return Serialize(value); }
	int operator<<(long long value) { return Serialize(value); }
	int operator<<(unsigned long long value) { return Serialize(value); }
	int operator<<(float value) { return Serialize(value); }
	int operator<<(double value) { return Serialize(value); }
	int operator<<(bool value) { return Serialize(value); }
	int operator<<(const char* value) { return Serialize(value); }
	int operator<<(const std::string value) { return Serialize(value); }
	int operator<<(const Vector2D value) { return Serialize(value); }
	template<typename T> int operator<<(std::vector<T> & value) { return Serialize(value); }
	template<typename T> int operator<<(std::list<T> & value) { return Serialize(value); }

//	virtual int Deserialize(Serializable * & value, const char* name = "") = 0;
	virtual int Deserialize(char & value, const char* name = "") = 0;
	virtual int Deserialize(unsigned char & value, const char* name = "") = 0;
	virtual int Deserialize(short & value, const char* name = "") = 0;
	virtual int Deserialize(unsigned short & value, const char* name = "") = 0;
	virtual int Deserialize(long & value, const char* name = "") = 0;
	virtual int Deserialize(unsigned long & value, const char* name = "") = 0;
	virtual int Deserialize(int & value, const char* name = "") = 0;
	virtual int Deserialize(unsigned int & value, const char* name = "") = 0;
	virtual int Deserialize(long long & value, const char* name = "") = 0;
	virtual int Deserialize(unsigned long long & value, const char* name = "") = 0;
	virtual int Deserialize(float & value, const char* name = "") = 0;
	virtual int Deserialize(double & value, const char* name = "") = 0;
	virtual int Deserialize(bool & value, const char* name = "") = 0;
	virtual int Deserialize(char* & value, const char* name = "") = 0;
	virtual int Deserialize(char* value, int len, const char* name = "") = 0;
	virtual int Deserialize(std::string & value, const char* name = "") = 0;
	virtual int Deserialize(Vector2D & value, const char* name = "") = 0;

	template<typename T> int operator>>(const Variable<T> & value) { return Deserialize(*value.value, value.name); }

	template<typename T> int operator>>(T & value) { return Deserialize(value); }
	int operator>>(char & value) { return Deserialize(value); }
	int operator>>(unsigned char & value) { return Deserialize(value); }
	int operator>>(short & value) { return Deserialize(value); }
	int operator>>(unsigned short & value) { return Deserialize(value); }
	int operator>>(int & value) { return Deserialize(value); }
	int operator>>(unsigned int & value) { return Deserialize(value); }
	int operator>>(long & value) { return Deserialize(value); }
	int operator>>(unsigned long & value) { return Deserialize(value); }
	int operator>>(long long & value) { return Deserialize(value); }
	int operator>>(unsigned long long & value) { return Deserialize(value); }
	int operator>>(float & value) { return Deserialize(value); }
	int operator>>(double & value) { return Deserialize(value); }
	int operator>>(bool & value) { return Deserialize(value); }
	int operator>>(char* & value) { return Deserialize(value); }
	int operator>>(std::string & value) { return Deserialize(value); }
	int operator>>(Vector2D & value) { return Deserialize(value); }
	template<typename T> int operator>>(std::vector<T> & value) { return Deserialize(value); }
	template<typename T> int operator>>(std::list<T> & value) { return Deserialize(value); }

protected:
	virtual int SerializeClassID(Serializable * value) { return 0; }
	virtual int SerializeClassID(char & value) { return 0; }
	virtual int SerializeClassID(unsigned char & value) { return 0; }
	virtual int SerializeClassID(short & value) { return 0; }
	virtual int SerializeClassID(unsigned short & value) { return 0; }
	virtual int SerializeClassID(int & value) { return 0; }
	virtual int SerializeClassID(unsigned int & value) { return 0; }
	virtual int SerializeClassID(long long & value) { return 0; }
	virtual int SerializeClassID(unsigned long long & value) { return 0; }
	virtual int SerializeClassID(float & value) { return 0; }
	virtual int SerializeClassID(double & value) { return 0; }
	virtual int SerializeClassID(bool & value) { return 0; }
	virtual int SerializeClassID(char* & value) { return 0; }
	virtual int SerializeClassID(Vector2D & value) { return 0; }

	virtual Serializable * CreateObject(Serializable * value) { return nullptr; };
	virtual char CreateObject(char & value) { return 0; }
	virtual unsigned char CreateObject(unsigned char & value) { return 0; }
	virtual short CreateObject(short & value) { return 0; }
	virtual unsigned short CreateObject(unsigned short & value) { return 0; }
	virtual int CreateObject(int & value) { return 0; }
	virtual unsigned int CreateObject(unsigned int & value) { return 0; }
	virtual long long CreateObject(long long & value) { return 0; }
	virtual unsigned long long CreateObject(unsigned long long & value) { return 0; }
	virtual float CreateObject(float & value) { return 0; }
	virtual double CreateObject(double & value) { return 0; }
	virtual bool CreateObject(bool & value) { return false; }
	virtual char * CreateObject(char* & value) { return nullptr; }
	virtual Vector2D CreateObject(Vector2D & value) { return 0; }
};

// -------------------------------------------------------------------------------------
// ByteStream implementation derrived from the BaseStream class
// 
// Note: ByteStream is not adding any new parameters, only behavior, so you can safly
//	cast a BaseStream to either a JSONStream or a ByteStream, depending on how you want
//	your data to be serialized / deserialized
// -------------------------------------------------------------------------------------

class ByteStream : public BaseStream
{
public:
	ByteStream() : ByteStream(1024) {}
	ByteStream(int defaultCapacity)
	{
		charBuffer = new char[defaultCapacity];
		capacity = defaultCapacity;
		writePos = 0;
		readPos = 0;
	}

	~ByteStream() { if (charBuffer) delete [] charBuffer; }

	virtual void Copy(BaseStream & stream)
	{
		ByteStream & other = static_cast<ByteStream &>(stream);
		Copy((char*)other.charBuffer, GetSize());
		writePos = other.writePos;
		readPos = other.readPos;
	}
	virtual void Copy(char * data, int length)
	{
		if (capacity < length)
		{
			if (charBuffer) delete[] charBuffer;
			charBuffer = new char[length];
			capacity = length;
		}
		memcpy_s(charBuffer, length, (char*)data, length);
		writePos = length;
		readPos = 0;
	}
	virtual void Duplicate(BaseStream & stream)
	{
		ByteStream & other = static_cast<ByteStream &>(stream);
		Duplicate((char*)other.charBuffer, GetSize());
		writePos = other.writePos;
		readPos = other.readPos;
	}
	virtual void Duplicate(char * data, int length)
	{
		charBuffer = data;
		capacity = length;
		writePos = length;
		readPos = 0;
	}
	virtual void Swap(BaseStream & stream)
	{
		ByteStream & other = static_cast<ByteStream &>(stream);
		char* vTemp = charBuffer; charBuffer = other.charBuffer; other.charBuffer = vTemp;
		long lTemp = capacity; capacity = other.capacity; other.capacity = lTemp;
		lTemp = writePos; writePos = other.writePos; other.writePos = lTemp;
		lTemp = readPos; readPos = other.readPos; other.readPos = lTemp;
	}

	virtual void ResizeBuffer(int newCapacity)
	{
		if (newCapacity < writePos) return;
		char* newBuffer = new char[newCapacity];
		memcpy_s(newBuffer, newCapacity, charBuffer, writePos);
		if (charBuffer) delete [] charBuffer;
		charBuffer = newBuffer;
		capacity = newCapacity;
	}

	virtual char* GetBuffer(bool packed = false)
	{
		return (char*)charBuffer;
	}
	int GetSize() override { return writePos; }
	int GetCapacity() override { return capacity; }
	void Reset() override { writePos = 0; readPos = 0; }
	bool IsAnythingToDeserialize() { return readPos >= writePos; }

	virtual int Serialize(Serializable * value, const char* name = "", bool writeTypeName = false)
	{
		int initialPos = writePos;
		if (writeTypeName) SerializeClassID(value);
		value->Serialize(*(Stream*)this);
		return writePos - initialPos;
	}
	virtual int Serialize(char value, const char* name = "", bool writeTypeName = false)
	{
		int ret = sizeof(value);
		if (capacity - writePos < ret)
			ResizeBuffer(GetCapacity() + max(1024, ret));
		*(char*)(&static_cast<char*>(charBuffer)[writePos]) = value;
		writePos += ret;
		return ret;
	}
	virtual int Serialize(unsigned char value, const char* name = "", bool writeTypeName = false)
	{
		int ret = sizeof(value);
		if (capacity - writePos < ret)
			ResizeBuffer(GetCapacity() + max(1024, ret));
		*(unsigned char*)(&static_cast<char*>(charBuffer)[writePos]) = value;
		writePos += ret;
		return ret;
	}
	virtual int Serialize(short value, const char* name = "", bool writeTypeName = false)
	{
		int ret = sizeof(value);
		if (capacity - writePos < ret)
			ResizeBuffer(GetCapacity() + max(1024, ret));
		*(short*)(&static_cast<char*>(charBuffer)[writePos]) = value;
		writePos += ret;
		return ret;
	}
	virtual int Serialize(unsigned short value, const char* name = "", bool writeTypeName = false)
	{
		int ret = sizeof(value);
		if (capacity - writePos < ret)
			ResizeBuffer(GetCapacity() + max(1024, ret));
		*(unsigned short*)(&static_cast<char*>(charBuffer)[writePos]) = value;
		writePos += ret;
		return ret;
	}
	virtual int Serialize(int value, const char* name = "", bool writeTypeName = false)
	{
		int ret = sizeof(value);
		if (capacity - writePos < ret)
			ResizeBuffer(GetCapacity() + max(1024, ret));
		*(int*)(&static_cast<char*>(charBuffer)[writePos]) = value;
		writePos += ret;
		return ret;
	}
	virtual int Serialize(unsigned int value, const char* name = "", bool writeTypeName = false)
	{
		int ret = sizeof(value);
		if (capacity - writePos < ret)
			ResizeBuffer(GetCapacity() + max(1024, ret));
		*(unsigned int*)(&static_cast<char*>(charBuffer)[writePos]) = value;
		writePos += ret;
		return ret;
	}
	virtual int Serialize(long value, const char* name = "", bool writeTypeName = false)
	{
		int ret = sizeof(value);
		if (capacity - writePos < ret)
			ResizeBuffer(GetCapacity() + max(1024, ret));
		*(long*)(&static_cast<char*>(charBuffer)[writePos]) = value;
		writePos += ret;
		return ret;
	}
	virtual int Serialize(unsigned long value, const char* name = "", bool writeTypeName = false)
	{
		int ret = sizeof(value);
		if (capacity - writePos < ret)
			ResizeBuffer(GetCapacity() + max(1024, ret));
		*(unsigned long*)(&static_cast<char*>(charBuffer)[writePos]) = value;
		writePos += ret;
		return ret;

	}
	virtual int Serialize(long long value, const char* name = "", bool writeTypeName = false)
	{
		int ret = sizeof(value);
		if (capacity - writePos < ret)
			ResizeBuffer(GetCapacity() + max(1024, ret));
		*(long long*)(&static_cast<char*>(charBuffer)[writePos]) = value;
		writePos += ret;
		return ret;
	}
	virtual int Serialize(unsigned long long value, const char* name = "", bool writeTypeName = false)
	{
		int ret = sizeof(value);
		if (capacity - writePos < ret)
			ResizeBuffer(GetCapacity() + max(1024, ret));
		*(unsigned long long*)(&static_cast<char*>(charBuffer)[writePos]) = value;
		writePos += ret;
		return ret;
	}
	virtual int Serialize(float value, const char* name = "", bool writeTypeName = false)
	{
		int ret = sizeof(value);
		if (capacity - writePos < ret)
			ResizeBuffer(GetCapacity() + max(1024, ret));
		*(float*)(&static_cast<char*>(charBuffer)[writePos]) = value;
		writePos += ret;
		return ret;
	}
	virtual int Serialize(double value, const char* name = "", bool writeTypeName = false)
	{
		int ret = sizeof(value);
		if (capacity - writePos < ret)
			ResizeBuffer(GetCapacity() + max(1024, ret));
		*(double*)(&static_cast<char*>(charBuffer)[writePos]) = value;
		writePos += ret;
		return ret;
	}
	virtual int Serialize(bool value, const char* name = "", bool writeTypeName = false)
	{
		int ret = sizeof(value);
		if (capacity - writePos < ret)
			ResizeBuffer(GetCapacity() + max(1024, ret));
		*(bool*)(&static_cast<char*>(charBuffer)[writePos]) = value;
		writePos += ret;
		return ret;
	}
	virtual int Serialize(const char* value, const char* name = "", bool writeTypeName = false)
	{
		size_t len = strlen(value);
		int ret = Serialize(len);
		ret += Serialize(value, static_cast<int>(len), name);
		return ret;
	}
	virtual int Serialize(const char* value, int len, const char* name = "", bool writeTypeName = false)
	{
		int ret = len;
		if (capacity - writePos < ret)
			ResizeBuffer(GetCapacity() + max(1024, ret));
		memcpy_s(&static_cast<char*>(charBuffer)[writePos], capacity - writePos, value, len);
		writePos += ret;
		return ret;
	}
	virtual int Serialize(const std::string value, const char* name = "", bool writeTypeName = false)
	{
		const size_t len = value.length();
		int ret = Serialize(len);
		if (capacity - writePos < len)
			ResizeBuffer(GetCapacity() + max(1024, static_cast<int>(len)));
		memcpy_s(&static_cast<char*>(charBuffer)[writePos], capacity - writePos, value.c_str(), len);
		writePos += static_cast<long>(len);
		return ret;
	}

	int Serialize(const Vector2D value, const char* name = "", bool writeTypeName = false) override
	{
		int ret = Serialize(value.x);
		ret += Serialize(value.y);
		return ret;
	}
	template<typename T> int Serialize(std::vector<T> & value, const char* name = "")
	{
		int len = (int)value.size();
		int ret = Serialize(len);
		for (T obj : value)
			ret += Serialize(obj, "", true);
		return ret;
	}
	template<typename T> int Serialize(std::list<T> & value, const char* name = "")
	{
		int len = (int)value.size();
		int ret = Serialize(len);
		for (T obj : value)
			ret += Serialize(obj, "", true);
		return ret;
	}

	template<typename T> int Deserialize(T & value, const char* name = "")
	{
		int initialPos = readPos;
		if (value == nullptr)
			value = CreateObject(value);
		value->Deserialize(*this);
		return readPos - initialPos;
	}
	virtual int Deserialize(char & value, const char* name = "")
	{
		if (capacity - readPos < sizeof(value)) return 0;
		value  = *(char*)(&charBuffer[readPos]);
		readPos += sizeof(value);
		return sizeof(value);
	}
	virtual int Deserialize(unsigned char & value, const char* name = "")
	{
		if (capacity - readPos < sizeof(value)) return 0;
		value = *(unsigned char*)(&charBuffer[readPos]);
		readPos += sizeof(value);
		return sizeof(value);
	}
	virtual int Deserialize(short & value, const char* name = "")
	{
		if (capacity - readPos < sizeof(value)) return 0;
		value = *(short*)(&charBuffer[readPos]);
		readPos += sizeof(value);
		return sizeof(value);
	}
	virtual int Deserialize(unsigned short & value, const char* name = "")
	{
		if (capacity - readPos < sizeof(value)) return 0;
		value = *(unsigned short*)(&charBuffer[readPos]);
		readPos += sizeof(value);
		return sizeof(value);
	}
	virtual int Deserialize(int & value, const char* name = "")
	{
		if (capacity - readPos < sizeof(value)) return 0;
		value = *(int*)(&charBuffer[readPos]);
		readPos += sizeof(value);
		return sizeof(value);
	}
	virtual int Deserialize(unsigned int & value, const char* name = "")
	{
		if (capacity - readPos < sizeof(value)) return 0;
		value = *(unsigned int*)(&charBuffer[readPos]);
		readPos += sizeof(value);
		return sizeof(value);
	}
	virtual int Deserialize(long & value, const char* name = "")
	{
		if (capacity - readPos < sizeof(value)) return 0;
		value = *(long*)(&charBuffer[readPos]);
		readPos += sizeof(value);
		return sizeof(value);
	}
	virtual int Deserialize(unsigned long & value, const char* name = "")
	{
		if (capacity - readPos < sizeof(value)) return 0;
		value = *(unsigned long*)(&charBuffer[readPos]);
		readPos += sizeof(value);
		return sizeof(value);
	}
	virtual int Deserialize(long long & value, const char* name = "")
	{
		if (capacity - readPos < sizeof(value)) return 0;
		value = *(long long*)(&charBuffer[readPos]);
		readPos += sizeof(value);
		return sizeof(value);
	}
	virtual int Deserialize(unsigned long long & value, const char* name = "")
	{
		if (capacity - readPos < sizeof(value)) return 0;
		value = *(unsigned long long*)(&charBuffer[readPos]);
		readPos += sizeof(value);
		return sizeof(value);
	}
	virtual int Deserialize(float & value, const char* name = "")
	{
		if (capacity - readPos < sizeof(value)) return 0;
		value = *(float*)(&charBuffer[readPos]);
		readPos += sizeof(value);
		return sizeof(value);
	}
	virtual int Deserialize(double & value, const char* name = "")
	{
		if (capacity - readPos < sizeof(value)) return 0;
		value = *(double*)(&charBuffer[readPos]);
		readPos += sizeof(value);
		return sizeof(value);
	}
	virtual int Deserialize(bool & value, const char* name = "")
	{
		if (capacity - readPos < sizeof(value)) return 0;
		value = *(bool*)(&charBuffer[readPos]);
		readPos += sizeof(value);
		return sizeof(value);
	}
	virtual int Deserialize(char* & value, const char* name = "")
	{
		int len, ret;
		ret = Deserialize(len);
		if (ret == 0) return ret;
		if (value == nullptr)
			value = new char[len + 1];
		value[len] = 0;
		ret += Deserialize(value, len, name);
		return ret;
	}
	virtual int Deserialize(char* value, int len, const char* name = "")
	{
		memcpy_s(value, len, &charBuffer[readPos], len);
		readPos += len;
		return len;
	}
	virtual int Deserialize(std::string & value, const char* name = "")
	{
		int len, ret;
		ret = Deserialize(len);
		if (ret == 0) return ret;
		value.assign(&charBuffer[readPos], len);
		readPos += len;
		return ret;
	}
	virtual int Deserialize(Vector2D & value, const char* name = "")
	{
		int ret = Deserialize(value.x);
		ret += Deserialize(value.y);
		return ret;
	}
	template<typename T> int Deserialize(std::vector<T> & value, const char* name = "")
	{
		int len;
		int ret = Deserialize(len);
		for (int i = 0; i < len; i++)
		{
			T obj = 0;
			ret += Deserialize(obj);
			value.push_back(obj);
		}
		return ret;
	}
	template<typename T> int Deserialize(std::list<T> & value, const char* name = "")
	{
		int len;
		int ret = Deserialize(len);
		for (int i = 0; i < len; i++)
		{
			T obj = 0;
			ret += Deserialize(obj);
			value.push_back(obj);
		}
		return ret;
	}

protected:
	virtual int SerializeClassID(Serializable * value)
	{
		return Serialize(value->GetClassAsID(), "ClassName");
	}
	template<typename T> T CreateObject(T value)
	{
		unsigned int classID;
		Deserialize(classID, "ClassName");
		return (T)Serializable::FactoryCreateInstance(classID);
	}

	char *		charBuffer;
	long		writePos;
	long		readPos;
	long		capacity;
};

// -------------------------------------------------------------------------------------
// JSONStream implementation derrived from the BaseStream class
// 
// Note: ByteStream is not adding any new parameters, only behavior, so you can safly
//	cast a BaseStream to either a JSONStream or a ByteStream, depending on how you want
//	your data to be serialized / deserialized
// -------------------------------------------------------------------------------------

class JSONStream : public BaseStream
{
public:
	JSONStream()
	{
		charBuffer = nullptr;
		jsonObject = json_object_new(1);
		useBuilderFree = true;
		readIterator.push(0);
	}
	JSONStream(int defaultCapacity) : JSONStream() { }

	~JSONStream()
	{
		Reset();
	}

	virtual void Copy(BaseStream & stream)
	{
		JSONStream & other = static_cast<JSONStream &>(stream);
		Reset();
		jsonObject = json_parse(other.GetBuffer(), other.GetSize());
		useBuilderFree = false;
		readIterator.push(0);
	}
	virtual void Copy(char * data, int length)
	{
		Reset();
		jsonObject = json_parse(data, length);
		useBuilderFree = false;
		readIterator.push(0);
	}
	virtual void Duplicate(BaseStream & stream)
	{
		Copy(stream);
	}
	virtual void Duplicate(char * data, int length)
	{
		Copy(data, length);
	}
	virtual void Swap(BaseStream & stream)
	{
		JSONStream & other = static_cast<JSONStream &>(stream);
		json_value * pTemp = jsonObject; jsonObject = other.jsonObject; other.jsonObject = pTemp;
		char * sTemp = charBuffer; charBuffer = other.charBuffer; other.charBuffer = sTemp;
		readIterator.swap(other.readIterator);
		bool bTemp = useBuilderFree; useBuilderFree = other.useBuilderFree; other.useBuilderFree = bTemp;
	}

	virtual void ResizeBuffer(int newCapacity) {}

	virtual char* GetBuffer(bool packed = false)
	{
		if (charBuffer == nullptr)
		{
			json_serialize_opts opts = { packed ? json_serialize_mode_single_line : json_serialize_mode_multiline, packed ? json_serialize_opt_pack_brackets | json_serialize_opt_no_space_after_comma | json_serialize_opt_no_space_after_colon : json_serialize_opt_use_tabs | json_serialize_opt_CRLF, packed ? 0 : 1 };
			charBuffer = new char[json_measure_ex(jsonObject, opts)];
			json_serialize_ex(charBuffer, jsonObject, opts);
		}
		return charBuffer;
	}
	virtual int GetSize()
	{
		if (charBuffer == nullptr)
			GetBuffer();
		return static_cast<int>(strlen(charBuffer) + 1);
	}
	virtual int GetCapacity() { return LONG_MAX; }
	void Reset() override
	{
		if (jsonObject)
		{
			if (useBuilderFree)
				json_builder_free(jsonObject);
			else
				json_value_free(jsonObject);
			jsonObject = nullptr;
		}
		if (charBuffer)
		{
			delete [] charBuffer;
			charBuffer = nullptr;
		}
		while (!readIterator.empty())
			readIterator.pop();
	}
	bool IsAnythingToDeserialize()
	{
		if (readIterator.size() > 1)
			return false;
		int idx = readIterator.top();
		return ((jsonObject->type == json_type::json_object && idx >= (int)jsonObject->u.object.length) || (jsonObject->type == json_type::json_array && idx >= (int)jsonObject->u.array.length));
	}

	virtual int Serialize(Serializable * value, const char* name = "", bool writeTypeName = false)
	{
		json_value * item = json_object_new(1);
		if (jsonObject->type == json_type::json_array)
			json_array_push(jsonObject, item);
		else
			json_object_push(jsonObject, name, item);
		json_value * parent = jsonObject;
		jsonObject = item;
		if (writeTypeName) SerializeClassID(value);
		int ret = value->Serialize(*(Stream*)this);
		jsonObject = parent;
		if (charBuffer) { delete [] charBuffer; charBuffer = nullptr; }
		return ret;
	}
	virtual int Serialize(char value, const char* name = "", bool writeTypeName = false)
	{
		return Serialize((long long)value, name, writeTypeName);
	}
	virtual int Serialize(unsigned char value, const char* name = "", bool writeTypeName = false)
	{
		return Serialize((unsigned long long)value, name, writeTypeName);
	}
	virtual int Serialize(short value, const char* name = "", bool writeTypeName = false)
	{
		return Serialize((long long)value, name, writeTypeName);
	}
	virtual int Serialize(unsigned short value, const char* name = "", bool writeTypeName = false)
	{
		return Serialize((unsigned long long)value, name, writeTypeName);
	}
	virtual int Serialize(int value, const char* name = "", bool writeTypeName = false)
	{
		return Serialize((long long)value, name, writeTypeName);
	}
	virtual int Serialize(unsigned int value, const char* name = "", bool writeTypeName = false)
	{
		return Serialize((unsigned long long)value, name, writeTypeName);
	}
	virtual int Serialize(long value, const char* name = "", bool writeTypeName = false)
	{
		return Serialize((long long)value, name, writeTypeName);
	}
	virtual int Serialize(unsigned long value, const char* name = "", bool writeTypeName = false)
	{
		return Serialize((unsigned long long)value, name, writeTypeName);
	}
	virtual int Serialize(long long value, const char* name = "", bool writeTypeName = false)
	{
		if (jsonObject->type == json_type::json_array)
		{
			if (*name == 0)
				json_array_push(jsonObject, json_integer_new(value));
			else
			{
				json_value * item = json_object_new(1);
				json_object_push(item, name, json_integer_new(value));
				json_array_push(jsonObject, item);
			}
		}
		else
			json_object_push(jsonObject, name, json_integer_new(value));
		if (charBuffer) { delete [] charBuffer; charBuffer = nullptr; }
		return 1;
	}
	virtual int Serialize(unsigned long long value, const char* name = "", bool writeTypeName = false)
	{
		if (jsonObject->type == json_type::json_array)
		{
			if (*name == 0)
				json_array_push(jsonObject, json_unsigned_integer_new(value));
			else
			{
				json_value * item = json_object_new(1);
				json_object_push(item, name, json_unsigned_integer_new(value));
				json_array_push(jsonObject, item);
			}
		}
		else
			json_object_push(jsonObject, name, json_unsigned_integer_new(value));
		if (charBuffer) { delete [] charBuffer; charBuffer = nullptr; }
		return 1;
	}
	virtual int Serialize(float value, const char* name = "", bool writeTypeName = false)
	{
		return Serialize((double)value, name, writeTypeName);
	}
	virtual int Serialize(double value, const char* name = "", bool writeTypeName = false)
	{
		if (jsonObject->type == json_type::json_array)
		{
			if (*name == 0)
				json_array_push(jsonObject, json_double_new(value));
			else
			{
				json_value * item = json_object_new(1);
				json_object_push(item, name, json_double_new(value));
				json_array_push(jsonObject, item);
			}
		}
		else
			json_object_push(jsonObject, name, json_double_new(value));
		if (charBuffer) { delete [] charBuffer; charBuffer = nullptr; }
		return 1;
	}
	virtual int Serialize(bool value, const char* name = "", bool writeTypeName = false)
	{
		if (jsonObject->type == json_type::json_array)
		{
			if (*name == 0)
				json_array_push(jsonObject, json_boolean_new(value));
			else
			{
				json_value * item = json_object_new(1);
				json_object_push(item, name, json_boolean_new(value));
				json_array_push(jsonObject, item);
			}
		}
		else
			json_object_push(jsonObject, name, json_boolean_new(value));
		if (charBuffer) { delete [] charBuffer; charBuffer = nullptr; }
		return 1;
	}
	virtual int Serialize(const char* value, const char* name = "", bool writeTypeName = false)
	{
		Serialize(value, static_cast<int>(strlen(value)), name);
		return 1;
	}
	virtual int Serialize(const char* value, int len, const char* name = "", bool writeTypeName = false)
	{
		if (jsonObject->type == json_type::json_array)
		{
			if (*name == 0)
				json_array_push(jsonObject, json_string_new_length(len, value));
			else
			{
				json_value * item = json_object_new(1);
				json_object_push(item, name, json_string_new_length(len, value));
				json_array_push(jsonObject, item);
			}
		}
		else
			json_object_push(jsonObject, name, json_string_new_length(len, value));
		if (charBuffer) { delete [] charBuffer; charBuffer = nullptr; }
		return 1;
	}
	virtual int Serialize(const std::string value, const char* name = "", bool writeTypeName = false)
	{
		if (jsonObject->type == json_type::json_array)
		{
			if (*name == 0)
				json_array_push(jsonObject, json_string_new_length(static_cast<unsigned>(value.length()), value.c_str()));
			else
			{
				json_value * item = json_object_new(1);
				json_object_push(item, name, json_string_new_length(static_cast<unsigned>(value.length()), value.c_str()));
				json_array_push(jsonObject, item);
			}
		}
		else
			json_object_push(jsonObject, name, json_string_new_length(static_cast<unsigned>(value.length()), value.c_str()));
		if (charBuffer) { delete [] charBuffer; charBuffer = nullptr; }
		return 1;
	}
	virtual int Serialize(const Vector2D value, const char* name = "", bool writeTypeName = false)
	{
		json_value * vec = json_array_new(2);

		if (jsonObject->type == json_type::json_array)
		{
			if (*name == 0)
				json_array_push(jsonObject, vec);
			else
			{
				json_value * item = json_object_new(1);
				json_object_push(item, name, vec);
				json_array_push(jsonObject, item);
			}
		}
		else
			json_object_push(jsonObject, name, vec);

		json_value * parent = jsonObject;
		jsonObject = vec;
		int ret = Serialize(value.x, "", true);
		ret += Serialize(value.y, "", true);
		jsonObject = parent;
		if (charBuffer) { delete[] charBuffer; charBuffer = nullptr; }
		return ret;
	}
	template<typename T> int Serialize(std::vector<T> & value, const char* name = "")
	{
		json_value * vec = json_array_new(value.size());

		if (jsonObject->type == json_type::json_array)
		{
			if (*name == 0)
				json_array_push(jsonObject, vec);
			else
			{
				json_value * item = json_object_new(1);
				json_object_push(item, name, vec);
				json_array_push(jsonObject, item);
			}
		}
		else
			json_object_push(jsonObject, name, vec);

		json_value * parent = jsonObject;
		jsonObject = vec;
		int ret = 0;
		for (T obj : value)
			ret += Serialize(obj, "", true);
		jsonObject = parent;
		if (charBuffer) { delete [] charBuffer; charBuffer = nullptr; }
		return ret;
	}
	template<typename T> int Serialize(std::list<T> & value, const char* name = "")
	{
		json_value * vec = json_array_new(value.size());

		if (jsonObject->type == json_type::json_array)
		{
			if (*name == 0)
				json_array_push(jsonObject, vec);
			else
			{
				json_value * item = json_object_new(1);
				json_object_push(item, name, vec);
				json_array_push(jsonObject, item);
			}
		}
		else
			json_object_push(jsonObject, name, vec);

		json_value * parent = jsonObject;
		jsonObject = vec;
		int ret = 0;
		for (T obj : value)
			ret += Serialize(obj, "", true);
		jsonObject = parent;
		if (charBuffer) { delete [] charBuffer; charBuffer = nullptr; }
		return ret;
	}

	template<typename T> int Deserialize(T & value, const char* name = "")
	{
		int ret = 0;
		int & idx = readIterator.top();
		if (jsonObject->type == json_type::json_object)
		{
			if (idx >= (int)jsonObject->u.object.length)
				return 0;
			assert((name[0] == 0 || strncmp(name, jsonObject->u.object.values[idx].name, jsonObject->u.object.values[idx].name_length) == 0) && "Deserialized object name mismatch");
			assert(jsonObject->u.object.values[idx].value->type == json_type::json_object && "Invalid json object type - expected [Object]");
		}
		else if (jsonObject->type == json_type::json_array)
		{
			if (idx >= (int)jsonObject->u.array.length)
				return 0;
			assert(jsonObject->u.array.values[idx]->type == json_type::json_object && "Invalid json object type - expected [Object]");
		}
		else
			assert(!"Invalid json object type");

		json_value * parent = jsonObject;
		readIterator.push(0);
		jsonObject = jsonObject->type == json_type::json_object ? jsonObject->u.object.values[idx].value : jsonObject->u.array.values[idx];
		if (value == nullptr)
			value = CreateObject(value);
		ret = value->Deserialize(*this);
		readIterator.pop();
		jsonObject = parent;

		idx++;
		return ret;
	}
	virtual int Deserialize(char & value, const char* name = "")
	{
		long long llValue;
		int ret = Deserialize(llValue, name);
		value = (char)llValue;
		return ret;
	}
	virtual int Deserialize(unsigned char & value, const char* name = "")
	{
		unsigned long long ullValue;
		int ret = Deserialize(ullValue, name);
		value = (unsigned char)ullValue;
		return ret;
	}
	virtual int Deserialize(short & value, const char* name = "")
	{
		long long llValue;
		int ret = Deserialize(llValue, name);
		value = (short)llValue;
		return ret;
	}
	virtual int Deserialize(unsigned short & value, const char* name = "")
	{
		unsigned long long ullValue;
		int ret = Deserialize(ullValue, name);
		value = (unsigned short)ullValue;
		return ret;
	}
	virtual int Deserialize(int & value, const char* name = "")
	{
		long long llValue;
		int ret = Deserialize(llValue, name);
		value = (int)llValue;
		return ret;
	}
	virtual int Deserialize(unsigned int & value, const char* name = "")
	{
		unsigned long long ullValue;
		int ret = Deserialize(ullValue, name);
		value = (unsigned int)ullValue;
		return ret;
	}
	virtual int Deserialize(long & value, const char* name = "")
	{
		long long llValue;
		int ret = Deserialize(llValue, name);
		value = (long)llValue;
		return ret;
	}
	virtual int Deserialize(unsigned long & value, const char* name = "")
	{
		unsigned long long ullValue;
		int ret = Deserialize(ullValue, name);
		value = (unsigned long)ullValue;
		return ret;
	}
	virtual int Deserialize(long long & value, const char* name = "")
	{
		int & idx = readIterator.top();
		if (jsonObject->type == json_type::json_object)
		{
			if (idx >= (int)jsonObject->u.object.length)
				return 0;
			assert((name[0] == 0 || strncmp(name, jsonObject->u.object.values[idx].name, jsonObject->u.object.values[idx].name_length) == 0) && "Deserialized object name mismatch");
			assert((jsonObject->u.object.values[idx].value->type == json_type::json_integer || jsonObject->u.object.values[idx].value->type == json_type::json_unsigned_integer || jsonObject->u.object.values[idx].value->type == json_type::json_double) && "Invalid json object type - expected [Integer]");
			value = (long long)(jsonObject->u.object.values[idx].value->type == json_type::json_double ? jsonObject->u.object.values[idx].value->u.dbl : jsonObject->u.object.values[idx].value->u.integer);
		}
		else if (jsonObject->type == json_type::json_array)
		{
			if (idx >= (int)jsonObject->u.array.length)
				return 0;
			assert((jsonObject->u.array.values[idx]->type == json_type::json_integer || jsonObject->u.array.values[idx]->type == json_type::json_unsigned_integer || jsonObject->u.array.values[idx]->type == json_type::json_double) && "Invalid json object type - expected [Integer]");
			value = (long long)(jsonObject->u.array.values[idx]->type == json_type::json_double ? jsonObject->u.array.values[idx]->u.dbl : jsonObject->u.array.values[idx]->u.integer);
		}
		else
			assert(!"Invalid json object type");
		idx++;
		return 1;
	}
	virtual int Deserialize(unsigned long long & value, const char* name = "")
	{
		int & idx = readIterator.top();
		if (jsonObject->type == json_type::json_object)
		{
			if (idx >= (int)jsonObject->u.object.length)
				return 0;
			assert((name[0] == 0 || strncmp(name, jsonObject->u.object.values[idx].name, jsonObject->u.object.values[idx].name_length) == 0) && "Deserialized object name mismatch");
			assert((jsonObject->u.object.values[idx].value->type == json_type::json_integer || jsonObject->u.object.values[idx].value->type == json_type::json_unsigned_integer || jsonObject->u.object.values[idx].value->type == json_type::json_double) && "Invalid json object type - expected [Integer]");
			value = (unsigned long long)(jsonObject->u.object.values[idx].value->type == json_type::json_double ? jsonObject->u.object.values[idx].value->u.dbl : jsonObject->u.object.values[idx].value->u.unsigned_integer);
		}
		else if (jsonObject->type == json_type::json_array)
		{
			if (idx >= (int)jsonObject->u.array.length)
				return 0;
			assert((jsonObject->u.array.values[idx]->type == json_type::json_integer || jsonObject->u.array.values[idx]->type == json_type::json_unsigned_integer || jsonObject->u.array.values[idx]->type == json_type::json_double) && "Invalid json object type - expected [Integer]");
			value = (unsigned long long)(jsonObject->u.array.values[idx]->type == json_type::json_double ? jsonObject->u.array.values[idx]->u.dbl : jsonObject->u.array.values[idx]->u.unsigned_integer);
		}
		else
			assert(!"Invalid json object type");
		idx++;
		return 1;
	}
	virtual int Deserialize(float & value, const char* name = "")
	{
		double dValue;
		int ret = Deserialize(dValue, name);
		value = (float)dValue;
		return ret;
	}
	virtual int Deserialize(double & value, const char* name = "")
	{
		int & idx = readIterator.top();
		if (jsonObject->type == json_type::json_object)
		{
			if (idx >= (int)jsonObject->u.object.length)
				return 0;
			assert((name[0] == 0 || strncmp(name, jsonObject->u.object.values[idx].name, jsonObject->u.object.values[idx].name_length) == 0) && "Deserialized object name mismatch");
			assert((jsonObject->u.object.values[idx].value->type == json_type::json_integer || jsonObject->u.object.values[idx].value->type == json_type::json_unsigned_integer || jsonObject->u.object.values[idx].value->type == json_type::json_double) && "Invalid json object type - expected [Double]");
			value = (double)(jsonObject->u.object.values[idx].value->type == json_type::json_double ? jsonObject->u.object.values[idx].value->u.dbl : jsonObject->u.object.values[idx].value->u.integer);
		}
		else if (jsonObject->type == json_type::json_array)
		{
			if (idx >= (int)jsonObject->u.array.length)
				return 0;
			assert((jsonObject->u.array.values[idx]->type == json_type::json_integer || jsonObject->u.array.values[idx]->type == json_type::json_unsigned_integer || jsonObject->u.array.values[idx]->type == json_type::json_double) && "Invalid json object type - expected [Double]");
			value = (double)(jsonObject->u.array.values[idx]->type == json_type::json_double ? jsonObject->u.array.values[idx]->u.dbl : jsonObject->u.array.values[idx]->u.integer);
		}
		else
			assert(!"Invalid json object type");
		idx++;
		return 1;
	}
	virtual int Deserialize(bool & value, const char* name = "")
	{
		int & idx = readIterator.top();
		if (jsonObject->type == json_type::json_object)
		{
			if (idx >= (int)jsonObject->u.object.length)
				return 0;
			assert((name[0] == 0 || strncmp(name, jsonObject->u.object.values[idx].name, jsonObject->u.object.values[idx].name_length) == 0) && "Deserialized object name mismatch");
			assert(jsonObject->u.object.values[idx].value->type == json_type::json_boolean && "Invalid json object type - expected [Boolean]");
			value = jsonObject->u.object.values[idx].value->u.boolean;
		}
		else if (jsonObject->type == json_type::json_array)
		{
			if (idx >= (int)jsonObject->u.array.length)
				return 0;
			assert(jsonObject->u.array.values[idx]->type == json_type::json_boolean && "Invalid json object type - expected [Boolean]");
			value = jsonObject->u.array.values[idx]->u.boolean;
		}
		else
			assert(!"Invalid json object type");
		idx++;
		return 1;
	}
	virtual int Deserialize(char* & value, const char* name = "")
	{
		int & idx = readIterator.top();
		if (jsonObject->type == json_type::json_object)
		{
			if (idx >= (int)jsonObject->u.object.length)
				return 0;
			assert((name[0] == 0 || strncmp(name, jsonObject->u.object.values[idx].name, jsonObject->u.object.values[idx].name_length) == 0) && "Deserialized object name mismatch");
			assert(jsonObject->u.object.values[idx].value->type == json_type::json_string && "Invalid json object type - expected [String]");
			int len = jsonObject->u.object.values[idx].value->u.string.length;
			if (value == nullptr)
				value = new char[len + 1];
			strncpy_s(value, len + 1, jsonObject->u.object.values[idx].value->u.string.ptr, len);
		}
		else if (jsonObject->type == json_type::json_array)
		{
			if (idx >= (int)jsonObject->u.array.length)
				return 0;
			assert(jsonObject->u.array.values[idx]->type == json_type::json_string && "Invalid json object type - expected [String]");
			int len = jsonObject->u.array.values[idx]->u.string.length;
			if (value == nullptr)
				value = new char[len + 1];
			strncpy_s(value, len + 1, jsonObject->u.array.values[idx]->u.string.ptr, len);
		}
		else
			assert(!"Invalid json object type");
		idx++;
		return 1;
	}
	virtual int Deserialize(char* value, int len, const char* name = "")
	{
		int & idx = readIterator.top();
		if (jsonObject->type == json_type::json_object)
		{
			if (idx >= (int)jsonObject->u.object.length)
				return 0;
			assert((name[0] == 0 || strncmp(name, jsonObject->u.object.values[idx].name, jsonObject->u.object.values[idx].name_length) == 0) && "Deserialized object name mismatch");
			assert(jsonObject->u.object.values[idx].value->type == json_type::json_string && "Invalid json object type - expected [String]");
			strncpy_s(value, len, jsonObject->u.object.values[idx].value->u.string.ptr, jsonObject->u.object.values[idx].value->u.string.length);
		}
		else if (jsonObject->type == json_type::json_array)
		{
			if (idx >= (int)jsonObject->u.array.length)
				return 0;
			assert(jsonObject->u.array.values[idx]->type == json_type::json_string && "Invalid json object type - expected [String]");
			strncpy_s(value, len, jsonObject->u.array.values[idx]->u.string.ptr, jsonObject->u.array.values[idx]->u.string.length);
		}
		else
			assert(!"Invalid json object type");
		idx++;
		return 1;
	}
	virtual int Deserialize(std::string & value, const char* name = "")
	{
		int & idx = readIterator.top();
		if (jsonObject->type == json_type::json_object)
		{
			if (idx >= (int)jsonObject->u.object.length)
				return 0;
			assert((name[0] == 0 || strncmp(name, jsonObject->u.object.values[idx].name, jsonObject->u.object.values[idx].name_length) == 0) && "Deserialized object name mismatch");
			assert(jsonObject->u.object.values[idx].value->type == json_type::json_string && "Invalid json object type - expected [String]");
			int len = jsonObject->u.object.values[idx].value->u.string.length;
			value.assign(jsonObject->u.object.values[idx].value->u.string.ptr, len);
		}
		else if (jsonObject->type == json_type::json_array)
		{
			if (idx >= (int)jsonObject->u.array.length)
				return 0;
			assert(jsonObject->u.array.values[idx]->type == json_type::json_string && "Invalid json object type - expected [String]");
			int len = jsonObject->u.array.values[idx]->u.string.length;
			value.assign(jsonObject->u.array.values[idx]->u.string.ptr, len);
		}
		else
			assert(!"Invalid json object type");
		idx++;
		return 1;
	}
	virtual int Deserialize(Vector2D & value, const char* name = "")
	{
		int & idx = readIterator.top();
		int len;
		json_value * vec = nullptr;
		if (jsonObject->type == json_type::json_object)
		{
			if (idx >= (int)jsonObject->u.object.length)
				return 0;
			assert((name[0] == 0 || strncmp(name, jsonObject->u.object.values[idx].name, jsonObject->u.object.values[idx].name_length) == 0) && "Deserialized object name mismatch");
			assert(jsonObject->u.object.values[idx].value->type == json_type::json_array && "Invalid json object type - expected [String]");
			vec = jsonObject->u.object.values[idx].value;
			len = (int)jsonObject->u.object.values[idx].value->u.array.length;
		}
		else if (jsonObject->type == json_type::json_array)
		{
			if (idx >= (int)jsonObject->u.array.length)
				return 0;
			assert(jsonObject->u.array.values[idx]->type == json_type::json_array && "Invalid json object type - expected [String]");
			vec = jsonObject->u.array.values[idx];
			len = (int)jsonObject->u.array.values[idx]->u.array.length;
		}
		else
		{
			assert(!"Invalid json object type");
			return 0;
		}

		json_value * parent = jsonObject;
		jsonObject = vec;
		readIterator.push(0);
		int ret = Deserialize(value.x);
		ret += Deserialize(value.y);
		readIterator.pop();
		jsonObject = parent;

		idx++;
		return ret;
	}
	template<typename T> int Deserialize(std::vector<T> & value, const char* name = "")
	{
		int & idx = readIterator.top();
		int ret = 0;
		int len;
		json_value * vec = nullptr;
		if (jsonObject->type == json_type::json_object)
		{
			if (idx >= (int)jsonObject->u.object.length)
				return 0;
			assert((name[0] == 0 || strncmp(name, jsonObject->u.object.values[idx].name, jsonObject->u.object.values[idx].name_length) == 0) && "Deserialized object name mismatch");
			assert(jsonObject->u.object.values[idx].value->type == json_type::json_array && "Invalid json object type - expected [String]");
			vec = jsonObject->u.object.values[idx].value;
			len = (int)jsonObject->u.object.values[idx].value->u.array.length;
		}
		else if (jsonObject->type == json_type::json_array)
		{
			if (idx >= (int)jsonObject->u.array.length)
				return 0;
			assert(jsonObject->u.array.values[idx]->type == json_type::json_array && "Invalid json object type - expected [String]");
			vec = jsonObject->u.array.values[idx];
			len = (int)jsonObject->u.array.values[idx]->u.array.length;
		}
		else
		{
			assert(!"Invalid json object type");
			return 0;
		}

		json_value * parent = jsonObject;
		jsonObject = vec;
		readIterator.push(0);
		for (int i = 0; i < len; i++)
		{
			T obj = 0;
			ret += Deserialize(obj);
			value.push_back(obj);
		}
		readIterator.pop();
		jsonObject = parent;

		idx++;
		return ret;
	}
	template<typename T> int Deserialize(std::list<T> & value, const char* name = "")
	{
		int & idx = readIterator.top();
		int ret = 0;
		int len;
		json_value * vec;
		if (jsonObject->type == json_type::json_object)
		{
			if (idx >= (int)jsonObject->u.object.length)
				return 0;
			assert((name[0] == 0 || strncmp(name, jsonObject->u.object.values[idx].name, jsonObject->u.object.values[idx].name_length) == 0) && "Deserialized object name mismatch");
			assert(jsonObject->u.object.values[idx].value->type == json_type::json_array && "Invalid json object type - expected [String]");
			vec = jsonObject->u.object.values[idx].value;
			len = (int)jsonObject->u.object.values[idx].value->u.array.length;
		}
		else if (jsonObject->type == json_type::json_array)
		{
			if (idx >= (int)jsonObject->u.array.length)
				return 0;
			assert(jsonObject->u.array.values[idx]->type == json_type::json_array && "Invalid json object type - expected [String]");
			vec = jsonObject->u.array.values[idx];
			len = (int)jsonObject->u.array.values[idx]->u.array.length;
		}
		else
		{
			assert(!"Invalid json object type");
			return 0;
		}

		json_value * parent = jsonObject;
		jsonObject = vec;
		readIterator.push(0);
		for (int i = 0; i < len; i++)
		{
			T obj = 0;
			ret += Deserialize(obj);
			value.push_back(obj);
		}
		readIterator.pop();
		jsonObject = parent;

		idx++;
		return ret;
	}

protected:
	virtual int SerializeClassID(Serializable * value)
	{
		return Serialize(value->GetClassAsString(), "ClassName");
	}
	template<typename T> T CreateObject(T value)
	{
		char className[256];
		Deserialize(className, 256, "ClassName");
		return (T)Serializable::FactoryCreateInstance(className);
	}

	json_value *	jsonObject;
	std::stack<int>	readIterator;
	char *			charBuffer;
	bool			useBuilderFree;
};
