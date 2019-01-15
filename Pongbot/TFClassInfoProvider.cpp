#include "TFClassInfoProvider.h"
#include "Util.h"

TFClassInfoProvider *_TFClassInfoProvider;

void TFClassInfoProvider::Init()
{
	if (!_TFClassInfoProvider)
	{
		Util::DebugLog("INIT TFClassInfoProvider");

		_TFClassInfoProvider = new TFClassInfoProvider();
	}
}

void TFClassInfoProvider::Destroy()
{
	if (_TFClassInfoProvider)
	{
		Util::DebugLog("DESTROY TFClassInfoProvider");

		delete _TFClassInfoProvider;
	}
}

TFClassInfo TFClassInfoProvider::GetClassInfo(TFClass tfClass) const
{
	return _ClassInfos.at(tfClass);
}