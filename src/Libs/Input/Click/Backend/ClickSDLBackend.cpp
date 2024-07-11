#include "ClickSDLBackend.h"

#include "../Click.h"

#include <optional>

namespace click {

static Gesture _toKey(SDL_Keycode sdlKey) {
	switch (sdlKey) {
	case SDLK_UNKNOWN			: return Gesture::Unknown;
	case SDLK_RETURN			: return Gesture::KeyReturn;
	case SDLK_ESCAPE			: return Gesture::KeyEscape;
	case SDLK_BACKSPACE			: return Gesture::KeyBackspace;
	case SDLK_TAB				: return Gesture::KeyTab;
	case SDLK_SPACE				: return Gesture::KeySpace;
	case SDLK_EXCLAIM			: return Gesture::KeyExclaim;
	case SDLK_QUOTEDBL			: return Gesture::KeyQuotedbl;
	case SDLK_HASH				: return Gesture::KeyHash;
	case SDLK_PERCENT			: return Gesture::KeyPercent;
	case SDLK_DOLLAR			: return Gesture::KeyDollar;
	case SDLK_AMPERSAND			: return Gesture::KeyAmpersand;
	case SDLK_QUOTE				: return Gesture::KeyQuote;
	case SDLK_LEFTPAREN			: return Gesture::KeyLeftparen;
	case SDLK_RIGHTPAREN		: return Gesture::KeyRightparen;
	case SDLK_ASTERISK			: return Gesture::KeyAsterisk;
	case SDLK_PLUS				: return Gesture::KeyPlus;
	case SDLK_COMMA				: return Gesture::KeyComma;
	case SDLK_MINUS				: return Gesture::KeyMinus;
	case SDLK_PERIOD			: return Gesture::KeyPeriod;
	case SDLK_SLASH				: return Gesture::KeySlash;
	case SDLK_0					: return Gesture::Key0;
	case SDLK_1					: return Gesture::Key1;
	case SDLK_2					: return Gesture::Key2;
	case SDLK_3					: return Gesture::Key3;
	case SDLK_4					: return Gesture::Key4;
	case SDLK_5					: return Gesture::Key5;
	case SDLK_6					: return Gesture::Key6;
	case SDLK_7					: return Gesture::Key7;
	case SDLK_8					: return Gesture::Key8;
	case SDLK_9					: return Gesture::Key9;
	case SDLK_COLON				: return Gesture::KeyColon;
	case SDLK_SEMICOLON			: return Gesture::KeySemicolon;
	case SDLK_LESS				: return Gesture::KeyLess;
	case SDLK_EQUALS			: return Gesture::KeyEquals;
	case SDLK_GREATER			: return Gesture::KeyGreater;
	case SDLK_QUESTION			: return Gesture::KeyQuestion;
	case SDLK_AT				: return Gesture::KeyAt;
	case SDLK_LEFTBRACKET		: return Gesture::KeyLeftbracket;
	case SDLK_BACKSLASH			: return Gesture::KeyBackslash;
	case SDLK_RIGHTBRACKET		: return Gesture::KeyRightbracket;
	case SDLK_CARET				: return Gesture::KeyCaret;
	case SDLK_UNDERSCORE		: return Gesture::KeyUnderscore;
	case SDLK_BACKQUOTE			: return Gesture::KeyBackquote;
	case SDLK_a					: return Gesture::KeyA;
	case SDLK_b					: return Gesture::KeyB;
	case SDLK_c					: return Gesture::KeyC;
	case SDLK_d					: return Gesture::KeyD;
	case SDLK_e					: return Gesture::KeyE;
	case SDLK_f					: return Gesture::KeyF;
	case SDLK_g					: return Gesture::KeyG;
	case SDLK_h					: return Gesture::KeyH;
	case SDLK_i					: return Gesture::KeyI;
	case SDLK_j					: return Gesture::KeyJ;
	case SDLK_k					: return Gesture::KeyK;
	case SDLK_l					: return Gesture::KeyL;
	case SDLK_m					: return Gesture::KeyM;
	case SDLK_n					: return Gesture::KeyN;
	case SDLK_o					: return Gesture::KeyO;
	case SDLK_p					: return Gesture::KeyP;
	case SDLK_q					: return Gesture::KeyQ;
	case SDLK_r					: return Gesture::KeyR;
	case SDLK_s					: return Gesture::KeyS;
	case SDLK_t					: return Gesture::KeyT;
	case SDLK_u					: return Gesture::KeyU;
	case SDLK_v					: return Gesture::KeyV;
	case SDLK_w					: return Gesture::KeyW;
	case SDLK_x					: return Gesture::KeyX;
	case SDLK_y					: return Gesture::KeyY;
	case SDLK_z					: return Gesture::KeyZ;
	case SDLK_CAPSLOCK			: return Gesture::KeyCapslock;
	case SDLK_F1				: return Gesture::KeyF1;
	case SDLK_F2				: return Gesture::KeyF2;
	case SDLK_F3				: return Gesture::KeyF3;
	case SDLK_F4				: return Gesture::KeyF4;
	case SDLK_F5				: return Gesture::KeyF5;
	case SDLK_F6				: return Gesture::KeyF6;
	case SDLK_F7				: return Gesture::KeyF7;
	case SDLK_F8				: return Gesture::KeyF8;
	case SDLK_F9				: return Gesture::KeyF9;
	case SDLK_F10				: return Gesture::KeyF10;
	case SDLK_F11				: return Gesture::KeyF11;
	case SDLK_F12				: return Gesture::KeyF12;
	case SDLK_PRINTSCREEN		: return Gesture::KeyPrintscreen;
	case SDLK_SCROLLLOCK		: return Gesture::KeyScrolllock;
	case SDLK_PAUSE				: return Gesture::KeyPause;
	case SDLK_INSERT			: return Gesture::KeyInsert;
	case SDLK_HOME				: return Gesture::KeyHome;
	case SDLK_PAGEUP			: return Gesture::KeyPageup;
	case SDLK_DELETE			: return Gesture::KeyDelete;
	case SDLK_END				: return Gesture::KeyEnd;
	case SDLK_PAGEDOWN			: return Gesture::KeyPagedown;
	case SDLK_RIGHT				: return Gesture::KeyRight;
	case SDLK_LEFT				: return Gesture::KeyLeft;
	case SDLK_DOWN				: return Gesture::KeyDown;
	case SDLK_UP				: return Gesture::KeyUp;
	case SDLK_NUMLOCKCLEAR		: return Gesture::KeyNumlockclear;
	case SDLK_KP_DIVIDE			: return Gesture::KeypadDivide;
	case SDLK_KP_MULTIPLY		: return Gesture::KeypadMultiply;
	case SDLK_KP_MINUS			: return Gesture::KeypadMinus;
	case SDLK_KP_PLUS			: return Gesture::KeypadPlus;
	case SDLK_KP_ENTER			: return Gesture::KeypadEnter;
	case SDLK_KP_1				: return Gesture::Keypad1;
	case SDLK_KP_2				: return Gesture::Keypad2;
	case SDLK_KP_3				: return Gesture::Keypad3;
	case SDLK_KP_4				: return Gesture::Keypad4;
	case SDLK_KP_5				: return Gesture::Keypad5;
	case SDLK_KP_6				: return Gesture::Keypad6;
	case SDLK_KP_7				: return Gesture::Keypad7;
	case SDLK_KP_8				: return Gesture::Keypad8;
	case SDLK_KP_9				: return Gesture::Keypad9;
	case SDLK_KP_0				: return Gesture::Keypad0;
	case SDLK_KP_PERIOD			: return Gesture::KeypadPeriod;
	case SDLK_APPLICATION		: return Gesture::KeyApplication;
	case SDLK_POWER				: return Gesture::KeyPower;
	case SDLK_KP_EQUALS			: return Gesture::KeypadEquals;
	case SDLK_F13				: return Gesture::KeyF13;
	case SDLK_F14				: return Gesture::KeyF14;
	case SDLK_F15				: return Gesture::KeyF15;
	case SDLK_F16				: return Gesture::KeyF16;
	case SDLK_F17				: return Gesture::KeyF17;
	case SDLK_F18				: return Gesture::KeyF18;
	case SDLK_F19				: return Gesture::KeyF19;
	case SDLK_F20				: return Gesture::KeyF20;
	case SDLK_F21				: return Gesture::KeyF21;
	case SDLK_F22				: return Gesture::KeyF22;
	case SDLK_F23				: return Gesture::KeyF23;
	case SDLK_F24				: return Gesture::KeyF24;
	case SDLK_EXECUTE			: return Gesture::KeyExecute;
	case SDLK_HELP				: return Gesture::KeyHelp;
	case SDLK_MENU				: return Gesture::KeyMenu;
	case SDLK_SELECT			: return Gesture::KeySelect;
	case SDLK_STOP				: return Gesture::KeyStop;
	case SDLK_AGAIN				: return Gesture::KeyAgain;
	case SDLK_UNDO				: return Gesture::KeyUndo;
	case SDLK_CUT				: return Gesture::KeyCut;
	case SDLK_COPY				: return Gesture::KeyCopy;
	case SDLK_PASTE				: return Gesture::KeyPaste;
	case SDLK_FIND				: return Gesture::KeyFind;
	case SDLK_MUTE				: return Gesture::KeyMute;
	case SDLK_VOLUMEUP			: return Gesture::KeyVolumeup;
	case SDLK_VOLUMEDOWN		: return Gesture::KeyVolumedown;
	case SDLK_KP_COMMA			: return Gesture::KeypadComma;
	case SDLK_KP_EQUALSAS400	: return Gesture::KeypadEqualsas400;
	case SDLK_ALTERASE			: return Gesture::KeyAlterase;
	case SDLK_SYSREQ			: return Gesture::KeySysreq;
	case SDLK_CANCEL			: return Gesture::KeyCancel;
	case SDLK_CLEAR				: return Gesture::KeyClear;
	case SDLK_PRIOR				: return Gesture::KeyPrior;
	case SDLK_RETURN2			: return Gesture::KeyReturn2;
	case SDLK_SEPARATOR			: return Gesture::KeySeparator;
	case SDLK_OUT				: return Gesture::KeyOut;
	case SDLK_OPER				: return Gesture::KeyOper;
	case SDLK_CLEARAGAIN		: return Gesture::KeyClearagain;
	case SDLK_CRSEL				: return Gesture::KeyCrsel;
	case SDLK_EXSEL				: return Gesture::KeyExsel;
	case SDLK_KP_00				: return Gesture::Keypad00;
	case SDLK_KP_000			: return Gesture::Keypad000;
	case SDLK_THOUSANDSSEPARATOR: return Gesture::KeyThousandsseparator;
	case SDLK_DECIMALSEPARATOR	: return Gesture::KeyDecimalseparator;
	case SDLK_CURRENCYUNIT		: return Gesture::KeyCurrencyunit;
	case SDLK_CURRENCYSUBUNIT	: return Gesture::KeyCurrencysubunit;
	case SDLK_KP_LEFTPAREN		: return Gesture::KeypadLeftparen;
	case SDLK_KP_RIGHTPAREN		: return Gesture::KeypadRightparen;
	case SDLK_KP_LEFTBRACE		: return Gesture::KeypadLeftbrace;
	case SDLK_KP_RIGHTBRACE		: return Gesture::KeypadRightbrace;
	case SDLK_KP_TAB			: return Gesture::KeypadTab;
	case SDLK_KP_BACKSPACE		: return Gesture::KeypadBackspace;
	case SDLK_KP_A				: return Gesture::KeypadA;
	case SDLK_KP_B				: return Gesture::KeypadB;
	case SDLK_KP_C				: return Gesture::KeypadC;
	case SDLK_KP_D				: return Gesture::KeypadD;
	case SDLK_KP_E				: return Gesture::KeypadE;
	case SDLK_KP_F				: return Gesture::KeypadF;
	case SDLK_KP_XOR			: return Gesture::KeypadXor;
	case SDLK_KP_POWER			: return Gesture::KeypadPower;
	case SDLK_KP_PERCENT		: return Gesture::KeypadPercent;
	case SDLK_KP_LESS			: return Gesture::KeypadLess;
	case SDLK_KP_GREATER		: return Gesture::KeypadGreater;
	case SDLK_KP_AMPERSAND		: return Gesture::KeypadAmpersand;
	case SDLK_KP_DBLAMPERSAND	: return Gesture::KeypadDblampersand;
	case SDLK_KP_VERTICALBAR	: return Gesture::KeypadVerticalbar;
	case SDLK_KP_DBLVERTICALBAR : return Gesture::KeypadDblverticalbar;
	case SDLK_KP_COLON			: return Gesture::KeypadColon;
	case SDLK_KP_HASH			: return Gesture::KeypadHash;
	case SDLK_KP_SPACE			: return Gesture::KeypadSpace;
	case SDLK_KP_AT				: return Gesture::KeypadAt;
	case SDLK_KP_EXCLAM			: return Gesture::KeypadExclam;
	case SDLK_KP_MEMSTORE		: return Gesture::KeypadMemstore;
	case SDLK_KP_MEMRECALL		: return Gesture::KeypadMemrecall;
	case SDLK_KP_MEMCLEAR		: return Gesture::KeypadMemclear;
	case SDLK_KP_MEMADD			: return Gesture::KeypadMemadd;
	case SDLK_KP_MEMSUBTRACT	: return Gesture::KeypadMemsubtract;
	case SDLK_KP_MEMMULTIPLY	: return Gesture::KeypadMemmultiply;
	case SDLK_KP_MEMDIVIDE		: return Gesture::KeypadMemdivide;
	case SDLK_KP_PLUSMINUS		: return Gesture::KeypadPlusminus;
	case SDLK_KP_CLEAR			: return Gesture::KeypadClear;
	case SDLK_KP_CLEARENTRY		: return Gesture::KeypadClearentry;
	case SDLK_KP_BINARY			: return Gesture::KeypadBinary;
	case SDLK_KP_OCTAL			: return Gesture::KeypadOctal;
	case SDLK_KP_DECIMAL		: return Gesture::KeypadDecimal;
	case SDLK_KP_HEXADECIMAL	: return Gesture::KeypadHexadecimal;
	case SDLK_LCTRL				: return Gesture::KeyLctrl;
	case SDLK_LSHIFT			: return Gesture::KeyLshift;
	case SDLK_LALT				: return Gesture::KeyLalt;
	case SDLK_LGUI				: return Gesture::KeyLgui;
	case SDLK_RCTRL				: return Gesture::KeyRctrl;
	case SDLK_RSHIFT			: return Gesture::KeyRshift;
	case SDLK_RALT				: return Gesture::KeyRalt;
	case SDLK_RGUI				: return Gesture::KeyRgui;
	case SDLK_MODE				: return Gesture::KeyMode;
	case SDLK_AUDIONEXT			: return Gesture::KeyAudionext;
	case SDLK_AUDIOPREV			: return Gesture::KeyAudioprev;
	case SDLK_AUDIOSTOP			: return Gesture::KeyAudiostop;
	case SDLK_AUDIOPLAY			: return Gesture::KeyAudioplay;
	case SDLK_AUDIOMUTE			: return Gesture::KeyAudiomute;
	case SDLK_MEDIASELECT		: return Gesture::KeyMediaselect;
	case SDLK_WWW				: return Gesture::KeyWww;
	case SDLK_MAIL				: return Gesture::KeyMail;
	case SDLK_CALCULATOR		: return Gesture::KeyCalculator;
	case SDLK_COMPUTER			: return Gesture::KeyComputer;
	case SDLK_AC_SEARCH			: return Gesture::KeyAcSearch;
	case SDLK_AC_HOME			: return Gesture::KeyAcHome;
	case SDLK_AC_BACK			: return Gesture::KeyAcBack;
	case SDLK_AC_FORWARD		: return Gesture::KeyAcForward;
	case SDLK_AC_STOP			: return Gesture::KeyAcStop;
	case SDLK_AC_REFRESH		: return Gesture::KeyAcRefresh;
	case SDLK_AC_BOOKMARKS		: return Gesture::KeyAcBookmarks;
	case SDLK_BRIGHTNESSDOWN	: return Gesture::KeyBrightnessdown;
	case SDLK_BRIGHTNESSUP		: return Gesture::KeyBrightnessup;
	case SDLK_DISPLAYSWITCH		: return Gesture::KeyDisplayswitch;
	case SDLK_KBDILLUMTOGGLE	: return Gesture::KeyKbdillumtoggle;
	case SDLK_KBDILLUMDOWN		: return Gesture::KeyKbdillumdown;
	case SDLK_KBDILLUMUP		: return Gesture::KeyKbdillumup;
	case SDLK_EJECT				: return Gesture::KeyEject;
	case SDLK_SLEEP				: return Gesture::KeySleep;
	case SDLK_APP1				: return Gesture::KeyApp1;
	case SDLK_APP2				: return Gesture::KeyApp2;
	case SDLK_AUDIOREWIND		: return Gesture::KeyAudiorewind;
	case SDLK_AUDIOFASTFORWARD	: return Gesture::KeyAudiofastforward;
	case SDLK_SOFTLEFT			: return Gesture::KeySoftleft;
	case SDLK_SOFTRIGHT			: return Gesture::KeySoftright;
	case SDLK_CALL				: return Gesture::KeyCall;
	case SDLK_ENDCALL			: return Gesture::KeyEndcall;
	default						: return Gesture::Unknown;
	}
}

static Gesture _toGamepadGesture(SDL_GameControllerButton sdlGamepadButton) {
	switch (sdlGamepadButton) {
	case SDL_CONTROLLER_BUTTON_A			: return Gesture::ButtonA;
	case SDL_CONTROLLER_BUTTON_B			: return Gesture::ButtonB;
	case SDL_CONTROLLER_BUTTON_X			: return Gesture::ButtonX;
	case SDL_CONTROLLER_BUTTON_Y			: return Gesture::ButtonY;
	case SDL_CONTROLLER_BUTTON_BACK			: return Gesture::ButtonBack;
	case SDL_CONTROLLER_BUTTON_GUIDE		: return Gesture::ButtonGuide;
	case SDL_CONTROLLER_BUTTON_START		: return Gesture::ButtonStart;
	case SDL_CONTROLLER_BUTTON_LEFTSTICK	: return Gesture::ButtonLeftStick;
	case SDL_CONTROLLER_BUTTON_RIGHTSTICK	: return Gesture::ButtonRightStick;
	case SDL_CONTROLLER_BUTTON_LEFTSHOULDER : return Gesture::ButtonLeftShoulder;
	case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER: return Gesture::ButtonRightShoulder;
	case SDL_CONTROLLER_BUTTON_DPAD_UP		: return Gesture::ButtonDPadUp;
	case SDL_CONTROLLER_BUTTON_DPAD_DOWN	: return Gesture::ButtonDPadDown;
	case SDL_CONTROLLER_BUTTON_DPAD_LEFT	: return Gesture::ButtonDPadLeft;
	case SDL_CONTROLLER_BUTTON_DPAD_RIGHT	: return Gesture::ButtonDPadRight;
	case SDL_CONTROLLER_BUTTON_MISC1		: return Gesture::ButtonMisc;
	case SDL_CONTROLLER_BUTTON_PADDLE1		: return Gesture::ButtonPaddle1;
	case SDL_CONTROLLER_BUTTON_PADDLE2		: return Gesture::ButtonPaddle2;
	case SDL_CONTROLLER_BUTTON_PADDLE3		: return Gesture::ButtonPaddle3;
	case SDL_CONTROLLER_BUTTON_PADDLE4		: return Gesture::ButtonPaddle4;
	case SDL_CONTROLLER_BUTTON_TOUCHPAD		: return Gesture::ButtonTouchpad;
	default									: return Gesture::Unknown;
	}
}

/*
static Gesture _toGamepadGesture(SDL_GameControllerAxis sdlAxis) {
	switch (sdlAxis) {
	case SDL_CONTROLLER_AXIS_LEFTX		 : return Gesture::AxisLeftX;
	case SDL_CONTROLLER_AXIS_LEFTY		 : return Gesture::AxisLeftY;
	case SDL_CONTROLLER_AXIS_RIGHTX		 : return Gesture::AxisRightX;
	case SDL_CONTROLLER_AXIS_RIGHTY		 : return Gesture::AxisRightY;
	case SDL_CONTROLLER_AXIS_TRIGGERLEFT : return Gesture::AxisTriggerLeft;
	case SDL_CONTROLLER_AXIS_TRIGGERRIGHT: return Gesture::AxisTriggerRight;
	default								 : return Gesture::Unknown;
	}
}
*/

static Gesture _toMouseGesture(Uint8 sdlButton) {
	switch (sdlButton) {
	case SDL_BUTTON_LEFT  : return Gesture::LeftButton;
	case SDL_BUTTON_MIDDLE: return Gesture::MiddleButton;
	case SDL_BUTTON_RIGHT : return Gesture::RightButton;
	default				  : return Gesture::Unknown;
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
	auto mouseGesture = _toMouseGesture(button);
	auto value = ActionValue{.scalar = axisValue};
	click::processInputEvent({mouseGesture, _mouseDeviceId(), value});
}

void _processKeyboardButtonEvent(float axisValue, SDL_Keycode keyCode, Uint8 repeat) {
	if (repeat == 0) {
		auto key = _toKey(keyCode);
		auto value = ActionValue{.scalar = axisValue};
		click::processInputEvent({key, _keyboardDeviceId(), value});
	}
}

void _processMouseMotionEvent(float xRel, float yRel) {
	auto mouseGesture = Gesture::MouseXY;
	auto axis2D = ActionValue{.vec2 = {xRel, yRel}};
	click::processInputEvent({mouseGesture, _mouseDeviceId(), axis2D});

	if (xRel != 0.0f) {
		auto axis1D = ActionValue{.scalar = {xRel}};
		mouseGesture = Gesture::MouseX;
		click::processInputEvent({mouseGesture, _mouseDeviceId(), axis1D});
	}

	if (yRel != 0.0f) {
		auto axis1D = ActionValue{.scalar = {yRel}};
		mouseGesture = Gesture::MouseY;
		click::processInputEvent({mouseGesture, _mouseDeviceId(), axis1D});
	}
}

void processSdlEvents(SDL_Event& event) {
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
		auto gamepadGesture = _toGamepadGesture(static_cast<SDL_GameControllerButton>(event.cbutton.button));
		processInputEvent({gamepadGesture, *_gamepadDeviceId(event.cbutton.which), {.scalar = 1.0f}});
		break;
	}
	case SDL_CONTROLLERBUTTONUP: {
		auto gamepadGesture = _toGamepadGesture(static_cast<SDL_GameControllerButton>(event.cbutton.button));
		processInputEvent({gamepadGesture, *_gamepadDeviceId(event.cbutton.which), {.scalar = 0.0f}});
		break;
	}
	case SDL_CONTROLLERAXISMOTION: {
		/*
		auto gamepadGesture = _toGamepadGesture(static_cast<SDL_GameControllerAxis>(event.caxis.axis));
		auto value = ActionValue{.axis1D = static_cast<float>(event.caxis.value) / 32767.0f};
		processInputEvent({gamepadGesture, *_gamepadDeviceId(event.caxis.which), value});*/
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
		//_processMouseMotionEvent(static_cast<float>(event.motion.xrel), static_cast<float>(event.motion.yrel));
	} break;
	}
}

