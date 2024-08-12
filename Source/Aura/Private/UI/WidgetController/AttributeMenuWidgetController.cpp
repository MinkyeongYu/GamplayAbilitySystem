// Copyright Stella Yu


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "AuraGameplayTags.h"

/* WBP_AttributeMenu에서 GetAttributeMenuWidgetController()을 호출하여 자동으로 BindCallbacksToDependencies()도 호출되고 있음 */
void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	for(auto& Pair : AS->TagsToAttributes)
	{
		// 각 Tag의 Value값이 바뀔때마다 자동으로 실행됨
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
    		[this, Pair](const FOnAttributeChangeData& Data)
    		{
    			BroadcastAttributeInfo(Pair.Key, Pair.Value());
    		}
    	);		
	}
}

/*
 * 1. UAuraAttributeSet 클래스의 인스턴스를 가져옵니다. AttributeInfo 객체가 유효한지 확인합니다.
 * 2. 맵의 각 쌍에 대해, 게임플레이 태그(Key)와 해당 태그에 연결된 속성(Signature)을 사용하여 값을 가져옵니다.
 * 3. 각 속성의 Tag에 대해 FAuraAttributeInfo 구조체의 Tag와 일치하는 것을 찾습니다.
 * 4. 바인딩된 델리게이트를 실행하여 FGameplayAttribute 객체를 얻습니다.
 * 5. FGameplayAttribute 객체에서 해당 속성의 실제 값을 가져옵니다.
 *
 * => 속성을 개별적으로 처리하는 대신, 맵을 순회하며 Gameplay Tag에 해당하는 Gameplay Attribute를 찾아 위젯에 전달합니다.
 * WBP_AttributeMenu에서 호출하고 있음
 */
void UAttributeMenuWidgetController::BroadCastInitialValues()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(AttributeInfo);
	
	for(auto& Pair : AS->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
