#pragma once

#include "core/core.h"
#include "core/stl/stack.h"
#include "core/stl/vector.h"

#include "hobj_metric_operation.h"

namespace dm
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
		static ObjectMetricTracker &get();
		void begin(ObjectOperationType type, u64 objectID = 0);
		void file_path(const char *path);
		void file_byte_read(u64 byteCount);
		void file_byte_write(u64 byteCount);
		void end();
		ObjectMetricEntry &get_latest_metric();
		void print_stats();

	private:
		stack<u64> m_MetricStack;
		vector<ObjectMetricEntry> m_StoredMetrics;
	};
}
