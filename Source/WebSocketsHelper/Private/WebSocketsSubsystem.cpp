// Fill out your copyright notice in the Description page of Project Settings.


#include "WebSocketsSubsystem.h"

#include "IWebSocket.h"
#include "WebSocketsModule.h"

void UWebSocketsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if(FModuleManager::Get().IsModuleLoaded("WebSockets") == false)
	{
		FModuleManager::Get().LoadModule("WebSockets");
	}
}

void UWebSocketsSubsystem::Deinitialize()
{
	Super::Deinitialize();

	WebSocketChannelMap.Empty();
}

bool UWebSocketsSubsystem::Connect(const FString& Channel, const FString& ServerAddress)
{
	if(WebSocketChannelMap.Contains(Channel)) return false;
	
	const TSharedRef<IWebSocket>& WebSocket = WebSocketChannelMap.Add(
		Channel,
		FWebSocketsModule::Get().CreateWebSocket(ServerAddress));
	
	WebSocket->OnConnected().AddLambda([this, Channel]()
	{
		if(IsValid(this) == false) return;
		OnConnectionConnectedInternal(Channel);
	});

	WebSocket->OnClosed().AddLambda([this, Channel](int32 StatusCode, const FString& Reason, bool bWasClean)
	{
		if(IsValid(this) == false) return;
		OnConnectionClosedInternal(Channel, Reason);
	});

	WebSocket->OnConnectionError().AddLambda([this, Channel](const FString& Error)
	{
		if(IsValid(this) == false) return;
		OnConnectionErrorInternal(Channel, Error);
	});

	WebSocket->OnMessage().AddLambda([this, Channel](const FString& MessageString)
	{
		if(IsValid(this) == false) return;
		OnReceivedMessageInternal(Channel, MessageString);
	});

	WebSocket->Connect();
	
	return true;
}

void UWebSocketsSubsystem::Close(const FString& Channel)
{
	if(WebSocketChannelMap.Contains(Channel) == false) return;
	
	WebSocketChannelMap[Channel]->Close();
}

bool UWebSocketsSubsystem::SendMessage(const FString& Channel, const FString& Message)
{
	if(WebSocketChannelMap.Contains(Channel) == false) return false;

	const TSharedRef<IWebSocket>& WebSocket = WebSocketChannelMap[Channel];
	if(WebSocket->IsConnected() == false) return false;
	
	WebSocket->Send(Message);
	return true;
}

void UWebSocketsSubsystem::OnConnectionConnectedInternal(const FString& Channel)
{
	if(WebSocketChannelMap.Contains(Channel) == false) return;
	
	OnConnectionConnected.Broadcast(Channel);

	UE_LOG(LogTemp, Log, TEXT("WebSocketsSubsystem, Connected, Channel: %s"), *Channel);
}

void UWebSocketsSubsystem::OnConnectionErrorInternal(const FString& Channel, const FString& Error)
{
	OnConnectionError.Broadcast(Channel, Error);

	UE_LOG(LogTemp, Log, TEXT("WebSocketsSubsystem, Connection Error, Channel: %s, Error: %s"), *Channel, *Error);
}

void UWebSocketsSubsystem::OnConnectionClosedInternal(const FString& Channel, const FString& Reason)
{
	WebSocketChannelMap.Remove(Channel);
	
	OnConnectionClosed.Broadcast(Channel, Reason);

	UE_LOG(LogTemp, Log, TEXT("WebSocketsSubsystem, Closed, Channel: %s, Reason: %s"), *Channel, *Reason);
}

void UWebSocketsSubsystem::OnReceivedMessageInternal(const FString& Channel, const FString& Message)
{
	if(WebSocketChannelMap.Contains(Channel) == false) return;
	
	OnReceivedMessage.Broadcast(Channel, Message);

	UE_LOG(LogTemp, Log, TEXT("WebSocketsSubsystem, Received Message, Channel: %s, Message: %s"), *Channel, *Message);
}
