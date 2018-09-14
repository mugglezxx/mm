/*
 * FemasMarketDataSession.hpp
 *
 *  Created on: Sep 13, 2018
 *      Author: suoalex
 */

#ifndef LIB_FEMAS_SRC_FEMASMARKETDATASESSION_HPP_
#define LIB_FEMAS_SRC_FEMASMARKETDATASESSION_HPP_

#include <memory>

#include <femas/USTPFtdcMduserApi.h>

#include <MarketDataMessage.hpp>
#include <SubscriberAdapter.hpp>

namespace mm
{
	//
	// This class defines a Femas market data session for market data updates.
	//
	//
	//
	class FemasMarketDataSession :
			public CUstpFtdcMduserSpi,
			public SubscriberAdapter<MarketDataMessage>
	{
	public:

		//
		// Constructor.
		//
		//
		//
		FemasMarketDataSession();

		//
		// Destructor.
		//
		virtual ~FemasMarketDataSession();

		//
		// Fired when the session is connected.
		//
		virtual void OnFrontConnected() override;

		//
		// Fired when the session is disconnected.
		//
		// reason : The reason number.
		//
		virtual void OnFrontDisconnected(int reason) override;

		//
		// Fired when the heart beat got error.
		//
		// timeLapse : The time passed.
		//
		virtual void OnHeartBeatWarning(int timeLapse) override;

		//
		// Fired when the packet start is received.
		//
		// topicID : The topic on which the packet is received.
		// sequenceNo : The sequence number.
		//
		virtual void OnPackageStart(int topicID, int sequenceNo) override;

		//
		// Fired when the packet end is received.
		//
		// topicID : The topic on which the packet is received.
		// sequenceNo : The sequence number.
		//
		virtual void OnPackageEnd(int topicID, int sequenceNo) override;

		//
		// Fired when the response got error.
		//
		// info : Structure with details.
		// requestID : The request got the error.
		// isLast : Flag if this is the last request.
		//
		virtual void OnRspError(CUstpFtdcRspInfoField *info, int requestID, bool isLast) override;

		//
		// Fired for the login response.
		//
		// userLogin : details for the login
		// info : information details.
		// requestID : The request ID.
		// isLast : Flag if this is the last response.
		//
		virtual void OnRspUserLogin(
				CUstpFtdcRspUserLoginField *userLogin,
				CUstpFtdcRspInfoField *info,
				int requestID,
				bool isLast) override;

		//
		// Fired for the logout response.
		//
		// userLogout : details for the logout
		// info : information details.
		// requestID : The request ID.
		// isLast : Flag if this is the last response.
		//
		virtual void OnRspUserLogout(
				CUstpFtdcRspUserLogoutField *userLogout,
				CUstpFtdcRspInfoField *info,
				int requestID,
				bool isLast) override;

		//
		// Fired for the subscription response.
		//
		// dissemination : details for the subscription.
		// info : information details.
		// requestID : The request ID.
		// isLast : Flag if this is the last response.
		//
		virtual void OnRspSubscribeTopic(
				CUstpFtdcDisseminationField *dissemination,
				CUstpFtdcRspInfoField *info,
				int requestID,
				bool isLast) override;

		//
		// Fired for the topic query response.
		//
		// dissemination : details for the subscription.
		// info : information details.
		// requestID : The request ID.
		// isLast : Flag if this is the last response.
		//
		virtual void OnRspQryTopic(
				CUstpFtdcDisseminationField *dissemination,
				CUstpFtdcRspInfoField *info,
				int requestID,
				bool isLast) override;

		//
		// Fired for the market data depth update.
		//
		// depthMarketData : market data depth update.
		//
		virtual void OnRtnDepthMarketData(CUstpFtdcDepthMarketDataField *depthMarketData) override;

		//
		// Fired for the market data subscription response.
		//
		// specificInstrument : instrument subscribed.
		// info : information details.
		// requestID : The request ID.
		// isLast : Flag if this is the last response.
		//
		virtual void OnRspSubMarketData(
				CUstpFtdcSpecificInstrumentField *specificInstrument,
				CUstpFtdcRspInfoField *info,
				int requestID,
				bool isLast) override;

		//
		// Fired for the market data unsubscription response.
		//
		// specificInstrument : instrument unsubscribed.
		// info : information details.
		// requestID : The request ID.
		// isLast : Flag if this is the last response.
		//
		virtual void OnRspUnSubMarketData(
				CUstpFtdcSpecificInstrumentField *specificInstrument,
				CUstpFtdcRspInfoField *info,
				int requestID,
				bool isLast) override;

	private:

		// The actual API session.
		std::unique_ptr<CUstpFtdcMduserApi> session;
	};
}



#endif /* LIB_FEMAS_SRC_FEMASMARKETDATASESSION_HPP_ */