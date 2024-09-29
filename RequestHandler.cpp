#include "RequestHandler.h"

RequestHandler::RequestHandler()
{
	m_tpool.start();
}

RequestHandler::~RequestHandler()
{
	m_tpool.stop();
}

res_type RequestHandler::pushRequest(FuncType func, int* arr, long arg1, long arg2)
{
	return m_tpool.push_task(func, arr, arg1, arg2);
}