void updateSdlBackend() {
	{
		auto x = 0;
		auto y = 0;
		SDL_GetMouseState(&x, &y);
		click::updateMouse(_mouseDeviceId(), static_cast<float>(x), static_cast<float>(y));
	}

	for (auto& gamepadDeviceId : click::gamepads()) {
		auto& gamepad = click::device(gamepadDeviceId);
		auto id = (SDL_JoystickID)(intptr_t)gamepad.customData;
		auto gameController = SDL_GameControllerFromInstanceID(id);
		if (gameController) {
			auto x = SDL_GameControllerGetAxis(gameController, SDL_CONTROLLER_AXIS_LEFTX);
			auto y = SDL_GameControllerGetAxis(gameController, SDL_CONTROLLER_AXIS_LEFTY);
			auto value = ActionValue{.vec2 = {static_cast<float>(x) / 32767.0f, static_cast<float>(y) / 32767.0f}};
			click::updateGamepadAxis(gamepadDeviceId, Gesture::AxisLeftXY, value);

			x = SDL_GameControllerGetAxis(gameController, SDL_CONTROLLER_AXIS_LEFTX);
			y = SDL_GameControllerGetAxis(gameController, SDL_CONTROLLER_AXIS_LEFTY);
			value = ActionValue{.vec2 = {static_cast<float>(x) / 32767.0f, static_cast<float>(y) / 32767.0f}};
			click::updateGamepadAxis(gamepadDeviceId, Gesture::AxisRightXY, value);
		}
	}
}

} // namespace click
