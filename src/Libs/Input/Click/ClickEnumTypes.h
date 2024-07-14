#pragma once

#include <cstdint>

namespace click {

enum class ActionType {
	Scalar,
	Axis2D,
	Axis3D
};

enum class TriggerType {
	Down,
	Press,
	Release,
	Hold,
	Continuous
};

enum class ModifierType {
	Negate,
	ToAxis
};

enum class TriggerState {
	Idle,
	Ongoing,
	Triggered
};

enum class DeviceType : uint8_t {
	None,
	Keyboard,
	Mouse,
	Gamepad,
	Touch
};

enum class InputState {
	None,
	Triggered,
	Started,
	Ongoing,
	Ended
};

enum class Axis {
	XYZ,
	YZX,
	ZXY
};

enum class InputCode {
	Unknown,
	// Mouse
	LeftButton,
	RightButton,
	MiddleButton,
	MouseX,
	MouseY,
	MouseZ,
	MouseXY,
	// Gamepad
	ButtonA,
	ButtonB,
	ButtonX,
	ButtonY,
	ButtonBack,
	ButtonGuide,
	ButtonStart,
	ButtonLeftStick,
	ButtonRightStick,
	ButtonLeftShoulder,
	ButtonRightShoulder,
	ButtonDPadUp,
	ButtonDPadDown,
	ButtonDPadRight,
	ButtonDPadLeft,
	ButtonMisc,
	ButtonPaddle1,
	ButtonPaddle2,
	ButtonPaddle3,
	ButtonPaddle4,
	ButtonTouchpad,
	AxisLeftX,
	AxisLeftY,
	AxisLeftXY,
	AxisRightX,
	AxisRightY,
	AxisRightXY,
	AxisTriggerLeft,
	AxisTriggerRight,
	// Touch
	Touch1,
	Touch2,
	Touch3,
	Touch4,
	Touch5,
	Touch6,
	Touch7,
	Touch8,
	Touch9,
	Touch10,
	// Keyboard
	KeyReturn,
	KeyEscape,
	KeyBackspace,
	KeyTab,
	KeySpace,
	KeyExclaim,
	KeyQuotedbl,
	KeyHash,
	KeyPercent,
	KeyDollar,
	KeyAmpersand,
	KeyQuote,
	KeyLeftparen,
	KeyRightparen,
	KeyAsterisk,
	KeyPlus,
	KeyComma,
	KeyMinus,
	KeyPeriod,
	KeySlash,
	Key0,
	Key1,
	Key2,
	Key3,
	Key4,
	Key5,
	Key6,
	Key7,
	Key8,
	Key9,
	KeyColon,
	KeySemicolon,
	KeyLess,
	KeyEquals,
	KeyGreater,
	KeyQuestion,
	KeyAt,
	KeyLeftbracket,
	KeyBackslash,
	KeyRightbracket,
	KeyCaret,
	KeyUnderscore,
	KeyBackquote,
	KeyA,
	KeyB,
	KeyC,
	KeyD,
	KeyE,
	KeyF,
	KeyG,
	KeyH,
	KeyI,
	KeyJ,
	KeyK,
	KeyL,
	KeyM,
	KeyN,
	KeyO,
	KeyP,
	KeyQ,
	KeyR,
	KeyS,
	KeyT,
	KeyU,
	KeyV,
	KeyW,
	KeyX,
	KeyY,
	KeyZ,
	KeyCapslock,
	KeyF1,
	KeyF2,
	KeyF3,
	KeyF4,
	KeyF5,
	KeyF6,
	KeyF7,
	KeyF8,
	KeyF9,
	KeyF10,
	KeyF11,
	KeyF12,
	KeyPrintscreen,
	KeyScrolllock,
	KeyPause,
	KeyInsert,
	KeyHome,
	KeyPageup,
	KeyDelete,
	KeyEnd,
	KeyPagedown,
	KeyRight,
	KeyLeft,
	KeyDown,
	KeyUp,
	KeyNumlockclear,
	KeypadDivide,
	KeypadMultiply,
	KeypadMinus,
	KeypadPlus,
	KeypadEnter,
	Keypad1,
	Keypad2,
	Keypad3,
	Keypad4,
	Keypad5,
	Keypad6,
	Keypad7,
	Keypad8,
	Keypad9,
	Keypad0,
	KeypadPeriod,
	KeyApplication,
	KeyPower,
	KeypadEquals,
	KeyF13,
	KeyF14,
	KeyF15,
	KeyF16,
	KeyF17,
	KeyF18,
	KeyF19,
	KeyF20,
	KeyF21,
	KeyF22,
	KeyF23,
	KeyF24,
	KeyExecute,
	KeyHelp,
	KeyMenu,
	KeySelect,
	KeyStop,
	KeyAgain,
	KeyUndo,
	KeyCut,
	KeyCopy,
	KeyPaste,
	KeyFind,
	KeyMute,
	KeyVolumeup,
	KeyVolumedown,
	KeypadComma,
	KeypadEqualsas400,
	KeyAlterase,
	KeySysreq,
	KeyCancel,
	KeyClear,
	KeyPrior,
	KeyReturn2,
	KeySeparator,
	KeyOut,
	KeyOper,
	KeyClearagain,
	KeyCrsel,
	KeyExsel,
	Keypad00,
	Keypad000,
	KeyThousandsseparator,
	KeyDecimalseparator,
	KeyCurrencyunit,
	KeyCurrencysubunit,
	KeypadLeftparen,
	KeypadRightparen,
	KeypadLeftbrace,
	KeypadRightbrace,
	KeypadTab,
	KeypadBackspace,
	KeypadA,
	KeypadB,
	KeypadC,
	KeypadD,
	KeypadE,
	KeypadF,
	KeypadXor,
	KeypadPower,
	KeypadPercent,
	KeypadLess,
	KeypadGreater,
	KeypadAmpersand,
	KeypadDblampersand,
	KeypadVerticalbar,
	KeypadDblverticalbar,
	KeypadColon,
	KeypadHash,
	KeypadSpace,
	KeypadAt,
	KeypadExclam,
	KeypadMemstore,
	KeypadMemrecall,
	KeypadMemclear,
	KeypadMemadd,
	KeypadMemsubtract,
	KeypadMemmultiply,
	KeypadMemdivide,
	KeypadPlusminus,
	KeypadClear,
	KeypadClearentry,
	KeypadBinary,
	KeypadOctal,
	KeypadDecimal,
	KeypadHexadecimal,
	KeyLctrl,
	KeyLshift,
	KeyLalt,
	KeyLgui,
	KeyRctrl,
	KeyRshift,
	KeyRalt,
	KeyRgui,
	KeyMode,
	KeyAudionext,
	KeyAudioprev,
	KeyAudiostop,
	KeyAudioplay,
	KeyAudiomute,
	KeyMediaselect,
	KeyWww,
	KeyMail,
	KeyCalculator,
	KeyComputer,
	KeyAcSearch,
	KeyAcHome,
	KeyAcBack,
	KeyAcForward,
	KeyAcStop,
	KeyAcRefresh,
	KeyAcBookmarks,
	KeyBrightnessdown,
	KeyBrightnessup,
	KeyDisplayswitch,
	KeyKbdillumtoggle,
	KeyKbdillumdown,
	KeyKbdillumup,
	KeyEject,
	KeySleep,
	KeyApp1,
	KeyApp2,
	KeyAudiorewind,
	KeyAudiofastforward,
	KeySoftleft,
	KeySoftright,
	KeyCall,
	KeyEndcall
};

} // namespace click
