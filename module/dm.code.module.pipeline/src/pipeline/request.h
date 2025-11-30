#pragma once

namespace dm
{

	template <typename T>
	struct Request
	{
	};

	template <typename T>
	struct Response
	{
	};

	enum class ResponseStatus
	{
		SUCCESS,
		FAILED,
	};

	template <typename T>
	Response<T> request_send(const Request<T> &req)
	{
		return request_handle(req);
	}

	template <typename T, typename... Args>
	Response<T> request_send(Args &&...args)
	{
		Request<T> req(std::forward<Args>(args)...);
		return request_send(req);
	}

	// TODO: request_send_async
}
