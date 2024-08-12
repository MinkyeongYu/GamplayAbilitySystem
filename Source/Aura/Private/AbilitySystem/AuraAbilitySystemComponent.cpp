// Copyright Stella Yu


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AuraGameplayTags.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	// FGameplayEffect가 적용된 오브젝트를 가져오는 델리게이트
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);

}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
	const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer Tagcontainer;
	EffectSpec.GetAllAssetTags(Tagcontainer);

	// Broadcast Tags to Widget Controller(script that calls EffectAssetTags Delegate)
	EffectAssetTags.Broadcast(Tagcontainer);
	
}
