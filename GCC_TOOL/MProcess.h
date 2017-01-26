#pragma once
#ifndef MPROCESS_H_
#define MPROCESS_H_
#include<memory>
#include<list>

class Process;
typedef std::shared_ptr<Process> StrongProcessPtr;
typedef std::weak_ptr<Process> WeakProcessPtr;

class Process
{
	friend class ProcessManager;
public:
	enum State
	{
		//Processes that are neither dead nor alive
		UNINITIALIZED = 0,	//create but not running
		REMOVED,				//removed from the process list but not destroyed; this can 
							//happen when a process that is already running is parented
							//to another process
		//Living processes
		RUNNING,				//initialized and running
		PAUSED,				//initialized but paused
		//Dead processed
		SUCCEEDED,			//completed successfullty
		FAILED,				//failed to complete
		ABORTED,				//aborted; may not have started
	};
private:
	State m_state;
	StrongProcessPtr	m_pChild;
public:
	//construction
	Process(void);
	virtual ~Process(void);
protected:
	//interface; these function should be overridden by the subclass as needed
	virtual void VOnInit(void) { m_state = RUNNING; }
	virtual void VOnUpdate(unsigned long deltaMs) = 0;
	virtual void VOnSuccess(void) {}
	virtual void VOnFail(void) {}
	virtual void VOnAbort(void) {}

public:
	//Functions for ending the process
	inline void Succeed(void);
	inline void Fail(void);

	//pause
	inline void Pause(void);
	inline void UnPause(void);

	//accessors
	State GetState(void)	const { return m_state; }
	bool IsAlive(void)const { return m_state == PAUSED || m_state == RUNNING; }
	bool IsDead(void)	const
	{
		return m_state == SUCCEEDED || m_state == FAILED || m_state == ABORTED;
	}
	bool IsRemoved(void)	const { return m_state == REMOVED; }
	bool IsPaused(void)	const { return m_state == PAUSED; }

	//child function
	inline void AttachChild(StrongProcessPtr pChild);
	StrongProcessPtr RemoveChild(void);	//release ownership of the child
	StrongProcessPtr PeekChild(void);		//doesn't release ownership of child

private:
	void SetState(State newState) { m_state = newState; }
};

class ProcessManager
{
private:
	typedef std::list<StrongProcessPtr> ProcessList;
	ProcessList m_processList;

public:
	//construction
	ProcessManager();
	~ProcessManager();

	//interface
	unsigned int UpdateProcess(unsigned long deltaMs);
	WeakProcessPtr AttachProcess(StrongProcessPtr pProcess);
	void AbortAllProcesses(bool immediate);

	//accessors
	unsigned int GetProcessCount(void)	const { return m_processList.size(); }

private:
	void ClearAllProcesses(void);	//should only be called by the destructor
};

#endif // !MPROCESS_H_
