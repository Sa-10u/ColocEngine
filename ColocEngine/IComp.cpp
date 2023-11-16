#include "IComp.h"

IComp_System::IComp_System():Runnable(true)
{
}

IComp_Data::IComp_Data(Entity e)
{
	entity = e;
}
