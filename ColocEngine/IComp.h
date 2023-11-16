#pragma once
#include <string>
#include <vector>
#include <stack>

using std::string;
typedef size_t Entity;

class IComp_Data
{
public:
	IComp_Data(Entity e);

	Entity entity;
};

class IComp_System
{
public:
	virtual void Run(Entity e) = 0;
	bool Runnable;
	
	IComp_System();
};

namespace GOBJS
{
//-------------
	const size_t Size = 100;//test
//-------------

	extern IComp_Data** data;
	extern IComp_System** system;

	std::stack<size_t> Vacancy;


}
