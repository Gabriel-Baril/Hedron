#include "async_thread.h"

namespace hdn
{
	IThread::IThread()
	{
	}

	void IThread::Start()
	{
		m_Thread = std::thread([this]() { this->Run(); });
	}

	bool IThread::Joinable()
	{
		return m_Thread.joinable();
	}

	void IThread::Join()
	{
		m_Thread.join();
	}
}
