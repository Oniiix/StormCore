// 

#pragma once

#include "CoreMinimal.h"
#include "MediaPlayer.h"
#include "Engine/DataAsset.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "TutorialPopupAsset.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UTutorialPopupAsset : public UDataAsset
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Custom")
	FString formatString = "<normal>{0}</>";

	UPROPERTY(EditAnywhere, Category = "Custom")
	FText text = TEXTSTR("change_text");

	UPROPERTY(EditAnywhere, Category = "Custom")
	TObjectPtr<UMediaPlayer> mediaPlayer = nullptr;

	UPROPERTY(EditAnywhere, Category = "Custom")
	TObjectPtr<UMediaSource> MediaSource = nullptr;

	UPROPERTY(EditAnywhere, Category = "Custom")
	TObjectPtr<UMaterial> mediaBrush = nullptr;

public:
	FORCEINLINE FString GetFormatString() const
	{
		return FORMAT(formatString, text.ToString());
	}

	FORCEINLINE TObjectPtr<UMediaPlayer> GetMediaPlayer() const
	{
		return mediaPlayer;
	}

	FORCEINLINE TObjectPtr<UMediaSource> GetMediaSource() const
	{
		return MediaSource;
	}

	FORCEINLINE TObjectPtr<UMaterial> GetMediaBrush() const
	{
		return mediaBrush;
	}
};
