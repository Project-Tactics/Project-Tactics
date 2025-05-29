#include "ClickSDLBackend.h"

#include "../Click.h"

#include <SDL.h>
#include <optional>

namespace click {

static InputCode _toKeyboardInputCode(SDL_Keycode sdlKey) {
	switch (sdlKey) {
	case SDLK_UNKNOWN			: return InputCode::Unknown;
	case SDLK_RETURN			: return InputCode::KeyReturn;
	case SDLK_ESCAPE			: return InputCode::KeyEscape;
	case SDLK_BACKSPACE			: return InputCode::KeyBackspace;
	case SDLK_TAB				: return InputCode::KeyTab;
	case SDLK_SPACE				: return InputCode::KeySpace;
	case SDLK_EXCLAIM			: return InputCode::KeyExclaim;
	case SDLK_QUOTEDBL			: return InputCode::KeyQuotedbl;
	case SDLK_HASH				: return InputCode::KeyHash;
	case SDLK_PERCENT			: return InputCode::KeyPercent;
	case SDLK_DOLLAR			: return InputCode::KeyDollar;
	case SDLK_AMPERSAND			: return InputCode::KeyAmpersand;
	case SDLK_QUOTE				: return InputCode::KeyQuote;
	case SDLK_LEFTPAREN			: return InputCode::KeyLeftparen;
	case SDLK_RIGHTPAREN		: return InputCode::KeyRightparen;
	case SDLK_ASTERISK			: return InputCode::KeyAsterisk;
	case SDLK_PLUS				: return InputCode::KeyPlus;
	case SDLK_COMMA				: return InputCode::KeyComma;
	case SDLK_MINUS				: return InputCode::KeyMinus;
	case SDLK_PERIOD			: return InputCode::KeyPeriod;
	case SDLK_SLASH				: return InputCode::KeySlash;
	case SDLK_0					: return InputCode::Key0;
	case SDLK_1					: return InputCode::Key1;
	case SDLK_2					: return InputCode::Key2;
	case SDLK_3					: return InputCode::Key3;
	case SDLK_4					: return InputCode::Key4;
	case SDLK_5					: return InputCode::Key5;
	case SDLK_6					: return InputCode::Key6;
	case SDLK_7					: return InputCode::Key7;
	case SDLK_8					: return InputCode::Key8;
	case SDLK_9					: return InputCode::Key9;
	case SDLK_COLON				: return InputCode::KeyColon;
	case SDLK_SEMICOLON			: return InputCode::KeySemicolon;
	case SDLK_LESS				: return InputCode::KeyLess;
	case SDLK_EQUALS			: return InputCode::KeyEquals;
	case SDLK_GREATER			: return InputCode::KeyGreater;
	case SDLK_QUESTION			: return InputCode::KeyQuestion;
	case SDLK_AT				: return InputCode::KeyAt;
	case SDLK_LEFTBRACKET		: return InputCode::KeyLeftbracket;
	case SDLK_BACKSLASH			: return InputCode::KeyBackslash;
	case SDLK_RIGHTBRACKET		: return InputCode::KeyRightbracket;
	case SDLK_CARET				: return InputCode::KeyCaret;
	case SDLK_UNDERSCORE		: return InputCode::KeyUnderscore;
	case SDLK_BACKQUOTE			: return InputCode::KeyBackquote;
	case SDLK_a					: return InputCode::KeyA;
	case SDLK_b					: return InputCode::KeyB;
	case SDLK_c					: return InputCode::KeyC;
	case SDLK_d					: return InputCode::KeyD;
	case SDLK_e					: return InputCode::KeyE;
	case SDLK_f					: return InputCode::KeyF;
	case SDLK_g					: return InputCode::KeyG;
	case SDLK_h					: return InputCode::KeyH;
	case SDLK_i					: return InputCode::KeyI;
	case SDLK_j					: return InputCode::KeyJ;
	case SDLK_k					: return InputCode::KeyK;
	case SDLK_l					: return InputCode::KeyL;
	case SDLK_m					: return InputCode::KeyM;
	case SDLK_n					: return InputCode::KeyN;
	case SDLK_o					: return InputCode::KeyO;
	case SDLK_p					: return InputCode::KeyP;
	case SDLK_q					: return InputCode::KeyQ;
	case SDLK_r					: return InputCode::KeyR;
	case SDLK_s					: return InputCode::KeyS;
	case SDLK_t					: return InputCode::KeyT;
	case SDLK_u					: return InputCode::KeyU;
	case SDLK_v					: return InputCode::KeyV;
	case SDLK_w					: return InputCode::KeyW;
	case SDLK_x					: return InputCode::KeyX;
	case SDLK_y					: return InputCode::KeyY;
	case SDLK_z					: return InputCode::KeyZ;
	case SDLK_CAPSLOCK			: return InputCode::KeyCapslock;
	case SDLK_F1				: return InputCode::KeyF1;
	case SDLK_F2				: return InputCode::KeyF2;
	case SDLK_F3				: return InputCode::KeyF3;
	case SDLK_F4				: return InputCode::KeyF4;
	case SDLK_F5				: return InputCode::KeyF5;
	case SDLK_F6				: return InputCode::KeyF6;
	case SDLK_F7				: return InputCode::KeyF7;
	case SDLK_F8				: return InputCode::KeyF8;
	case SDLK_F9				: return InputCode::KeyF9;
	case SDLK_F10				: return InputCode::KeyF10;
	case SDLK_F11				: return InputCode::KeyF11;
	case SDLK_F12				: return InputCode::KeyF12;
	case SDLK_PRINTSCREEN		: return InputCode::KeyPrintscreen;
	case SDLK_SCROLLLOCK		: return InputCode::KeyScrolllock;
	case SDLK_PAUSE				: return InputCode::KeyPause;
	case SDLK_INSERT			: return InputCode::KeyInsert;
	case SDLK_HOME				: return InputCode::KeyHome;
	case SDLK_PAGEUP			: return InputCode::KeyPageup;
	case SDLK_DELETE			: return InputCode::KeyDelete;
	case SDLK_END				: return InputCode::KeyEnd;
	case SDLK_PAGEDOWN			: return InputCode::KeyPagedown;
	case SDLK_RIGHT				: return InputCode::KeyRight;
	case SDLK_LEFT				: return InputCode::KeyLeft;
	case SDLK_DOWN				: return InputCode::KeyDown;
	case SDLK_UP				: return InputCode::KeyUp;
	case SDLK_NUMLOCKCLEAR		: return InputCode::KeyNumlockclear;
	case SDLK_KP_DIVIDE			: return InputCode::KeypadDivide;
	case SDLK_KP_MULTIPLY		: return InputCode::KeypadMultiply;
	case SDLK_KP_MINUS			: return InputCode::KeypadMinus;
	case SDLK_KP_PLUS			: return InputCode::KeypadPlus;
	case SDLK_KP_ENTER			: return InputCode::KeypadEnter;
	case SDLK_KP_1				: return InputCode::Keypad1;
	case SDLK_KP_2				: return InputCode::Keypad2;
	case SDLK_KP_3				: return InputCode::Keypad3;
	case SDLK_KP_4				: return InputCode::Keypad4;
	case SDLK_KP_5				: return InputCode::Keypad5;
	case SDLK_KP_6				: return InputCode::Keypad6;
	case SDLK_KP_7				: return InputCode::Keypad7;
	case SDLK_KP_8				: return InputCode::Keypad8;
	case SDLK_KP_9				: return InputCode::Keypad9;
	case SDLK_KP_0				: return InputCode::Keypad0;
	case SDLK_KP_PERIOD			: return InputCode::KeypadPeriod;
	case SDLK_APPLICATION		: return InputCode::KeyApplication;
	case SDLK_POWER				: return InputCode::KeyPower;
	case SDLK_KP_EQUALS			: return InputCode::KeypadEquals;
	case SDLK_F13				: return InputCode::KeyF13;
	case SDLK_F14				: return InputCode::KeyF14;
	case SDLK_F15				: return InputCode::KeyF15;
	case SDLK_F16				: return InputCode::KeyF16;
	case SDLK_F17				: return InputCode::KeyF17;
	case SDLK_F18				: return InputCode::KeyF18;
	case SDLK_F19				: return InputCode::KeyF19;
	case SDLK_F20				: return InputCode::KeyF20;
	case SDLK_F21				: return InputCode::KeyF21;
	case SDLK_F22				: return InputCode::KeyF22;
	case SDLK_F23				: return InputCode::KeyF23;
	case SDLK_F24				: return InputCode::KeyF24;
	case SDLK_EXECUTE			: return InputCode::KeyExecute;
	case SDLK_HELP				: return InputCode::KeyHelp;
	case SDLK_MENU				: return InputCode::KeyMenu;
	case SDLK_SELECT			: return InputCode::KeySelect;
	case SDLK_STOP				: return InputCode::KeyStop;
	case SDLK_AGAIN				: return InputCode::KeyAgain;
	case SDLK_UNDO				: return InputCode::KeyUndo;
	case SDLK_CUT				: return InputCode::KeyCut;
	case SDLK_COPY				: return InputCode::KeyCopy;
	case SDLK_PASTE				: return InputCode::KeyPaste;
	case SDLK_FIND				: return InputCode::KeyFind;
	case SDLK_MUTE				: return InputCode::KeyMute;
	case SDLK_VOLUMEUP			: return InputCode::KeyVolumeup;
	case SDLK_VOLUMEDOWN		: return InputCode::KeyVolumedown;
	case SDLK_KP_COMMA			: return InputCode::KeypadComma;
	case SDLK_KP_EQUALSAS400	: return InputCode::KeypadEqualsas400;
	case SDLK_ALTERASE			: return InputCode::KeyAlterase;
	case SDLK_SYSREQ			: return InputCode::KeySysreq;
	case SDLK_CANCEL			: return InputCode::KeyCancel;
	case SDLK_CLEAR				: return InputCode::KeyClear;
	case SDLK_PRIOR				: return InputCode::KeyPrior;
	case SDLK_RETURN2			: return InputCode::KeyReturn2;
	case SDLK_SEPARATOR			: return InputCode::KeySeparator;
	case SDLK_OUT				: return InputCode::KeyOut;
	case SDLK_OPER				: return InputCode::KeyOper;
	case SDLK_CLEARAGAIN		: return InputCode::KeyClearagain;
	case SDLK_CRSEL				: return InputCode::KeyCrsel;
	case SDLK_EXSEL				: return InputCode::KeyExsel;
	case SDLK_KP_00				: return InputCode::Keypad00;
	case SDLK_KP_000			: return InputCode::Keypad000;
	case SDLK_THOUSANDSSEPARATOR: return InputCode::KeyThousandsseparator;
	case SDLK_DECIMALSEPARATOR	: return InputCode::KeyDecimalseparator;
	case SDLK_CURRENCYUNIT		: return InputCode::KeyCurrencyunit;
	case SDLK_CURRENCYSUBUNIT	: return InputCode::KeyCurrencysubunit;
	case SDLK_KP_LEFTPAREN		: return InputCode::KeypadLeftparen;
	case SDLK_KP_RIGHTPAREN		: return InputCode::KeypadRightparen;
	case SDLK_KP_LEFTBRACE		: return InputCode::KeypadLeftbrace;
	case SDLK_KP_RIGHTBRACE		: return InputCode::KeypadRightbrace;
	case SDLK_KP_TAB			: return InputCode::KeypadTab;
	case SDLK_KP_BACKSPACE		: return InputCode::KeypadBackspace;
	case SDLK_KP_A				: return InputCode::KeypadA;
	case SDLK_KP_B				: return InputCode::KeypadB;
	case SDLK_KP_C				: return InputCode::KeypadC;
	case SDLK_KP_D				: return InputCode::KeypadD;
	case SDLK_KP_E				: return InputCode::KeypadE;
	case SDLK_KP_F				: return InputCode::KeypadF;
	case SDLK_KP_XOR			: return InputCode::KeypadXor;
	case SDLK_KP_POWER			: return InputCode::KeypadPower;
	case SDLK_KP_PERCENT		: return InputCode::KeypadPercent;
	case SDLK_KP_LESS			: return InputCode::KeypadLess;
	case SDLK_KP_GREATER		: return InputCode::KeypadGreater;
	case SDLK_KP_AMPERSAND		: return InputCode::KeypadAmpersand;
	case SDLK_KP_DBLAMPERSAND	: return InputCode::KeypadDblampersand;
	case SDLK_KP_VERTICALBAR	: return InputCode::KeypadVerticalbar;
	case SDLK_KP_DBLVERTICALBAR : return InputCode::KeypadDblverticalbar;
	case SDLK_KP_COLON			: return InputCode::KeypadColon;
	case SDLK_KP_HASH			: return InputCode::KeypadHash;
	case SDLK_KP_SPACE			: return InputCode::KeypadSpace;
	case SDLK_KP_AT				: return InputCode::KeypadAt;
	case SDLK_KP_EXCLAM			: return InputCode::KeypadExclam;
	case SDLK_KP_MEMSTORE		: return InputCode::KeypadMemstore;
	case SDLK_KP_MEMRECALL		: return InputCode::KeypadMemrecall;
	case SDLK_KP_MEMCLEAR		: return InputCode::KeypadMemclear;
	case SDLK_KP_MEMADD			: return InputCode::KeypadMemadd;
	case SDLK_KP_MEMSUBTRACT	: return InputCode::KeypadMemsubtract;
	case SDLK_KP_MEMMULTIPLY	: return InputCode::KeypadMemmultiply;
	case SDLK_KP_MEMDIVIDE		: return InputCode::KeypadMemdivide;
	case SDLK_KP_PLUSMINUS		: return InputCode::KeypadPlusminus;
	case SDLK_KP_CLEAR			: return InputCode::KeypadClear;
	case SDLK_KP_CLEARENTRY		: return InputCode::KeypadClearentry;
	case SDLK_KP_BINARY			: return InputCode::KeypadBinary;
	case SDLK_KP_OCTAL			: return InputCode::KeypadOctal;
	case SDLK_KP_DECIMAL		: return InputCode::KeypadDecimal;
	case SDLK_KP_HEXADECIMAL	: return InputCode::KeypadHexadecimal;
	case SDLK_LCTRL				: return InputCode::KeyLctrl;
	case SDLK_LSHIFT			: return InputCode::KeyLshift;
	case SDLK_LALT				: return InputCode::KeyLalt;
	case SDLK_LGUI				: return InputCode::KeyLgui;
	case SDLK_RCTRL				: return InputCode::KeyRctrl;
	case SDLK_RSHIFT			: return InputCode::KeyRshift;
	case SDLK_RALT				: return InputCode::KeyRalt;
	case SDLK_RGUI				: return InputCode::KeyRgui;
	case SDLK_MODE				: return InputCode::KeyMode;
	case SDLK_AUDIONEXT			: return InputCode::KeyAudionext;
	case SDLK_AUDIOPREV			: return InputCode::KeyAudioprev;
	case SDLK_AUDIOSTOP			: return InputCode::KeyAudiostop;
	case SDLK_AUDIOPLAY			: return InputCode::KeyAudioplay;
	case SDLK_AUDIOMUTE			: return InputCode::KeyAudiomute;
	case SDLK_MEDIASELECT		: return InputCode::KeyMediaselect;
	case SDLK_WWW				: return InputCode::KeyWww;
	case SDLK_MAIL				: return InputCode::KeyMail;
	case SDLK_CALCULATOR		: return InputCode::KeyCalculator;
	case SDLK_COMPUTER			: return InputCode::KeyComputer;
	case SDLK_AC_SEARCH			: return InputCode::KeyAcSearch;
	case SDLK_AC_HOME			: return InputCode::KeyAcHome;
	case SDLK_AC_BACK			: return InputCode::KeyAcBack;
	case SDLK_AC_FORWARD		: return InputCode::KeyAcForward;
	case SDLK_AC_STOP			: return InputCode::KeyAcStop;
	case SDLK_AC_REFRESH		: return InputCode::KeyAcRefresh;
	case SDLK_AC_BOOKMARKS		: return InputCode::KeyAcBookmarks;
	case SDLK_BRIGHTNESSDOWN	: return InputCode::KeyBrightnessdown;
	case SDLK_BRIGHTNESSUP		: return InputCode::KeyBrightnessup;
	case SDLK_DISPLAYSWITCH		: return InputCode::KeyDisplayswitch;
	case SDLK_KBDILLUMTOGGLE	: return InputCode::KeyKbdillumtoggle;
	case SDLK_KBDILLUMDOWN		: return InputCode::KeyKbdillumdown;
	case SDLK_KBDILLUMUP		: return InputCode::KeyKbdillumup;
	case SDLK_EJECT				: return InputCode::KeyEject;
	case SDLK_SLEEP				: return InputCode::KeySleep;
	case SDLK_APP1				: return InputCode::KeyApp1;
	case SDLK_APP2				: return InputCode::KeyApp2;
	case SDLK_AUDIOREWIND		: return InputCode::KeyAudiorewind;
	case SDLK_AUDIOFASTFORWARD	: return InputCode::KeyAudiofastforward;
	case SDLK_SOFTLEFT			: return InputCode::KeySoftleft;
	case SDLK_SOFTRIGHT			: return InputCode::KeySoftright;
	case SDLK_CALL				: return InputCode::KeyCall;
	case SDLK_ENDCALL			: return InputCode::KeyEndcall;
	default						: return InputCode::Unknown;
	}
}

static InputCode _toGamepadInputCode(SDL_GameControllerButton sdlGamepadButton) {
	switch (sdlGamepadButton) {
	case SDL_CONTROLLER_BUTTON_A			: return InputCode::ButtonA;
	case SDL_CONTROLLER_BUTTON_B			: return InputCode::ButtonB;
	case SDL_CONTROLLER_BUTTON_X			: return InputCode::ButtonX;
	case SDL_CONTROLLER_BUTTON_Y			: return InputCode::ButtonY;
	case SDL_CONTROLLER_BUTTON_BACK			: return InputCode::ButtonBack;
	case SDL_CONTROLLER_BUTTON_GUIDE		: return InputCode::ButtonGuide;
	case SDL_CONTROLLER_BUTTON_START		: return InputCode::ButtonStart;
	case SDL_CONTROLLER_BUTTON_LEFTSTICK	: return InputCode::ButtonLeftStick;
	case SDL_CONTROLLER_BUTTON_RIGHTSTICK	: return InputCode::ButtonRightStick;
	case SDL_CONTROLLER_BUTTON_LEFTSHOULDER : return InputCode::ButtonLeftShoulder;
	case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER: return InputCode::ButtonRightShoulder;
	case SDL_CONTROLLER_BUTTON_DPAD_UP		: return InputCode::ButtonDPadUp;
	case SDL_CONTROLLER_BUTTON_DPAD_DOWN	: return InputCode::ButtonDPadDown;
	case SDL_CONTROLLER_BUTTON_DPAD_LEFT	: return InputCode::ButtonDPadLeft;
	case SDL_CONTROLLER_BUTTON_DPAD_RIGHT	: return InputCode::ButtonDPadRight;
	case SDL_CONTROLLER_BUTTON_MISC1		: return InputCode::ButtonMisc;
	case SDL_CONTROLLER_BUTTON_PADDLE1		: return InputCode::ButtonPaddle1;
	case SDL_CONTROLLER_BUTTON_PADDLE2		: return InputCode::ButtonPaddle2;
	case SDL_CONTROLLER_BUTTON_PADDLE3		: return InputCode::ButtonPaddle3;
	case SDL_CONTROLLER_BUTTON_PADDLE4		: return InputCode::ButtonPaddle4;
	case SDL_CONTROLLER_BUTTON_TOUCHPAD		: return InputCode::ButtonTouchpad;
	default									: return InputCode::Unknown;
	}
}

static InputCode _toMouseInputCode(Uint8 sdlButton) {
	switch (sdlButton) {
	case SDL_BUTTON_LEFT  : return InputCode::LeftButton;
	case SDL_BUTTON_MIDDLE: return InputCode::MiddleButton;
	case SDL_BUTTON_RIGHT : return InputCode::RightButton;
	default				  : return InputCode::Unknown;
	}
}

static DeviceId _mouseDeviceId() {
	return click::mice()[0];
}

static DeviceId _keyboardDeviceId() {
	return click::keyboards()[0];
}

static std::optional<DeviceId> _gamepadDeviceId(SDL_JoystickID joystickId) {
	for (auto& gamepadDeviceId : click::gamepads()) {
		auto& gamepad = click::device(gamepadDeviceId);
		auto id = (SDL_JoystickID)(intptr_t)gamepad.customData;
		if (id == joystickId) {
			return gamepadDeviceId;
		}
	}
	return std::nullopt;
}

void initSdlBackend() {
	click::installKeyboard();
	click::installMouse();

	for (int i = 0; i < SDL_NumJoysticks(); i++) {
		if (SDL_IsGameController(i)) {
			SDL_GameController* gameController = SDL_GameControllerOpen(i);
			auto joystickId = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(gameController));
			click::installGamepad((void*)(intptr_t)(joystickId));
		}
	}
}

