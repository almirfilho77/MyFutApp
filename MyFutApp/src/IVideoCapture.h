#ifndef __IVIDEO_CAPTURE_H__
#define __IVIDEO_CAPTURE_H__
#pragma once

class IVideoCapture
{
public:
	virtual bool Open() = 0;
	virtual bool Close() = 0;
	virtual bool ReadNextFrame() = 0;
};

#endif //__IVIDEO_CAPTURE_H__