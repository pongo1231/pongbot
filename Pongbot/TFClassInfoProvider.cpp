#include "TFClassInfoProvider.h"

TFClassInfoProvider *_TFClassInfoProvider;

void TFClassInfoProvider::Init()
{
	if (!_TFClassInfoProvider)
		_TFClassInfoProvider = new TFClassInfoProvider();
}

void TFClassInfoProvider::Destroy()
{
	if (_TFClassInfoProvider)
		delete _TFClassInfoProvider;
}

TFClassInfo TFClassInfoProvider::GetClassInfo(TFClass tfClass) const
{
	return _ClassInfos.at(tfClass);
}