void shutdownSdlBackend() {
	click::uninstallKeyboard(click::keyboards()[0]);
	click::uninstallMouse(click::mice()[0]);
	for (auto& gamepadDeviceId : click::gamepads()) {
		auto& gamepad = click::device(gamepadDeviceId);
		auto joystickId = (SDL_JoystickID)(intptr_t)gamepad.customData;
		SDL_GameControllerClose(SDL_GameControllerFromInstanceID(joystickId));
		click::uninstallGamepad(gamepadDeviceId);
	}
}

void _processMouseButtonEvent(float axisValue, Uint8 button) {
	auto inputCode = _toMouseInputCode(button);
	auto value = ActionValue{.scalar = axisValue};
	click::processInputEvent({inputCode, _mouseDeviceId(), value});
}

void _processKeyboardButtonEvent(float axisValue, SDL_Keycode keyCode, Uint8 repeat) {
	if (repeat == 0) {
		auto key = _toKeyboardInputCode(keyCode);
		auto value = ActionValue{.scalar = axisValue};
		click::processInputEvent({key, _keyboardDeviceId(), value});
	}
}

void processSdlEvents(const SDL_Event& event) {
	switch (event.type) {
	case SDL_JOYDEVICEADDED: {
		SDL_GameController* gameController = SDL_GameControllerOpen(event.jdevice.which);
		auto joystickId = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(gameController));
		if (!_gamepadDeviceId(joystickId)) {
			click::installGamepad((void*)(intptr_t)(joystickId));
		}
		break;
	}
	case SDL_JOYDEVICEREMOVED: {
		auto deviceId = _gamepadDeviceId(event.jdevice.which);
		if (deviceId) {
			click::uninstallGamepad(*deviceId);
		}
		break;
	}
	case SDL_CONTROLLERBUTTONDOWN: {
		auto inputCode = _toGamepadInputCode(static_cast<SDL_GameControllerButton>(event.cbutton.button));
		processInputEvent({inputCode, *_gamepadDeviceId(event.cbutton.which), {.scalar = 1.0f}});
		break;
	}
	case SDL_CONTROLLERBUTTONUP: {
		auto inputCode = _toGamepadInputCode(static_cast<SDL_GameControllerButton>(event.cbutton.button));
		processInputEvent({inputCode, *_gamepadDeviceId(event.cbutton.which), {.scalar = 0.0f}});
		break;
	}
	case SDL_KEYDOWN: {
		_processKeyboardButtonEvent(1.0f, event.key.keysym.sym, event.key.repeat);
		break;
	}
	case SDL_KEYUP: {
		_processKeyboardButtonEvent(0.0f, event.key.keysym.sym, event.key.repeat);
		break;
	}
	case SDL_MOUSEBUTTONDOWN: {
		_processMouseButtonEvent(1.0f, event.button.button);
		break;
	}
	case SDL_MOUSEBUTTONUP: {
		_processMouseButtonEvent(0.0f, event.button.button);
		break;
	}
	}
}

