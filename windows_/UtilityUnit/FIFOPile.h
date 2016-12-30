


#ifndef _FIFO_PIPE_
#define _FIFO_PIPE_

#include "Windows.h"


template<class DATA_TYPE, unsigned int QUEUE_SIZE, unsigned int REAL_QUEUE_SZIE = QUEUE_SIZE+1>
class TFIFO_Pipe
{
public:
	TFIFO_Pipe(): 
	  m_Ahead(0),
	  m_Behind(0)
	{

	}

	~TFIFO_Pipe()
	{

	}

	bool Push(const DATA_TYPE & inData)
	{
		const unsigned int behind = m_Behind;

		if (behind == (m_Ahead+1)%REAL_QUEUE_SZIE)
		{
			return false;
		}
	
		m_Array[m_Ahead] = inData;
		AtomicForware(&m_Ahead);
		return true;
	}

	bool Pop(DATA_TYPE & outDate)
	{
		const unsigned int ahead = m_Ahead;

		if (ahead == m_Behind)
		{
			return false;
		}
		
		outDate = m_Array[m_Behind];
		AtomicForware(&m_Behind);
		return true;
	}

private:
	void AtomicForware(long volatile * cusror)
	{
		InterlockedExchange(cusror, (*cusror+1)%REAL_QUEUE_SZIE);
	}

private:
	long volatile m_Ahead;
	long volatile m_Behind;

	DATA_TYPE	m_Array[REAL_QUEUE_SZIE];
};
#endif