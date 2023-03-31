//

#include"tosh.h"


//*****************************************************
// return buf's length . 
//


int vsprintf(char *buf , const char *fmt , va_list args)
{
        int len = 0 ; 
        char*	p;
	char	tmp[256];
	va_list	p_next_arg = args;

	for (p=buf;*fmt;fmt++) {
		if (*fmt != '%') {
			*p++ = *fmt;
			continue;
		}

		fmt++;

		switch (*fmt) {
		case 'x':
			_i_to_h(tmp, *((int*)p_next_arg));
                        len = _str_len(tmp) ;
			_str_cpy(p, tmp , len );
			p_next_arg += 4;
			p += len ; 
			break;
		case 's':
			break;
		default:
			break;
		}
	}

	return (p - buf);    //length  
}

