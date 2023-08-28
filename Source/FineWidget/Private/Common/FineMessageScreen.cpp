// (c) 2023 Pururum LLC. All rights reserved.


#include "Common/FineMessageScreen.h"

#include "FineWidgetLog.h"
#include "Common/FineMessageBanner.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"

UFineMessageScreen::UFineMessageScreen(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
}

void UFineMessageScreen::ShowMessage(const FText NewMessage, const float NewDuration)
{
	auto NewItem = FFineMessageQueueItem();
	NewItem.Message = NewMessage;
	NewItem.Duration = NewDuration;
	// if current banner is not valid, show the message immediately.
	if (!CurrentBanner.IsValid())
	{
		// dequeue a banner
		CurrentBanner = DequeueBanner();
		// check if the banner is valid
		if (CurrentBanner.IsValid())
		{
			// if so, set the message and show it
			CurrentBanner->SetMessage(NewMessage, NewDuration);
			CurrentBanner->Show();
		}
		else
		{
			FW_WARNING("Invalid banner.");
		}
		return;
	}
	else
	{
		// if current banner is valid, enqueue the message
		MessageQueue.Add(NewItem);
	}
}

void UFineMessageScreen::OnBannerDismissed()
{
	if (CurrentBanner.IsValid())
	{
		EnqueueBanner(CurrentBanner.Get());
		CurrentBanner = nullptr;
	}
	// check if there are any messages in the queue
	if (MessageQueue.Num() > 0)
	{
		// if so, dequeue the next message and show it
		const auto NextItem = MessageQueue.Pop();
		ShowMessage(NextItem.Message, NextItem.Duration);
	}
	else
	{
		OnAllMessagesDismissed.Broadcast();
	}
}

UFineMessageBanner* UFineMessageScreen::DequeueBanner()
{
	UFineMessageBanner* Banner = nullptr;
	// check if free banners exist, if so, remove one and return it.
	if (!FreeBanners.IsEmpty())
	{
		Banner = FreeBanners.Pop();
	}
	// if not, create a new one and return it.
	if (IsValid(BannerClass.Get()))
	{
		UFineMessageBanner* NewBanner = CreateWidget<UFineMessageBanner>(GetWorld(), BannerClass);
		NewBanner->OnBannerDismissed.AddDynamic(this, &UFineMessageScreen::OnBannerDismissed);
		Banner = NewBanner;
	}
	if (IsValid(Banner))
	{
		// Add the banner to the overlay.
		auto _Slot = BannerOverlay->AddChildToOverlay(Banner);
		_Slot->SetHorizontalAlignment(HAlign_Fill);
		_Slot->SetVerticalAlignment(VAlign_Fill);
	}
	else
	{
		FW_WARNING("Invalid banner class.");
	}
	return Banner;
}

void UFineMessageScreen::EnqueueBanner(UFineMessageBanner* Banner)
{
	if (!IsValid(Banner))
	{
		return;
	}
	Banner->RemoveFromParent();
	// unbind the banner's delegate
	Banner->OnBannerDismissed.RemoveDynamic(this, &UFineMessageScreen::OnBannerDismissed);
	// Clear the banner's text
	Banner->ClearMessage();
	// add the banner to the free banners array
	FreeBanners.Add(Banner);
}
