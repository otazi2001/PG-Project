#pragma once
#include <functional>
#include "KeyboardEvent.h"
#include "MouseEvent.h"

typedef std::function<void(KeyboardEvent)> KeyboardCallback;

typedef std::function<void(MouseEvent)> MouseCallback;