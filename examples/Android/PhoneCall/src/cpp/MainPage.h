#pragma once

#include "app.h"

class MainPage : public ui::MainPage
{
	SLIB_DECLARE_SINGLETON(MainPage)

public:
	void initPage();

protected:
	void onOpen() override;

	void onResume() override;

private:
	void showRecording();

	void stopRecording();

	void refreshTimeLabel();

private:
	Ref<AudioRecorder> m_recorder;
	Ref<AudioPlayerBuffer> m_player;

	MemoryBuffer m_bufRecordedAudio;
	sl_size m_nSamplesPlayed = 0;
	Ref<Timer> m_timerUpdateTimeLabel;

};
