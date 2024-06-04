/* UI.cpp - Various user interface routines and code for the management of an sdl window
 *
 * NVGT - NonVisual Gaming Toolkit
 * Copyright (c) 2022-2024 Sam Tupy
 * https://nvgt.gg
 * This software is provided "as-is", without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.
 * Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
 * 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
*/

#define NOMINMAX
#include <angelscript.h>
#include "srspeech.h"
#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	#define VC_EXTRALEAN
	#include <windows.h>
	#include "InputBox.h"
#elif defined(__APPLE__)
	#include "apple.h"
#endif
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <obfuscate.h>
#include <thread.h>
#include <string>
#include <vector>
#include <Poco/Thread.h>
#include <Poco/UnicodeConverter.h>
#include <Poco/Util/Application.h>
#include "input.h"
#include "misc_functions.h"
#include "scriptstuff.h"
#include "timestuff.h"
#include "UI.h"

int message_box(const std::string& title, const std::string& text, const std::vector<std::string>& buttons, unsigned int mb_flags) {
	// Start with the buttons.
	std::vector<SDL_MessageBoxButtonData> sdlbuttons;
	for (int i = 0; i < buttons.size(); i++) {
		const std::string& btn = buttons[i];
		int skip = 0;
		unsigned int button_flag = 0;
		if (btn.substr(0, 1) == "`") {
			button_flag |= SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT;
			skip += 1;
		}
		if (btn.substr(skip, 1) == "~") {
			button_flag |= SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT;
			skip += 1;
		}
		SDL_MessageBoxButtonData& sdlbtn = sdlbuttons.emplace_back();
		sdlbtn.flags = button_flag;
		sdlbtn.buttonid = i + 1;
		sdlbtn.text = buttons[i].c_str() + skip;
	}
	SDL_MessageBoxData box = {mb_flags, g_WindowHandle, title.c_str(), text.c_str(), int(sdlbuttons.size()), sdlbuttons.data(), NULL};
	int ret;
	if (SDL_ShowMessageBox(&box, &ret) != 0) return 0; // failure.
	return ret;
}
int message_box_script(const std::string& title, const std::string& text, CScriptArray* buttons, unsigned int flags) {
	std::vector<std::string> v_buttons(buttons->GetSize());
	for (unsigned int i = 0; i < buttons->GetSize(); i++) v_buttons[i] = (*(std::string*)(buttons->At(i)));
	return message_box(title, text, v_buttons, flags);
}
int alert(const std::string& title, const std::string& text, bool can_cancel, unsigned int flags) {
	std::vector<std::string> buttons = {can_cancel ? "`OK" : "`~OK"};
	if (can_cancel) buttons.push_back("~Cancel");
	return message_box(title, text, buttons, flags);
}
int question(const std::string& title, const std::string& text, bool can_cancel, unsigned int flags) {
	std::vector<std::string> buttons = {"`Yes", "No"};
	if (can_cancel) buttons.push_back("~Cancel");
	return message_box(title, text, buttons, flags);
}
void message(const std::string& text, const std::string& header) { // Usually used internally by NVGT's c++ code to print an error first to stdout if that's available, then to a message box if that's enabled.
	std::string tmp = header;
	tmp += ": ";
	tmp += text;
	printf("%s", tmp.c_str());
	if (Poco::Util::Application::instance().config().hasOption("application.gui")) {
		alert(header, text);
		return;
	}
}
std::string ClipboardGetText() {
	InputInit();
	char* r = SDL_GetClipboardText();
	std::string cb_text(r);
	SDL_free(r);
	return cb_text;
}
bool ClipboardSetText(const std::string& text) {
	InputInit();
	return SDL_SetClipboardText(text.c_str()) == 0;
}
bool ClipboardSetRawText(const std::string& text) {
	#ifdef _WIN32
	if (!OpenClipboard(nullptr))
		return false;
	EmptyClipboard();
	if (text == "") {
		CloseClipboard();
		return true;
	}
	HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, text.size() + 1);
	if (!hMem) {
		CloseClipboard();
		return false;
	}
	char* cbText = (char*)GlobalLock(hMem);
	memcpy(cbText, text.c_str(), text.size());
	cbText[text.size()] = 0;
	GlobalUnlock(hMem);
	SetClipboardData(CF_TEXT, hMem);
	CloseClipboard();
	return true;
	#else
	return FALSE;
	#endif
}
bool urlopen(const std::string& url) {
	return !SDL_OpenURL(url.c_str());
}
void next_keyboard_layout() {
	#ifdef _WIN32
	ActivateKeyboardLayout((HKL)HKL_NEXT, 0);
	#endif
}
std::string input_box(const std::string& title, const std::string& text, const std::string& default_value) {
	#ifdef _WIN32
	std::wstring titleU, textU, defaultU;
	Poco::UnicodeConverter::convert(title, titleU);
	Poco::UnicodeConverter::convert(text, textU);
	Poco::UnicodeConverter::convert(default_value, defaultU);
	std::wstring r = InputBox(titleU, textU, defaultU);
	if (r == L"\xff") {
		g_LastError = -12;
		return "";
	}
	std::string resultA;
	Poco::UnicodeConverter::convert(r, resultA);
	return resultA;
	#elif defined(__APPLE__)
	std::string r = apple_input_box(title, text, default_value, false, false);
	if (g_WindowHandle) SDL_RaiseWindow(g_WindowHandle);
	if (r == "\xff") {
		g_LastError = -12;
		return "";
	}
	return r;
	#else
	return "";
	#endif
}
bool info_box(const std::string& title, const std::string& text, const std::string& value) {
	#ifdef _WIN32
	std::wstring titleU, textU, valueU;
	Poco::UnicodeConverter::convert(title, titleU);
	Poco::UnicodeConverter::convert(text, textU);
	Poco::UnicodeConverter::convert(value, valueU);
	return InfoBox(titleU, textU, valueU);
	#elif defined(__APPLE__)
	apple_input_box(title, text, value, false, false);
	return true;
	#else
	return false;
	#endif
}

