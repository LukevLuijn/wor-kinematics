#ifndef SESSION_HPP_
#define SESSION_HPP_

#include "Config.hpp"

#include "CommunicationService.hpp"
#include "Message.hpp"
#include "MessageHandler.hpp"
#include "MessageTypes.hpp"

#include <boost/asio.hpp>

#include <sstream>
#include <string>

namespace Messaging
{
	/**
	 * A session is an encapsulation of a request/response transaction sequence.
	 */
	class Session
	{
		public:
			/**
			 *
			 */
			explicit Session() :
					socket( CommunicationService::getCommunicationService().getIOContext())
			{
			}
			/**
			 *
			 */
			explicit Session( const Message& aMessage) :
					socket( CommunicationService::getCommunicationService().getIOContext()),
					message(aMessage)
			{
			}
			/**
			 *
			 */
			virtual ~Session()
			{
				if(socket.is_open())
				{
					socket.close();
				}
			}
			/**
			 * Typically a ServerSession has a read/write sequence,
			 * a ClientSession a write/read sequence
			 */
			virtual void start() = 0;
			/**
			 * Handle the fact that a message is read. This function is called by the framework
			 * after the message (header + body) is read. Normally this is the only function
			 * that a ServerSession or ClientSession has to implement.
			 *
			 * If message.getMessageType is CommunicationReadError the reading of the message failed
			 * so an appropriate action should be taken.
			 */
			virtual void handleMessageRead() = 0;
			/**
			 * Handle the fact that a message is written. This function is called by the framework
			 * after the message (header + body) is written. Normally this is the only function
			 * that a ServerSession or ClientSession has to implement.
			 *
			 * If message.getMessageType is CommunicationWriteError the writing of the message failed
			 * so an appropriate action should be taken.
			 */
			virtual void handleMessageWritten() = 0;
			/**
			 * This function must be public or Client and Server should be friend of Session
			 *
			 * @return the socket of this Session
			 */
			boost::asio::ip::tcp::socket& getSocket()
			{
				return socket;
			}
		protected:
			/**
			 * readMessage will read the message in 2 a-sync reads, 1 for the header and 1 for the body.
			 * After each read a callback will be called that should handle the bytes just read.
			 * After reading the full message handleMessageRead will be called
			 * whose responsibility it is to handle the message as a whole.
			 *
			 * @see Session::handleHeaderRead
			 * @see Session::handleBodyRead
			 * @see Session::handleMessageRead
			 */
			void readMessage()
			{
				headerBuffer.resize( message.getHeader().getHeaderLength());
				boost::asio::async_read( socket, // @suppress("Invalid arguments")
										 boost::asio::buffer( headerBuffer),
										 [this](const boost::system::error_code& error,size_t bytes_transferred)
										 {
											handleHeaderRead(error,bytes_transferred);
										 });
			}
			/**
			 * This function is called after the header bytes are read.
			 */
			void handleHeaderRead( 	const boost::system::error_code& error,
									size_t UNUSEDPARAM(bytes_transferred))
			{
				if (!error)
				{
					message.setHeader( Message::MessageHeader(std::string( headerBuffer.begin(), headerBuffer.end()))); // @suppress("Symbol is not resolved")
					bodyBuffer.resize( message.getHeader().getMessageLength());
					boost::asio::async_read( socket, // @suppress("Invalid arguments")
											 boost::asio::buffer( bodyBuffer),
											 [this](const boost::system::error_code& error,size_t bytes_transferred)
											 {
												handleBodyRead(error,bytes_transferred);
											 });
				} else
				{
					message.setMessageType(CommunicationReadError);
					message.setBody("*** Session::handleHeaderRead: " + error.message());
					handleMessageRead();
				}
			}
			/**
			 * This function as called after the body bytes are read.
			 *
			 * Any error handling (throwing an exception ;-)) is done in this function and
			 * then the function with the same name but without the error is called.
			 */
			void handleBodyRead( 	const boost::system::error_code& error,
									size_t bytes_transferred)
			{
				if (!error)
				{
					message.setBody( std::string( bodyBuffer.begin(), bodyBuffer.end())); // @suppress("Symbol is not resolved")
					handleMessageRead( error, bytes_transferred);
				} else
				{
					message.setMessageType(CommunicationReadError);
					message.setBody("*** Session::handleBodyRead: " + error.message());
					handleMessageRead();
				}
			}
			/**
			 * This function is called after both the header and body bytes are read.
			 */
			void handleMessageRead( const boost::system::error_code& error,
									size_t UNUSEDPARAM(bytes_transferred))
			{
				if (!error)
				{
					handleMessageRead();
				} else
				{
					message.setMessageType(CommunicationReadError);
					message.setBody("*** Session::handleMessageRead: " + error.message());
					handleMessageRead();
				}
			}
			/**
			 * writeMessage will write the message in 2 a-sync writes, 1 for the header and 1 for the body.
			 * After each write a callback will be called that should handle the bytes just read.
			 * After writing the full message handleMessageWritten will be called.
			 *
			 * @see Session::handleHeaderWritten
			 * @see Session::handleBodyWritten
			 * @see Session::handleMessageWritten
			 */
			void writeMessage( const Message& aMessage)
			{
				message = aMessage;
				boost::asio::async_write(socket, // @suppress("Invalid arguments")
										 boost::asio::buffer( message.getHeader().toString(), message.getHeader().getHeaderLength()),
										 [this, aMessage](const boost::system::error_code& error, std::size_t UNUSEDPARAM(bytes_transferred))
										 {
											handleHeaderWritten(error);
										 });
			}
			/**
			 * This function is called after the header bytes are written.
			 */
			void handleHeaderWritten(	const boost::system::error_code& error)
			{
				if (!error)
				{
					boost::asio::async_write(	socket, // @suppress("Invalid arguments")
												boost::asio::buffer( message.getBody(), message.length()),
												[this](const boost::system::error_code& error, std::size_t UNUSEDPARAM(bytes_transferred))
												{
													handleBodyWritten(error);
												});
				} else
				{
					message.setMessageType(CommunicationWriteError);
					message.setBody("*** Session::handleHeaderWritten: " + error.message());
					handleMessageWritten();
				}
			}
			/**
			 * This function is called after the body bytes are written.
			 */
			void handleBodyWritten( const boost::system::error_code& error)
			{
				if (!error)
				{
					handleMessageWritten( error);
				} else
				{
					message.setMessageType(CommunicationWriteError);
					message.setBody("*** Session::handleBodyWritten: " + error.message());
					handleMessageWritten();
				}
			}
			/**
			 * This function is called after both the header and body bytes are written.
			 *
			 * Any error handling (throwing an exception ;-)) is done in this function and
			 * then the function with the same name but without the error is called.
			 */
			void handleMessageWritten( 	const boost::system::error_code& error)
			{
				if (!error)
				{
					handleMessageWritten();
				} else
				{
					message.setMessageType(CommunicationWriteError);
					message.setBody("*** Session::handleMessageWritten: " + error.message());
					handleMessageWritten();
				}
			}
			/*
			 *
			 */
			boost::asio::ip::tcp::socket socket;
			/*
			 *
			 */
			Message message;
			/**
			 *
			 */
			std::vector< char > headerBuffer;
			/**
			 *
			 */
			std::vector< char > bodyBuffer;
	};
	// class Session
	/**
	 *
	 */
	class ServerSession : virtual public Session
	{
		public:
			/**
			 *
			 * @param aRequestHandler
			 */
			explicit ServerSession( RequestHandlerPtr aRequestHandler) :
									requestHandler( aRequestHandler)
			{
				sessionNumber = ++sessionCounter;
			}
			/**
			 *
			 */
			~ServerSession()
			{
			}
			/**
			 * @see Session::start()
			 */
			virtual void start() override
			{
				readMessage();
			}
			/**
			 * @see Session::handleMessageRead( Message& aMessage)
			 */
			virtual void handleMessageRead() override
			{
				if(message.getMessageType() != CommunicationReadError)
				{
					requestHandler->handleRequest( message);
					writeMessage( message);
				}else
				{
					TRACE_DEVELOP("*** ServerSession::handleMessageRead: " + message.asString());
					delete this;
				}
			}
			/**
			 * @see Session::handleMessageWritten( Message& aMessage)
			 */
			virtual void handleMessageWritten() override
			{
				if(message.getMessageType() != CommunicationWriteError)
				{
					//
				}else
				{
					TRACE_DEVELOP("*** ServerSession::handleMessageWritten: " + message.asString());
				}
				// See https://isocpp.org/wiki/faq/freestore-mgmt#delete-this
				delete this;
			}

