// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/AllMid/WSelectedTarget.h"

void UWSelectedTarget::SetTarget(UCHAttributeSet* ThisAttributes, FName Name)
{
	this->Attributes = ThisAttributes;
	BP_SetTarget(Name);
}
