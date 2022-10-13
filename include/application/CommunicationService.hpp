#ifndef COMMUNICATIONSERVICE_HPP_
#define COMMUNICATIONSERVICE_HPP_

#include "Config.hpp"

#include "Trace.hpp"

#include <boost/asio.hpp>

#include <condition_variable>
#include <map>
#include <mutex>

namespace Messaging
{
    class Server;
    typedef std::shared_ptr<Server> ServerPtr;

    /*
	 *
	 */
    class CommunicationService
    {
    public:
        /**
			 *
			 */
        static CommunicationService& getCommunicationService();
        /**
			 * This function is public because otherwise it the classes Session, Server and Client
			 * have to be friends
			 */
        boost::asio::io_context& getIOContext()
        {
            return io_context;
        }
        /**
			 *
			 */
        void registerServer(ServerPtr aServer, bool start = true);
        /**
			 *
			 */
        void startServer(ServerPtr aServer);
        /**
			 *
			 */
        void startServer(unsigned short aPort);
        /**
			 *
			 */
        void stopServer(ServerPtr aServer, bool deregister = true);
        /**
			 *
			 */
        void stopServer(unsigned short aPort, bool deregister = true);
        /**
			 *
			 */
        void deregisterServer(ServerPtr aServer);
        /**
			 *
			 */
        void deregisterServer(unsigned short aPort);
        /**
			 *
			 */
        void stop();
        /**
			 *
			 */
        bool isStopped();
        /**
			 *
			 */
        void restart();
        /**
			 *
			 */
        void wait();

    private:
        /**
			 *
			 */
        CommunicationService();
        /**
			 *
			 */
        virtual ~CommunicationService();
        /**
			 *
			 */
        void start_io_context_thread();
        /**
			 *
			 */
        void run_io_context();
        /**
			 *
			 */
        boost::asio::io_context io_context;
        /**
			 *
			 */
        std::thread io_contextThread;
        /**
			 *
			 */
        std::map<unsigned short, ServerPtr> servers;
        /**
			 *
			 */
        boost::asio::deadline_timer timer;
    };
}// namespace Messaging

#endif// COMMUNICATIONSERVICE_HPP_
