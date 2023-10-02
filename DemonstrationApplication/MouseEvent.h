#pragma once
class MouseEvent {
public:
	const double elapsedTime;
	const float deltaYaw;
	const float deltaPitch;

	MouseEvent(double t, float x, float y) : elapsedTime(t), deltaYaw(x), deltaPitch(y) {}
};