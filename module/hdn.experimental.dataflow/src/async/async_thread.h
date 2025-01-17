#pragma once

#include <thread>

namespace hdn
{
	class IThread
	{
	public:
		IThread();
		void Start();
		bool Joinable();
		void Join();
	protected:
		virtual void Run() = 0;
	private:
		std::thread m_Thread;
	};
}