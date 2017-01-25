#pragma once
#ifndef GCC_TOOL
#define GCC_TOOL
//include head file and library
#include<map>
#include<string>
#include<memory>
#include<tinyxml.h>
//Front class delcaler
class ActorFactory;
class ActorComponent;

//type define 
typedef unsigned long ActorId;
typedef unsigned long CompentId;
typedef ActorComponent *(*ActorComponentCreator)(void);
typedef std::map<std::string, ActorComponentCreator> ActorComponentCreatorMap;
typedef std::shared_ptr<Actor> StrongActorPtr;
typedef std::shared_ptr<ActorComponent> StrongActorComponentPtr;

//class define

//Compent base class
class Actor
{
	friend class ActorFactory;
private:
	ActorId m_id;
public:
	const ActorId GetId(void) const { return m_id; }
};

class ActorComponent
{

};

class ActorFactory
{
private:
	ActorId m_lastActorId;
	ActorId GetNextActorId(void) { ++m_lastActorId; return m_lastActorId; }
protected:
	ActorComponentCreatorMap m_actorComponentCreators;
	virtual StrongActorComponentPtr CreateComponent(TiXmlAttribute);
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
