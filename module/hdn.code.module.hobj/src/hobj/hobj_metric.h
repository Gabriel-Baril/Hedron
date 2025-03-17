#pragma once

#include "core/core.h"
#include "core/stl/stack.h"
#include "core/stl/vector.h"

#include "hobj_metric_operation.h"

namespace hdn
{
	struct ObjectMetricEntry
	{
		u64 timestamp = 0;
		u64 parentOperationID = 0;
		u64 objectID = 0;
		std::thread::id threadId;
		int processId = 0;
		u64 transactionTimeNano = 0;
		ObjectOperationType opType = ObjectOperationType::UNDEFINED;
		
		u64 fileByteRead = 0;
		u64 fileByteWrite = 0;
		string filePath = "";
	};

	class ObjectMetricTracker
	{
	public:
		static ObjectMetricTracker& Get();
		void Begin(ObjectOperationType type, u64 objectID = 0);
		void FilePath(const char* path);
		void FileByteRead(u64 byteCount);
		void FileByteWrite(u64 byteCount);
		void End();
		ObjectMetricEntry& GetLatestMetric();
		void PrintStats();
	private:
		stack<u64> m_MetricStack;
		vector<ObjectMetricEntry> m_StoredMetrics;
	};
}