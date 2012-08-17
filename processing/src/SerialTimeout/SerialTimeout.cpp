#include "SerialTimeout.h"
#include <string>
#include <algorithm>
#include <iostream>
#include <boost/bind.hpp>

using namespace std;
using namespace boost;

SerialTimeout::SerialTimeout(): io(), port(io), timer(io),
	timeout(posix_time::seconds(0)) {}

SerialTimeout::SerialTimeout(const std::string& devname, unsigned int baud_rate,
		asio::serial_port_base::parity opt_parity,
		asio::serial_port_base::character_size opt_csize,
		asio::serial_port_base::flow_control opt_flow,
		asio::serial_port_base::stop_bits opt_stop)
: io(), port(io), timer(io), timeout(posix_time::seconds(0))
{
	open(devname,baud_rate,opt_parity,opt_csize,opt_flow,opt_stop);
}

void SerialTimeout::open(const std::string& devname, unsigned int baud_rate,
		asio::serial_port_base::parity opt_parity,
		asio::serial_port_base::character_size opt_csize,
		asio::serial_port_base::flow_control opt_flow,
		asio::serial_port_base::stop_bits opt_stop)
{
	if(isOpen()) close();
	port.open(devname);
	port.set_option(asio::serial_port_base::baud_rate(baud_rate));
	port.set_option(opt_parity);
	port.set_option(opt_csize);
	port.set_option(opt_flow);
	port.set_option(opt_stop);
}

bool SerialTimeout::isOpen() const
{
	return port.is_open();
}

void SerialTimeout::close()
{
	if(isOpen()==false) return;
	port.close();
}

void SerialTimeout::setTimeout(const posix_time::time_duration& t)
{
	timeout=t;
}

void SerialTimeout::write(const unsigned char byte)
{
	char data[1];
	data[0] = static_cast<unsigned char>(byte);
	size_t size = 1;

	asio::write(port,asio::buffer(data,size));
}

void SerialTimeout::write(const char *data, size_t size)
{
	asio::write(port,asio::buffer(data,size));
}

void SerialTimeout::write(const std::vector<char>& data)
{
	asio::write(port,asio::buffer(&data[0],data.size()));
}

void SerialTimeout::writeString(const std::string& s)
{
	asio::write(port,asio::buffer(s.c_str(),s.size()));
}

unsigned char SerialTimeout::read()
{                       
	size_t size = 1;
	char* data = new char[size]; 

	if(readData.size()>0)//If there is some data from a previous read
	{
		istream is(&readData);
		size_t toRead=min(readData.size(),size);//How many bytes to read?
		is.read(data,toRead);
		data+=toRead;
		size-=toRead;
		if(size==0)//If read data was enough, just return
		{
			unsigned char byte = static_cast<unsigned char>(data[0]);
			delete[] data;
			return byte;
		}
	}

	setupParameters=ReadSetupParameters(data,size);
	performReadSetup(setupParameters);

	//For this code to work, there should always be a timeout, so the
	//request for no timeout is translated into a very long timeout
	if(timeout!=posix_time::seconds(0)) timer.expires_from_now(timeout);
	else timer.expires_from_now(posix_time::hours(100000));

	timer.async_wait(boost::bind(&SerialTimeout::timeoutExpired,this,
				asio::placeholders::error));

	result=resultInProgress;
	bytesTransferred=0;
	for(;;)
	{
		io.run_one();
		if (resultSuccess == result)
		{
			timer.cancel();
			unsigned char byte = static_cast<unsigned char>(data[0]);
			delete[] data;
			return byte;
		}
		else if (resultTimeoutExpired == result)
		{
			port.cancel();
			delete[] data;
			throw(timeout_exception("Timeout expired"));
		}
		else if (resultError == result)
		{
			timer.cancel();
			port.cancel();
			delete[] data;
			throw(boost::system::system_error(boost::system::error_code(),
						"Error while reading"));
			//if resultInProgress remain in the loop
		}
	}

	unsigned char byte = static_cast<unsigned char>(data[0]);
	delete[] data;
	return byte;
}

void SerialTimeout::read(unsigned char byte)
{
	size_t size = 1;
	char* data = new char[size]; 

	if(readData.size()>0)//If there is some data from a previous read
	{
		istream is(&readData);
		size_t toRead=min(readData.size(),size);//How many bytes to read?
		is.read(data,toRead);
		data+=toRead;
		size-=toRead;
		if(size==0) //If read data was enough, just return
		{
			byte = static_cast<unsigned char>(data[0]);
			delete[] data;
			return;
		}

	}

	setupParameters=ReadSetupParameters(data,size);
	performReadSetup(setupParameters);

	//For this code to work, there should always be a timeout, so the
	//request for no timeout is translated into a very long timeout
	if(timeout!=posix_time::seconds(0)) timer.expires_from_now(timeout);
	else timer.expires_from_now(posix_time::hours(100000));

	timer.async_wait(boost::bind(&SerialTimeout::timeoutExpired,this,
				asio::placeholders::error));

	result=resultInProgress;
	bytesTransferred=0;
	for(;;)
	{
		io.run_one();
		switch(result)
		{
			case resultSuccess:
				timer.cancel();
				byte = static_cast<unsigned char>(data[0]);
				delete[] data;
				return;
			case resultTimeoutExpired:
				port.cancel();
				byte = static_cast<unsigned char>(data[0]);
				delete[] data;
				throw(timeout_exception("Timeout expired"));
			case resultError:
				timer.cancel();
				port.cancel();
				byte = static_cast<unsigned char>(data[0]);
				delete[] data;
				throw(boost::system::system_error(boost::system::error_code(),
							"Error while reading"));
				//if resultInProgress remain in the loop
		}
	}
}

