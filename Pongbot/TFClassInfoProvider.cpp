#include "TFClassInfoProvider.h"
#include <hlsdk/public/edict.h> // For Assert, too lazy to search which header defines it directly

TFClassInfoProvider *_TFClassInfoProvider;

void TFClassInfoProvider::Init()
{
	Assert(!_TFClassInfoProvider);
	_TFClassInfoProvider = new TFClassInfoProvider();
}

void TFClassInfoProvider::Destroy()
{
	Assert(_TFClassInfoProvider);
	delete _TFClassInfoProvider;
}

TFClassInfo TFClassInfoProvider::GetClassInfo(TFClass tfClass) const
{
	return _ClassInfos.at(tfClass);
}