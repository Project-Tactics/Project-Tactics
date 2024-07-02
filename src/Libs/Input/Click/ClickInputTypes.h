#pragma once

#include <string>
#include <vector>

namespace click {

using InputActionId = unsigned int;

#if not defined(CLICK_NAME_ID)
using NameId = std::string;
#else
using NameId = CLICK_NAME_ID;
#endif

enum class ActionType {
	Impulse,
	Axis1D,
	Axis2D,
	Axis3D
};

enum class ActionTrigger {
	Press,
	Release,
	Hold
};

enum class DeviceType {
	Keyboard,
	Mouse,
	Gamepad,
	Touch
};

// clang-format off
enum class KeyboardAction {
	KeyUnknown,
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
// clang-format on

enum class MouseAction {
	LeftButton,
	RightButton,
	MiddleButton,
	MouseX,
	MouseY,
	MouseZ,
	MouseXY
};

enum class GamePadAction {
	ButtonA,
	ButtonB,
	ButtonX,
	ButtonY,
	ButtonStart,
	ButtonSelect,
	ButtonL1,
	ButtonL2,
	ButtonR1,
	ButtonR2,
	ButtonL3,
	ButtonR3,
	DPadUp,
	DPadDown,
	DPadLeft,
	DPadRight,
	ThumbLX,
	ThumbLY,
	ThumbRX,
	ThumbRY
};

enum class TouchAction {
	Touch1,
	Touch2,
	Touch3,
	Touch4,
	Touch5,
	Touch6,
	Touch7,
	Touch8,
	Touch9,
	Touch10
};

enum class InputState {
	None,
	Triggered,
	Started,
	Ended
};

struct InputAction {
	InputActionId id{};
	NameId name;
	ActionType type{};
	InputState state{};
};

union DeviceAction {
	KeyboardAction key;
	MouseAction mouse;
	GamePadAction gamePad;
	TouchAction touch;
	int32_t other{};
};

struct DeviceInputMapping {
	DeviceType deviceType{};
	DeviceAction deviceAction{};
	std::vector<ActionTrigger> triggers;
};

struct Mapping {
	InputActionId actionId{};
	std::vector<DeviceInputMapping> mapping;
};

struct Context {
	std::vector<InputAction> actions;
	std::vector<Mapping> mappings;
};

} // namespace click
