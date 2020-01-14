/*
brief:c可变参数使用模板

*/
#pragma once






int WriteLog(const char * format, ...)
{
    va_list arg_ptr;    //point
    va_start(arg_ptr, format); // init point
    int nWrittenBytes = vfprintf(logfile, format, arg_ptr);
    my_use(arg_ptr)
    va_end(arg_ptr);  //point=0
    return nWrittenBytes;
}

void use(va_list arg_ptr)
{
    int nSqSum = 0, n = n1;
    while (n > 0)
    {
        //取一个值，然后指针偏移
        n = va_arg(arg_ptr, int/*注意，可变参数必须每个都为int类型，不然就完蛋*/);
    }
    return nSqSum;
}

void testVaList(int c, ...)
{
    va_list arg_ptr;    
    va_start(arg_ptr, c); 
    int n = 1;
    for (;0!=n;)
    {
        n = va_arg(arg_ptr, int);//保证读取最后一个参数后，再读取一个字节是 0， （int是不是0带验证）
        cout << n << " ";
    }

    va_end(arg_ptr); 
}







