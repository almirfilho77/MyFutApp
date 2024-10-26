#ifndef __VIDEOIO_H__
#define __VIDEOIO_H__
#pragma once

#include <string>

namespace VideoIO {

	typedef struct _Frame {
		void* Image;
		int GPUSlot;
		int FrameIndex;
	} Frame;

	class IVideoSource
	{
		/*
		* @brief Loads video file and enables client to call Get methods.
		* @param [in] filepath: Video file path.
		* @return [bool] true if the video file was opened successfully, false otherwise.
		*/
		virtual bool Load(const std::string &file_path) = 0;

		/*
		* @brief Unloads video file.
		* @param [in] filepath: Video file path.
		* @return [bool] true if the video file was opened successfully, false otherwise.
		*/
		virtual bool Unload() = 0;

		/*
		* @brief Returns video frame at index 'frame_index'.
		* @param [in] frame_index: the index of the frame to get.
		* @return [Frame *] Pointer to a frame struct. The user is responsible for destroying the memory.
		*/
		virtual Frame *GetFrame(int frame_index) = 0;

		/*
		* @brief Returns video frame rate.
		* @return [int] Video frame rate.
		*/
		virtual int GetFrameRate() const = 0;

		/*
		* @brief Returns video frame width.
		* @return [int] Video frame width.
		*/
		virtual int GetWidth() const = 0;

		/*
		* @brief Returns video frame height.
		* @return [int] Video frame height.
		*/
		virtual int GetHeight() const = 0;

		/*
		* @brief Returns total number of frames in the video.
		* @return [int] Total number of frames in the video.
		*/
		virtual int GetNumberOfFrames() const = 0;
	};

}//VideoIO

#endif /*__VIDEOIO_H__*/