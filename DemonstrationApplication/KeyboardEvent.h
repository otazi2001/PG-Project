#pragma once
class KeyboardEvent {
public:
	const double elapsedTime;
	const int keyType;

	KeyboardEvent(double t, int k) : elapsedTime(t), keyType(k) {}
};