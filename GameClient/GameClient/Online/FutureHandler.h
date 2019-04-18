#pragma once

#include <future>

template <class FutureResultType>
class FutureResultHandler
{
public:
	using Base = Controller;
	using FutureType = std::future <FutureResultType>;
	using HandlerType = std::function <void(FutureResultType&&)>;

	void update();
	void handle(FutureType&& future, HandlerType handler);

private:
	FutureType m_future;
	HandlerType m_handler;
};

template <class FutureResultType>
void FutureResultHandler<FutureResultType>::update()
{

	bool futureReady = m_future.valid();
	futureReady = futureReady && m_future.wait_for(std::chrono::microseconds(0)) == std::future_status::ready;

	if (futureReady && m_handler)
	{
		m_handler(m_future.get());
	}
}

template <class FutureResultType>
void FutureResultHandler<FutureResultType>::handle(FutureType&& future, HandlerType handler)
{
	m_future = std::move(future);
	m_handler = std::move(handler);
}
