#pragma once

namespace hdn
{

	template<typename T>
	struct Request {};

	template<typename T>
	struct Response {};

	enum class ResponseStatus
	{
		SUCCESS,
		FAILED,
	};

	template<typename T, typename... Args>
	Response<T> request_send(Args&&... args)
	{
		Request<T> req(std::forward<Args>(args)...);
		return request_handle(req);
	}

	// TODO: request_send_async
}