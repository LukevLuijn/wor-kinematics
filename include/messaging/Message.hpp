#ifndef MESSAGE_HPP_
#define MESSAGE_HPP_

#include "Config.hpp"

#include <iomanip>
#include <sstream>
#include <string>

/**
 *
 */
namespace Messaging
{
	const int charWidth = 3; // char : 255, ergo 3 numbers
	const int intWidth = 10; // unsigned long : 4,294,967,295 ergo 10 numbers
	/**
	 *
	 */
	struct Message
	{
			typedef std::string MessageBody;

			/**
			 *
			 */
			struct MessageHeader
			{
					/**
					 *
					 */
					MessageHeader() :
									messageType( 0),
									messageLength( 0)
					{
					}
					/**
					 *
					 * @param aMessageType
					 * @param aMessageLength
					 */
					MessageHeader( 	char aMessageType,
									unsigned long aMessageLength) :
									messageType( aMessageType),
									messageLength( aMessageLength)
					{
					}
					/**
					 *
					 * @param aMessageHeaderBuffer
					 */
					explicit MessageHeader(	const std::string& aMessageHeaderBuffer) :
									messageType( 0),
									messageLength( 0)
					{
						fromString( aMessageHeaderBuffer);
					}
					/**
					 * The ASCII representation is suitable for parsing by MessageHeader::fromString.
					 *
					 * @return ASCII string representation of the message header.
					 */
					std::string toString() const
					{
						std::ostringstream os;
						os << magicNumber1 << magicNumber2 << magicNumber3 << magicNumber4 << majorVersion << minorVersion << std::setw(charWidth) << static_cast<int>(messageType) << std::setw(intWidth) << messageLength;
						return os.str();
					}
					/**
					 * Stores a ASCII representation of a message header into this header.
					 * The format of the ASCII representation by MessageHeader::toString should
					 * be suitable.
					 *
					 * @param aString in the same format as MessageHeader::toString.
					 */
					void fromString( const std::string& aString)
					{
						std::istringstream is( aString);
						char magic[4];
						char major;
						char minor;
						is >> magic[0] >> magic[1] >> magic[2] >> magic[3] >> major >> minor >> std::setw(charWidth) >> reinterpret_cast<int&>(messageType) >> std::setw( intWidth) >> messageLength;
					}
					/**
					 * @return The length of the header in bytes
					 */
					unsigned long getHeaderLength() const
					{
						static size_t l = toString().length();
						return static_cast<unsigned long>(l);
					}
					/**
					 *
					 * @return The message type which should be one of the types in MessageTypes.hpp
					 */
					char getMessageType() const
					{
						return messageType;
					}
					/**
					 * @return The length of the message in bytes
					 */
					unsigned long getMessageLength() const
					{
						return messageLength;
					}
					/**
					 * @name Debug functions
					 */
					//@{
					/**
					 * Returns a 1-line description of the object
					 */
					std::string asString() const
					{
						std::ostringstream os;
						os << magicNumber1 << magicNumber2 << magicNumber3 << magicNumber4 << " " << majorVersion << " " <<  minorVersion << " " << static_cast<int>(messageType) << " " << messageLength;
						return os.str();
					}
					/**
					 *
					 */
					std::string asDebugString() const
					{
						return asString();
					}
					//@}

					static const char magicNumber1 = 'A';
					static const char magicNumber2 = 'S';
					static const char magicNumber3 = 'I';
					static const char magicNumber4 = 'O';
					static const char majorVersion = '1';
					static const char minorVersion = '0';
					char messageType;
					unsigned long messageLength;
			}; // struct MessageHeader
			/**
			 *
			 */
			Message() :
							messageType( 0)
			{
			}
			/**
			 *
			 * @param aMessageType
			 */
			explicit Message( char aMessageType) :
							messageType( aMessageType)
			{
			}
			/**
			 *
			 * @param aMessageType, should be one of the values mentioned in MessageTypes.hpp
			 * @param aMessage, which is in a ordinary OO-RPC a specification of the object, the function and the arguments to call.
			 */
			Message( 	char aMessageType,
						const std::string& aMessage) :
							messageType( aMessageType),
							message( aMessage)
			{
			}
			/**
			 *
			 * @param aMessage
			 */
			Message( const Message& aMessage) :
							messageType( aMessage.messageType),
							message( aMessage.message)
			{
			}
			/**
			 *
			 */
			virtual ~Message() = default;
			/**
			 *
			 */
			Message& operator=(const Message& aMessage) = default;
			/**
			 *
			 * @return The header of this message
			 */
			MessageHeader getHeader() const
			{
				return MessageHeader( messageType, message.length());
			}
			/**
			 *
			 * @param Sets the header of this message
			 */
			void setHeader( const MessageHeader& aHeader)
			{
				setMessageType( aHeader.messageType);
				message.resize( aHeader.messageLength);
			}
			/**
			 *
			 * @return
			 */
			char getMessageType() const
			{
				return messageType;
			}
			/**
			 *
			 * @param aMessageType
			 */
			void setMessageType( char aMessageType)
			{
				messageType = aMessageType;
			}
			/**
			 *
			 * @return
			 */
			MessageBody getBody() const
			{
				return message;
			}
			/**
			 *
			 * @param aBody
			 */
			void setBody( const std::string& aBody)
			{
				message = aBody;
			}
			/**
			 * @return The length of the message in bytes
			 */
			unsigned long length() const
			{
				return message.length();
			}
			/**
			 * @name Debug functions
			 */
			//@{
			/**
			 * Returns a 1-line description of the object
			 */
			virtual std::string asString() const
			{
				std::ostringstream os;
				os << getHeader().asString() << ":\t" << message;
				return os.str();
			}
			/**
			 * Returns a description of the object with all data of the object usable for debugging
			 */
			virtual std::string asDebugString() const
			{
				return asDebugString();
			}
			//@}
			/**
			 *
			 */
			char messageType;
			/**
			 *
			 */
			MessageBody message;
	}; // struct Message

} // namespace Messaging

#endif // MESSAGE_HPP_