SDL_Window* g_WindowHandle = 0;
#ifdef _WIN32
	HWND g_OSWindowHandle = NULL;
#elif defined(__APPLE__)
	#include "apple.h"
	NSWindow* g_OSWindowHandle = NULL;
#else
	void* g_OSWindowHandle = NULL;
#endif
thread_id_t g_WindowThreadId = 0;
bool g_WindowHidden = false;

static std::vector<SDL_Event> post_events; // holds events that should be processed after the next wait() call.
#ifdef _WIN32
void sdl_windows_messages(void* udata, void* hwnd, unsigned int message, uint64_t wParam, int64_t lParam) {
	if (message == WM_KEYDOWN && wParam == 'V' && lParam == 1) {
		SDL_Event e{};
		e.type = SDL_KEYDOWN;
		e.key.timestamp = SDL_GetTicks();
		e.key.keysym.scancode = SDL_SCANCODE_PASTE;
		e.key.keysym.sym = SDLK_PASTE;
		SDL_PushEvent(&e);
		e.type = SDL_KEYUP;
		post_events.push_back(e);
	}
}
#endif
bool set_application_name(const std::string& name) {
	return SDL_SetHintWithPriority(SDL_HINT_APP_NAME, name.c_str(), SDL_HINT_OVERRIDE);
}
bool ShowNVGTWindow(std::string& window_title) {
	if (g_WindowHandle) {
		SDL_SetWindowTitle(g_WindowHandle, window_title.c_str());
		if (g_WindowHidden) {
			g_WindowHidden = false;
			SDL_ShowWindow(g_WindowHandle);
			SDL_RaiseWindow(g_WindowHandle);
		}
		return true;
	}
	InputInit();
	#ifdef _WIN32
	SDL_SetWindowsMessageHook(sdl_windows_messages, NULL);
	#endif
	g_WindowHandle = SDL_CreateWindow(window_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 640, 0);
	if (!g_WindowHandle) return false;
	SDL_SysWMinfo winf;
	SDL_VERSION(&winf.version);
	SDL_GetWindowWMInfo(g_WindowHandle, &winf);
	#ifdef _WIN32
	g_OSWindowHandle = winf.info.win.window;
	#elif defined(SDL_VIDEO_DRIVER_COCOA)
	g_OSWindowHandle = winf.info.cocoa.window;
	SDL_ShowWindow(g_WindowHandle);
	SDL_RaiseWindow(g_WindowHandle);
	voice_over_window_created();
	#endif
	g_WindowThreadId = thread_current_thread_id();
	return true;
}
bool DestroyNVGTWindow() {
	if (!g_WindowHandle) return false;
	SDL_DestroyWindow(g_WindowHandle);
	InputDestroy();
	g_WindowHandle = NULL;
	g_OSWindowHandle = NULL;
	return true;
}
BOOL HideNVGTWindow() {
	if (!g_WindowHandle) return false;
	SDL_HideWindow(g_WindowHandle);
	g_WindowHidden = true;
	return true;
}
BOOL FocusNVGTWindow() {
	if (!g_WindowHandle) return false;
	SDL_RaiseWindow(g_WindowHandle);
	return true;
}
BOOL WindowIsFocused() {
	if (!g_WindowHandle) return false;
	return g_WindowHandle == SDL_GetKeyboardFocus();
}
std::string get_window_text() {
	if (!g_WindowHandle) return "";
	return std::string(SDL_GetWindowTitle(g_WindowHandle));
}
void handle_sdl_event(SDL_Event* evt) {
	if (evt->type == SDL_KEYDOWN || evt->type == SDL_KEYUP || evt->type == SDL_TEXTINPUT || evt->type == SDL_MOUSEMOTION || evt->type == SDL_MOUSEBUTTONDOWN || evt->type == SDL_MOUSEBUTTONUP || evt->type == SDL_MOUSEWHEEL)
		InputEvent(evt);
	else if (evt->type == SDL_WINDOWEVENT && evt->window.event == SDL_WINDOWEVENT_FOCUS_LOST)
		SDL_ResetKeyboard();
}
void wait(int ms) {
	if (!g_WindowHandle || g_WindowThreadId != thread_current_thread_id()) {
		Poco::Thread::sleep(ms);
		return;
	}
	while (ms >= 0) {
		int MS = (ms > 25 ? 25 : ms);
		if (g_GCMode == 2)
			garbage_collect_action();
		Poco::Thread::sleep(MS);
		SDL_PumpEvents();
		ms -= MS;
		if (ms < 1) break;
	}
	SDL_Event evt;
	#ifdef __APPLE__
	bool left_just_pressed = false, right_just_pressed = false;
	#endif
	while (SDL_PollEvent(&evt)) {
		#ifdef __APPLE__
		// Hack to fix voiceover's weird handling of the left and right arrow keys. If a left/right arrow down/up event get generated in the same frame, we need to move the up event to the next frame.
		bool evt_handled = false;
		if (evt.type == SDL_KEYDOWN) {
			if (evt.key.keysym.scancode == SDL_SCANCODE_LEFT) left_just_pressed = true;
			if (evt.key.keysym.scancode == SDL_SCANCODE_RIGHT) right_just_pressed = true;
		} else if ((left_just_pressed || right_just_pressed) && evt.type == SDL_KEYUP) {
			evt_handled = true;
			if (left_just_pressed && evt.key.keysym.scancode == SDL_SCANCODE_LEFT) post_events.push_back(evt);
			else if (right_just_pressed && evt.key.keysym.scancode == SDL_SCANCODE_RIGHT) post_events.push_back(evt);
			else evt_handled = false;
		}
		if (evt_handled) continue;
		#endif
		handle_sdl_event(&evt);
	}
	if (post_events.size() > 0) {
		for (SDL_Event& e : post_events)
			SDL_PushEvent(&e);
		post_events.clear();
	}
}
void RegisterUI(asIScriptEngine* engine) {
	engine->SetDefaultAccessMask(NVGT_SUBSYSTEM_UI);
	engine->RegisterEnum(_O("message_box_flags"));
	engine->RegisterEnumValue(_O("message_box_flags"), _O("MESSAGE_BOX_ERROR"), SDL_MESSAGEBOX_ERROR);
	engine->RegisterEnumValue(_O("message_box_flags"), _O("MESSAGE_BOX_WARNING"), SDL_MESSAGEBOX_WARNING);
	engine->RegisterEnumValue(_O("message_box_flags"), _O("MESSAGE_BOX_INFORMATION"), SDL_MESSAGEBOX_INFORMATION);
	engine->RegisterEnumValue(_O("message_box_flags"), _O("MESSAGE_BOX_BUTTONS_LEFT_TO_RIGHT"), SDL_MESSAGEBOX_BUTTONS_LEFT_TO_RIGHT);
	engine->RegisterEnumValue(_O("message_box_flags"), _O("MESSAGE_BOX_BUTTONS_RIGHT_TO_LEFT"), SDL_MESSAGEBOX_BUTTONS_RIGHT_TO_LEFT);
	engine->RegisterGlobalFunction(_O("int message_box(const string& in, const string& in, string[]@, uint = 0)"), asFUNCTION(message_box_script), asCALL_CDECL);
	engine->RegisterGlobalFunction(_O("int alert(const string &in, const string &in, bool = false, uint = 0)"), asFUNCTION(alert), asCALL_CDECL);
	engine->RegisterGlobalFunction(_O("int question(const string& in, const string& in, bool = false, uint = 0)"), asFUNCTION(question), asCALL_CDECL);
	engine->SetDefaultAccessMask(NVGT_SUBSYSTEM_OS);
	engine->RegisterGlobalFunction(_O("string clipboard_get_text()"), asFUNCTION(ClipboardGetText), asCALL_CDECL);
	engine->RegisterGlobalFunction(_O("bool clipboard_set_text(const string& in)"), asFUNCTION(ClipboardSetText), asCALL_CDECL);
	engine->RegisterGlobalFunction(_O("bool clipboard_set_raw_text(const string& in)"), asFUNCTION(ClipboardSetRawText), asCALL_CDECL);
	engine->RegisterGlobalFunction(_O("bool urlopen(const string &in)"), asFUNCTION(urlopen), asCALL_CDECL);
	engine->SetDefaultAccessMask(NVGT_SUBSYSTEM_UI);
	engine->RegisterGlobalFunction(_O("string input_box(const string& in, const string& in, const string& in = '', uint64 = 0)"), asFUNCTION(input_box), asCALL_CDECL);
	engine->RegisterGlobalFunction(_O("bool info_box(const string& in, const string& in, const string& in, uint64 = 0)"), asFUNCTION(info_box), asCALL_CDECL);
	engine->RegisterGlobalFunction(_O("void next_keyboard_layout()"), asFUNCTION(next_keyboard_layout), asCALL_CDECL);
	engine->RegisterGlobalFunction("bool set_application_name(const string& in)", asFUNCTION(set_application_name), asCALL_CDECL);
	engine->RegisterGlobalFunction("bool show_window(const string& in)", asFUNCTION(ShowNVGTWindow), asCALL_CDECL);
	engine->RegisterGlobalFunction("bool destroy_window()", asFUNCTION(DestroyNVGTWindow), asCALL_CDECL);
	engine->RegisterGlobalFunction("bool hide_window()", asFUNCTION(HideNVGTWindow), asCALL_CDECL);
	engine->RegisterGlobalFunction("bool focus_window()", asFUNCTION(FocusNVGTWindow), asCALL_CDECL);
	engine->RegisterGlobalFunction("bool is_window_active()", asFUNCTION(WindowIsFocused), asCALL_CDECL);
	engine->RegisterGlobalFunction("string get_window_text()", asFUNCTION(get_window_text), asCALL_CDECL);
	engine->RegisterGlobalFunction("void wait(int)", asFUNCTIONPR(wait, (int), void), asCALL_CDECL);
}
