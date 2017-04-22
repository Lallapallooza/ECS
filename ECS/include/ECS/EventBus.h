#pragma once
#include "Event.h"


namespace ECS
{
	namespace tools
	{
		template<std::size_t N>
		class EventBus
		{
		public:
			void raise() noexcept;
			void addHandle(const std::function<void()> &func) noexcept;
			bool removeHandle(const std::function<void()> &func) noexcept;
		private:
			Event<void> events;
		};

		template <std::size_t N>
		void EventBus<N>::raise() noexcept
		{
			events.notify();
		}

		template <std::size_t N>
		void EventBus<N>::addHandle(const std::function<void()>& func) noexcept
		{
			events += func;
		}

		template <std::size_t N>
		bool EventBus<N>::removeHandle(const std::function<void()>& func) noexcept
		{
			bool is_erased = false;
			events.erase(func, is_erased);
			return is_erased;
		}
	}
}