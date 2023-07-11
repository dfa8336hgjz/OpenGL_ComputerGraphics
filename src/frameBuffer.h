#pragma once

class FrameBuffer7 {
	unsigned int ID;

public:
	int width, height;
	unsigned int colorTXT, depthRB;
	FrameBuffer7(int width, int height);
	~FrameBuffer7();

	void init();
	void bind();
	void bindDefault();
};