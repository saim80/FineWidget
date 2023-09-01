// (c) 2023 Pururum LLC. All rights reserved.


#include "Common/FineKeyPanel.h"

#include "Components/Image.h"
#include "Components/Spacer.h"
#include "Components/TextBlock.h"

void UFineKeyPanel::SetKey(const FKey& NewKey)
{
	if (Key != NewKey)
	{
		Key = NewKey;
		UpdateKeyPresentation();
	}
}

void UFineKeyPanel::NativePreConstruct()
{
	Super::NativePreConstruct();
	UpdateKeyPresentation();
}

void UFineKeyPanel::NativeConstruct()
{
	Super::NativeConstruct();
	UpdateKeyPresentation();
}

void UFineKeyPanel::UpdateKeyPresentation()
{
	KeyTextValue = FText::FromString(Key.ToString());
	if (IsValid(KeySpacer))
	{
		if (!KeyTextValue.IsEmpty() && !KeyIconTexture.IsNull())
		{
			KeySpacer->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			KeySpacer->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	if (IsValid(KeyText))
	{
		if (!KeyTextValue.IsEmpty())
		{
			KeyText->SetText(KeyTextValue);
			KeyText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			KeyText->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	if (IsValid(KeyImage))
	{
		if (!KeyIconTexture.IsNull())
		{
			KeyImage->SetBrushFromSoftTexture(KeyIconTexture);
			KeyImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			KeyImage->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}
