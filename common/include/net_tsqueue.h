/*
	MMO Client/Server Framework using ASIO
	"Happy Birthday Mrs Javidx9!" - javidx9
	Videos:
	Part #1: https://youtu.be/2hNdkYInj4g
	Part #2: https://youtu.be/UbjxGvrDrbw
	License (OLC-3)
	~~~~~~~~~~~~~~~
	Copyright 2018 - 2020 OneLoneCoder.com
	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions
	are met:
	1. Redistributions or derivations of source code must retain the above
	copyright notice, this list of conditions and the following disclaimer.
	2. Redistributions or derivative works in binary form must reproduce
	the above copyright notice. This list of conditions and the following
	disclaimer must be reproduced in the documentation and/or other
	materials provided with the distribution.
	3. Neither the name of the copyright holder nor the names of its
	contributors may be used to endorse or promote products derived
	from this software without specific prior written permission.
	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
	A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
	HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
	SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
	LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
	THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
	Links
	~~~~~
	YouTube:	https://www.youtube.com/javidx9
	Discord:	https://discord.gg/WhwHUMV
	Twitter:	https://www.twitter.com/javidx9
	Twitch:		https://www.twitch.tv/javidx9
	GitHub:		https://www.github.com/onelonecoder
	Homepage:	https://www.onelonecoder.com
	Author
	~~~~~~
	David Barr, aka javidx9, ©OneLoneCoder 2019, 2020
*/

#pragma once

#include "net_common.h"

namespace olc
{
	namespace net
	{
		template<typename T>
		class tsqueue
		{
		public:
			tsqueue() = default;
			tsqueue(const tsqueue<T>&) = delete;
			virtual ~tsqueue() { clear(); }

		public:
			// Returns and maintains item at front of Queue
			const T& front();
			// Returns and maintains item at back of Queue
			const T& back();
			// Removes and returns item from front of Queue
			T pop_front();
			// Removes and returns item from back of Queue
			T pop_back();
			// Adds an item to back of Queue
			void push_back(const T& item);
			// Adds an item to front of Queue
			void push_front(const T& item);
			// Returns true if Queue has no items
			bool empty();
			// Returns number of items in Queue
			size_t count();
			// Clears Queue
			void clear();

			void wait();

		protected:
			std::mutex muxQueue;
			std::deque<T> deqQueue;
			std::condition_variable cvBlocking;
			std::mutex muxBlocking;
		};

		
		template<typename T>
		const T& tsqueue<T>::front()
		{
			std::scoped_lock lock(muxQueue);
			return deqQueue.front();
		}

		template<typename T>
		const T& tsqueue<T>::back()
		{
			std::scoped_lock lock(muxQueue);
			return deqQueue.back();
		}

		template<typename T>
		T tsqueue<T>::pop_front()
		{
			std::scoped_lock lock(muxQueue);
			auto t = std::move(deqQueue.front());
			deqQueue.pop_front();
			return t;
		}

		template<typename T>
		T tsqueue<T>::pop_back()
		{
			std::scoped_lock lock(muxQueue);
			auto t = std::move(deqQueue.back());
			deqQueue.pop_back();
			return t;
		}

		template<typename T>
		void tsqueue<T>::push_back(const T& item)
		{
			std::scoped_lock lock(muxQueue);
			deqQueue.emplace_back(std::move(item));

			std::unique_lock<std::mutex> ul(muxBlocking);
			cvBlocking.notify_one();
		}

		template<typename T>
		void tsqueue<T>::push_front(const T& item)
		{
			std::scoped_lock lock(muxQueue);
			deqQueue.emplace_front(std::move(item));

			std::unique_lock<std::mutex> ul(muxBlocking);
			cvBlocking.notify_one();
		}

		template<typename T>
		bool tsqueue<T>::empty()
		{
			std::scoped_lock lock(muxQueue);
			return deqQueue.empty();
		}

		template<typename T>
		size_t tsqueue<T>::count()
		{
			std::scoped_lock lock(muxQueue);
			return deqQueue.size();
		}

		template<typename T>
		void tsqueue<T>::clear()
		{
			std::scoped_lock lock(muxQueue);
			deqQueue.clear();
		}

		template<typename T>
		void tsqueue<T>::wait()
		{
			while (empty())
			{
				std::unique_lock<std::mutex> ul(muxBlocking);
				cvBlocking.wait(ul);
			}
		}

	}
}