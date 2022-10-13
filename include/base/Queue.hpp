#ifndef QUEUE_HPP_
#define QUEUE_HPP_

#include "Config.hpp"

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>
#include <stddef.h>

namespace Base
{
    template<typename QueueContentType>
    class Queue
    {
    public:
        /**
			 *
			 */
        void enqueue(const QueueContentType& anElement)
        {
            std::unique_lock<std::mutex> lock(queueBusy);
            queue.push(anElement);
            queueFull.notify_one();
        }
        /**
			 *
			 */
        std::optional<QueueContentType> dequeue()
        {
            std::unique_lock<std::mutex> lock(queueBusy);
            while (queue.empty() && stop.load() == false)
                queueFull.wait(lock);

            if (queue.empty())
            {
                return {};
            }
            else
            {
                QueueContentType front = queue.front();
                queue.pop();
                return front;
            }
        }
        /**
			 *
			 */
        size_t size() const
        {
            return queue.size();
        }
        /**
			 *
			 */
        void shutDown()
        {
            stop.store(true);
        }

    private:
        std::queue<QueueContentType> queue;
        std::mutex queueBusy;
        std::condition_variable queueFull;
        std::atomic<bool> stop = false;
    };
}// namespace Base
#endif /* QUEUE_HPP_ */
