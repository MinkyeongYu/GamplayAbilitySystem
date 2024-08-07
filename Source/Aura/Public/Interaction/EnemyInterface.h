// Copyright Stella Yu

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnemyInterface.generated.h"

// UInterface를 상속받아 블루프린트에서 사용할 수 있도록 하는 클래스
UINTERFACE(MinimalAPI)
class UEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IEnemyInterface
{
	GENERATED_BODY()

	// 실제 인터페이스를 정의하는 클래스. 순수 가상 함수를 선언하여 해당 인터페이스를 구현하는 클래스가 이 함수를 반드시 정의하도록 강제
public:
	virtual void HighlightActor() = 0;
	virtual void UnHighlightActor() = 0;
};
