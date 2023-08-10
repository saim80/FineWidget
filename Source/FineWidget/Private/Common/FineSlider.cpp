// (c) 2023 Pururum LLC. All rights reserved.


#include "Common/FineSlider.h"

#include "Components/Slider.h"

void UFineSlider::NativeConstruct()
{
	Super::NativeConstruct();
	InnerSlider->OnValueChanged.AddUniqueDynamic(this, &UFineSlider::InternalOnValueChanged);
}

void UFineSlider::NativeDestruct()
{
	InnerSlider->OnValueChanged.RemoveDynamic(this, &UFineSlider::InternalOnValueChanged);
	Super::NativeDestruct();
}

void UFineSlider::InternalOnValueChanged(float Value)
{
	OnValueChanged.Broadcast(Value);
}
