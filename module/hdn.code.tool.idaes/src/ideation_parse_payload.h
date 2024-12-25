#pragma once
#include "ideation.h"

bool parse_ideation_payload(struct json_object_s* jsonObject, IdeationNode& node);

bool parse_ideation_payload_text(struct json_object_s* jsonObject, IdeationPayload& payload);