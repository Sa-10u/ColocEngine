#pragma once
#include <string>
using std::string;
typedef unsigned int Entity;

class IComp_Data
{
};

class IComp_System
{
};

namespace ECSManager
{
	constexpr unsigned int OBJECTS_AMMOUNT;
	IComp_Data* data;
	IComp_System* system;
	Entity* entity;


}