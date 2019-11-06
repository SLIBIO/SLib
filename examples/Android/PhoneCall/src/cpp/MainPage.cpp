#include "MainPage.h"

SLIB_DEFINE_SINGLETON(MainPage)

#define SAMPLE_PER_SECOND 16000

void MainPage::initPage()
{
	Device::addOnIncomingCall([this](String callId, String phoneNumber) {
		Println("Incoming: %s, %s", callId, phoneNumber);
		groupCall->setVisible(sl_true);
		lblCallInfo->setText("In: " + phoneNumber);
		btnAnswer->setVisible(sl_true);
		btnAnswer->setOnClick([this, callId](View*) {
			Device::answerCall(callId);
			btnAnswer->setVisibility(Visibility::Gone);
			showRecording();
		});
		btnEndCall->setOnClick([callId](View*) {
			Device::endCall(callId);
		});
	});
	Device::addOnOutgoingCall([this](String callId, String phoneNumber) {
		Println("Outgoing: %s, %s", callId, phoneNumber);
		groupCall->setVisible(sl_true);
		lblCallInfo->setText("Out: " + phoneNumber);
		btnAnswer->setVisibility(Visibility::Gone);
		btnEndCall->setOnClick([callId](View*) {
			Device::endCall(callId);
		});
		showRecording();
	});
	Device::addOnEndCall([this](String callId, String phoneNumber) {
		Println("End: %s, %s", callId, phoneNumber);
		groupCall->setVisible(sl_false);
		stopRecording();
	});

}

void MainPage::onOpen()
{
	Application::grantPermissions(AppPermissions::ReadPhoneState, [this]() {
		if (selectSIM->getItemsCount() < 2) {
			sl_uint32 nSIM = Device::getSimSlotsCount();
			if (nSIM > 0) {
				for (sl_uint32 i = 0; i < nSIM; i++) {
					String phoneNumber = Device::getPhoneNumber(i);
					if (phoneNumber.isNotEmpty()) {
						selectSIM->addItem(String::fromUint32(i), String::format("SIM%d(%s)", i+1, phoneNumber), UIUpdateMode::Init);
					} else {
						selectSIM->addItem(String::fromUint32(i), String::format("SIM%d(Empty)", i+1), UIUpdateMode::Init);
					}
				}
			}
		}
	});

	switchSetDefault->setOnChange([](SwitchView*, sl_bool value) {
		if (value) {
			Application::setDefaultCallingApp([]() {
				Println("Set Default Calling App Result: %d", Application::isDefaultCallingApp());
			});
		} else {
			Application::openDefaultAppsSetting();
		}
	});
	switchSystemOverlay->setOnChange([](SwitchView*, sl_bool value) {
		Application::openSystemOverlaySetting();
	});

	btnCall->setOnClick([this](View*) {
		Application::grantPermissions(AppPermissions::CallPhone, [this]() {
			String value = selectSIM->getSelectedValue();
			if (value.isNotEmpty()) {
				Device::callPhone(txtPhoneNumber->getText(), value.parseUint32());
			} else {
				Device::callPhone(txtPhoneNumber->getText());
			}
		});
	});

	MobileApp::getApp()->setOnOpenUrl([this](MobileApp* app, UIEvent* ev) {
		String phoneNumber = Url::getPhoneNumber(ev->getUrl());
		if (phoneNumber.isNotEmpty()) {
			Toast::show("Open Dial: " + phoneNumber);
			txtPhoneNumber->setText(phoneNumber);
		}
	});
}

void MainPage::onResume()
{
	switchSetDefault->setValue(Application::isDefaultCallingApp());
	switchSystemOverlay->setValue(Application::isEnabledSystemOverlay());
}

void MainPage::showRecording()
{
	if (m_player.isNotNull()) {
		m_player->stop();
		m_player.setNull();
	}
	if (m_recorder.isNotNull()) {
		m_recorder->stop();
		m_recorder.setNull();
	}

	m_nSamplesPlayed = 0;
	m_bufRecordedAudio.clear();
	refreshTimeLabel();

	btnPlay->setVisibility(Visibility::Gone);
	btnStop->setVisibility(Visibility::Gone);
	btnRecord->setVisibility(Visibility::Visible);

	btnRecord->setOnClick([this](View*) {
		Application::grantPermissions(AppPermissions::RecordAudio, [this]() {
			if (!(Application::checkPermissions(AppPermissions::RecordAudio))) {
				return;
			}
			AudioRecorderParam param;
			param.recordingPreset = AudioRecordingPreset::Unprocessed;
			param.samplesPerSecond = SAMPLE_PER_SECOND;
			param.onRecordAudio = [this](AudioRecorder*, AudioData& in) {
				m_bufRecordedAudio.add(Memory::create(in.data, in.count * sizeof(sl_int16)));
				m_nSamplesPlayed = m_bufRecordedAudio.getSize() / sizeof(sl_int16);
				refreshTimeLabel();
			};
			m_recorder = AudioRecorder::create(param);
			btnRecord->setVisibility(Visibility::Gone);
			lblTime->setTextColor(Color::Red);
		});
	});

	Device::setAudioMode(DeviceAudioMode::InCall);
	Dispatch::setTimeout([]() {
		Device::setSpeakerOn();
	}, 500);
}

void MainPage::stopRecording()
{
	lblTime->setTextColor(Color::Black);

	btnRecord->setVisibility(Visibility::Gone);
	btnPlay->setVisibility(Visibility::Gone);
	btnStop->setVisibility(Visibility::Gone);

	if (m_recorder.isNull()) {
		return;
	}
	m_recorder->stop();
	m_recorder.setNull();

	if (!m_nSamplesPlayed) {
		return;
	}

	btnPlay->setVisible(sl_true);
	btnPlay->setOnClick([this](View*) {
		if (m_player.isNull()) {
			AudioPlayerBufferParam param;
			param.samplesPerSecond = SAMPLE_PER_SECOND;
			param.onPlayAudio = [this](AudioPlayerBuffer* buffer, sl_uint32 nSamples) {
				if (buffer->getSamplesCountInQueue()) {
					m_nSamplesPlayed += nSamples;
				} else {
					btnStop->dispatchClick();
				}
			};
			m_player = AudioPlayerBuffer::create(param);
		}
		btnPlay->setVisibility(Visibility::Gone);
		btnStop->setVisibility(Visibility::Visible);

		Memory mem = m_bufRecordedAudio.merge();
		AudioData audio;
		audio.data = mem.getData();
		audio.format = AudioFormat::Int16_Mono;
		audio.count = mem.getSize() / sizeof(sl_int16);
		m_nSamplesPlayed = 0;
		m_player->flush();
		m_player->write(audio);
		m_player->start();
		m_timerUpdateTimeLabel = startTimer([this](Timer*) {
			refreshTimeLabel();
		}, 500);
	});

	btnStop->setOnClick([this](View*) {
		btnPlay->setVisibility(Visibility::Visible);
		btnStop->setVisibility(Visibility::Gone);
		m_player->stop();
		m_timerUpdateTimeLabel.setNull();
	});
}

void MainPage::refreshTimeLabel()
{
	sl_size c = m_nSamplesPlayed / SAMPLE_PER_SECOND;
	sl_size d = m_bufRecordedAudio.getSize() / sizeof(sl_int16) / SAMPLE_PER_SECOND;
	lblTime->setText(String::format("%02d:%02d/%02d:%02d", c / 60, c % 60, d / 60, d % 60));
}
