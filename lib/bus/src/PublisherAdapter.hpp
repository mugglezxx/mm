/*
 * PublisherAdapter.hpp
 *
 *  Created on: Sep 22, 2018
 *      Author: suoalex
 */

#ifndef LIB_BUS_SRC_PUBLISHERADAPTER_HPP_
#define LIB_BUS_SRC_PUBLISHERADAPTER_HPP_

#include <algorithm>
#include <atomic>
#include <mutex>
#include <unordered_map>
#include <vector>

#include <Dispatcher.hpp>
#include <IPublisher.hpp>
#include <SpinLockGuard.hpp>

namespace mm
{
	//
	// This class provides basic implementation for publisher.
	//
	template<typename Message, typename Key = KeyType, typename Mutex = std::mutex> class PublisherAdapter : public IPublisher<Message, Key>
	{
	public:

		//
		// Constructor.
		//
		// dispatcher : The dispatcher used.
		//
		PublisherAdapter(const std::shared_ptr<Dispatcher>& dispatcher) : dispatcher(dispatcher), count(0)
		{
		}

		virtual ~PublisherAdapter();

		virtual void publish(const Subscription& subscription, const std::shared_ptr<const Message>& message) override
		{
			SpinLockGuard guard(mutex);

			auto it = consumerMap.find(subscription);
			if (it == consumerMap.end())
			{
				LOGWARN("Cannot find consumers for subscription {}", subscription);
				return;
			}

			for (const ConsumerDetail& detail : it->second)
			{
				// TODO: this piece of code needs to be reviewed for performance.
				const std::shared_ptr<IConsumer<Message> > consumer = detail.consumer;

				dispatcher->submit(detail.dispatchKey, [] (consumer, message) {
					consumer->consumer(message);
				});
			}
		}

		virtual void subscribe(const Subscription& subscription, const std::shared_ptr<IConsumer<Message> >& consumer) override
		{
			SpinLockGuard guard(mutex);

			consumerMap[subscription].push_back(ConsumerDetail(consumer));
			++count;
		}

		virtual void unsubscribe(const Subscription& subscription, const std::shared_ptr<IConsumer<Message> >& consumer) override
		{
			SpinLockGuard guard(mutex);

			std::vector<ConsumerDetail>& consumers = consumerMap[subscription];

			consumers.erase(std::remove_if(consumers.begin(), consumers.end(), [] (ConsumerDetail& detail) {
				detail.consumer == consumer;
			}));
		}

		virtual size_t getConsumerCount() const override
		{
			SpinLockGuard guard(mutex);
			return count;
		}

		virtual void removeAll() override
		{
			SpinLockGuard guard(mutex);
			consumerMap.clear();
		}

	private:

		//
		// The struct holding consumer details.
		//
		struct ConsumerDetail
		{
			//
			// Constructor.
			//
			// consuemr : The consumer detail captured.
			//
			ConsumerDetail(const std::shared_ptr<IConsumer<Message> >& consumer) :
				dispatchKey(consumer->getDispatchKey()), consumer(consumer)
			{
			}

			// The cached dispatch key.
			const Key dispatchKey;

			// The consumer.
			const std::shared_ptr<IConsumer<Message> > consumer;
		};

		// The dispatcher.
		const std::shared_ptr<Dispatcher> dispatcher;

		// The map where key is the subscription object and value is the consumer(s) subscribing to it.
		std::unordered_map<Subscription, std::vector<ConsumerDetail> > consumerMap;

		// The total number of consumers.
		std::size_t count;

		// The mutex used to access the consumer map.
		Mutex mutex;
	};
}



#endif /* LIB_BUS_SRC_PUBLISHERADAPTER_HPP_ */