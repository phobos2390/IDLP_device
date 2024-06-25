/// @file IDLP/RLE_compressor_device.cpp

#include <IDLP/RLE_compressor_device.h>
#include <memory>
#include <vector>
#include <cstring>

namespace IDLP
{

struct RLE_compressor_device::Impl
{
public:
   IDLP::I_device& m_device;

    Impl(IDLP::I_device& device):m_device(device){}
    virtual ~Impl(){}
};

RLE_compressor_device::RLE_compressor_device(IDLP::I_device& device)
    :m_p_impl(new Impl(device))
{
}

RLE_compressor_device::~RLE_compressor_device()
{
    delete m_p_impl;
    m_p_impl = 0;
}
    
int64_t RLE_compressor_device::write(uint8_t* out_bytes, size_t bytes_size, size_t timeout_ms)
{
   std::vector<uint8_t> bytes_to_write;
   uint8_t previous = 0;
   bool previous_set = false;
   bool will_add = true;
   uint8_t run_size = 1;
   for(size_t i = 0; i < bytes_size; i++)
   {
       if ((previous_set == true))
       {
           if (previous == out_bytes[i] && (run_size < 0xFE))
           {
               run_size++;
               will_add = false;
           }
           else
           {
               bytes_to_write.push_back(0xFF);
               bytes_to_write.push_back(run_size);
               run_size = 1;
               will_add = true;
           }
       }
       if (will_add == true)
       {
           if (out_bytes[i] == 0xFF)
           {
               bytes_to_write.push_back(out_bytes[i]);
           }
           bytes_to_write.push_back(out_bytes[i]);
       }
       previous = out_bytes[i];
       previous_set = true;
   }
   if (run_size > 1)
   {
       bytes_to_write.push_back(0xFF);
       bytes_to_write.push_back(run_size);
   }

   return m_p_impl->m_device.write(bytes_to_write.data(),bytes_to_write.size(),timeout_ms);
}

int64_t RLE_compressor_device::read(uint8_t* in_bytes, size_t max_bytes_size, size_t timeout_ms)
{
   std::vector<uint8_t> output;
   int64_t read_bytes = 0;
   int64_t compressed_bytes_size = m_p_impl->m_device.read(in_bytes,max_bytes_size,timeout_ms);
   if (compressed_bytes_size > 0)
   {
       size_t current_index = 0;
       uint8_t current = 0;
       uint8_t previous = 0;
       bool output_value = true;
       while (current_index < compressed_bytes_size)
       {
           current = in_bytes[current_index++];
           if (current == 0xFF)
           {
               current = in_bytes[current_index++];
               if (current != 0xFF)
               {
                   for (uint8_t i = 0; i < current - 1; i++)
                   {
                       output.push_back(previous);
                   }
                   output_value = false;
               }
           }
           if (output_value == true)
           {
               output.push_back(current);
           }
           output_value = true;
           previous = current;
       }
       std::memset (&in_bytes[0], 0, max_bytes_size);
       std::memcpy (&in_bytes[0], output.data(), output.size());
       read_bytes = output.size();
   }
   else
   {
       read_bytes = compressed_bytes_size;
   }  
   return read_bytes;
}

}
