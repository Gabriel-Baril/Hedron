#include "hobj_metric.h"

namespace hdn
{
	ObjectMetricTracker& ObjectMetricTracker::get()
	{
		static ObjectMetricTracker s_Instance;
		return s_Instance;
	}

	void ObjectMetricTracker::begin(ObjectOperationType type, u64 objectID)
	{
		ObjectMetricEntry entry;
		entry.opType = type;
		entry.threadId = std::this_thread::get_id();
		entry.processId = getpid();
		entry.transactionTimeNano = -std::chrono::high_resolution_clock::now().time_since_epoch().count();

		if (!m_MetricStack.empty())
		{
			ObjectMetricEntry& parentEntry = get_latest_metric();
			entry.parentOperationID = parentEntry.parentOperationID;
			entry.objectID = parentEntry.objectID;
		}
		m_StoredMetrics.push_back(entry);
		m_MetricStack.push(m_StoredMetrics.size() - 1);
	}

	void ObjectMetricTracker::file_path(const char* path)
	{
		ObjectMetricEntry& top = get_latest_metric();
		top.filePath = path;
	}

	void ObjectMetricTracker::file_byte_read(u64 byteCount)
	{
		ObjectMetricEntry& top = get_latest_metric();
		top.fileByteRead += byteCount;
	}

	void ObjectMetricTracker::file_byte_write(u64 byteCount)
	{
		ObjectMetricEntry& top = get_latest_metric();
		top.fileByteWrite += byteCount;
	}

	void ObjectMetricTracker::end()
	{
		if (!m_MetricStack.empty()) {
			auto endTime = std::chrono::high_resolution_clock::now().time_since_epoch().count();
			ObjectMetricEntry& completedMetric = get_latest_metric();
			completedMetric.transactionTimeNano += endTime;
			m_MetricStack.pop();
		}
	}
	ObjectMetricEntry& ObjectMetricTracker::get_latest_metric()
	{
		HASSERT(!m_MetricStack.empty(), "Empty stack!");
		u64 completedMetricIndex = m_MetricStack.top();
		return m_StoredMetrics.at(completedMetricIndex);
	}

	void ObjectMetricTracker::print_stats()
	{
		HINFO("Metric Entry Count: {0}", m_StoredMetrics.size());
		for (int i = 0; i < m_StoredMetrics.size(); i++)
		{
			const auto& current = m_StoredMetrics[i];

			float timeMS = (current.transactionTimeNano / 1000.0f) / 1000.0f;

			HINFO("Operation({0}), ObjectID({1}), Time({2}ms)", object_operation_type_to_string(current.opType), current.objectID, timeMS);
		}
	}
}
