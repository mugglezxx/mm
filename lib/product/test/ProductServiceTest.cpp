/*
 * ProductServiceTest.cpp
 *
 *  Created on: Sep 30, 2018
 *      Author: suoalex
 */

/*
 * ServiceContextTest.cpp
 *
 *  Created on: Sep 25, 2018
 *      Author: suoalex
 */

#include <atomic>
#include <chrono>
#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>
#include <thread>

#include <gtest/gtest.h>

#include <StringBuffer.hpp>

#include "ProductService.hpp"

namespace mm
{
	struct DummyFactory : IServiceFactory
	{
		virtual ~DummyFactory()
		{
		}

		virtual std::shared_ptr<IService> createService(
				const std::string serviceClass,
				const std::shared_ptr<IConfig> config,
				ServiceContext& context) override
		{
			return std::shared_ptr<IService> ();
		}
	};

	class DummyServiceContext : public ServiceContext
	{
	public:

		DummyServiceContext(std::istream&& is, IServiceFactory& factory) : ServiceContext(is, factory)
		{
		}

		bool setService(const std::string& serviceName, const std::shared_ptr<IService>& service)
		{
			return ServiceContext::setService(serviceName, service);
		}
	};

	TEST(ProductServiceTest, DummyCase)
	{
		DummyFactory factory;
		DummyServiceContext serviceContext(std::stringstream(""), factory);

		std::stringstream ss;
		std::shared_ptr<ProductService> service(new ProductService(1, "DummyProductService", serviceContext, ss));

		ASSERT_TRUE(serviceContext.setService("DummyProductService", service));
		ASSERT_TRUE(service->start());

		service->stop();
	}

	TEST(ProductServiceTest, SimpleProductCase)
	{
		DummyFactory factory;
		DummyServiceContext serviceContext(std::stringstream(""), factory);

		std::stringstream ss;
		{
			StringBuffer buffer;

			ProductMessage message;
			message.id = 1;
			message.underlyerId = 0;
			message.symbol = "IF1503";
			message.exchange = Exchange::SHFE;
			message.productType = ProductType::FUTURE;
			message.currency = Currency::CNY;
			message.callPut = CallPutType::CALL;
			message.lotSize = 1;

			message.serialize(buffer);
			ss << buffer;
		}

		std::shared_ptr<ProductService> service(new ProductService(1, "DummyProductService", serviceContext, ss));

		ASSERT_TRUE(serviceContext.setService("DummyProductService", service));
		ASSERT_TRUE(service->start());

		service->stop();
	}

}




