#include "Server.hpp"

namespace Messaging
{
    /**
	 *
	 */
    Server::Server(unsigned short aPort, RequestHandlerPtr aRequestHandler) :
					port(aPort),
					acceptor(CommunicationService::getCommunicationService().getIOContext()/*,
							 boost::asio::ip::tcp::endpoint( boost::asio::ip::tcp::v4(),aPort)*/),
					requestHandler(aRequestHandler),
					timer( Messaging::CommunicationService::getCommunicationService().getIOContext())
    {
    }
    /**
	 *
	 */
    Server::~Server()
    {
    }
} /* namespace Messaging */
