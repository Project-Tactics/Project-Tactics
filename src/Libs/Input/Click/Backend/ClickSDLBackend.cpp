#include "ClickSDLBackend.h"

#include "../Click.h"

namespace click {

static Key _toKey(SDL_Keycode sdlKey) {
	switch (sdlKey) {
	case SDLK_UNKNOWN			: return Key::KeyUnknown;
	case SDLK_RETURN			: return Key::KeyReturn;
	case SDLK_ESCAPE			: return Key::KeyEscape;
	case SDLK_BACKSPACE			: return Key::KeyBackspace;
	case SDLK_TAB				: return Key::KeyTab;
	case SDLK_SPACE				: return Key::KeySpace;
	case SDLK_EXCLAIM			: return Key::KeyExclaim;
	case SDLK_QUOTEDBL			: return Key::KeyQuotedbl;
	case SDLK_HASH				: return Key::KeyHash;
	case SDLK_PERCENT			: return Key::KeyPercent;
	case SDLK_DOLLAR			: return Key::KeyDollar;
	case SDLK_AMPERSAND			: return Key::KeyAmpersand;
	case SDLK_QUOTE				: return Key::KeyQuote;
	case SDLK_LEFTPAREN			: return Key::KeyLeftparen;
	case SDLK_RIGHTPAREN		: return Key::KeyRightparen;
	case SDLK_ASTERISK			: return Key::KeyAsterisk;
	case SDLK_PLUS				: return Key::KeyPlus;
	case SDLK_COMMA				: return Key::KeyComma;
	case SDLK_MINUS				: return Key::KeyMinus;
	case SDLK_PERIOD			: return Key::KeyPeriod;
	case SDLK_SLASH				: return Key::KeySlash;
	case SDLK_0					: return Key::Key0;
	case SDLK_1					: return Key::Key1;
	case SDLK_2					: return Key::Key2;
	case SDLK_3					: return Key::Key3;
	case SDLK_4					: return Key::Key4;
	case SDLK_5					: return Key::Key5;
	case SDLK_6					: return Key::Key6;
	case SDLK_7					: return Key::Key7;
	case SDLK_8					: return Key::Key8;
	case SDLK_9					: return Key::Key9;
	case SDLK_COLON				: return Key::KeyColon;
	case SDLK_SEMICOLON			: return Key::KeySemicolon;
	case SDLK_LESS				: return Key::KeyLess;
	case SDLK_EQUALS			: return Key::KeyEquals;
	case SDLK_GREATER			: return Key::KeyGreater;
	case SDLK_QUESTION			: return Key::KeyQuestion;
	case SDLK_AT				: return Key::KeyAt;
	case SDLK_LEFTBRACKET		: return Key::KeyLeftbracket;
	case SDLK_BACKSLASH			: return Key::KeyBackslash;
	case SDLK_RIGHTBRACKET		: return Key::KeyRightbracket;
	case SDLK_CARET				: return Key::KeyCaret;
	case SDLK_UNDERSCORE		: return Key::KeyUnderscore;
	case SDLK_BACKQUOTE			: return Key::KeyBackquote;
	case SDLK_a					: return Key::KeyA;
	case SDLK_b					: return Key::KeyB;
	case SDLK_c					: return Key::KeyC;
	case SDLK_d					: return Key::KeyD;
	case SDLK_e					: return Key::KeyE;
	case SDLK_f					: return Key::KeyF;
	case SDLK_g					: return Key::KeyG;
	case SDLK_h					: return Key::KeyH;
	case SDLK_i					: return Key::KeyI;
	case SDLK_j					: return Key::KeyJ;
	case SDLK_k					: return Key::KeyK;
	case SDLK_l					: return Key::KeyL;
	case SDLK_m					: return Key::KeyM;
	case SDLK_n					: return Key::KeyN;
	case SDLK_o					: return Key::KeyO;
	case SDLK_p					: return Key::KeyP;
	case SDLK_q					: return Key::KeyQ;
	case SDLK_r					: return Key::KeyR;
	case SDLK_s					: return Key::KeyS;
	case SDLK_t					: return Key::KeyT;
	case SDLK_u					: return Key::KeyU;
	case SDLK_v					: return Key::KeyV;
	case SDLK_w					: return Key::KeyW;
	case SDLK_x					: return Key::KeyX;
	case SDLK_y					: return Key::KeyY;
	case SDLK_z					: return Key::KeyZ;
	case SDLK_CAPSLOCK			: return Key::KeyCapslock;
	case SDLK_F1				: return Key::KeyF1;
	case SDLK_F2				: return Key::KeyF2;
	case SDLK_F3				: return Key::KeyF3;
	case SDLK_F4				: return Key::KeyF4;
	case SDLK_F5				: return Key::KeyF5;
	case SDLK_F6				: return Key::KeyF6;
	case SDLK_F7				: return Key::KeyF7;
	case SDLK_F8				: return Key::KeyF8;
	case SDLK_F9				: return Key::KeyF9;
	case SDLK_F10				: return Key::KeyF10;
	case SDLK_F11				: return Key::KeyF11;
	case SDLK_F12				: return Key::KeyF12;
	case SDLK_PRINTSCREEN		: return Key::KeyPrintscreen;
	case SDLK_SCROLLLOCK		: return Key::KeyScrolllock;
	case SDLK_PAUSE				: return Key::KeyPause;
	case SDLK_INSERT			: return Key::KeyInsert;
	case SDLK_HOME				: return Key::KeyHome;
	case SDLK_PAGEUP			: return Key::KeyPageup;
	case SDLK_DELETE			: return Key::KeyDelete;
	case SDLK_END				: return Key::KeyEnd;
	case SDLK_PAGEDOWN			: return Key::KeyPagedown;
	case SDLK_RIGHT				: return Key::KeyRight;
	case SDLK_LEFT				: return Key::KeyLeft;
	case SDLK_DOWN				: return Key::KeyDown;
	case SDLK_UP				: return Key::KeyUp;
	case SDLK_NUMLOCKCLEAR		: return Key::KeyNumlockclear;
	case SDLK_KP_DIVIDE			: return Key::KeypadDivide;
	case SDLK_KP_MULTIPLY		: return Key::KeypadMultiply;
	case SDLK_KP_MINUS			: return Key::KeypadMinus;
	case SDLK_KP_PLUS			: return Key::KeypadPlus;
	case SDLK_KP_ENTER			: return Key::KeypadEnter;
	case SDLK_KP_1				: return Key::Keypad1;
	case SDLK_KP_2				: return Key::Keypad2;
	case SDLK_KP_3				: return Key::Keypad3;
	case SDLK_KP_4				: return Key::Keypad4;
	case SDLK_KP_5				: return Key::Keypad5;
	case SDLK_KP_6				: return Key::Keypad6;
	case SDLK_KP_7				: return Key::Keypad7;
	case SDLK_KP_8				: return Key::Keypad8;
	case SDLK_KP_9				: return Key::Keypad9;
	case SDLK_KP_0				: return Key::Keypad0;
	case SDLK_KP_PERIOD			: return Key::KeypadPeriod;
	case SDLK_APPLICATION		: return Key::KeyApplication;
	case SDLK_POWER				: return Key::KeyPower;
	case SDLK_KP_EQUALS			: return Key::KeypadEquals;
	case SDLK_F13				: return Key::KeyF13;
	case SDLK_F14				: return Key::KeyF14;
	case SDLK_F15				: return Key::KeyF15;
	case SDLK_F16				: return Key::KeyF16;
	case SDLK_F17				: return Key::KeyF17;
	case SDLK_F18				: return Key::KeyF18;
	case SDLK_F19				: return Key::KeyF19;
	case SDLK_F20				: return Key::KeyF20;
	case SDLK_F21				: return Key::KeyF21;
	case SDLK_F22				: return Key::KeyF22;
	case SDLK_F23				: return Key::KeyF23;
	case SDLK_F24				: return Key::KeyF24;
	case SDLK_EXECUTE			: return Key::KeyExecute;
	case SDLK_HELP				: return Key::KeyHelp;
	case SDLK_MENU				: return Key::KeyMenu;
	case SDLK_SELECT			: return Key::KeySelect;
	case SDLK_STOP				: return Key::KeyStop;
	case SDLK_AGAIN				: return Key::KeyAgain;
	case SDLK_UNDO				: return Key::KeyUndo;
	case SDLK_CUT				: return Key::KeyCut;
	case SDLK_COPY				: return Key::KeyCopy;
	case SDLK_PASTE				: return Key::KeyPaste;
	case SDLK_FIND				: return Key::KeyFind;
	case SDLK_MUTE				: return Key::KeyMute;
	case SDLK_VOLUMEUP			: return Key::KeyVolumeup;
	case SDLK_VOLUMEDOWN		: return Key::KeyVolumedown;
	case SDLK_KP_COMMA			: return Key::KeypadComma;
	case SDLK_KP_EQUALSAS400	: return Key::KeypadEqualsas400;
	case SDLK_ALTERASE			: return Key::KeyAlterase;
	case SDLK_SYSREQ			: return Key::KeySysreq;
	case SDLK_CANCEL			: return Key::KeyCancel;
	case SDLK_CLEAR				: return Key::KeyClear;
	case SDLK_PRIOR				: return Key::KeyPrior;
	case SDLK_RETURN2			: return Key::KeyReturn2;
	case SDLK_SEPARATOR			: return Key::KeySeparator;
	case SDLK_OUT				: return Key::KeyOut;
	case SDLK_OPER				: return Key::KeyOper;
	case SDLK_CLEARAGAIN		: return Key::KeyClearagain;
	case SDLK_CRSEL				: return Key::KeyCrsel;
	case SDLK_EXSEL				: return Key::KeyExsel;
	case SDLK_KP_00				: return Key::Keypad00;
	case SDLK_KP_000			: return Key::Keypad000;
	case SDLK_THOUSANDSSEPARATOR: return Key::KeyThousandsseparator;
	case SDLK_DECIMALSEPARATOR	: return Key::KeyDecimalseparator;
	case SDLK_CURRENCYUNIT		: return Key::KeyCurrencyunit;
	case SDLK_CURRENCYSUBUNIT	: return Key::KeyCurrencysubunit;
	case SDLK_KP_LEFTPAREN		: return Key::KeypadLeftparen;
	case SDLK_KP_RIGHTPAREN		: return Key::KeypadRightparen;
	case SDLK_KP_LEFTBRACE		: return Key::KeypadLeftbrace;
	case SDLK_KP_RIGHTBRACE		: return Key::KeypadRightbrace;
	case SDLK_KP_TAB			: return Key::KeypadTab;
	case SDLK_KP_BACKSPACE		: return Key::KeypadBackspace;
	case SDLK_KP_A				: return Key::KeypadA;
	case SDLK_KP_B				: return Key::KeypadB;
	case SDLK_KP_C				: return Key::KeypadC;
	case SDLK_KP_D				: return Key::KeypadD;
	case SDLK_KP_E				: return Key::KeypadE;
	case SDLK_KP_F				: return Key::KeypadF;
	case SDLK_KP_XOR			: return Key::KeypadXor;
	case SDLK_KP_POWER			: return Key::KeypadPower;
	case SDLK_KP_PERCENT		: return Key::KeypadPercent;
	case SDLK_KP_LESS			: return Key::KeypadLess;
	case SDLK_KP_GREATER		: return Key::KeypadGreater;
	case SDLK_KP_AMPERSAND		: return Key::KeypadAmpersand;
	case SDLK_KP_DBLAMPERSAND	: return Key::KeypadDblampersand;
	case SDLK_KP_VERTICALBAR	: return Key::KeypadVerticalbar;
	case SDLK_KP_DBLVERTICALBAR : return Key::KeypadDblverticalbar;
	case SDLK_KP_COLON			: return Key::KeypadColon;
	case SDLK_KP_HASH			: return Key::KeypadHash;
	case SDLK_KP_SPACE			: return Key::KeypadSpace;
	case SDLK_KP_AT				: return Key::KeypadAt;
	case SDLK_KP_EXCLAM			: return Key::KeypadExclam;
	case SDLK_KP_MEMSTORE		: return Key::KeypadMemstore;
	case SDLK_KP_MEMRECALL		: return Key::KeypadMemrecall;
	case SDLK_KP_MEMCLEAR		: return Key::KeypadMemclear;
	case SDLK_KP_MEMADD			: return Key::KeypadMemadd;
	case SDLK_KP_MEMSUBTRACT	: return Key::KeypadMemsubtract;
	case SDLK_KP_MEMMULTIPLY	: return Key::KeypadMemmultiply;
	case SDLK_KP_MEMDIVIDE		: return Key::KeypadMemdivide;
	case SDLK_KP_PLUSMINUS		: return Key::KeypadPlusminus;
	case SDLK_KP_CLEAR			: return Key::KeypadClear;
	case SDLK_KP_CLEARENTRY		: return Key::KeypadClearentry;
	case SDLK_KP_BINARY			: return Key::KeypadBinary;
	case SDLK_KP_OCTAL			: return Key::KeypadOctal;
	case SDLK_KP_DECIMAL		: return Key::KeypadDecimal;
	case SDLK_KP_HEXADECIMAL	: return Key::KeypadHexadecimal;
	case SDLK_LCTRL				: return Key::KeyLctrl;
	case SDLK_LSHIFT			: return Key::KeyLshift;
	case SDLK_LALT				: return Key::KeyLalt;
	case SDLK_LGUI				: return Key::KeyLgui;
	case SDLK_RCTRL				: return Key::KeyRctrl;
	case SDLK_RSHIFT			: return Key::KeyRshift;
	case SDLK_RALT				: return Key::KeyRalt;
	case SDLK_RGUI				: return Key::KeyRgui;
	case SDLK_MODE				: return Key::KeyMode;
	case SDLK_AUDIONEXT			: return Key::KeyAudionext;
	case SDLK_AUDIOPREV			: return Key::KeyAudioprev;
	case SDLK_AUDIOSTOP			: return Key::KeyAudiostop;
	case SDLK_AUDIOPLAY			: return Key::KeyAudioplay;
	case SDLK_AUDIOMUTE			: return Key::KeyAudiomute;
	case SDLK_MEDIASELECT		: return Key::KeyMediaselect;
	case SDLK_WWW				: return Key::KeyWww;
	case SDLK_MAIL				: return Key::KeyMail;
	case SDLK_CALCULATOR		: return Key::KeyCalculator;
	case SDLK_COMPUTER			: return Key::KeyComputer;
	case SDLK_AC_SEARCH			: return Key::KeyAcSearch;
	case SDLK_AC_HOME			: return Key::KeyAcHome;
	case SDLK_AC_BACK			: return Key::KeyAcBack;
	case SDLK_AC_FORWARD		: return Key::KeyAcForward;
	case SDLK_AC_STOP			: return Key::KeyAcStop;
	case SDLK_AC_REFRESH		: return Key::KeyAcRefresh;
	case SDLK_AC_BOOKMARKS		: return Key::KeyAcBookmarks;
	case SDLK_BRIGHTNESSDOWN	: return Key::KeyBrightnessdown;
	case SDLK_BRIGHTNESSUP		: return Key::KeyBrightnessup;
	case SDLK_DISPLAYSWITCH		: return Key::KeyDisplayswitch;
	case SDLK_KBDILLUMTOGGLE	: return Key::KeyKbdillumtoggle;
	case SDLK_KBDILLUMDOWN		: return Key::KeyKbdillumdown;
	case SDLK_KBDILLUMUP		: return Key::KeyKbdillumup;
	case SDLK_EJECT				: return Key::KeyEject;
	case SDLK_SLEEP				: return Key::KeySleep;
	case SDLK_APP1				: return Key::KeyApp1;
	case SDLK_APP2				: return Key::KeyApp2;
	case SDLK_AUDIOREWIND		: return Key::KeyAudiorewind;
	case SDLK_AUDIOFASTFORWARD	: return Key::KeyAudiofastforward;
	case SDLK_SOFTLEFT			: return Key::KeySoftleft;
	case SDLK_SOFTRIGHT			: return Key::KeySoftright;
	case SDLK_CALL				: return Key::KeyCall;
	case SDLK_ENDCALL			: return Key::KeyEndcall;
	default						: return Key::KeyUnknown;
	}
}

static GamepadGesture _toGamepadGesture(SDL_GameControllerButton sdlGamepadButton) {
	switch (sdlGamepadButton) {
	case SDL_CONTROLLER_BUTTON_A			: return GamepadGesture::ButtonA;
	case SDL_CONTROLLER_BUTTON_B			: return GamepadGesture::ButtonB;
	case SDL_CONTROLLER_BUTTON_X			: return GamepadGesture::ButtonX;
	case SDL_CONTROLLER_BUTTON_Y			: return GamepadGesture::ButtonY;
	case SDL_CONTROLLER_BUTTON_BACK			: return GamepadGesture::ButtonBack;
	case SDL_CONTROLLER_BUTTON_GUIDE		: return GamepadGesture::ButtonGuide;
	case SDL_CONTROLLER_BUTTON_START		: return GamepadGesture::ButtonStart;
	case SDL_CONTROLLER_BUTTON_LEFTSTICK	: return GamepadGesture::ButtonLeftStick;
	case SDL_CONTROLLER_BUTTON_RIGHTSTICK	: return GamepadGesture::ButtonRightStick;
	case SDL_CONTROLLER_BUTTON_LEFTSHOULDER : return GamepadGesture::ButtonLeftShoulder;
	case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER: return GamepadGesture::ButtonRightShoulder;
	case SDL_CONTROLLER_BUTTON_DPAD_UP		: return GamepadGesture::ButtonDPadUp;
	case SDL_CONTROLLER_BUTTON_DPAD_DOWN	: return GamepadGesture::ButtonDPadDown;
	case SDL_CONTROLLER_BUTTON_DPAD_LEFT	: return GamepadGesture::ButtonDPadLeft;
	case SDL_CONTROLLER_BUTTON_DPAD_RIGHT	: return GamepadGesture::ButtonDPadRight;
	case SDL_CONTROLLER_BUTTON_MISC1		: return GamepadGesture::ButtonMisc;
	case SDL_CONTROLLER_BUTTON_PADDLE1		: return GamepadGesture::ButtonPaddle1;
	case SDL_CONTROLLER_BUTTON_PADDLE2		: return GamepadGesture::ButtonPaddle2;
	case SDL_CONTROLLER_BUTTON_PADDLE3		: return GamepadGesture::ButtonPaddle3;
	case SDL_CONTROLLER_BUTTON_PADDLE4		: return GamepadGesture::ButtonPaddle4;
	case SDL_CONTROLLER_BUTTON_TOUCHPAD		: return GamepadGesture::ButtonTouchpad;
	default									: return GamepadGesture::Unknown;
	}
}

static GamepadGesture _toGamepadGesture(SDL_GameControllerAxis sdlAxis) {
	switch (sdlAxis) {
	case SDL_CONTROLLER_AXIS_LEFTX		 : return GamepadGesture::AxisLeftX;
	case SDL_CONTROLLER_AXIS_LEFTY		 : return GamepadGesture::AxisLeftY;
	case SDL_CONTROLLER_AXIS_RIGHTX		 : return GamepadGesture::AxisRightX;
	case SDL_CONTROLLER_AXIS_RIGHTY		 : return GamepadGesture::AxisRightY;
	case SDL_CONTROLLER_AXIS_TRIGGERLEFT : return GamepadGesture::AxisTriggerLeft;
	case SDL_CONTROLLER_AXIS_TRIGGERRIGHT: return GamepadGesture::AxisTriggerRight;
	default								 : return GamepadGesture::Unknown;
	}
}

static MouseGesture _toMouseGesture(Uint8 sdlButton) {
	switch (sdlButton) {
	case SDL_BUTTON_LEFT  : return MouseGesture::LeftButton;
	case SDL_BUTTON_MIDDLE: return MouseGesture::MiddleButton;
	case SDL_BUTTON_RIGHT : return MouseGesture::RightButton;
	default				  : return MouseGesture::Unknown;
	}
}

static DeviceId _mouseDeviceId() {
	return click::mice()[0];
}

static DeviceId _keyboardDeviceId() {
	return click::keyboards()[0];
}

static DeviceId _gamepadDeviceId(int sdlGamepadId) {
	for (auto& gamepadDeviceId : click::gamepads()) {
		auto& gamepad = click::device(gamepadDeviceId);
		auto joystickId = (SDL_JoystickID)(intptr_t)gamepad.customData;
		if (joystickId == sdlGamepadId) {
			return gamepadDeviceId;
		}
	}
	return 0;
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
	auto mouseGesture = _toMouseGesture(button);
	auto value = ActionValue{.axis1D = axisValue};
	processMouseEvent({mouseGesture, _mouseDeviceId(), value});
}

void _processKeyboardButtonEvent(float axisValue, SDL_Keycode keyCode, Uint8 repeat) {
	if (repeat == 0) {
		auto key = _toKey(keyCode);
		auto value = ActionValue{.axis1D = axisValue};
		processKeyboardEvent({key, _keyboardDeviceId(), value});
	}
}

void _processMouseMotionEvent(float xRel, float yRel) {
	auto mouseGesture = MouseGesture::MouseXY;
	auto axis2D = ActionValue{.axis2D = {xRel, yRel}};
	processMouseEvent({mouseGesture, _mouseDeviceId(), axis2D});

	if (xRel != 0.0f) {
		auto axis1D = ActionValue{.axis1D = {xRel}};
		mouseGesture = MouseGesture::MouseX;
		processMouseEvent({mouseGesture, _mouseDeviceId(), axis1D});
	}

	if (yRel != 0.0f) {
		auto axis1D = ActionValue{.axis1D = {yRel}};
		mouseGesture = MouseGesture::MouseY;
		processMouseEvent({mouseGesture, _mouseDeviceId(), axis1D});
	}
}

void processSdlEvents(SDL_Event& event) {
	switch (event.type) {
	case SDL_JOYDEVICEADDED: {
		SDL_GameController* gameController = SDL_GameControllerOpen(event.jdevice.which);
		auto joystickId = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(gameController));
		click::installGamepad((void*)(intptr_t)(joystickId));
		break;
	}
	case SDL_JOYDEVICEREMOVED: {
		for (auto& gamepadDeviceId : click::gamepads()) {
			auto& gamepad = click::device(gamepadDeviceId);
			auto joystickId = (SDL_JoystickID)(intptr_t)gamepad.customData;
			if (joystickId == event.jdevice.which) {
				click::uninstallGamepad(gamepadDeviceId);
				break;
			}
		}
		break;
	}
	case SDL_CONTROLLERBUTTONDOWN: {
		auto gamepadGesture = _toGamepadGesture(static_cast<SDL_GameControllerButton>(event.cbutton.button));
		processGamepadEvent({gamepadGesture, _gamepadDeviceId(event.cbutton.which), {.axis1D = 1.0f}});
		break;
	}
	case SDL_CONTROLLERBUTTONUP: {
		auto gamepadGesture = _toGamepadGesture(static_cast<SDL_GameControllerButton>(event.cbutton.button));
		processGamepadEvent({gamepadGesture, _gamepadDeviceId(event.cbutton.which), {.axis1D = 0.0f}});
		break;
	}
	case SDL_CONTROLLERAXISMOTION: {
		auto gamepadGesture = _toGamepadGesture(static_cast<SDL_GameControllerAxis>(event.caxis.axis));
		auto value = ActionValue{.axis1D = static_cast<float>(event.caxis.value) / 32767.0f};
		processGamepadEvent({gamepadGesture, _gamepadDeviceId(event.caxis.which), value});
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
	case SDL_MOUSEMOTION: {
		_processMouseMotionEvent(static_cast<float>(event.motion.xrel), static_cast<float>(event.motion.yrel));
	} break;
	}
}

} // namespace click
