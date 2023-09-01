// (c) 2023 Pururum LLC. All rights reserved.


#include "Common/FineKeyPanel.h"

#include "Components/Image.h"
#include "Components/Spacer.h"
#include "Components/TextBlock.h"

void UFineKeyPanel::SetKey(const FKey& NewKey, const bool& RequiresHold)
{
	bool NeedsUpdate = false;
	if (Key != NewKey)
	{
		Key = NewKey;
		NeedsUpdate = true;
	}
	if (bRequiresHold != RequiresHold)
	{
		bRequiresHold = RequiresHold;
		NeedsUpdate = true;
	}
	if (NeedsUpdate)
	{
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
	if (IsValid(HoldImage))
	{
		if (!KeyTextValue.IsEmpty())
		{
			if (bRequiresHold)
			{
				HoldImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
			else
			{
				HoldImage->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
		else
		{
			HoldImage->SetVisibility(ESlateVisibility::Collapsed);
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
