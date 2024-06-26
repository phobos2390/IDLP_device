#include <iostream>
#include <IDLP/File_device.h>
#include <IDLP/RLE_compressor_device.h>
#include <cstdint>
#include <string>
#include <cstring>
#include <sstream>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <fcntl.h>

int32_t main(int32_t argc, char** argv)
{
    if(argc > 1)
    {
        std::string filename = argv[1];
        std::cout << filename << "\n";
        IDLP::File_device file_device;
        IDLP::IDLP_Error error = file_device.open_file(filename,O_RDONLY,0644);
        if(error == IDLP::IDLP_ERROR_NONE)
        {
            std::stringstream output_sstr;
            output_sstr << filename << ".rle";
            IDLP::File_device write_device;
            IDLP::RLE_compressor_device output_device(write_device);
            write_device.open_file(output_sstr.str(),(O_WRONLY | O_CREAT),0644);
            
            std::vector<uint8_t> bytes;
            uint8_t buffer [0x100];
            bool continuing = true;
            int64_t read_bytes = 0;
            while (continuing)
            {
                std::memset(&buffer[0], 0, sizeof(buffer));
                read_bytes = file_device.read(&buffer[0], sizeof(buffer), 0);
                if(read_bytes > 0)
                {
                    bytes.insert(bytes.end(), &buffer[0], &buffer[sizeof(buffer)]);
                }
                else
                {
                    continuing = false;
                }
            }
            if (read_bytes != 0)
            {
                std::cout << "Error reading from the device: " << read_bytes << "\n";
            }
            else
            {
                int64_t written_bytes = output_device.write(bytes.data(), bytes.size(), 0);
                std::cout << "Wrote " << written_bytes << " (of " << bytes.size() << ") bytes to " << output_sstr.str() << "\n";
            }
        }
    }
    return 0;
}
