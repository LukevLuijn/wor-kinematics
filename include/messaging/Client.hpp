#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include "Config.hpp"

#include "Session.hpp"

#include <string>

namespace Messaging
{
    /**
	 *
	 */
    class Client
    {
    public:
        /**
			 *
			 */
        Client(const std::string& aHostName, unsigned short aPort, ResponseHandlerPtr aResponseHandler)
            : host(aHostName), port(aPort), responseHandler(aResponseHandler)
        {
        }
        /**
			 *
			 */
        ~Client()
        {
        }
        /**
			 *
			 */
        void dispatchMessage(const Message& aMessage)
        {
            // Create the session that will handle the next outgoing connection
            ClientSession* session = new ClientSession(aMessage, responseHandler);

            // Build up the remote address to which we will connect
            boost::asio::ip::tcp::resolver resolver(CommunicationService::getCommunicationService().getIOContext());
            boost::asio::ip::tcp::resolver::query query(boost::asio::ip::tcp::v4(), host, std::to_string(port));
            boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
            boost::asio::ip::tcp::endpoint endpoint = *endpoint_iterator;

            // Let the session handle any outgoing messages
            session->getSocket().async_connect(endpoint,// @suppress("Method cannot be resolved")
                                               [this, session](const boost::system::error_code& error) {
                                                   handleConnect(session, error);
                                               });
        }

    private:
        /**
			 *
			 */
        void handleConnect(ClientSession* aSession, const boost::system::error_code& error) const
        {
            if (!error)
            {
                aSession->start();
            }
            else
            {
                delete aSession;

                std::ostringstream os;
                os << __PRETTY_FUNCTION__ << ": error connecting to " << host << " at port " << port
                   << ", reason: " << error.message();
                TRACE_DEVELOP(os.str());
            }
        }
        /**
			 *
			 */
        std::string host;
        /**
			 *
			 */
        unsigned short port;
        /**
			 *
			 */
        ResponseHandlerPtr responseHandler;
    };
}// namespace Messaging

#endif// CLIENT_HPP_
