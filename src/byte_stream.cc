#include "byte_stream.hh"

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ), available_capacity_( capacity )
{
  buffer_.reserve( capacity_ );
}

void Writer::push( string data )
{
  if ( is_closed() || has_error() ) {
    return;
  }

  const uint64_t len_to_push = min( available_capacity(), data.size() );
  if ( len_to_push == 0 ) {
    return;
  }

  // discard the remaining chars in data if data size is greater than available capacity
  buffer_.append( data.substr( 0, len_to_push ) );
  available_capacity_ -= len_to_push;
  bytes_pushed_ += len_to_push;
  bytes_buffered_ += len_to_push;
}

void Writer::close()
{
  closed_ = true;
}

bool Writer::is_closed() const
{
  return closed_;
}

uint64_t Writer::available_capacity() const
{
  return available_capacity_;
}

uint64_t Writer::bytes_pushed() const
{
  return bytes_pushed_;
}

string_view Reader::peek() const
{
  return string_view { buffer_ };
}

void Reader::pop( uint64_t len )
{
  const uint64_t len_to_pop = min( bytes_buffered(), len );
  buffer_.erase( 0, len_to_pop );
  bytes_buffered_ -= len_to_pop;
  bytes_popped_ += len_to_pop;
  available_capacity_ += len_to_pop;
}

bool Reader::is_finished() const
{
  return closed_ && bytes_buffered() == 0;
}

uint64_t Reader::bytes_buffered() const
{
  return bytes_buffered_;
}

uint64_t Reader::bytes_popped() const
{
  return bytes_popped_;
}
