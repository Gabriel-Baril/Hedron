#pragma once

#include <vector>
#include <streambuf>

namespace hdn
{
	class DynamicMemoryBuffer : public std::basic_streambuf<char> {
	public:
		DynamicMemoryBuffer() {
			resizeBuffer( 64 ); // Start with an initial buffer size
		}

		// Get the internal buffer as a std::vector<char>
		const std::vector<char> &getBuffer() const { return buffer; }
		void ResetHead()
		{
			setg( buffer.data(), buffer.data(), buffer.data() + buffer.size() ); // **Fixed here**
		}

	protected:
		// Override overflow() to handle writing (expanding buffer if needed)
		int_type overflow( int_type ch ) override {
			if ( ch == traits_type::eof() ) return traits_type::not_eof( ch );

			// Expand buffer dynamically if needed
			size_t oldSize = buffer.size();
			resizeBuffer( oldSize * 2 );

			// Write the character and move the put pointer
			pbump( 1 );
			buffer[oldSize] = static_cast<char>(ch);
			return ch;
		}

		// Override xsputn() to handle writing multiple characters
		std::streamsize xsputn( const char *s, std::streamsize n ) override {
			size_t available = epptr() - pptr();
			if ( n > available ) {
				size_t oldSize = buffer.size();
				resizeBuffer( oldSize + n );
			}

			std::memcpy( pptr(), s, n );
			pbump( n );
			return n;
		}

		// Override underflow() to handle reading (expanding buffer if needed)
		int_type underflow() override {
			if ( gptr() == egptr() ) { // If no more data to read, return EOF
				return traits_type::eof();
			}

			return traits_type::to_int_type( *gptr() );
		}

		// Override xsgetn() to handle reading multiple characters
		std::streamsize xsgetn( char *s, std::streamsize n ) override {
			size_t available = egptr() - gptr();
			size_t toRead = (n > available) ? available : n;

			std::memcpy( s, gptr(), toRead );
			gbump( toRead );
			return toRead;
		}

	private:
		std::vector<char> buffer;

		// Resize the buffer to accommodate more data
		void resizeBuffer( size_t newSize ) {
			size_t oldSize = buffer.size();
			buffer.resize( newSize );

			// Update put area (output area)
			setp( buffer.data(), buffer.data() + buffer.size() );
			pbump( oldSize ); // Move put pointer to the previous write position

			// Update get area (input area) for reading
			setg( buffer.data(), buffer.data(), buffer.data() + oldSize ); // **Fixed here**
		}
	};
}
