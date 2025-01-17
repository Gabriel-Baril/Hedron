#pragma once

#include "core/core.h"
#include "../hobj.h"

namespace hdn
{

	class IPipelineTask
	{
	public:
		virtual void Init() = 0;
		virtual void Execute() = 0;
	private:
	};

	class ComputeHashTask : public IPipelineTask
	{
	public:
		ComputeHashTask() = default;

		virtual void Init() override
		{
			// No dependency
		}

		virtual void Execute() override
		{
			m_ObjectHash = m_Object->GetTypeHash();
		}

		void SetObject(HObjPtr<HObject> object)
		{
			m_Object = object;
		}

		HObjectTypeHash GetHash()
		{
			return m_ObjectHash;
		}
	private:
		HObjPtr<HObject> m_Object = nullptr;
		HObjectTypeHash m_ObjectHash;
	};

	class CompileMapTask : public IPipelineTask
	{
	public:
		CompileMapTask()
		{

		}

		virtual void Init() override
		{
			HObjPtr<HObject> obj0;
			HObjPtr<HObject> obj1;
			HObjPtr<HObject> obj2;
			m_ComputeHashTask[0].SetObject(obj0);
			m_ComputeHashTask[1].SetObject(obj1);
			m_ComputeHashTask[2].SetObject(obj2);
			m_Scheduler.AddDependency(this, &m_ComputeHashTask[0]);
			m_Scheduler.AddDependency(this, &m_ComputeHashTask[1]);
			m_Scheduler.AddDependency(this, &m_ComputeHashTask[2]);
			m_Scheduler.Resolve();
		}

		virtual void Execute() override
		{
			m_Scheduler.Execute();
		}
	private:
		ComputeHashTask m_ComputeHashTask[3];
		PipelineDAGScheduler m_Scheduler;
	};
}