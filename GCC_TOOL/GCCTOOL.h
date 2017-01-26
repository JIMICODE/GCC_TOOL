#pragma once
#ifndef GCC_TOOL
#define GCC_TOOL
//include head file and library
#include<map>
#include<string>
#include<memory>
#include<tinyxml.h>
#include<Windows.h>

//Front class delcaler
class ActorFactory;
class ActorComponent;
//Macro define
#define GCC_NEW new(_NORMAL_BLOCK_,_FILE_,_LINE)
#define GCC_ERROR(text) MessageBox(NULL,text,text,NULL )

//type define 
typedef unsigned long ActorId;
typedef unsigned long ComponentId;
typedef ActorComponent *(*ActorComponentCreator)(void);
typedef std::map<std::string, ActorComponentCreator> ActorComponentCreatorMap;
typedef std::shared_ptr<Actor> StrongActorPtr;
typedef std::shared_ptr<ActorComponent> StrongActorComponentPtr;

//class define

//Compent base class
class Actor
{
	friend class ActorFactory;
	typedef std::map<ComponentId, StrongActorComponentPtr> ActorComponents;
private:
	ActorId m_id;				//unique id for the actor
	ActorComponents m_components;	//all components this actor has
public:
	explicit Actor(ActorId id);
	~Actor(void);

	bool Init(TiXmlElement* pDate);
	void PostInit(void);
	void Destory(void);
	void Update(int deltaMs);

	//accessors
	ActorId GetId(void) const { return m_id; }

	//template function for retrieving components
	template<class ComponentType>
	std::weak_ptr<ComponentType> GetComponent(ComponentId id)
	{
		ActorComponents::iterator findIt = m_components.find(id);
		if (findIt != m_components.end())
		{
			StrongActorComponentPtr pBase(findIt->second);
			//cast to subclass version of the pointer
			std::shared_ptr<ComponentType> pSub(std::tr1::static_pointer_cast<ComponentType>(pBase));

			std::weak_ptr<ComponentType> pWeakSub(pSub);

			return pWeakSub;	//return the weak pointer
		}
		else
		{
			return std::weak_ptr<ComponentType>();
		}
	}

private:
	//This is called by the ActorFactory; no one else should be
	//adding components.
	void AddComponent(StrongActorComponentPtr pComponent);
};

class ActorComponent
{
	friend class ActorFactory;
protected:
	StrongActorPtr m_pOwner;

public:
	virtual ~ActorComponent(void) {}
	//These functions are meant to be overridden by the implementation
	//classes of the components
	virtual bool VInit(TiXmlElement* pData) = 0;
	virtual void VPostInit(void) {}
	virtual void VUpdate(int deltaMs) {}

	//this function should be overridden by the interface class
	virtual ComponentId VGetComponentId(void) const = 0;

private:
	void SetOwner(StrongActorPtr pOwner) { m_pOwner = pOwner; }
};

class ActorFactory
{
private:
	ActorId m_lastActorId;
	ActorId GetNextActorId(void) { ++m_lastActorId; return m_lastActorId; }
protected:
	ActorComponentCreatorMap m_actorComponentCreators;
	virtual StrongActorComponentPtr CreateComponent(TiXmlElement* pData);
public:
	ActorFactory(void);
	StrongActorPtr CreateActor(const char* actorResource);
};

class GCCRandom
{
public:
	GCCRandom();
	~GCCRandom();
	unsigned int Random(unsigned int range);
	double Random();
	void SetRandomSeed(unsigned int seed);
	unsigned int GetRandomSeed();
private:

};
#endif // !GCC_TOOL
