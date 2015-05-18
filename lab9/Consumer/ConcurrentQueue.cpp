#include "stdafx.h"
#include <Windows.h>
#include "ConcurrentQueue.h"

std::string getMessage(queueStruct &queue)
{
	if (queue.current == 0) return 0;
	std::string res = queue.messages[0];
	for (int i = 0; i < queue.current; i++)
		queue.messages[i] = queue.messages[i + 1];
	queue.current--;
	return res;
}

BOOL setMessage(queueStruct &queue, std::string message)
{
	if (queue.current == 10) return FALSE;
	//queue.messages[queue.current]
	//for (int i = 0; i < message.length(); i++)
	queue.messages[queue.current++].append(message);
	//queue.current++;
	return TRUE;
}