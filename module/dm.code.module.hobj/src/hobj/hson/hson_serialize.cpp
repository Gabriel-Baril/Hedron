#include "hson.h"

namespace dm
{
	void hson_serialize(hostream &stream, const Hson &h)
	{
		stream << h.version;
		stream << h.flags;
		stream << h.fieldCount;
		stream << h.payloadByteSize;
		stream << h.namePayloadByteSize;

		const HsonFlags hsonFlags = h.flags;
		stream.write_pod(h.sortedFieldHashes, h.fieldCount);
		if (flag_on(hsonFlags, HsonFlags::SERIALIZE_FIELD_TYPE))
		{
			stream.write_pod(h.sortedFieldTypeHashes, h.fieldCount);
		}
		stream.write_pod(h.sortedFieldFlags, h.fieldCount);

		if (flag_on(hsonFlags, HsonFlags::SERIALIZE_FIELD_PAYLOAD_SIZE))
		{
			stream.write_pod(h.sortedFieldPayloadByteSizes, h.fieldCount);
		}

		stream.write_pod(h.sortedFieldPayloadByteOffsets, h.fieldCount);

		if (flag_on(hsonFlags, HsonFlags::SERIALIZE_FIELD_NAME))
		{
			stream.write_pod(h.sortedFieldNamePayloadByteOffsets, h.fieldCount);
		}

		if (
				flag_on(hsonFlags, HsonFlags::SERIALIZE_FIELD_HIERARCHY) &&
				flag_on(hsonFlags, HsonFlags::SERIALIZE_INTERMEDIATE_FIELD))
		{
			stream.write_pod(h.packedFieldHierarchy, h.fieldCount);
		}

		if (flag_on(hsonFlags, HsonFlags::SERIALIZE_FIELD_NAME))
		{
			stream.write_pod(h.sortedFieldNamePayload, h.namePayloadByteSize);
		}
		stream.write_pod(h.sortedFieldPayload, h.payloadByteSize);
	}

	void hson_deserialize(histream &stream, Hson &h)
	{
		stream >> h.version;
		stream >> h.flags;
		stream >> h.fieldCount;
		stream >> h.payloadByteSize;
		stream >> h.namePayloadByteSize;
		hson_alloc(h); // Now that we have enough information about the hson memory we can properly allocate memory

		const HsonFlags hsonFlags = h.flags;
		stream.read_pod(h.sortedFieldHashes, h.fieldCount);

		if (flag_on(hsonFlags, HsonFlags::SERIALIZE_FIELD_TYPE))
		{
			stream.read_pod(h.sortedFieldTypeHashes, h.fieldCount);
		}

		stream.read_pod(h.sortedFieldFlags, h.fieldCount);

		if (flag_on(hsonFlags, HsonFlags::SERIALIZE_FIELD_PAYLOAD_SIZE))
		{
			stream.read_pod(h.sortedFieldPayloadByteSizes, h.fieldCount);
		}

		stream.read_pod(h.sortedFieldPayloadByteOffsets, h.fieldCount);

		if (flag_on(hsonFlags, HsonFlags::SERIALIZE_FIELD_NAME))
		{
			stream.read_pod(h.sortedFieldNamePayloadByteOffsets, h.fieldCount);
		}

		if (
				flag_on(hsonFlags, HsonFlags::SERIALIZE_FIELD_HIERARCHY) &&
				flag_on(hsonFlags, HsonFlags::SERIALIZE_INTERMEDIATE_FIELD))
		{
			stream.read_pod(h.packedFieldHierarchy, h.fieldCount);
		}

		if (flag_on(hsonFlags, HsonFlags::SERIALIZE_FIELD_NAME))
		{
			stream.read_pod(h.sortedFieldNamePayload, h.namePayloadByteSize);
		}
		stream.read_pod(h.sortedFieldPayload, h.payloadByteSize);
	}
}
