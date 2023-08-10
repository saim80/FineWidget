// (c) 2023 Pururum LLC. All rights reserved.


#include "Common/FineCheckbox.h"

void UFineCheckbox::NativeConstruct()
{
	Super::NativeConstruct();
	InnerCheckbox->OnCheckStateChanged.AddUniqueDynamic(this, &UFineCheckbox::InternalOnCheckStateChanged);
}

void UFineCheckbox::NativeDestruct()
{
	InnerCheckbox->OnCheckStateChanged.RemoveDynamic(this, &UFineCheckbox::InternalOnCheckStateChanged);
	Super::NativeDestruct();
}

void UFineCheckbox::InternalOnCheckStateChanged(bool bIsChecked)
{
	OnCheckStateChanged.Broadcast(bIsChecked);
}
