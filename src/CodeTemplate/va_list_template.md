/*
brief:c�ɱ����ʹ��ģ��

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
        //ȡһ��ֵ��Ȼ��ָ��ƫ��
        n = va_arg(arg_ptr, int/*ע�⣬�ɱ��������ÿ����Ϊint���ͣ���Ȼ���군*/);
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
        n = va_arg(arg_ptr, int);//��֤��ȡ���һ���������ٶ�ȡһ���ֽ��� 0�� ��int�ǲ���0����֤��
        cout << n << " ";
    }

    va_end(arg_ptr); 
}







