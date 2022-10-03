#ifndef MESSAGEHANDLER_HPP_
#define MESSAGEHANDLER_HPP_

#include "Config.hpp"

#include <memory>

/**
 *
 */
namespace Messaging
{
	class Message;

	/**
	 * Base server interface for handling remote requests
	 * Classes derived from this interface can serve as a server in the Messaging protocol
	 * by implementing this interface.
	 *
	 * A RequestHandler that is passed to the Server and to its ServerSessions will live at least
	 * as long as the Server and any outstanding ServerSession.
	 */
	class RequestHandler
	{
		public:
			virtual ~RequestHandler(){}
			/**
			 * After this function is called the aMessage is returned as response to the requesting client,
			 * i.e. it should contain the result/response of/to the request.
			 *
			 * @param aMessage in/out message containing the request (in) and response(out).
			 */
			virtual void handleRequest( Message& aMessage) = 0;
	}; // class RequestHandler
	typedef std::shared_ptr< RequestHandler > RequestHandlerPtr;

	/**
	 * Base client interface for handling remote responses
	 * Classes derived from this interface can serve as client in the Messaging protocol
	 * by implementing this interface.
	 *
	 * A ResponseHandler that is passed to the Client and to its ClientSessions will live at least
	 * as long as the Client and any outstanding ClientSessions.
	 */
	class ResponseHandler
	{
		public:
			virtual ~ResponseHandler(){}
			/**
			 * The given argument contains the result/response of/to a previous request.
			 *
			 * @param aMessage
			 */
			virtual void handleResponse( const Message& aMessage) = 0;

	}; // class ResponseHandler
	typedef std::shared_ptr< ResponseHandler > ResponseHandlerPtr;

	/**
	 * Convenience interface class for a class that acts both as server and client in the Messaging protocol.
	 *
	 * For lifetime management:
	 * @see RequestHandler
	 * @see ResponseHandler
	 *
	 * For the message handling:
	 * @see RequestHandler::handleRequest( Message& aMessage)
	 * @see ResponseHandler::handleResponse( Message& aMessage)
	 */
	class MessageHandler : public RequestHandler, public ResponseHandler
	{
	}; // class MessageHandler

} // namespace Messaging

#endif // MESSAGEHANDLER_HPP_
