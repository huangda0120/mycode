#ifndef _OBJECT_POOL_
#define _OBJECT_POOL_

#include "new"
#include "queue"

template<class ObjectType, unsigned int ObjectNum>
class CObjectPool
{
public:
//Constructor:
	CObjectPool()
	{
		m_pBlock = 0;
		assert(m_HoldObjects.empty());

		if (!ObjectNum)
		{
			printf("Temptelate param ObjectNum is zero....\n");
		}
		else
		{
			unsigned int memorySize = sizeof(ObjectType) * ObjectNum;
			ObjectType * m_pBlock = static_cast<ObjectType *>(malloc(memorySize));

			if (!m_pBlock)
			{
				printf("The system memory was lack...\n");
				exit(1);
			}
			
			ObjectType * temp = m_pBlock;

			for (unsigned int i = 0; i<ObjectNum; ++i)
			{
				m_HoldObjects.push(temp);
				temp++;
			}
		}
	}
//Destructor:
	~CObjectPool()
	{
		free(m_pBlock);
	}


//Methods:
	ObjectType * Allocate()
	{
		ObjectType * result = 0;

		if (m_HoldObjects.size())
		{
			result = m_HoldObjects.front();
			m_HoldObjects.pop();
			new(result)ObjectType();
		}

		return result;
		
	}

	void Deallocate(ObjectType * pObj)
	{
		if (pObj)
		{
			pObj->~ObjectType();
			m_HoldObjects.push(pObj);
		}
	}

private:
	ObjectType * m_pBlock;
	std::queue<ObjectType *> m_HoldObjects;
};


#endif