void updateSdlBackend() {
	{
		auto x{0};
		auto y{0};
		auto xRel{0};
		auto yRel{0};
		SDL_GetMouseState(&x, &y);
		SDL_GetRelativeMouseState(&xRel, &yRel);
		click::updateMouse(_mouseDeviceId(), static_cast<float>(x), static_cast<float>(y), static_cast<float>(xRel), static_cast<float>(yRel));
	}

	for (auto& gamepadDeviceId : click::gamepads()) {
		auto& gamepad = click::device(gamepadDeviceId);
		auto id = (SDL_JoystickID)(intptr_t)gamepad.customData;
		auto gameController = SDL_GameControllerFromInstanceID(id);
		if (gameController) {
			auto x = SDL_GameControllerGetAxis(gameController, SDL_CONTROLLER_AXIS_LEFTX) / 32767.0f;
			auto y = SDL_GameControllerGetAxis(gameController, SDL_CONTROLLER_AXIS_LEFTY) / 32767.0f;
			click::updateGamepadAxis(gamepadDeviceId, InputCode::AxisLeftXY, {.vec2 = {x, y}});

			click::updateGamepadAxis(gamepadDeviceId, InputCode::AxisLeftX, ActionValue{.scalar = x});
			click::updateGamepadAxis(gamepadDeviceId, InputCode::AxisLeftY, ActionValue{.scalar = y});

			x = SDL_GameControllerGetAxis(gameController, SDL_CONTROLLER_AXIS_RIGHTX) / 32767.0f;
			y = SDL_GameControllerGetAxis(gameController, SDL_CONTROLLER_AXIS_RIGHTY) / 32767.0f;
			click::updateGamepadAxis(gamepadDeviceId, InputCode::AxisRightXY, {.vec2 = {x, y}});

			click::updateGamepadAxis(gamepadDeviceId, InputCode::AxisRightX, ActionValue{.scalar = x});
			click::updateGamepadAxis(gamepadDeviceId, InputCode::AxisRightY, ActionValue{.scalar = y});

			auto triggerLeft = SDL_GameControllerGetAxis(gameController, SDL_CONTROLLER_AXIS_TRIGGERLEFT) / 32767.0f;
			auto triggerRight = SDL_GameControllerGetAxis(gameController, SDL_CONTROLLER_AXIS_TRIGGERRIGHT) / 32767.0f;
			click::updateGamepadAxis(gamepadDeviceId, InputCode::AxisTriggerLeft, ActionValue{.scalar = triggerLeft});
			click::updateGamepadAxis(gamepadDeviceId, InputCode::AxisTriggerRight, ActionValue{.scalar = triggerRight});
		}
	}
}

} // namespace click
