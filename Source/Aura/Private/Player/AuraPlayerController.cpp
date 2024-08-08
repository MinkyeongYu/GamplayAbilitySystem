// Copyright Stella Yu


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	// ECC_Visibility: 가시성 채널로, 캐릭터의 시야나 투명한 물체를 감지하는 데 사용
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	// 충돌이 발생하지 않으면 종료
	if(!CursorHit.bBlockingHit) return;

	// 커서에 닿은 물체가 IEnemyInterface로 캐스트 가능하면(IEnemyInterface를 구현하고 있다면) ThisActor에 저장됨
	LastActor = ThisActor;
	ThisActor = CursorHit.GetActor();

	/**
	 * Line Trace from Cursor. There are several scenarios:
	 * A. LastActor is null && ThisAcotr is null
	 *		- Do nothing
	 * B. LastActor is null && ThisActor is valid
	 *		- Highlight This Actor
	 * C. LastActor is valid && ThisActor is null
	 *		- UnHighlight This Actor
	 * D. both Actors are valid, But LastActor != ThisActor (Hovering Different Actors)
	 *		- UnHighlight LastActor, Highlight ThisActor
	 * E. Both actors are valid, and are the same actor
	 *		- Do nothing
	 */
	if(LastActor == nullptr)
	{
		if(ThisActor != nullptr)
		{
			// Case B:
			ThisActor->HighlightActor();
		}
		else
		{
			// Case A: Do nothing
		}
	}
	else
	{
		if(ThisActor == nullptr)
		{
			// Case C:
			LastActor->UnHighlightActor();
		}
		else
		{
			if(LastActor != ThisActor)
			{
				// Case D:
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
			else
			{
				// Case E: Do nothing
			}
		}
	}
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	// 유효한지 확인
	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if(Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}
	
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	// 키보드, 마우스, UI 인풋 모두 가능
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
	
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	// y축 회전각, z축 회전각, x축 회전각
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	// 위에서 z축 기준으로 캐릭터가 회전하면, 회전된 좌표계에서 x축 방향의 단위 벡터를 가져와, ForwardDirection에 저장한다.
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	if(APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}
