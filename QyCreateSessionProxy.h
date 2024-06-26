// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionDelegates.h"
#include "Net/OnlineBlueprintCallProxyBase.h"
#include "QyCreateSessionProxy.generated.h"

/**
 * 
 */
UCLASS()
class LASTDROPGAME_API UQyCreateSessionProxy : public UOnlineBlueprintCallProxyBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "QyOnline")
	static UQyCreateSessionProxy* CreateSession(APlayerController *PlayerController, FName SessionName, int PublicConnections);
	
	virtual void Activate() override;
	
	// DELEGATE
	UPROPERTY(BlueprintAssignable)
	FEmptyOnlineDelegate OnSuccess;
	UPROPERTY(BlueprintAssignable)
	FEmptyOnlineDelegate OnFailure;
	void OnCreateCompleted(FName InSessionName, bool HasZeroError) const;
	
	// VARIABLE
	// The player controller that wanted to create the session
	APlayerController* PlayerController = nullptr;

	
	FName SessionName = FName("Default name");
	int NumPublicConnections = 10;
};
