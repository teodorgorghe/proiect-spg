#pragma once

class drawable
{
public:
	virtual ~drawable() = default;
	virtual void draw(const camera&) = 0;
};