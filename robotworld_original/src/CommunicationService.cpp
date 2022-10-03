#include "CommunicationService.hpp"

#include "Server.hpp"

#include <sstream>

namespace Messaging
{
	/**
	 *
	 */
	/* static */CommunicationService& CommunicationService::getCommunicationService()
	{
		static CommunicationService communicationService;
		return communicationService;
	}
	/**
	 *
	 */
	void CommunicationService::registerServer(ServerPtr aServer,
											  bool start /* = true */)
	{
		// TODO Should this be an assert during development only in the limited context of this example?
		auto result = servers.find(aServer->getPort());
		if(result != servers.end())
		{
			std::ostringstream os;
			os << __PRETTY_FUNCTION__ << ": only one server per port allowed, port = " << aServer->getPort();
			throw std::runtime_error( os.str());
		}

		servers.insert(std::make_pair(aServer->getPort(),aServer));

		if(start)
		{
			startServer(aServer);
		}
	}
	/**
	 *
	 */
	void CommunicationService::startServer(	ServerPtr aServer)
	{
		startServer(aServer->getPort());
	}

	/**
	 *
	 */
	void CommunicationService::startServer(	unsigned short aPort)
	{
		// TODO See above
		auto result = servers.find(aPort);
		if(result != servers.end())
		{
			result->second->startHandlingRequests();
		}else
		{
			std::ostringstream os;
			os << __PRETTY_FUNCTION__ << ": no server registered for port " << aPort;
			throw std::runtime_error( os.str());
		}
	}
	/**
	 *
	 */
	void CommunicationService::stopServer(ServerPtr aServer,
										  bool deregister /* = true */)
	{
		stopServer(aServer->getPort(), deregister);
	}
	/**
	 *
	 */
	void CommunicationService::stopServer(	unsigned short aPort,
											bool deregister /* = true */)
	{
		// TODO See above
		auto result = servers.find(aPort);
		if(result != servers.end())
		{
			ServerPtr server = result->second;
			if(deregister)
			{
				deregisterServer(server);
			}
			server->stopHandlingRequests();

			// This is only done in the limited context of this example!!
			if(servers.empty())
			{
				// The number of seconds needs to be larger than the number of seconds it takes
				// the server to stop handling requests (which is 1 second now)??
				timer.expires_from_now(boost::posix_time::seconds(2));
				timer.async_wait( [ this]( const boost::system::error_code&UNUSEDPARAM(e)) // @suppress("Method cannot be resolved")
				{
					boost::asio::defer( [this]() // @suppress("Invalid arguments")
									{
										this->stop();
									});
				});
			}

		}else
		{
			std::ostringstream os;
			os << __PRETTY_FUNCTION__ << ": no server registered for port " << aPort;
			throw std::runtime_error( os.str());
		}
	}
	/**
	 *
	 */
	void CommunicationService::deregisterServer(ServerPtr aServer)
	{
		deregisterServer(aServer->getPort());
	}
	/**
	 *
	 */
	void CommunicationService::deregisterServer(unsigned short aPort)
	{
		// TODO See above
		auto result = servers.find(aPort);
		if(result != servers.end())
		{
			servers.erase(result->second->getPort());
		}else
		{
			std::ostringstream os;
			os << __PRETTY_FUNCTION__ << ": no server registered for port " << aPort;
			throw std::runtime_error( os.str());
		}
	}
	/**
	 *
	 */
	void CommunicationService::stop()
	{
		io_context.stop();
	}
	/**
	 *
	 */
	bool CommunicationService::isStopped()
	{
		return io_context.stopped();
	}
	/**
	 *
	 */
	void CommunicationService::restart()
	{
		if(io_contextThread.joinable())
		{
			io_contextThread.detach();
		}
		start_io_context_thread();
	}
	/**
	 *
	 */
	void CommunicationService::wait()
	{
		io_contextThread.join();
	}

	/**
	 *
	 */
	CommunicationService::CommunicationService() : 	timer( io_context)

	{
		start_io_context_thread();
	}
	/**
	 *
	 */
	CommunicationService::~CommunicationService()
	{
		if(io_contextThread.joinable())
		{
			io_contextThread.detach();
		}
	}
	/**
	 *
	 */
	/**
	 *
	 */
	void CommunicationService::start_io_context_thread()
	{
		std::thread new_io_contextThread([this]{run_io_context();});
		io_contextThread.swap(new_io_contextThread);
		//io_contextThread.detach();
	}
	/**
	 *
	 */
	void CommunicationService::run_io_context()
	{
		if(io_context.stopped())
		{
			io_context.restart();
		}

		// @see https://www.boost.org/doc/libs/1_76_0/doc/html/boost_asio/reference/io_context.html
		// for the loop and the work...
		for(;;)
		{
			try
			{
				auto work = boost::asio::require(io_context.get_executor(), boost::asio::execution::outstanding_work.tracked);
				io_context.run();
				break;
			}
			catch (std::exception& e)
			{
				std::ostringstream os;
				os << "Exception in " << __PRETTY_FUNCTION__ << ": " << e.what();
				TRACE_DEVELOP(os.str());
			}
			catch (...)
			{
				std::ostringstream os;
				os << "Unknown exception in " << __PRETTY_FUNCTION__ ;
				TRACE_DEVELOP(os.str());
			}
		}
	}

} /* namespace Base */
