#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Assert(cond) if(!(cond)) *(int*)0 = 0

struct Buffer
{
  char* data;
  int gap_start;
  int gap_end;
  int size;
};

Buffer buffer_create(int size)
{
  Buffer result;
  result.data = (char*)malloc(size);
  result.size = result.gap_end = size;
  result.gap_start = 0;

  return result;
}

void buffer_move_gap_to_position(Buffer* buffer, int position)
{
  int gap_size = buffer->gap_end - buffer->gap_start;
  if(buffer->gap_start < position)
  {
	Assert(position >= buffer->gap_end);
	int delta = position - buffer->gap_end;
	memmove(buffer->data + buffer->gap_start , buffer->data + buffer->gap_end, delta);
	buffer->gap_start += delta;
	buffer->gap_end   += delta;
  }
  else if(buffer->gap_start > position)
  {
	int delta = buffer->gap_start - position;
	memmove(buffer->data + position + gap_size, buffer->data + position, delta);
	buffer->gap_start -= delta;
	buffer->gap_end   -= delta;
  }
}

void buffer_ensure_gap_size(Buffer* buffer, int minimum_size)
{
  if(buffer->gap_end - buffer->gap_start < minimum_size)
  {
	buffer_move_gap_to_position(buffer, buffer->size); // move the gap to the end
	int new_size = 2 * buffer->size;
	buffer->data = (char*)realloc(buffer->data, new_size);
	buffer->size = buffer->gap_end = new_size;
  }
}

void buffer_insert_at(Buffer* buffer, int position, char c)
{
  buffer_ensure_gap_size(buffer, 1);
  buffer_move_gap_to_position(buffer, position);
  buffer->data[position] = c;
  ++buffer->gap_start;
}

void DEBUG_output_buffer(Buffer* buffer)
{
  char temp[1024];
  memcpy(temp, buffer->data, buffer->gap_start);
  temp[buffer->gap_start] = 0;
  printf(temp);
  memcpy(temp, buffer->data + buffer->gap_end, buffer->size - buffer->gap_end);
  temp[buffer->size - buffer->gap_end] = 0;
  printf(temp);
}

int main()
{
  auto buffer = buffer_create(2);
  for(char c = 'a'; c <= 'z'; ++c)
  {
	buffer_insert_at(&buffer, 0, c);
	buffer_insert_at(&buffer, 1, c + ('A' - 'a'));
  }
	
  DEBUG_output_buffer(&buffer);

  return 0;
}
