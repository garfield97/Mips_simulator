
bool RW_error(int access_addr)
{
    if (access_addr > 0x4000000) return true;
    return false;
}


bool mem_range_error(int access_addr)
{
    if (access_addr > 0x30000008) return true;
    return false;
}


bool invalid_instruction(int access_addr)
{
    if (access_addr > 0x1FFFFFFF || access_addr < 0x4) return true;
    return false;
}