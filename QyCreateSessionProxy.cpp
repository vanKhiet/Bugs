// Fill out your copyright notice in the Description page of Project Settings.

#include "QyCreateSessionProxy.h"

#include "BlueprintDataDefinitions.h"
#include "GlobalHeader/OnlineHeader.h"
#include "Interfaces/OnlineSessionInterface.h"

UQyCreateSessionProxy* UQyCreateSessionProxy::CreateSession(APlayerController* PlayerController, FName SessionName,
                                                            int PublicConnections)
{
	UQyCreateSessionProxy* Proxy = NewObject<UQyCreateSessionProxy>();
	Proxy->PlayerController = PlayerController;
	Proxy->SessionName = SessionName;
	Proxy->NumPublicConnections = PublicConnections;
	return Proxy;
}

void UQyCreateSessionProxy::Activate()
{
	Super::Activate();

	FOnlineSubsystemBPCallHelperAdvanced Helper(TEXT("CreateSession"), GEngine->GetWorldFromContextObject(PlayerController, EGetWorldErrorMode::LogAndReturnNull));
	Helper.QueryIDFromPlayerController(PlayerController);
	const FUniqueNetId* DebugID1 = Helper.UserID.Get();
	
	IOnlineSession* DebugSessionInterface = IOnlineSubsystem::Get()->GetSessionInterface().Get();
	
	const IOnlineSessionPtr SessionInterface = IOnlineSubsystem::Get()->GetSessionInterface();
	const FUniqueNetId* PlayerId = OnlineHeader::GetPlayerID(PlayerController);
	const int PlayerIdInt = PlayerController->GetPlayerState<APlayerState>()->GetPlayerId();
	
	if (SessionInterface)
	{
		// Bind delegates
		
		FOnlineSessionSettings SessionSettings = FOnlineSessionSettings();
		SessionSettings.NumPublicConnections = NumPublicConnections;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bAllowJoinInProgress = true;
		SessionSettings.bIsLANMatch = false;
		SessionSettings.bIsDedicated = false;
		SessionSettings.bAllowInvites = true;
		SessionSettings.bAllowJoinViaPresence = true;
		SessionSettings.bAllowJoinViaPresenceFriendsOnly = true;
		// For Steam
		SessionSettings.bUsesPresence = true;
		SessionSettings.bUseLobbiesIfAvailable = true;
		
		SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UQyCreateSessionProxy::OnCreateCompleted);
		SessionInterface->CreateSession(PlayerIdInt, NAME_GameSession, SessionSettings);
		
		return;
	} 

	// Fail immediately
	OnFailure.Broadcast();
}

void UQyCreateSessionProxy::OnCreateCompleted(FName InSessionName, bool HasZeroError) const
{
	IOnlineSession* DebugSessionInterface = IOnlineSubsystem::Get()->GetSessionInterface().Get();
	
	const IOnlineSessionPtr SessionInterface = IOnlineSubsystem::Get()->GetSessionInterface();

	SessionInterface->OnCreateSessionCompleteDelegates.Clear();
	
	if (HasZeroError)
	{
		OnSuccess.Broadcast();
	}
	else
	{
		OnFailure.Broadcast();
	}
}
