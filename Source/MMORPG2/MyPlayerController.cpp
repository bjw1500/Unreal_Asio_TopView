// Fill out your copyright notice in the Description page of Project Settings.
#include "MyPlayerController.h"
#include "Network/Protocol.pb.h"
#include "Network/NetworkSession.h"
#include "Network/Buffer.h"
#include "Network/FWorker.h"
#include "Network/Service.h"
#include "Network/BufferHelper.h"
#include "Network/SocketUtils.h"
#include "Managers/NetworkManager.h"
#include "Network/ClientPacketHandler.h"
#include "Kismet/GameplayStatics.h"
#include "Network/GameManager.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


AMyPlayerController::AMyPlayerController()
{

}

void AMyPlayerController::BeginPlay()
{
	//GameInstance->GetNetworkManager()->Init();
	Super::BeginPlay();

	check(PlayerInputContext);

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		//뒤에 숫자는 입력 우선 순위.
		Subsystem->AddMappingContext(PlayerInputContext, 0);
	}

	bShowMouseCursor = true;
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);

}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent)) {


		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyPlayerController::Move);

	}
}

void AMyPlayerController::Tick(float DeltaSeconds)
{
	
	TArray<PacketMessage> list = GameInstance->GetPacketQueue()->PopAll();
	for (PacketMessage message : list)
	{
		GameInstance->GetPacketHandler()->HandlePacket(message);
	}

	if (GameInstance->bConnected == false)
		return;
	if (bCanControll == false)
		return;
	SendPlayerInfo();

}

void AMyPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();

}

void AMyPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//GameInstance->GetNetworkManager()->Disconnect();
	//GameInstance->GetPacketHandler()->Make_C_DisConnect(TEXT("Game End"));

}

void AMyPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, CursorHit);

	//커서에 찍힌게 없으면 리턴.
	if (CursorHit.bBlockingHit == false)
		return;

	LastActor = ThisActor;
	ThisActor = Cast<IObjectInterface>(CursorHit.GetActor());

	/*
		커서 이벤트 경우의 수
		1.Last Actor = null && This Actor = null
			X
		2.Last Actor && This Actor = null
			Un Last Actor
		3.	Last Actor = null && This Actor
			HighLight This Actor
		4.Last Actor && This Actor
			Un Last Actor, Highlight this Actor;
		5.Last Actor == This Actor
			X
	*/

	if (LastActor == nullptr && ThisActor == nullptr)
	{

	}
	else if (LastActor && ThisActor == nullptr)
	{
		LastActor->UnHighlightActor();
	}
	else if (LastActor == nullptr && ThisActor)
	{
		ThisActor->HighlightActor();
	}
	else if (LastActor && ThisActor)
	{
		LastActor->UnHighlightActor();
		ThisActor->HighlightActor();

	}
	else if (LastActor == ThisActor)
	{

	}
}


void AMyPlayerController::Move(const FInputActionValue& InputActionValue)
{
	APawn* ControlledPawn = GetPawn();
	if (IsValid(ControlledPawn) == true)
	{
		FVector2D MovementVector = InputActionValue.Get<FVector2D>();
		// find out which way is forward
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		//Horizontal = MovementVector.X;
		//Vertical = MovementVector.Y;

		UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::Printf(TEXT("%d %d"), MovementVector.Y, MovementVector.X));
		ControlledPawn->AddMovementInput(ForwardDirection, MovementVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, MovementVector.X);
	}
}

/************************
* Network
* ***********************/

void AMyPlayerController::SendPlayerInfo()
{
	//ACreature* player = Cast<ACreature>(UGameplayStatics::GetPlayerCharacter(this, 0));
	//if (IsValid(player) == false)
	//	return;

	//const auto& location = player->GetActorLocation();
	//const auto & rotation = player->GetActorRotation();
	//const auto & velocity = player->GetVelocity();

	//ObjectInfo를 어떻게 해줄까?
	//Protocol::ObjectInfo* newInfo = player->GetInfo();
	//Protocol::Position* updatePosition = newInfo->mutable_position();

	//updatePosition->set_locationx(location.X);
	//updatePosition->set_locationy(location.Y);
	//updatePosition->set_locationz(location.Z);

	//updatePosition->set_rotationx(rotation.Roll);
	//updatePosition->set_rotationy(rotation.Pitch);
	//updatePosition->set_rotationz(rotation.Yaw);

	//updatePosition->set_velocityx(velocity.X);
	//updatePosition->set_velocityy(velocity.Y);
	//updatePosition->set_velocityz(velocity.Z);

	//서버와 연결된 상태가 아니라면, 
	//if (GameInstance->bConnected == false)
	//	return;

	//GameInstance->GetPacketHandler()->Make_C_Move(*newInfo);
}


