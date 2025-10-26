#include "core/core.h"
#include "core/stl/vector.h"
#include "core/stl/ds_base.h"

#include "core/async/async.h"

namespace hdn
{
	class ExampleTaskLeaf : public ITaskLeaf
	{
	public:
		ExampleTaskLeaf(const string& prefixMessage)
			: m_PrefixMessage{ prefixMessage }
		{
		}

		void Execute() override
		{
			for (int i = 0;i < 10000;i++)
			{
				HWARN("[{0}]", m_PrefixMessage.c_str());
			}
			Complete();
		}

		virtual const char* GetName() const override
		{
			return m_PrefixMessage.c_str();
		}
	private:
		string m_PrefixMessage;
	};

	class ExampleTaskParallel : public ITaskParallel
	{
	public:
		ExampleTaskParallel()
		{
			AddTask(&simpleTask0);
			AddTask(&simpleTask1);
			AddTask(&simpleTask2);
			AddTask(&simpleTask3);
			AddTask(&simpleTask4);
			AddTask(&simpleTask5);
			AddTask(&simpleTask6);
			AddTask(&simpleTask7);
			AddTask(&simpleTask8);
			AddTask(&simpleTask9);
			AddTask(&simpleTask10);
			AddTask(&simpleTask11);
		}

		ExampleTaskLeaf simpleTask0{ "SIM_0" };
		ExampleTaskLeaf simpleTask1{ "SIM_1" };
		ExampleTaskLeaf simpleTask2{ "SIM_2" };
		ExampleTaskLeaf simpleTask3{ "SIM_3" };
		ExampleTaskLeaf simpleTask4{ "SIM_4" };
		ExampleTaskLeaf simpleTask5{ "SIM_5" };
		ExampleTaskLeaf simpleTask6{ "SIM_6" };
		ExampleTaskLeaf simpleTask7{ "SIM_7" };
		ExampleTaskLeaf simpleTask8{ "SIM_8" };
		ExampleTaskLeaf simpleTask9{ "SIM_9" };
		ExampleTaskLeaf simpleTask10{ "SIM_10" };
		ExampleTaskLeaf simpleTask11{ "SIM_11" };
	};

	class ExampleTaskQueue : public ITaskQueue
	{
	public:
		ExampleTaskQueue()
		{
			AddTask(&simpleTask0);
			AddTask(&simpleTask1);
			AddTask(&simpleTask2);
			AddTask(&simpleTask3);
		}

		ExampleTaskLeaf simpleTask0{ "SIM_0" };
		ExampleTaskLeaf simpleTask1{ "SIM_1" };
		ExampleTaskLeaf simpleTask2{ "SIM_2" };
		ExampleTaskLeaf simpleTask3{ "SIM_3" };
	};


	class ExampleTaskGraph : public ITaskGraph
	{
	public:
		ExampleTaskGraph()
		{
			AddEdge(&simpleTask0, &simpleTask3);
			AddEdge(&simpleTask1, &simpleTask3);
			AddEdge(&simpleTask4, &simpleTask3);
			AddEdge(&simpleTask3, &simpleTask2);
		}

		ExampleTaskQueue simpleTask0;
		ExampleTaskLeaf simpleTask1{ "SIM_GRAPH_1" };
		ExampleTaskLeaf simpleTask4{ "SIM_GRAPH_4" };
		ExampleTaskLeaf simpleTask2{ "SIM_GRAPH_2" };
		ExampleTaskLeaf simpleTask3{ "SIM_GRAPH_3" };
	};

	class ExampleTaskQueue3 : public ITaskQueue
	{
	public:
		ExampleTaskQueue3()
		{
			AddTask(&simpleTask0);
			AddTask(&simpleTask1);
			AddTask(&simpleTask2);
			AddTask(&queue);
			AddTask(&simpleTask3);
		}

		ExampleTaskLeaf simpleTask0{ "SIM_4" };
		ExampleTaskLeaf simpleTask1{ "SIM_5" };
		ExampleTaskLeaf simpleTask2{ "SIM_6" };
		ExampleTaskQueue queue;
		ExampleTaskLeaf simpleTask3{ "SIM_7" };
	};


	class ExampleTaskQueue2 : public ITaskQueue
	{
	public:
		ExampleTaskQueue2()
		{
			AddTask(&simpleTask2_0);
			AddTask(&queue0);
			AddTask(&simpleTask2_1);
			AddTask(&simpleTask2_2);
			AddTask(&queue1);
		}

		ExampleTaskLeaf simpleTask2_0{ "SIM2_0" };
		ExampleTaskQueue queue0;
		ExampleTaskLeaf simpleTask2_1{ "SIM2_1" };
		ExampleTaskLeaf simpleTask2_2{ "SIM2_2" };
		ExampleTaskQueue3 queue1;
	};
}

int main()
{
	using namespace hdn;
	using namespace std::chrono_literals;
	log_init();

	{
		ExampleTaskGraph task;
		task.Enqueue();
		while (!task.Completed())
		{
			std::this_thread::sleep_for(2000ms);
		}

		HINFO("-----------------------");

		task.PrintTimeHierarchy();

		AsyncOrchestrator::Get().Shutdown();
	}

	HINFO("Allocation Byte: {0}", GetMemStat().allocated);
	HINFO("Allocation Count: {0}", GetMemStat().allocationCount);
	HINFO("Deallocation Count: {0}", GetMemStat().deallocationCount);
}