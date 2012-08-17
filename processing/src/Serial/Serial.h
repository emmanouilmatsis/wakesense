#include <string>
#include <boost/asio.hpp>

class Serial
{
	public:
		/**
		 * Constructor.
		 * \param port device name, example "/dev/ttyUSB0" or "COM4"
		 * \param baud_rate communication speed, example 9600 or 115200
		 * \throws boost::system::system_error if cannot open the
		 * serial device
		 */
		Serial(std::string port, unsigned int baud_rate);

		/**
		 * Write a string to the serial device.
		 * \param s string to write
		 * \throws boost::system::system_error on failure
		 */
		void writeString(std::string s);

		/**
		 * Blocks until a line is received from the serial device.
		 * Eventual '\n' or '\r\n' characters at the end of the string are removed.
		 * \return a string containing the received line
		 * \throws boost::system::system_error on failure
		 */
		std::string readLine();

	private:
		boost::asio::io_service io;
		boost::asio::serial_port serial;
};