		private:
			/**
			 *
			 */
			RequestHandlerPtr  requestHandler;
			/**
			 *
			 */
			unsigned long sessionNumber = 0;
			/**
			 *
			 */
			inline static unsigned long sessionCounter = 0;
	};
	// class ServerSession
	/**
	 *
	 */
	class ClientSession : virtual public Session
	{
		public:
			/**
			 *
			 * @param aMessage
			 * @param aResponseHandler
			 */
			ClientSession( 	const Message& aMessage,
							ResponseHandlerPtr aResponseHandler) :
							Session( aMessage),
							responseHandler( aResponseHandler)
			{
				sessionNumber = ++sessionCounter;
			}
			/**
			 *
			 */
			~ClientSession()
			{
			}
			/**
			 * @see Session::start()
			 */
			virtual void start() override
			{
				writeMessage( message);
			}
			/**
			 * @see Session::handleMessageRead( Message& aMessage)
			 */
			virtual void handleMessageRead() override
			{
				if(message.getMessageType() != CommunicationReadError)
				{
					// This is the place where any reply message from the server should
					// be handled
					responseHandler->handleResponse( message);
				}else
				{
					TRACE_DEVELOP("*** ClientSession::handleMessageRead: " + message.asString());
				}

				// See https://isocpp.org/wiki/faq/freestore-mgmt#delete-this
				delete this;
			}
			/**
			 * @see Session::handleMessageWritten( Message& aMessage)
			 */
			virtual void handleMessageWritten() override
			{
				if(message.getMessageType() != CommunicationWriteError)
				{
					// This *must* be the last function that is called after
					// sending a message because it will read the response...
					readMessage();
				}else
				{
					TRACE_DEVELOP("*** ClientSession::handleMessageWritten: " + message.asString());
					delete this;
				}
			}
		private:
			/**
			 *
			 */
			ResponseHandlerPtr responseHandler;
			/**
			 *
			 */
			unsigned long sessionNumber = 0;
			/**
			 *
			 */
			inline static unsigned long sessionCounter = 0;
	};
//	class ClientSession

}// namespace Messaging

#endif // SESSION_HPP_
