/*
 * OrderSummaryMessage.hpp
 *
 *  Created on: Sep 18, 2018
 *      Author: suoalex
 */

#ifndef LIB_MESSAGING_SRC_ORDERSUMMARYMESSAGE_HPP_
#define LIB_MESSAGING_SRC_ORDERSUMMARYMESSAGE_HPP_

#include <cstdint>

#include "Message.hpp"
#include "OrderStatus.hpp"
#include "Side.hpp"

namespace mm
{
	//
	// This class defines an order summary message.
	//
	class OrderSummaryMessage : public Message
	{
	public:

		//
		// Default constructor.
		//
		OrderSummaryMessage();

		//
		// Get the remain qty of the order.
		//
		// return : remain qty as total qty - traded qty.
		//
		inline std::int64_t remainQty() const
		{
			return totalQty - tradedQty;
		}

		// The order ID.
		std::int64_t orderId;

		// The instrument ID.
		std::int64_t instrumentId;

		// The ordr side.
		Side side;

		// Total qty of the order.
		std::int64_t totalQty;

		// Traded qty of the order.
		std::int64_t tradedQty;

		// Market price.
		double price;

		// Average traded price.
		double avgTradedPrice;

		// current order status.
		OrderStatus status;
	};

	DEFINE_OPERATORS(OrderSummaryMessage)

}



#endif /* LIB_MESSAGING_SRC_ORDERSUMMARYMESSAGE_HPP_ */