void SerialTimeout::read(char *data, size_t size)
{
	if(readData.size()>0)//If there is some data from a previous read
	{
		istream is(&readData);
		size_t toRead=min(readData.size(),size);//How many bytes to read?
		is.read(data,toRead);
		data+=toRead;
		size-=toRead;
		if(size==0) return;//If read data was enough, just return
	}

	setupParameters=ReadSetupParameters(data,size);
	performReadSetup(setupParameters);

	//For this code to work, there should always be a timeout, so the
	//request for no timeout is translated into a very long timeout
	if(timeout!=posix_time::seconds(0)) timer.expires_from_now(timeout);
	else timer.expires_from_now(posix_time::hours(100000));

	timer.async_wait(boost::bind(&SerialTimeout::timeoutExpired,this,
				asio::placeholders::error));

	result=resultInProgress;
	bytesTransferred=0;
	for(;;)
	{
		io.run_one();
		switch(result)
		{
			case resultSuccess:
				timer.cancel();
				return;
			case resultTimeoutExpired:
				port.cancel();
				throw(timeout_exception("Timeout expired"));
			case resultError:
				timer.cancel();
				port.cancel();
				throw(boost::system::system_error(boost::system::error_code(),
							"Error while reading"));
				//if resultInProgress remain in the loop
		}
	}
}

std::vector<char> SerialTimeout::read(size_t size)
{
	vector<char> result(size,'\0');//Allocate a vector with the desired size
	read(&result[0],size);//Fill it with values
	return result;
}

std::string SerialTimeout::readString(size_t size)
{
	string result(size,'\0');//Allocate a string with the desired size
	read(&result[0],size);//Fill it with values
	return result;
}

std::string SerialTimeout::readStringUntil(const std::string& delim)
{
	// Note: if readData contains some previously read data, the call to
	// async_read_until (which is done in performReadSetup) correctly handles
	// it. If the data is enough it will also immediately call readCompleted()
	setupParameters=ReadSetupParameters(delim);
	performReadSetup(setupParameters);

	//For this code to work, there should always be a timeout, so the
	//request for no timeout is translated into a very long timeout
	if(timeout!=posix_time::seconds(0)) timer.expires_from_now(timeout);
	else timer.expires_from_now(posix_time::hours(100000));

	timer.async_wait(boost::bind(&SerialTimeout::timeoutExpired,this,
				asio::placeholders::error));

	result=resultInProgress;
	bytesTransferred=0;
	for(;;)
	{
		io.run_one();
		switch(result)
		{
			case resultSuccess:
				{
					timer.cancel();
					bytesTransferred-=delim.size();//Don't count delim
					istream is(&readData);
					string result(bytesTransferred,'\0');//Alloc string
					is.read(&result[0],bytesTransferred);//Fill values
					is.ignore(delim.size());//Remove delimiter from stream
					return result;
				}
			case resultTimeoutExpired:
				port.cancel();
				throw(timeout_exception("Timeout expired"));
			case resultError:
				timer.cancel();
				port.cancel();
				throw(boost::system::system_error(boost::system::error_code(),
							"Error while reading"));
				//if resultInProgress remain in the loop
		}
	}
}

SerialTimeout::~SerialTimeout() {}

void SerialTimeout::performReadSetup(const ReadSetupParameters& param)
{
	if(param.fixedSize)
	{
		asio::async_read(port,asio::buffer(param.data,param.size),boost::bind(
					&SerialTimeout::readCompleted,this,asio::placeholders::error,
					asio::placeholders::bytes_transferred));
	} else {
		asio::async_read_until(port,readData,param.delim,boost::bind(
					&SerialTimeout::readCompleted,this,asio::placeholders::error,
					asio::placeholders::bytes_transferred));
	}
}

void SerialTimeout::timeoutExpired(const boost::system::error_code& error)
{
	if(!error && result==resultInProgress) result=resultTimeoutExpired;
}

void SerialTimeout::readCompleted(const boost::system::error_code& error,
		const size_t bytesTransferred)
{
	if(!error)
	{
		result=resultSuccess;
		this->bytesTransferred=bytesTransferred;
		return;
	}

	//In case a asynchronous operation is cancelled due to a timeout,
	//each OS seems to have its way to react.
#ifdef _WIN32
	if(error.value()==995) return; //Windows spits out error 995
#elif defined(__APPLE__)
	if(error.value()==45)
	{
		//Bug on OS X, it might be necessary to repeat the setup
		//http://osdir.com/ml/lib.boost.asio.user/2008-08/msg00004.html
		performReadSetup(setupParameters);
		return;
	}
#else //Linux
	if(error.value()==125) return; //Linux outputs error 125
#endif

	result=resultError;
}
