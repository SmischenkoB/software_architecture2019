#pragma once

// -------------------------------------------------------------------------------------
// Initialization and cleanup functions for the Network library;
// NetworkAPI.Init should be called before any other library call
// -------------------------------------------------------------------------------------

class NetworkAPI
{
public:
	static int Init()
	{
		WSADATA wsaData;
		return  WSAStartup(MAKEWORD(2, 2), &wsaData);
	}

	static int Close()
	{
		return WSACleanup();
	}
};
