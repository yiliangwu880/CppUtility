#pragma once

#include <functional>

struct scope_guard {
	typedef std::function<void()> Fn;
	Fn action;
	bool active;

	scope_guard(Fn action) : action(action), active(true) {}

	void dismiss() {
		active = false;
	}
	~scope_guard() {
		if (active) action();
	}
};
test:



#include <windows.h>

#include <iostream>
using namespace std;

#include "scope_guard.h"

void test() {
	void* memory = malloc(100);
	scope_guard g([&]() {
		free(memory);
		cout << "memory freed" << endl;
	});

	bool error_occur = true;
	if (error_occur) return;

	g.dismiss();
}
int main() {
	test();
}