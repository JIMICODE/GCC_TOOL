#include "MProcess.h"

unsigned int ProcessManager::UpdateProcess(unsigned long deltaMs)
{
	unsigned short int successCount = 0;
	unsigned short int failCount = 0;

	ProcessList::iterator it = m_processList.begin();
	while (it != m_processList.end())
	{
		//grab the next process
		StrongProcessPtr pCurrProcess = (*it);

		//save the iterator and increment the old one in case we need to remove
		//this process from the list
		ProcessList::iterator thisIt = it;
		++it;

		//process is uninitialized so intialize it
		if (pCurrProcess->GetState() == Process::UNINITIALIZED)
			pCurrProcess->VOnInit();

		//give the process an update tick if it's running
		if (pCurrProcess->GetState() == Process::RUNNING)
			pCurrProcess->VOnUpdate(deltaMs);

		//check to see if the process is dead
		if (pCurrProcess->IsDead())
		{
			//run the sppropriate exit function
			switch (pCurrProcess->GetState())
			{
			case Process::SUCCEEDED:
			{
				pCurrProcess->VOnSuccess();
				StrongProcessPtr pChild = pCurrProcess->RemoveChild();
				if (pChild)
					AttachProcess(pChild);
				else
					++successCount;	//only counts if the whole chain complete
				break;
			}
			case Process::FAILED:
			{
				pCurrProcess->VOnFail();
				++failCount;
				break;
			}
			case Process::ABORTED:
			{
				pCurrProcess->VOnAbort();
				++failCount;
				break;
			}
			}
			//remove the process and destory it
			m_processList.erase(thisIt);
		}
	}
	return ((successCount << 16) | failCount);
}
