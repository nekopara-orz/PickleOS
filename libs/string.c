 #include "string.h"
inline void memcpy(uint8_t *dest, const uint8_t *src, uint32_t len)
{  
	while(len--) {
	 *dest++ = *src++;
	} 
}

inline void memset(void *dest, uint8_t val, uint32_t len)
{
	 uint8_t *dst = (uint8_t *)dest;
	 while(len--) {
		 *dst++ = val;
	}
}

inline void bzero(void *dest, uint32_t len)
{
	uint8_t *dst = (uint8_t *)dest;
	while(len--) {
		*dst = 0;
	}
}

inline int strcmp(const char *str1, const char *str2)
{
	//char *t_str1,*t_str2;
	//t_str1 = str1;
	//t_str2 = str2;
	while((*str1) != '\0' && (*str2) != '\0')
	{
		if( (*str1) > (*str2) ) return 1;
		else if((*str1) < (*str2)) return -1;
		str1++;
		str2++;
	}
	if((*str1) != '\0') return 1;
	else if((*str2) != '\0') return -1;
	return 0;
}

inline char *strcpy(char *dest, const char *src)
{
	char *dst = dest;
	while((*src) != '\0'){
		*dst++ = *src++;	
	}
	*dst = '\0';
	return dest;
}


inline char *strcat(char *dest, const char *src)
{
	char *cp = dest;
	//char *t_src = src;
	while (*cp) {
	      cp++;
	}

	while ((*cp++ = *src++));

	return dest;
}

inline int strlen(const char *src)
{
	int len = 0;
	//char* t_src = src;
	while((*src) != '\0')
	{
		len++;
		src++;
	}
	return len;
}
