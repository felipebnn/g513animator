#pragma once

#include <iostream>
#include <vector>

#include <hidapi/hidapi.h>

class G513 {
	std::vector<unsigned char> keyData { 0x00, 0x12, 0xff, 0x0c, 0x3a, 0x00, 0x01, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	std::vector<unsigned char> commitData { 0x00, 0x11, 0xff, 0x0c, 0x5a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	hid_device* m_hidHandle;

	bool sendData(const std::vector<unsigned char>& data);

public:
	enum class Key : uint8_t {
		undef = 0x00,
		a = 0x04,
		b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z,
		n1, n2, n3, n4, n5, n6, n7, n8, n9, n0,
		enter, esc, backspace, tab, space, minus, equal, open_bracket, close_bracket,
		backslash, dollar, semicolon, quote, tilde, comma, period, slash, caps_lock,
		f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12,
		print_screen, scroll_lock, pause_break, insert, home, page_up, del, end, page_down,
		arrow_right, arrow_left, arrow_bottom, arrow_top, num_lock, num_slash, num_asterisk,
		num_minus, num_plus, num_enter,
		num_1, num_2, num_3, num_4, num_5, num_6, num_7, num_8, num_9, num_0,
		num_dot, intl_backslash, menu,

		ctrl_left = 0xe0,
		shift_left, alt_left, win_left,
		ctrl_right, shift_right, alt_right, win_right

	};

	struct Color {
		uint8_t red;
		uint8_t green;
		uint8_t blue;
	};

	G513();

	void setKey(G513::Key k, const G513::Color& c, bool commit=true);

	void setAllKeys(const G513::Color& c, bool commit=true);

	void commit();
};
